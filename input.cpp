#include <iostream>
#include <cstdint>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>
#include "input.h"

using namespace std;

// int N = config::pageSizeKb; // page size

task::task(size_t task_no, size_t virtual_address, size_t size, size_t offset)
{
    this->task_no = task_no;
    this->virtual_address = virtual_address;
    this->size = size;
    this->offset = offset;
}

// for two level
task2::task2(size_t task_no, size_t virtual_address1, size_t virtual_address2, size_t size, size_t offset)
{
    this->task_no = task_no;
    this->virtual_address1 = virtual_address1;
    this->virtual_address2 = virtual_address2;
    this->size = size;
    this->offset = offset;
}

// convert hex virtual address to int virtual address
static size_t Hex_to_int(string ss)
{
    unordered_map<std::string, size_t> map;
    map["0"] = 0;
    map["1"] = 1;
    map["2"] = 2;
    map["3"] = 3;
    map["4"] = 4;
    map["5"] = 5;
    map["6"] = 6;
    map["7"] = 7;
    map["8"] = 8;
    map["9"] = 9;
    map["A"] = 10;
    map["B"] = 11;
    map["C"] = 12;
    map["D"] = 13;
    map["E"] = 14;
    map["F"] = 15;

    size_t result = 0;
    size_t hex = 1;

    for (int i = ss.size() - 1; i >= 0; --i)
    {
        char c = ss[i];
        size_t digit = 0;
        if (c >= '0' && c <= '9')
        {
            digit = c - '0';
        }
        else if (c >= 'A' && c <= 'F')
        {
            digit = c - 'A' + 10;
        }
        else if (c >= 'a' && c <= 'f')
        { // Handle lowercase hex digits
            digit = c - 'a' + 10;
        }
        result += digit * hex;
        hex *= 16;
    }
    return result;
}

// convert hex virtual address to int virtual address
static size_t binary_to_int(string ss)
{
    // size_t result = 0;
    // size_t pow = 1;

    // for (size_t i = ss.length() ; i >0; i--)
    // {
        //string str(1, ss[i]);
        size_t digit = stol(ss,0,2);//ss[i]-'0';
        // result += digit * pow;
        // pow *= 2;
    // }
    return digit;
}

static string hex_to_binary(string ss)
{
    unordered_map<std::string, string> map;
    map["0"] = "0000";
    map["1"] = "0001";
    map["2"] = "0010";
    map["3"] = "0011";
    map["4"] = "0100";
    map["5"] = "0101";
    map["6"] = "0110";
    map["7"] = "0111";
    map["8"] = "1000";
    map["9"] = "1001";
    map["A"] = "1010";
    map["B"] = "1011";
    map["C"] = "1100";
    map["D"] = "1101";
    map["E"] = "1110";
    map["F"] = "1111";

    string result = "";
    // size_t hex=1;

    for (size_t i = 0; i < ss.size(); i++)
    {
        string str(1, ss[i]);
        string digit = map[str];
        result += digit;
    }
    return result;
}

// changes the size of request (KB) to no of physical addresses requied
static size_t pow_of_two(string s, size_t N)
{
    size_t n = stoul(s);
    size_t result = 0;
    return n / N;
}

