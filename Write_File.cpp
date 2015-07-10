#include "Write_File.h"
#include <iostream>
#include <bits/stdc++.h>
#include <fstream>
#include <string>
#include <stdio.h>
#include <regex>

using namespace std;

Write_File::Write_File()
{

}

bool Write_File::open_file()
{
    writter.open("listfile.txt",ofstream::out);
}

bool Write_File::open_sym()
{
    sym_writter.open("symfile.txt",ofstream::out);
}

void Write_File::write_list(string line,string warning,int address)
{

        string zeros = "0000";
        stringstream stream;
        stream << hex << address;
        string hex_value( stream.str() );
        if(warning == "")
        {
            int i ;
            for(i=0 ; i<zeros.length()-hex_value.length() ; i++)
            {
                writter << zeros[i] ;
            }
            writter << hex_value;
            writter << "   ";
            writter << line;
            writter << "\n";
        }

        else if(warning=="comment")
        {
            writter << hex_value;
            writter << "   ";
            writter << line ;
            writter << "\n";
        }

        else
        {
            int i ;
            for(i=0 ; i<zeros.length()-hex_value.length() ; i++)
            {
                writter << zeros[i] ;
            }
            writter << hex_value;
            writter << "   ";
            writter << line;
            writter << "\n";
            writter << "    ****";
            writter << warning;
            writter << "****    \n";
        }

}

void Write_File::write_sym(string label,int address)
{

        string zeros = "0000";
        stringstream stream;
        stream << hex << address;
        string hex_value( stream.str() );

        int i ;
        for(i=0 ; i<zeros.length()-hex_value.length() ; i++)
        {
            sym_writter << zeros[i] ;
        }
        sym_writter << hex_value;
        sym_writter << "   ";
        sym_writter << label;
        sym_writter << "\n";

}

void Write_File::close_file()
{
    writter.close();
}

void Write_File::close_sym()
{
    sym_writter.close();
}
