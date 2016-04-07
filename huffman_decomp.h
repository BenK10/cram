#ifndef HUFFMAN_DECOMP_H_INCLUDED
#define HUFFMAN_DECOMP_H_INCLUDED



//extract tree
std::vector<bool> extract_tree(std::vector<char>& contents, int tree_bits);

//extract encoded data
std::vector<bool> extract_msg(std::vector<char>& contents, int msg_idx, int msg_final_bits);

//use tree to decode message
std::vector<char> decode(std::vector<bool>& msg, huffnode* tree);


#endif // HUFFMAN_DECOMP_H_INCLUDED