// changing each line(request) to vector of task and
task string_manipulate(string line, size_t N,size_t offset_size,size_t v1,size_t v2)
{
    vector<string> split;

    string delimiter = ":";

    string token;
    size_t start = 0;
    size_t end = line.find(delimiter);
    while (end != std::string::npos)
    {
        token = line.substr(start, end - start);
        split.push_back(token);
        start = end + 1;
        end = line.find(delimiter, start);
    }

    split.push_back(line.substr(start));

    size_t pid = 0;
    size_t offset = 0;
    size_t virtual_address = 0;
    size_t size = 0;

    split[0] = split[0].erase(0, 1);
    pid = stoul(split[0]);

  split[1] = split[1].erase(0, 2);
    string vir = hex_to_binary(split[1]);

    //offset = Hex_to_int(split[1].substr(split[1].length() - 3, 3));
    offset = binary_to_int(vir.substr(vir.length() - offset_size, offset_size));

    //split[1] = split[1].erase(split[1].length() - offset_size, offset_size);
    //split[1] = split[1].erase(0, 2);
    // cout<<split[1]<<"h"<<endl;
    //  split[1]=split[1].erase(0,1);
    //  split[1]=split[1].erase(split[1].size()-2,2);
    string vv=vir.substr(0,v1+v2);
    virtual_address = binary_to_int(vv);//Hex_to_int(split[1]);

    split[2] = split[2].erase(split[2].length() - 2, 2);
    size = pow_of_two(split[2], N);

    task temp_task = task(pid, virtual_address, size, offset);

    return temp_task;
}

// input from txt file and read it line by line
vector<task> get_input(size_t N,size_t offset,size_t v1,size_t v2)
{
    vector<task> task_list;
    ifstream inputFile("input.txt");
    if (!inputFile.is_open())
    {
        cerr << "Error opening the file!" << endl;
        return task_list;
    }
    string line;

    while (getline(inputFile, line))
    {
        task_list.push_back(string_manipulate(line, N,offset,v1,v2));
    }
    // for(auto t:task_list)
    // {
    //     cout<<t.task_no<<":"<<t.virtual_address<<":"<<t.size<<":"<<t.offset<<endl;
    // }

    inputFile.close();
    return task_list;
}

// changing each line(request) to vector of task and
task2 string_manipulate2(string line, size_t N,size_t offset_size,size_t v1,size_t v2)
{
    vector<string> split;

    string delimiter = ":";

    std::string token;
    size_t start = 0;
    size_t end = line.find(delimiter);
    while (end != std::string::npos)
    {
        token = line.substr(start, end - start);
        split.push_back(token);
        start = end + 1;
        end = line.find(delimiter, start);
    }

    split.push_back(line.substr(start));

    size_t pid = 0;
    size_t offset = 0;
    size_t virtual_address1 = 0;
    size_t virtual_address2 = 0;
    size_t size = 0;

    split[0] = split[0].erase(0, 1);
    pid = stoul(split[0]);

    //offset = Hex_to_int(split[1].substr(split[1].length() - 3, 3));

    //split[1] = split[1].erase(split[1].length() - 3, 3);
    split[1] = split[1].erase(0, 2);
    string vir = hex_to_binary(split[1]);

    offset = binary_to_int(vir.substr(vir.length() - offset_size, offset_size));


    string vir1 = vir.substr(0, v1);
    string vir2 = vir.substr(v1,v2);

    virtual_address1 = binary_to_int(vir1);
    virtual_address2 = binary_to_int(vir2);

    // cout<<"Virtual_address1:"<<vir1<<":"<<vir2<<"t"<<endl;
    split[2] = split[2].erase(split[2].length() - 2, 2);
    size = pow_of_two(split[2], N);

    task2 temp_task = task2(pid, virtual_address1, virtual_address2, size, offset);

    return temp_task;
}

// input from txt file and read it line by line
vector<task2> get_input2(size_t N,size_t offset,size_t v1,size_t v2)
{
    vector<task2> task_list;
    ifstream inputFile("input.txt");
    if (!inputFile.is_open())
    {
        cerr << "Error opening the file!" << endl;
        return task_list;
    }
    string line;

    while (getline(inputFile, line))
    {
        task_list.push_back(string_manipulate2(line, N,offset,v1,v2));
    }
    // for(auto t:task_list)
    // {
    //     cout<<t.task_no<<":"<<t.virtual_address1<<":"<<t.virtual_address2<<":"<<t.size<<":"<<t.offset<<endl;
    // }
    inputFile.close();
    return task_list;
}

// int main()
// {
//     get_input(4,12,10,10);
//     return 0;
// }