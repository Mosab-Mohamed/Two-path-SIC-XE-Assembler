#include <iostream>
#include <bits/stdc++.h>
#include "Read_File.h"
#include <regex>

using namespace std;

bool validate_choice(string);
bool is_number(string);


int main()
{

    int choice = 1 ;

    while(choice == 1 ){
        while (true) {
            string input = "" ;
            cout << "Please enter a valid choice : " << endl ;
            cout << "1) enter the source file name." << endl;
            getline(cin, input);
            stringstream myStream(input);
            if (myStream >> choice)
                break;
            cout << "Invalid number, please try again !!" << endl;
        }

        if(choice==1){
            string file_name ;
            getline(cin, file_name);
            char * str = new char[file_name.size() + 1];
            std::copy(file_name.begin(), file_name.end(), str);
            str[file_name.size()] = '\0';
            char * pch;
            pch = strtok (str,"<>");
            while (pch != NULL)
            {
                pch = strtok (NULL, "<>");
                if(pch != NULL)
                {
                    file_name = pch ;
                }

            }

            Read_File reader (file_name);
            if(reader.open_file())
            {
                reader.scan_file();
            }

            else
            {
                cout << "can't open this file " << endl ;
            }

        }

    }

    return 0;
}

bool is_number(string operand)
{
    int i ;
    for(i=0 ; i<operand.length() ; i++)
    {

        if ( operand[i]>'9' || operand[i]<'0' )
        {
            cout << operand[i];
            return false ;
        }
        else
            cout << operand[i] ;
    }
    cout << "\n" ;
    return true ;
}

bool validate_choice(string choice){

    if(choice=="1" || choice=="2" || choice=="3" || choice=="4")
        return true ;

    return false ;
}
