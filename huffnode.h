#ifndef HUFFNODE_H_INCLUDED
#define HUFFNODE_H_INCLUDED

struct huffnode
{
    char data;
    int freq;
    huffnode* left;
    huffnode* right;

    huffnode(char, int, huffnode*, huffnode*);
};

#endif // HUFFNODE_H_INCLUDED
