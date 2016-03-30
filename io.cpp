#include "io.h"
#include <fstream>
#include <exception>

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

void write_out (std::vector<char> contents, std::string filename)
{
 //TODO
}
