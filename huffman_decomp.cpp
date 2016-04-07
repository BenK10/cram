#include <cmath>
#include <vector>
#include "io.h"
#include "huffnode.h"

//extract tree
std::vector<bool> extract_tree(std::vector<char>& contents, int tree_bits)
{
    std::vector<bool> tree;
    int bytes = ceil(tree_bits/8);
    for(int i=0; i<bytes; ++i)
    {
        std::vector<bool> bits = char_to_bools(contents[i]);
        for(bool b : bits)
            tree.push_back(b);
    }

    //throw away extra bits
    while(tree.size() > tree_bits)
        tree.pop_back();

    return tree;
}

//extract encoded data
std::vector<bool> extract_msg(std::vector<char>& contents, int msg_idx, int msg_final_bits)
{
    std::vector<bool> msg;
    for(int i=msg_idx; i<contents.size(); ++i)
    {
        std::vector<bool> bits = char_to_bools(contents[i]);
        for(bool b : bits)
            msg.push_back(b);
    }

    //throw away extra bits
    while(msg.size() > ( (contents.size()-msg_idx)*8 - (8-msg_final_bits) ))
        msg.pop_back();

    return msg;
}

//use tree to decode message
std::vector<char> decode(std::vector<bool>& msg, huffnode* tree)
{
    std::vector<char> decoded_msg;
    huffnode* root = tree;
    for(int i=0; i<msg.size(); ++i)
    {
        tree = root; //reset

        if(msg[i]==0)
            tree=tree->left;
        if(msg[i]==1)
            tree=tree->right;
        if(tree!=nullptr && tree->data!=0)
            {decoded_msg.push_back(tree->data); i+=8;}
    }
    return decoded_msg;
}
