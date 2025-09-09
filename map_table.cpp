#include <iostream>
#include <cstdint>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <cmath>
#include "config.h"
#include "input.h"
#include "performance_module.h"

using namespace std;

// calculating the performance
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

// each pagetable entery is add to each task pagetable for mapping of virtual address and physical address
class PageTableEntry
{

public:
    size_t physicalAddress;
    bool write = false;
    bool read = false;
    bool execute = false;
    bool validBit = false;

    PageTableEntry(size_t physicalAddress = 0, bool write = false, bool read = false, bool execute = false, bool validBit = false)
    {
        this->physicalAddress = physicalAddress;
        this->write = write;
        this->read = read;
        this->execute = execute;
        this->validBit = validBit;
    }
};

// take requst and allocate memory for page table
class Memory_Manager
{
public:
    size_t numberOfPhysicalPages;
    size_t nextFreeIndex = 0;
    unordered_map<size_t, unordered_map<size_t, PageTableEntry>> PageTables;
    size_t pageSizeBytes;

    Memory_Manager(size_t pageSizeBytes, size_t PhysicalMomory)
    {
        numberOfPhysicalPages = PhysicalMomory / pageSizeBytes;
        this->pageSizeBytes = pageSizeBytes;
    }
    // requst the physical memory
    RequestResult requestMemory(size_t task_no, size_t virtual_address, size_t offset, size_t pagesRequired, module_performance &modulePerformance)
    {
        // condition for physical memory check
        if (nextFreeIndex + pagesRequired > numberOfPhysicalPages)
        {
            cout << "Physical Memory full\n";
        }

        auto index_no = PageTables.find(task_no);
        // if task is all ready recived check for overlapping condition
        if (index_no != PageTables.end())
        {
            bool spaceAvailable = true;

            auto &innermap = index_no->second;
            for (int i = 0; i < pagesRequired; i++)
            {
                auto vir = innermap.find(virtual_address + i);

                if (vir != innermap.end())
                {
                    spaceAvailable = false;
                    // break;
                    modulePerformance.mp[task_no].page_hit++;
                    modulePerformance.total_hit++;
                }
                else
                {
                    PageTableEntry pt(nextFreeIndex, false, false, false, true);

                    PageTables[task_no][virtual_address + i] = pt;

                    modulePerformance.mp[task_no].page_miss++;
                    modulePerformance.mp[task_no].physical_memory++;
                    modulePerformance.physicalMemoryUsed++;
                    modulePerformance.total_miss++;
                    //   cout<<task_no<<" : "<<virtual_address<<" : "<<nextFreeIndex<<endl;
                    nextFreeIndex++;
                }
            }
        }
        else
        {
            // allocate space in available array
            for (size_t i = 0; i < pagesRequired; i++)
            {
                PageTableEntry pt(nextFreeIndex, false, false, false, true);

                PageTables[task_no][virtual_address + i] = pt;

                modulePerformance.mp[task_no].page_miss++;
                modulePerformance.mp[task_no].physical_memory++;
                modulePerformance.physicalMemoryUsed++;
                modulePerformance.total_miss++;
                //   cout<<task_no<<" : "<<virtual_address<<" : "<<nextFreeIndex<<endl;
                nextFreeIndex++;
            }
        }
        return RequestResult(true, nextFreeIndex - pagesRequired);
    }
};

int main()
{

    module_performance modulePerformance = module_performance(0, 0, config::physicalMemory, config::pageSizeBytes);
    modulePerformance.physicalMemoryUsed = 0;
    Memory_Manager mm(config::pageSizeBytes, config::physicalMemory);

    vector<task> tasks = get_input(config::pageSizeKb, config::offset, config::v1, config::v2);

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

    modulePerformance.memoryRequiredForPageTables = 0; // sizeof(PageTableEntry) * mm.PageTables.size();

    for (auto i : mm.PageTables)
    {
        modulePerformance.memoryRequiredForPageTables += sizeof(PageTableEntry) * i.second.size();
    }

    modulePerformance.printPerformanceData();

    return 0;
}
