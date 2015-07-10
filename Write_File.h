#ifndef WRITE_FILE_H
#define WRITE_FILE_H
#include <iostream>
#include <bits/stdc++.h>
#include <fstream>
#include <string>
#include <regex>

using namespace std;


class Write_File
{

    public:
        Write_File();
        void write_list(string,string,int);
        void close_file();
        bool open_file();
        void write_sym(string,int);
        void close_sym();
        bool open_sym();


    protected:
    private:
        ofstream writter;
        ofstream sym_writter;
};

#endif // WRITE_FILE_H
