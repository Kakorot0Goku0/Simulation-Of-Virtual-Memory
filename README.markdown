# Virtual Memory Simulation

This project implements a virtual memory system simulation in C++, designed to demonstrate low-level memory management concepts. It includes implementations of single-level and multi-level page tables, as well as a map-based page table for optimized performance. The simulation evaluates page table performance using metrics like page hits/misses and execution time.

## Table of Contents
- [About](#about)
- [Features](#features)
- [Project Structure](#project-structure)
- [Installation](#installation)
- [Usage](#usage)
- [Performance Notes](#performance-notes)
- [Authors](#authors)
- [License](#license)

## About
This project simulates a virtual memory system with a focus on page table implementations. It supports single-level and multi-level page tables, as well as a map-based approach for improved efficiency. The simulation handles virtual addresses, page sizes (4KB), and memory configurations (4GB virtual/physical memory), providing insights into memory management performance.

## Features
- **Single-Level Page Table:** Implements a basic page table using static memory allocation.
- **Multi-Level Page Table:** Simulates a two-level page table hierarchy (note: currently incomplete but conceptually implemented).
- **Map-Based Page Table:** Utilizes C++ `map` for faster page table operations.
- **Performance Evaluation:** Measures page hits/misses and execution time for each implementation.
- **Trace File Generation:** Creates test cases for simulation with configurable process counts and requests.

## Project Structure
- `input.cpp`: Reads input data (task number, virtual address in hex, request size) for all page table implementations.
- `map_table.cpp`: Implements a map-based page table for optimized performance.
- `multi_level.cpp`: Implements a two-level page table (incomplete but includes core concepts).
- `performance_module.cpp`: Evaluates performance metrics (time, size, page hits/misses) for all implementations.
- `single_level.cpp`: Implements a single-level page table using static memory.
- `trace.cpp`: Generates trace files with test cases for a 4KB page size and 4GB virtual/physical memory.

## Installation
1. Clone the repository:
   ```bash
   git clone https://github.com/Kakorot0Goku0/Simulation-Of-Virtual-Memory.git
   ```
2. Navigate to the project directory:
   ```bash
   cd Simulation-Of-Virtual-Memory
   ```
3. Ensure a C++ compiler (e.g., `g++`) is installed.

## Usage
1. **Generate Trace File:**
   ```bash
   g++ trace.cpp -o trace
   ./trace
   ```
   *Note: Adjust global variables in `trace.cpp` to change the number of test cases (default: 100 cases for 10 processes).*

2. **Run Map-Based Page Table:**
   ```bash
   g++ map_table.cpp performance_module.cpp input.cpp -o map_table
   ./map_table
   ```

3. **Run Single-Level Page Table:**
   ```bash
   g++ single_level.cpp performance_module.cpp input.cpp -o single_level
   ./single_level
   ```
   *Note: This implementation is slower, taking ~40 seconds for 1000 requests.*

4. **Run Multi-Level Page Table:**
   ```bash
   g++ multi_level.cpp performance_module.cpp input.cpp -o multi_level
   ./multi_level
   ```

## Performance Notes
- The map-based implementation (`map_table.cpp`) is significantly faster due to the use of C++ `map`.
- The single-level page table (`single_level.cpp`) is slower due to static memory allocation.
- The multi-level implementation (`multi_level.cpp`) is incomplete but demonstrates the concept of hierarchical page tables.
- Performance metrics (execution time, page hits/misses) are logged via `performance_module.cpp`.

## Authors
- Prashant Rawat (2024CSM1015)
- Nitin Kumar Das (2024CSM1014)

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.