#include "io.h"
#include <fstream>
#include <exception>
#include <algorithm>
#include <iostream> //TODO remove

//"insane_coder"'s binary file read, benchmarked to be the fastest way
//see http://insanecoding.blogspot.it/2011/11/how-to-read-in-file-in-c.html
std::vector<char> get_contents(std::string filename)
{
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    if(in)
    {
        std::vector<char> contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());

        return contents;
    }
    else throw(std::exception());
}

//convert a char into a vector<bool>
std::vector<bool> char_to_bools(char c)
{
    std::vector<bool> result(8);
    char mask = (char)1;
    for(int i=0; i<8; ++i)
        result[i]=((c>>(i)) & mask);

    std::reverse(result.begin(), result.end());

    return result;
}

//pack 8 bools into a char
char pack_bools(std::vector<bool>& bits, int index)
{
    char result = (char)0;
    char mask = (char)1;

    for(int i=0; i<8; ++i)
    {
        result = result | ((char)bits[i+index] & mask);
        if( (i+1)%8 != 0) result <<= 1; //don't shift if at end of byte
    }

    return result;
}
