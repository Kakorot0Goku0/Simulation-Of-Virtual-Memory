#ifndef INPUT_H
#define INPUT_H

#include <iostream>
#include <cstdint>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>

using namespace std;

class task
{
public:
    size_t task_no;
    size_t virtual_address;
    size_t size;
    size_t offset;

    task(size_t, size_t, size_t, size_t);
};

class task2
{
public:
    size_t task_no;
    size_t virtual_address1;
    size_t virtual_address2;
    size_t size;
    size_t offset;

    task2(size_t task_no, size_t virtual_address1, size_t virtual_address2, size_t size, size_t offset);
};

// convert hex virtual address to int virtual address
static size_t Hex_to_int(string ss);

// convert hex virtual address to int virtual address
static size_t binary_to_int(string ss);

static string hex_to_binary(string ss);

// changes the size of request (KB) to no of physical addresses requied
static size_t pow_of_two(string s,size_t N);

// changing each line(request) to vector of task and
task string_manipulate(string line, size_t N,size_t offset_size,size_t v1,size_t v2);

// input from txt file and read it line by line
vector<task> get_input(size_t,size_t,size_t v1,size_t v2);

// changing each line(request) to vector of task and
task2 string_manipulate2(string line, size_t N);

// input from txt file and read it line by line
vector<task2> get_input2(size_t N,size_t,size_t,size_t);

#endif