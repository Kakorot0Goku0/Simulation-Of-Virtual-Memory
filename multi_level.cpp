#include <iostream>
#include <math.h>
#include <vector>
#include <map>
#include <string>
#include <cstdint>
#include <fstream>
#include <unordered_map>
#include "input.h"
#include "performance_module.h"
#include "config.h"

using namespace std;

/* Paging Code starts here  */

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

class PageTable1Entry
{

public:
    size_t l2PageTableIndex;
    bool write = false;
    bool read = false;
    bool execute = false;
    bool validBit = false;

    PageTable1Entry(size_t l2PageTableIndex = 0, bool write = false, bool read = false, bool execute = false, bool validBit = false)
    {
        this->l2PageTableIndex = l2PageTableIndex;
        this->write = write;
        this->read = read;
        this->execute = execute;
        this->validBit = validBit;
    }
};

class PageTable2Entry
{
public:
    size_t physicalAddress;
    bool write = false;
    bool read = false;
    bool execute = false;
    bool validBit = false;

    PageTable2Entry(size_t physicalAddress = 0, bool write = false, bool read = false, bool execute = false, bool validBit = false)
    {
        this->physicalAddress = physicalAddress;
        this->write = write;
        this->read = read;
        this->execute = execute;
        this->validBit = validBit;
    }
};

class Level2PageTable
{
public:
    map<size_t, PageTable2Entry> entries;
    Level2PageTable()
    {
        entries = map<size_t, PageTable2Entry>();
    }
};

class Level1PageTable
{
public:
    size_t numberOfVirtualPages;
    vector<PageTable1Entry> entries;
    map<size_t, Level2PageTable *> level2PageTables;
    size_t pid;

    Level1PageTable(size_t pid = 0)
    {
        size_t numberOfLevel1Entries = pow(2, config::v1);
        entries = vector<PageTable1Entry>(numberOfLevel1Entries, PageTable1Entry(0, false, false, false, false));
        level2PageTables = map<size_t, Level2PageTable *>();
        this->pid = pid;
    }
};

class MemoryManager
{
public:
    size_t numberOfPhysicalPages;
    vector<bool> available;
    size_t nextFreeIndex = 0;
    map<size_t, Level1PageTable> pageTables;
    size_t pageSizeBytes;
    module_performance &mp;

    MemoryManager(size_t pageSizeBytes, size_t physicalMemory, module_performance &mp) : mp(mp)
    {
        numberOfPhysicalPages = physicalMemory;
        // cout << "Number of Physical Pages: " << numberOfPhysicalPages << endl;
        available = vector<bool>(numberOfPhysicalPages, true);
        pageTables = map<size_t, Level1PageTable>();
        this->pageSizeBytes = pageSizeBytes;
        mp.physicalMemoryUsed = 0;
    }

    RequestResult requestMemory(size_t pid, size_t virtualAddress1, size_t virtualAddress2, size_t offset, size_t pagesRequired)
    {
        if (nextFreeIndex + pagesRequired > numberOfPhysicalPages)
        {
            cout << "Physical Memory full\n";
        }

        // if not found then create
        if (pageTables.find(pid) == pageTables.end())
        {
            Level1PageTable newPageTable(pid);
            auto newPair = make_pair(pid, newPageTable);
            pageTables.insert(newPair);
        }
        auto l1PageTable = &pageTables[pid];

        if (l1PageTable->entries[virtualAddress1].validBit == false)
        {
            // creating entry in the first level page table
            l1PageTable->entries[virtualAddress1].l2PageTableIndex = virtualAddress2;
            l1PageTable->entries[virtualAddress1].read = true;
            l1PageTable->entries[virtualAddress1].write = true;
            l1PageTable->entries[virtualAddress1].execute = true;
            l1PageTable->entries[virtualAddress1].validBit = true;
        }

        size_t l2PageTableIndex = l1PageTable->entries[virtualAddress1].l2PageTableIndex;
        if (l1PageTable->level2PageTables.find(l2PageTableIndex) == l1PageTable->level2PageTables.end())
        {
            // creating level 2 page table - dynamic allocation
            Level2PageTable *newLevel2Entry = new Level2PageTable();
            l1PageTable->level2PageTables.insert(make_pair(l2PageTableIndex, newLevel2Entry));
        }

        Level2PageTable *l2pt = l1PageTable->level2PageTables[l2PageTableIndex];
        for (int i = 0; i < pagesRequired; i++)
        {
            if (l2pt->entries.find(virtualAddress2 + i) == l2pt->entries.end())
            {
                // creating entry in level 2 page table - page miss
                PageTable2Entry level2PageEntry(nextFreeIndex, true, true, true, true);
                l2pt->entries.insert(make_pair(virtualAddress2 + i, level2PageEntry));

                available[nextFreeIndex] = false;
                nextFreeIndex++;

                mp.mp[pid].page_miss++;
                mp.mp[pid].physical_memory++;

                mp.total_miss++;
                mp.physicalMemoryUsed++;
                // cout << "Page miss\n";
            }
            else
            {
                if (l2pt->entries[virtualAddress2 + i].validBit == false)
                {
                    PageTable2Entry level2PageEntry(nextFreeIndex, true, true, true, true);
                    l2pt->entries.insert(make_pair(virtualAddress2 + i, level2PageEntry));

                    available[nextFreeIndex] = false;
                    nextFreeIndex++;

                    mp.mp[pid].page_miss++;
                    mp.mp[pid].physical_memory++;

                    mp.total_miss++;
                    mp.physicalMemoryUsed++;
                    // cout << "Page miss\n";
                }
                else
                {
                    // page hit
                    mp.mp[pid].page_hit++;
                    mp.total_hit++;
                    // cout << "Page hit\n";
                }
            }
        }

        // cout << "Page(s) allocated\n";
        return RequestResult(false, nextFreeIndex - pagesRequired);
    }
};

int main()
{
    module_performance modulePerformance = module_performance(0, 0, config::physicalMemory, config::pageSizeBytes);
    MemoryManager mm(config::pageSizeBytes, config::physicalMemory, modulePerformance);
    auto tasks = get_input2(config::pageSizeKb, config::offset, config::v1, config::v2);

    modulePerformance.start = clock();
    for (auto t : tasks)
    {
        if (modulePerformance.mp.find(t.task_no) == modulePerformance.mp.end())
        {
            // cout << "Making task in performance module :" << t.task_no << endl;
            modulePerformance.mp.insert(make_pair(t.task_no, task_performance(0, 0, 0)));
        }
        // cout << t.task_no << " " << t.virtual_address1 << " " << t.virtual_address2 << " " << t.offset << " " << t.size << endl;
        RequestResult res = mm.requestMemory(t.task_no, t.virtual_address1, t.virtual_address2, t.offset, t.size);
    }
    modulePerformance.end = clock();

    size_t pageTableSize = 0;
    // calculating size of level 1 page tables
    pageTableSize += mm.pageTables.size() * (pow(2, config::v1) * sizeof(PageTable1Entry));
    size_t sizeOfL2PageEntry = sizeof(Level2PageTable);
    for (auto l1Pt : mm.pageTables)
    {
        for (auto l1PtEntry : l1Pt.second.entries)
        {
            if (l1PtEntry.validBit == true)
            {
                pageTableSize += sizeOfL2PageEntry;
            }
        }
    }
    modulePerformance.memoryRequiredForPageTables = pageTableSize;

    modulePerformance.printPerformanceData();

    return 0;
}