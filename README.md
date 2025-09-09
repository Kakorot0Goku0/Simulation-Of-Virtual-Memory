## Computer Systems Assignment 2

Name: Prashant Rawat<br>
Entry number: 2024CSM1015

Name: Nitin Kumar Das<br>
Entry Number: 2024CSM1014

### About the files:

1. `input.cpp:` This files is used to read the input text file. It reads the data in format (task number, Virtual Address in hex, Request Size). It is a common module which works for both normal single page table and map page table implementation.

2. `map_table.cpp:` This file consists of implementation of page table using map. Map makes the working of page tables faster.

3. `multi_level.cpp:` This is the multi-level implementation of page table. It consists of two level page table. It is not working properly but the concept is used via code.

4. `performance_module.cpp:` This is the common module to evaluate the perfomance of the page table implementations. It also measures time, size and number of page hit or miss.

5. `single_level.cpp:` Basic implementation of single level page table using static memory which makes this one more slow and time consuming.

6. `trace.cpp:` Used to generate trace file for testing the different modules. It generates test cases for 4KB page size, 4GB virtual and 4GB physical memory.


### How to run the different files:

1. To run trace file to generate the sample inputs.
> g++ trace.cpp ; ./a.out

2. To run the map implementation of page tables
> g++ map_table.cpp performance_module.cpp input.cpp ; ./a.out

3. To run the normal implementation of page tables. (Takes 40 secs to run for 1000 requests)
> g++ single_level.cpp performance_module.cpp input.cpp ; ./a.out

4. To run the multilevel implementation of page tables.
> g++ multi_level.cpp performance_module.cpp input.cpp ; ./a.out

<br>

`Note:` Currently the trace file is generating only 100 test cases for 10 different process. This can be changed in the global variables in the trace.cpp file. It runs very slow in single_level.cpp but for map_table.cpp it runs in very short time due to use of map in the implementation.