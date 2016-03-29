#ifndef HUFFMAN_COMP_H_INCLUDED
#define HUFFMAN_COMP_H_INCLUDED

#include <vector>
#include <unordered_map>

struct huffnode
{
    char data;
    int freq;
    huffnode* left;
    huffnode* right;

    huffnode(char, int, huffnode*, huffnode*);
};

std::vector<int> get_histogram(std::vector<char> bytes);

void build_codebook(huffnode* root, std::unordered_map<char, std::vector<bool>>& codebook);
void get_code(huffnode* node, huffnode* root, std::unordered_map<char, std::vector<bool>>& codebook, std::vector<bool> code, bool bit=0);

//pack 8 bools in vector<bool> into bits in a char
char pack_bools(std::vector<bool>& bits, int index);


#endif // HUFFMAN_COMP_H_INCLUDED
