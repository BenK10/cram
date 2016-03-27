//Cram, a simple lossless compression algorithm

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_map>



using namespace std;

//node class. Immutable but move-assignable and move-constructible. The latter two are required for make_heap used by queue
//they become necessary because of the const members
class HuffTreeNode
{
    public:
    const int freq{0};
    const HuffTreeNode* left{nullptr};
    const HuffTreeNode* right{nullptr};

    HuffTreeNode(int freq, HuffTreeNode* left, HuffTreeNode* right) : freq{freq}, left{left}, right{right}
    {}

    HuffTreeNode& operator=(const HuffTreeNode&)
    {
        //heap wants the items to be assignable
    }
};

class HuffTreeLeafNode : public HuffTreeNode
{
    public:
    const char data=0x00;

    HuffTreeLeafNode(char data, int freq, HuffTreeNode* left, HuffTreeNode* right)
        : HuffTreeNode(freq, left, right), data(data)
    {}
};

//actually, the class is overkill. The advantage of the struct is that I don't need to define
//move-assign and copy construct to make it work with the queue
struct huffnode
{
    char data;
    int freq;
    huffnode* left;
    huffnode* right;

    huffnode(char data, int freq, huffnode* left, huffnode* right) : data{data}, freq{freq}, left{left}, right{right}
    {}
};

//helper
void get_code(huffnode* node, unordered_map<char, vector<bool>>& codebook, vector<bool> code)
{
    //cout<<"entering getcode"<<endl; //test
    if(node==nullptr) return;
    cout<<node->data<<" "<<node->freq<<" "<<endl;//test

    if(node->left==nullptr && node->right==nullptr)
        {codebook.insert(make_pair(node->data, code)); return;}

    if(node->left != nullptr)
    {
        code.push_back(0);
        get_code(node->left, codebook, code);
        cout<<"going left"<<endl;//test
    }


    if(node->right != nullptr)
    {
        code.push_back(1);
        get_code(node->right, codebook, code);
        cout<<"going right"<<endl;//test
    }

}

void build_codebook(huffnode* root, unordered_map<char, vector<bool>>& codebook)
{
    vector<bool> code;
    get_code(root, codebook, code);
}

vector<int> get_histogram(vector<char> bytes)
{
    vector<int> histogram(256, 0);

    for(auto byte : bytes)
        ++histogram[(int)byte];

    return histogram;
}

//insane_coder's file read, benchmarked to be the fastest C++ way
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

//the idiomatic but slower way:
/*
vector<char> container( (std::istreambuf_iterator<char>( in )),
                         (std::istreambuf_iterator<char>()) );
See James Kanze's post about the nuances of this:
http://stackoverflow.com/questions/7796857/streaming-into-vector

the above functions don't work using vector<bool>. So read as chars (bytes)
and use vector<bool> to represent the bitstrings for Huffman table.
*/

int main()
{
    const string test_text = "example.txt";
  //  const string test_png = "cpp.png";

    //test
   vector<char> v1 = get_contents(test_text);
  // vector<char> v2 = get_contents(test_png);
  vector<int> histogram = get_histogram(v1);

   //queue with lowest frequency having highest priority
   auto comp = [](huffnode n1, huffnode n2){return n1.freq > n2.freq;};
   priority_queue<huffnode, vector<huffnode>, decltype(comp)> q(comp);

   unordered_map<char, vector<bool>> codebook(256); //vector<bool> instead of bitset bc bitset is fixed length

   //populate queue
   for(int i=0; i<histogram.size(); ++i)
   {
       if(histogram[i]==0) continue;
       q.push(huffnode((char)i, histogram[i], nullptr, nullptr));
   }

   for(int val : histogram)
    cout<<val<<" ";

    cout<<"q populated"<<endl; //test

    //build huffman tree
    while(q.size() > 1)
    {
        huffnode n1 = q.top();
        q.pop();
        huffnode n2 = q.top();
        q.pop();

        q.push(huffnode((char)0, n1.freq+n2.freq, &n1, &n2));

        cout<<"node pushed"<<endl;//test
    }

    cout<<"tree built"<<endl; //test

    //now only the root remains
    huffnode root = q.top();

    //build the codebook - traverse huff tree
    build_codebook(&root, codebook);

    cout<<"codebook built"<<endl; //test;

    for(const auto entry : codebook)
    {
        cout<<entry.first<<" ";
        for(auto itr = entry.second.begin(); itr!= entry.second.end(); ++itr)
            cout<<*itr;
        cout<<"";
        cout<<entry.second.size()<<endl;
    }

    return 0;
}
