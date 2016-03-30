#ifndef IO_H_INCLUDED
#define IO_H_INCLUDED

#include <vector>
#include <string>

std::vector<char> get_contents(std::string filename);
void write_out (std::vector<char> contents, std::string filename);


#endif // IO_H_INCLUDED
