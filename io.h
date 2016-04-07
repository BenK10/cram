#ifndef IO_H_INCLUDED
#define IO_H_INCLUDED

#include <vector>
#include <string>

std::vector<char> get_contents(std::string filename);
std::vector<bool> char_to_bools(char c);
char pack_bools(std::vector<bool>& bits, int index);

#endif // IO_H_INCLUDED
