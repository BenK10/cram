#include "huffman_comp.h"
#include <set>

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
;       get_code(node->left, root, codebook, code, 0);
    }

}

char pack_bools(std::vector<bool>& bits, int index)
{
    char result = (char)0;
    char mask = (char)0;
    for(int i=index; i<bits.size(); ++i)
    {
        if(i%8==0) break;
        if(bits[i]==true)
            mask=(char)1;
        else mask=(char)0;

        result = result | mask;
        result <<= result;
    }
    return result;
}
