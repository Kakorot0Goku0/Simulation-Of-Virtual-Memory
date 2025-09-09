#ifndef CONFIG_H
#define CONFIG_H
#include <stdlib.h>
#include <math.h>
#include <string>

namespace config
{

    size_t global_increment = 4294967296;

    //4kb 4gb 4gb
    size_t N = 32;
    size_t P=32;
    int pageSizeKb = 4;
    // Test module
    std::string textSection = "0x10000000";
    std::string dataSection = "0x20000000";
    std::string stackSection = "0x30000000";
    std::string sharedLibrarySection = "0x40000000";
    std::string heapSection = "0x50000000";
    std::string limit= "0x60000000";
    size_t v1=10;
    size_t v2=10;
    size_t offset=12;

    // //2 kb 4gb 8gb
    // size_t N = 32;
    // size_t P = 33;
    // int pageSizeKb = 2;
    //  //Test module
    // std::string textSection = "0x10000000";
    // std::string dataSection = "0x20000000";
    // std::string stackSection = "0x10000000";
    // std::string sharedLibrarySection = "0x40000000";
    // std::string heapSection = "0x50000000";
    // std::string limit= "0x60000000";
    // size_t v1=10;
    // size_t v2=11;
    // size_t offset=11;

    // // 1kb 8gb 16gb
    // size_t N = 33;
    // size_t P = 34;
    // int pageSizeKb = 1;
    // std::string textSection = "0x110000000";
    // std::string dataSection = "0x120000000";
    // std::string stackSection = "0x130000000";
    // std::string sharedLibrarySection = "0x140000000";
    // std::string heapSection = "0x150000000";
    // std::string limit = "0x160000000";
    // size_t v1 = 11;
    // size_t v2 = 12;
    // size_t offset = 10;

    // dynamic part
    size_t virtualMemory = pow(2, N);
    size_t physicalMemory = pow(2, P);
    size_t pageSizeBytes = pageSizeKb * pow(2, 10);

    // trace generation
    size_t no_of_task = 10;
    size_t no_of_trace = 100;
    size_t limt_max_size = 100; // max memory size in KB

};
#endif