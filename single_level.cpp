#include <iostream>
#include <math.h>
#include <vector>
#include <string>
#include <ctime>
#include "input.h"
#include "performance_module.h"
#include "config.h"

using namespace std;

/* Paging Code starts here  */

// class for page hit and miss check
class RequestResult
{
public:
    bool status;
    size_t physicalAddress;
    RequestResult(bool status, size_t physicalAddress)
    {
        this->status = status;
        this->physicalAddress = physicalAddress;
    }
};

// content of each page
class PageTableEntry
{

public:
    size_t physicalAddress;
    bool write = false;
    bool read = false;
    bool execute = false;
    bool validBit = false;

    PageTableEntry(size_t physicalAddress, bool write, bool read, bool execute, bool validBit)
    {
        this->physicalAddress = physicalAddress;
        this->write = write;
        this->read = read;
        this->execute = execute;
        this->validBit = validBit;
    }
};

// each task has its page table with PageTableEntry as a vector
class PageTable
{
public:
    size_t numberOfVirtualPages;
    vector<PageTableEntry> entries;
    size_t pid;

    PageTable(size_t pageSizeBytes, size_t virtualMemory, size_t pid)
    {
        numberOfVirtualPages = virtualMemory / pageSizeBytes;
        PageTableEntry pte(0, false, false, false, false);
        entries = vector<PageTableEntry>(numberOfVirtualPages, pte);
        this->pid = pid;
    }
};

class MemoryManager
{
public:
    size_t numberOfPhysicalPages;
    vector<bool> available;
    size_t nextFreeIndex = 0;
    vector<PageTable> pageTables;
    size_t pageSizeBytes;
    size_t virtualMemory;

    // initailize the physical pages
    MemoryManager(size_t pageSizeBytes, size_t physicalMemory,size_t virtualMemory)
    {
        numberOfPhysicalPages = physicalMemory / pageSizeBytes;
        available = vector<bool>(numberOfPhysicalPages, true);
        pageTables = vector<PageTable>();
        this->pageSizeBytes = pageSizeBytes;
        this->virtualMemory = virtualMemory;
    }
    // accepts the request and proceses it
    RequestResult requestMemory(size_t pid, size_t virtualAddress, size_t offset, size_t pagesRequired, module_performance &modulePerformance)
    {
        // if physical memory is full condition
        if (nextFreeIndex + pagesRequired > numberOfPhysicalPages)
        {
            cout << "Physical Memory full\n";
        }

        int index = -1;

        // loop through pagetalbes for alreadry existing task
        for (int i = 0; i < pageTables.size(); i++)
        {
            PageTable pt = pageTables[i];
            // checkfor pid
            if (pt.pid == pid)
            {
                index = i;
                break;
            }
        }

        // if not found then create
        if (index == -1)
        {
            pageTables.push_back(PageTable(pageSizeBytes, virtualMemory, pid));
            index = pageTables.size() - 1;
            for (int i = 0; i < pagesRequired; i++)
            {
                pageTables[index].entries[virtualAddress + i].physicalAddress = nextFreeIndex;
                pageTables[index].entries[virtualAddress + i].validBit = true;

                available[nextFreeIndex] = true;
                nextFreeIndex++;

                modulePerformance.mp[pid].page_miss++;
                modulePerformance.mp[pid].physical_memory++;

                modulePerformance.total_miss++;
                modulePerformance.physicalMemoryUsed++;
            }
        }
        else
        {
            // cout << "Page Table already present\n";
            // checking if page already allocated or not
            bool spaceAvailable = true;
            for (int i = 0; i < pagesRequired; i++)
            {
                if (pageTables[index].entries[virtualAddress + i].validBit == true)
                {
                    spaceAvailable = false;
                    modulePerformance.mp[pid].page_hit++;
                    modulePerformance.total_hit++;
                    // break;
                }
                else
                {
                    pageTables[index].entries[virtualAddress + i].physicalAddress = nextFreeIndex;
                    pageTables[index].entries[virtualAddress + i].validBit = true;

                    available[nextFreeIndex] = false;
                    nextFreeIndex++;

                    modulePerformance.mp[pid].page_miss++;
                    modulePerformance.mp[pid].physical_memory++;

                    modulePerformance.total_miss++;
                    modulePerformance.physicalMemoryUsed++;
                }
            }
        }
        // cout << "Page(s) allocated\n";
        return RequestResult(true, nextFreeIndex - pagesRequired);
    }
};

int main()
{
    module_performance modulePerformance = module_performance(0, 0, config::physicalMemory, config::pageSizeBytes);
    modulePerformance.physicalMemoryUsed=0;
    MemoryManager mm(config::pageSizeBytes, config::physicalMemory,config::virtualMemory);
    auto tasks = get_input(config::pageSizeKb,config::offset,config::v1,config::v2);
    modulePerformance.start = clock();
    for (auto t : tasks)
    {
            if (modulePerformance.mp.find(t.task_no) == modulePerformance.mp.end())
        {
            modulePerformance.mp.insert(make_pair(t.task_no, task_performance(0, 0, 0)));
        }
        RequestResult res = mm.requestMemory(t.task_no, t.virtual_address, t.offset, t.size, modulePerformance);
    
    }
    modulePerformance.end = clock();
    size_t pageTableSize = 0;
    for (auto pt : mm.pageTables)
    {
        pageTableSize += pt.entries.size() * sizeof(PageTableEntry);
    }
    modulePerformance.memoryRequiredForPageTables = pageTableSize;
    modulePerformance.printPerformanceData();
    return 0;
}