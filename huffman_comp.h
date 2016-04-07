#ifndef HUFFMAN_COMP_H_INCLUDED
#define HUFFMAN_COMP_H_INCLUDED

#include <vector>
#include <unordered_map>
#include "huffnode.h"

std::vector<int> get_histogram(std::vector<char> bytes);

void build_codebook(huffnode* root, std::unordered_map<char, std::vector<bool>>& codebook);
void get_code(huffnode* node, huffnode* root, std::unordered_map<char, std::vector<bool>>& codebook, std::vector<bool> code, bool bit=0);

std::vector<bool> char_to_bools(char c);
std::vector<bool> encode_tree(huffnode* root);
void _encode_tree(huffnode* root, std::vector<bool>& encoded_tree);

std::vector<char> build_header(int final_bits, int num_tree_bits, std::vector<char> encoded_tree);


//pack 8 bools in vector<bool> into bits in a char
char pack_bools(std::vector<bool>& bits, int index);


#endif // HUFFMAN_COMP_H_INCLUDED
