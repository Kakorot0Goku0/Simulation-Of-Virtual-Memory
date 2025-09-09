#include <iostream>
#include <time.h>
#include <map>
#include "performance_module.h"

using namespace std;

task_performance::task_performance(size_t page_hit, size_t page_miss, size_t physical_memory)
{
    this->page_hit = page_hit;
    this->page_miss = page_miss;
    this->physical_memory = physical_memory;
}

module_performance::module_performance(size_t hit, size_t miss, size_t physical_memory, size_t pageSizeBytes)
{
    this->total_hit = hit;
    this->total_miss = miss;
    this->total_physical_memory = physical_memory;
    this->pageSizeBytes = pageSizeBytes;
    mp = map<size_t, task_performance>();
}

void module_performance::printPerformanceData()
{
    cout << "<----------- Total Stats -------------->\n";
    cout << "Total Page hit: " << total_hit << endl;
    cout << "Total Page miss: " << total_miss << endl;
    cout << "Total Physical Memory: " << total_physical_memory << " pages\n";
    cout << "Physical Memory Used: " << physicalMemoryUsed << " pages\n";
    cout << "Physical Memory available: " << (total_physical_memory - physicalMemoryUsed) << " pages\n";
    cout << "Memory Required for page tables: " << memoryRequiredForPageTables << " B" << endl;
    cout << "Total time required: " << (float)(end - start) / CLOCKS_PER_SEC << " seconds\n";

    for (auto s : mp)
    {
        size_t taskId = s.first;
        task_performance task = s.second;

        cout << "\n\n";
        cout << "<----------- Task " << taskId << " Stats -------------->\n";
        cout << "Hits: " << task.page_hit << endl;
        cout << "Misses: " << task.page_miss << endl;
        cout << "Physical pages allocated: " << task.physical_memory << endl;
    }

    cout << "\n\n";
}
