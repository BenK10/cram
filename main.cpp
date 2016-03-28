//Cram, a simple lossless compression algorithm

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <list>
#include <set>



using namespace std;

struct huffnode
{
    char data;
    int freq;
    huffnode* left;
    huffnode* right;

    huffnode(char data, int freq, huffnode* left, huffnode* right) : data{data}, freq{freq}, left{left}, right{right}
    {}
};

//traverse tree to get codebook
void get_code(huffnode* node, huffnode* root, unordered_map<char, vector<bool>>& codebook, vector<bool> code, bool bit=0)
{
    static set<huffnode*> visited;
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

void build_codebook(huffnode* root, unordered_map<char, vector<bool>>& codebook)
{
    vector<bool> code;
    get_code(root, root, codebook, code);
}

vector<int> get_histogram(vector<char> bytes)
{
    vector<int> histogram(256, 0);

    for(auto byte : bytes)
        ++histogram[(int)byte];

    return histogram;
}

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

char pack_bools(vector<bool>& bits, int index)
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

int main()
{
    const string test_text = "example.txt";

   vector<char> v1 = get_contents(test_text);
   vector<int> histogram = get_histogram(v1);
   list<huffnode> nodes;

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

    cout<<"compressed codes bytes "<<compressed.size()<<endl;//test
    cout<<"packed bytes "<<output.size();

    //write out
   // ofstream out("out.crm", ios::out | ios::binary);
  //  for(auto itr = output.begin(); itr!=output.end(); ++itr)
   //     out.write(&(*itr),1);

   //decompress.
   //TODO place in separate file, embed huffman code/tree in compressed file, embed number of valid bits in final byte

    return 0;
}
