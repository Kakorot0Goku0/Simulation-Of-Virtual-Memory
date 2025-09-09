#pragma once
#include <iostream>
#include <time.h>
#include <map>
using namespace std;

// for claulating performance of each task
class task_performance
{
public:
    size_t page_hit;
    size_t page_miss;
    size_t physical_memory;

    task_performance(size_t = 0, size_t = 0, size_t = 0);
};

// calculates odual performance map, single,milti level
class module_performance
{
public:
    size_t total_hit;
    size_t total_miss;
    size_t total_physical_memory;
    size_t pageSizeBytes;
    time_t start;
    time_t end;
    map<size_t, task_performance> mp;
    size_t memoryRequiredForPageTables;
    size_t physicalMemoryUsed;

    module_performance(size_t, size_t, size_t, size_t);
    void printPerformanceData();
};
