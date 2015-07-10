#ifndef PASS2_H
#define PASS2_H
#include <iostream>
#include <bits/stdc++.h>
#include <fstream>
#include <string>
#include <unordered_map>

using namespace std;

class pass2
{
    private:
        class Node
        {
            public:
                string address;
                bool flag;
        };
        unordered_map <string, Node> symTable;
        unordered_map <string, int> formatTable;
    public:
        pass2(unordered_map <string, Node> , unordered_map <string, int> );
    protected:


};

#endif // PASS2_H
