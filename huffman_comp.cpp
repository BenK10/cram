#include "huffman_comp.h"
#include "io.h"
#include <set>
#include <algorithm>

huffnode::huffnode(char data, int freq, huffnode* left, huffnode* right) : data{data}, freq{freq}, left{left}, right{right}
{}

std::vector<int> get_histogram(std::vector<char> bytes)
{
    std::vector<int> histogram(256, 0);

    for(auto byte : bytes)
        ++histogram[(int)byte];

    return histogram;
}

void build_codebook(huffnode* root, std::unordered_map<char, std::vector<bool>>& codebook)
{
    std::vector<bool> code;
    get_code(root, root, codebook, code);
}

//traverse tree to get codebook
void get_code(huffnode* node, huffnode* root, std::unordered_map<char, std::vector<bool>>& codebook, std::vector<bool> code, bool bit)
{
    static std::set<huffnode*> visited;
    if(node==nullptr) return;

    if(node->left==nullptr && node->right==nullptr)
    {
        if(node!=root) code.push_back(bit);
        codebook.insert(make_pair(node->data, code));
        return;
    }

    if(node->right != nullptr)
    {
        if(node!=root && visited.find(node)==visited.end()) code.push_back(bit);
        visited.insert(node);
        get_code(node->right, root, codebook, code, 1);
    }

    if(node->left != nullptr)
    {
        if(node!=root && visited.find(node)==visited.end()) code.push_back(bit);
        visited.insert(node);
        get_code(node->left, root, codebook, code, 0);
    }

}

std::vector<bool> encode_tree(huffnode* node)
{
    std::vector<bool> encoded_tree;
     _encode_tree(node, encoded_tree);
     return encoded_tree;
}

void _encode_tree(huffnode* node, std::vector<bool>& encoded_tree)  //BUGGED
{
    static std::set<huffnode*> visited;

    if(node==nullptr) return;
    if(visited.find(node)!=visited.end()) return;

    if(node->left==nullptr && node->right==nullptr)
    {
        encoded_tree.push_back(1);
        std::vector<bool> char_as_bools = char_to_bools(node->data); //push char into encoded_tree
        for(bool b : char_as_bools)
            encoded_tree.push_back(b);

        return;
    }

    else
    {
        visited.insert(node);
        encoded_tree.push_back(0);
    }

    _encode_tree(node->left, encoded_tree);
    _encode_tree(node->right, encoded_tree);
}

std::vector<char> build_header(int final_bits, int final_tree_bits, std::vector<char> encoded_tree)
{
    //final_bits is # of meaningful bits in last byte of compressed data, 1-7
    //final_tree_bits # of bits in encoded huffman tree (not just in its last byte)
    std::vector<char> header = {0,0};

    header[0]=(char)final_bits;
    header[1]=(char)final_tree_bits;

    //copy encoded tree into header
    for(char c : encoded_tree)
        header.push_back(c);

    return header;
}
