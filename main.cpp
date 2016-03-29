//Cram, a simple lossless Huffman compression algorithm

#include "huffman_comp.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_map>

using namespace std;

//"insane_coder"'s file read, benchmarked to be the fastest C++ way
//see http://insanecoding.blogspot.it/2011/11/how-to-read-in-file-in-c.html
vector<char> get_contents(string filename)
{
    ifstream in(filename, ios::in | ios::binary);
    if(in)
    {
        vector<char> contents;
        in.seekg(0, ios::end);
        contents.resize(in.tellg());
        in.seekg(0, ios::beg);
        in.read(&contents[0], contents.size());

        return contents;
    }
    throw(errno);
}

int main()
{
    const string test_text = "example.txt";

   vector<char> v1 = get_contents(test_text);
   vector<int> histogram = get_histogram(v1);

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
    compressed.reserve(v1.size()*4);
    for(char c : v1)
    {
        vector<bool> code = codebook.find(c)->second;
        for(auto bit : code)
            compressed.push_back(bit);
    }

    //test
    for(auto bit : compressed)
        cout<<bit;

    //pack bools into chars
    vector<char> output;
    for(int i=0; i<compressed.size(); i+=8)
        output.push_back(pack_bools(compressed, i));

    //number of valid bits in final code byte is 8-(8*output.size - compressed.size)

    //write out
    //TODO buffer it
   // ofstream out("out.crm", ios::out | ios::binary);
  //  for(auto itr = output.begin(); itr!=output.end(); ++itr)
   //     out.write(&(*itr),1);

   //decompress.
   //TODO place in separate file, embed huffman code/tree in compressed file, embed number of valid bits in final byte

    return 0;
}
