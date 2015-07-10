#ifndef READ_FILE_H
#define READ_FILE_H
#include <iostream>
#include <bits/stdc++.h>
#include "Write_File.h"
#include <fstream>
#include <string>
#include <unordered_map>
#include <regex>

using namespace std;

class Read_File
{
    private:
        string validate(string);
        string validate_label(string);
        string validate_operand(string,string);
        string Read_File3(string);
        string Read_File2(string);
        string Read_File1(string);
        int format(string);
        vector<string> paerser(string);
        void make_symTable(string, string);
        bool is_register(string);
        bool is_number(string);
        bool is_hexa_number(string);
        bool is_right_symbol(string);
        bool is_right_indexed(string);
        int is_expression(string);
        void set_FormatHash();
        int evaluate_expression(string);
        int is_in_SYMTAB(string);
        class Node
        {
            public:
                string address;
                bool flag;
        };

    public:
        Read_File(string);

        bool open_file();
        void scan_file();

        ifstream reader ;
        string source_file_name ;
        unordered_map <string, Node> symTable;
        unordered_map <string, int> Format;




};

#endif // READ_FILE_H
