#include <iostream>
#include <random>
#include <time.h>
#include <fstream>
#include <string>

#include <sstream>
#include <algorithm>
#include <cctype>
#include "config.h"
#include <pthread.h> 

using namespace std;
pthread_mutex_t fileMutex;

struct argument
{
    size_t tarce_no;
    //vector<string>* traceEntry;
    ofstream* outFile;
};

size_t page_size = config::pageSizeKb;                        // page size multiple of
size_t virtual_size = config::virtualMemory; // virtual memory size

string generate_virtual_add(string lowerlimit,string uperrlimit) // generate virtual adress
{
    size_t limit=stol(lowerlimit,nullptr,16);
    size_t upp=stol(uperrlimit,nullptr,16);
    size_t offset=upp-limit-100*1024;

    size_t tt = rand();
    size_t virtual_address = limit+(tt)%offset;

    stringstream ss;
    ss << hex << virtual_address;

    string hexString = ss.str();
    transform(hexString.begin(), hexString.end(), hexString.begin(), ::toupper);
    // if (hexString.size() < config::N/8)
    // {
    //     for (int i = 0; i < 8 - hexString.size(); i++)
    //         hexString = "0" + hexString;
    // }
    return hexString;
}
string generate_size() // generate the size of pages
{
    size_t size = (rand() % config::limt_max_size);
    size = page_size+((size / page_size))* page_size;
    return to_string(size);
}
void* generate_trace_entry(void* arg) // generate traces and entry
{
    argument* args = static_cast<argument*>(arg);
    size_t trace_no = (size_t)args->tarce_no;
    //vector<string>* traces = args->traceEntry;
    ofstream* outFile = args->outFile;

    vector<string> section;
    section.push_back(config::textSection);
    section.push_back(config::dataSection);
     section.push_back(config::stackSection);
    section.push_back(config::sharedLibrarySection);
       section.push_back(config::heapSection);
    section.push_back(config::limit);
    
    int t=0;
    for(int i = 0; i <config::no_of_trace; i++)
    {
        string tarce = "T" + to_string(args->tarce_no);
        string virtual_address = generate_virtual_add(section[(t)%5],section[t+1]);
        t=(t+1)%5;
        
        string size = generate_size();

        string trace_entry = tarce + ":0x" + virtual_address + ":" + size + "KB";
       
   
        pthread_mutex_lock(&fileMutex); 
    
        //traces->push_back(trace_entry);
        (*outFile)<< trace_entry << std::endl;

        pthread_mutex_unlock(&fileMutex); 
        
       
    }
    
    pthread_exit(nullptr);
}

int main()
{
    std::ofstream outfile("input.txt");
    if (!outfile.is_open())
    {
        cerr << "Failed to open file for writing." << endl;
    }
    
    // Seed the random number generator with the current time
    srand(static_cast<unsigned int>(time(0)));
    vector<pthread_t> threads(config::no_of_task); // Vector to hold thread identifiers
    vector<argument> thread_ids(config::no_of_task);   
    //vector<size_t> thread_ids(config::no_of_task);   
    vector<string> traceEntry;

    pthread_mutex_init(&fileMutex, nullptr);

    for (int i = 0; i < config::no_of_task; i++)
    {

        //thread_ids[i] = i+1;
        thread_ids[i].tarce_no = i+1;
        //thread_ids[i].traceEntry = &traceEntry;
        thread_ids[i].outFile = &outfile;
      
        pthread_create(&threads[i], nullptr, generate_trace_entry, &thread_ids[i]);
        
     
    }
     
    for(int i=0; i < config::no_of_task; i++)
    {
        pthread_join(threads[i], NULL);
    }
     // pthread_mutex_lock(&fileMutex); 
    
    // for(string a:traceEntry)
    // {
    //     cout<<a<<endl;
    //  //   outfile<< a<< std::endl;

    // }
      //pthread_mutex_unlock(&fileMutex);   
    cout<<"1";
    outfile.close();    
    pthread_mutex_destroy(&fileMutex);
    return 0;
}