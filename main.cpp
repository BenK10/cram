//Cram, a simple lossless Huffman compression algorithm

#include "huffman_comp.h"
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

   if(argc != 3)
   {
       cout<<"usage: cram.exe file_to_compress compressed_file";
       return -1;
   }

   string filename;
   string compressed_filename;
   filename = argv[1];
   compressed_filename = argv[2];

   vector<char> file_contents;
   try{
    file_contents = get_contents(filename);
   }
   catch(exception& e) {cerr<<e.what();}
   vector<int> histogram = get_histogram(file_contents);

   //queue with lowest frequency having highest priority
   auto comp = [](huffnode n1, huffnode n2){return n1.freq > n2.freq;};
   priority_queue<huffnode, vector<huffnode>, decltype(comp)> q(comp);

   unordered_map<char, vector<bool>> codebook(256);

   //populate queue
   for(int i=0; i<histogram.size(); ++i)
   {
       if(histogram[i]==0) continue;
       q.push(huffnode((char)i, histogram[i], nullptr, nullptr));
   }

    //build huffman tree
    while(q.size() > 1)
    {
        huffnode* n1 = new huffnode(q.top().data, q.top().freq, q.top().left, q.top().right);
        q.pop();
        huffnode* n2 = new huffnode(q.top().data, q.top().freq, q.top().left, q.top().right);
        q.pop();

        q.push(huffnode((char)0, n1->freq + n2->freq, n1, n2));
    }

    //now only the root remains
    huffnode root = q.top();

    //build the codebook - traverse huff tree
    build_codebook(&root, codebook);

    //encode
    vector<bool> compressed;
    compressed.reserve(file_contents.size()*4);
    for(char c : file_contents)
    {
        vector<bool> code = codebook.find(c)->second;
        for(auto bit : code)
            compressed.push_back(bit);
    }

    //pack bools into chars
    vector<char> output;
    for(int i=0; i<compressed.size(); i+=8)
        output.push_back(pack_bools(compressed, i));

    //write out
    ofstream out(compressed_filename, ios::out | ios::binary);
    out.write(&output[0],output.size());

   //decompress.

    return 0;
}
