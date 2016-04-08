//Cram, a simple lossless Huffman compression algorithm

#include "huffnode.h"
#include "huffman_comp.h"
#include "huffman_decomp.h"
#include "io.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <exception>

using namespace std;

int main(int argc, char* argv[])
{

   string filename;
   string out_filename;
   vector<char> file_contents;

   if(argc != 4 || (string(argv[3]) != "-c" && string(argv[3]) != "-d"))
   {
       cout<<"usage: cram.exe file_to_compress compressed_file (-c)compress/(-d)ecompress";
       return -1;
   }

   if(string(argv[3]) == "-c") //compress
   {
   filename = argv[1];
   out_filename = argv[2];

   try{
    file_contents = get_contents(filename);
   }
   catch(exception& e) {cerr<<e.what();}

   vector<int> histogram = get_histogram(file_contents);
   unordered_map<char, vector<bool>> codebook(256);
   vector<bool> compressed;
   vector<char> output;
   vector<char> compressed_tree;
   vector<bool> encoded_tree;

   //queue with lowest frequency having highest priority
   auto comp = [](huffnode n1, huffnode n2){return n1.freq > n2.freq;};
   priority_queue<huffnode, vector<huffnode>, decltype(comp)> q(comp);


   //populate queue
   for(int i=0; i<histogram.size(); ++i)
   {
       if(histogram[i]==0) continue;
       q.push(huffnode((char)i, histogram[i], nullptr, nullptr));
   }
    int num_nodes = q.size();

    //build huffman tree
    while(q.size() > 1)
    {
        huffnode* n1 = new huffnode(q.top().data, q.top().freq, q.top().left, q.top().right);
        q.pop();
        huffnode* n2 = new huffnode(q.top().data, q.top().freq, q.top().left, q.top().right);
        q.pop();

        q.push(huffnode((char)0, n1->freq + n2->freq, n1, n2));
        ++num_nodes;
    }

    //now only the root remains
    huffnode root = q.top();

    //build the codebook - traverse huff tree
    build_codebook(&root, codebook);

    //encode
    compressed.reserve(file_contents.size()*4);
    for(char c : file_contents)
    {
        vector<bool> code = codebook.find(c)->second;
        for(auto bit : code)
            compressed.push_back(bit);
    }

    //prepend header to compressed data
    //header has Huffman tree and number of valid bits in final byte
    int final_bits = compressed.size() % 8;
    int final_tree_bits = num_nodes + 8*codebook.size();
    encoded_tree = encode_tree(&root);

    //pack tree
    for(int i=0; i<encoded_tree.size(); i+=8)
        compressed_tree.push_back(pack_bools(encoded_tree, i));

    output = build_header(final_bits, final_tree_bits, compressed_tree);

    //pack data & append to output
    for(int i=0; i<compressed.size(); i+=8)
        output.push_back(pack_bools(compressed, i));

    //write out
    ofstream out(out_filename, ios::out | ios::binary);
    out.write(&output[0],output.size());

   }

   if(string(argv[3])=="-d") //decompress
   {
   filename = argv[1];
   out_filename = argv[2];

   try{
    file_contents = get_contents(filename);
   }
   catch(exception& e) {cerr<<e.what();}

   int last_bits = (int)file_contents[0]; // # of valid bits in last message byte
   int tree_bits = (int)file_contents[1]; // # of bits in tree
   int msg_idx = 3 + tree_bits/8; //3 instead of 2 bc truncates
   vector<bool> encoded_msg = extract_msg(file_contents, msg_idx, last_bits);


   //build huffman tree of original message
   vector<bool> encoded_hufftree = extract_tree(file_contents, tree_bits);

   vector<huffnode*> hufftree;
   huffnode* root;
   for(int i=0; i<encoded_hufftree.size(); ++i)
   {
       if(encoded_hufftree[i]==0)
       {
           bool ok=false;
           huffnode* node = new huffnode(char(0), 0, nullptr, nullptr); //don't care about arg2 (freq)

           while(!ok)
           {
            int last_idx = (hufftree.size()==0) ? -1 : hufftree.size()-1;
            if(last_idx!= -1)
            {
            huffnode* last_node = hufftree[last_idx];

           if(last_node->left==nullptr)
            {last_node->left=node; ok=true;}
           else if(last_node->left!=nullptr && last_node->right==nullptr)
            {last_node->right=node; ok=true;}
           else
               hufftree.pop_back();
           }
            if(hufftree.size()==0) break; //edge case
           }
            if(i==0) root = node;
           hufftree.push_back(node);
       }

       if(encoded_hufftree[i]==1) //TODO DRY
       {

           bool ok=false;
           char c = pack_bools(encoded_hufftree, i+1);
           huffnode* node = new huffnode(c, 0, nullptr, nullptr);

           while(!ok)
           {
            int last_idx = (hufftree.size()==0) ? -1 : hufftree.size()-1;
             if(last_idx!= -1)
            {
            huffnode* last_node = hufftree[last_idx];

           if(last_node->left==nullptr)
            {last_node->left=node; ok=true;}
           else if(last_node->left!=nullptr && last_node->right==nullptr)
            {last_node->right=node; ok=true;}
           else
               hufftree.pop_back();
           }
        //   if(hufftree.size()==0) break; //edge case
           }
           if(i==0) root = node;
           i+=8;
       }
   }

   vector<char> decoded_msg = decode(encoded_msg, root);

   ofstream out(out_filename, ios::out | ios::binary);
   out.write(&decoded_msg[0],decoded_msg.size());
   }


    return 0;
}
