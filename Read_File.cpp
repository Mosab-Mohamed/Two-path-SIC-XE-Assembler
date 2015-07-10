#include "Read_File.h"
#include <iostream>
#include <bits/stdc++.h>
#include "Write_File.h"
#include <fstream>
#include <string>
#include <ctype.h>
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>
#include <regex>
#include <stdlib.h>

using namespace std;

Read_File::Read_File(string file_name)
{
    source_file_name = file_name ;
}

bool Read_File::open_file()
{
    reader.open(source_file_name.c_str());
    if (reader.is_open())
    {
        return true ;
    }

    else
    {
        return false ;
    }
}

void Read_File::scan_file()
{
    set_FormatHash();
    string line ;
    int LOCCTR = 0;
    Write_File writter ;
    writter.open_file();
    writter.open_sym();
    int first_formate ;
    string start_adress ;
    int prevoiusORG = -1 ;

    while ( getline (reader,line) )
    {
        int operation_format ;
        std::transform(line.begin(), line.end(), line.begin(), ::tolower);
        string warning = validate(line);
        int current_address = 0;

        if(warning == "")
        {
            vector<string> statement = paerser(line);
            string label = statement[0];
            string OPCode = statement[1];
            string operand = statement[2];
            label.erase(find_if(label.rbegin(), label.rend(), not1(ptr_fun<int, int>(isspace))).base(), label.end());
            OPCode.erase(find_if(OPCode.rbegin(), OPCode.rend(), not1(ptr_fun<int, int>(isspace))).base(), OPCode.end());
            operand.erase(find_if(operand.rbegin(), operand.rend(), not1(ptr_fun<int, int>(isspace))).base(), operand.end());

            current_address = LOCCTR ;

            if(OPCode==("start"))
            {
                if(operand=="")
                {
                    LOCCTR = 0 ;
                    current_address = LOCCTR ;
                }

                else if(is_hexa_number(operand))
                {
                    stringstream ss;
                    ss << hex << operand;
                    ss >> LOCCTR ;
                    current_address = LOCCTR ;
                }

                else
                {
                    warning = "number format error in the operand field";
                }


                if(label!="")
                {
                    make_symTable(label, operand);
                    writter.write_sym(label, LOCCTR);
                }



            }

            else if(OPCode != "end")
            {
                if(label != "")
                {
                    stringstream ss;
                    ss << LOCCTR;
                    string address = ss.str() ;
                    make_symTable(label, address);
                    writter.write_sym(label, LOCCTR);
                }

                operation_format = format(OPCode);

                if(operation_format != -1)
                {
                    if(operation_format==2)
                    {
                        if(OPCode=="clear" || OPCode=="tixr")
                        {
                            if(! is_register(operand))
                                warning = "bad format error in the operand field";
                        }

                        else if(OPCode=="shiftl" || OPCode=="shiftr")
                        {
                            if(! (operand.substr(1,1)=="," && is_register(operand.substr(0,1)) && is_number( operand.substr(2,operand.length()-2) ) ) )
                                    warning = "bad format error in the operand field";
                        }

                        else
                        {
                            if(operand.length()==3)
                            {
                                if(! (operand.substr(1,1)=="," && is_register(operand.substr(0,1)) && is_register(operand.substr(2,1)) ) )
                                    warning = "bad format error in the operand field";
                            }

                            else
                            {
                                warning = "bad format error in the operand field";
                            }
                        }
                    }
                     LOCCTR += operation_format ;
                }


                else if(OPCode == "word")
                {
                    if(! is_number(operand) )
                        warning = "number format error";

                    else
                    {
                        LOCCTR += 3 ;
                    }

                }

                else if(OPCode == "resw")
                {
                    if(! is_number(operand) )
                        warning = "number format error";

                    else if(operand == "")
                        warning = "error operand left blank";

                    else
                    {
                        LOCCTR += 3* atoi(operand.c_str()) ;
                    }

                }

                else if(OPCode == "resb")
                {
                    if(! is_number(operand) )
                        warning = "number format error";

                    else if(operand == "")
                        warning = "error operand left blank";

                    else
                    {
                        LOCCTR += atoi(operand.c_str()) ;
                    }

                }

                else if(OPCode == "byte")
                {
                    int length = operand.length() ;

                    if( operand[0]=='c' && operand[1]==39 && operand[length-1]==39 )
                    {
                        LOCCTR += length - 3 ;
                    }

                    else if( operand[0]=='x' && operand[1]==39 && operand[length-1]==39 )
                    {
                        if( (length-3)%2 == 0 )
                        {
                            if( is_hexa_number(operand.substr(2,length-3)) )
                            {
                                LOCCTR += (length-3)/2 ;
                            }

                            else
                                warning = "hexa format error in the operand field";
                        }


                        else
                            warning = "error the number of hexadecimal digits must be even";
                    }

                    else
                        warning = "bad format error in the operand field";
                }

                else if(OPCode=="equ")
                {
                    if(label=="")
                        warning = "label field left blank";

                    else
                    {
                        stringstream myStream ;
                        string address = "";
                        if(operand=="" || operand=="*")
                        {
                            myStream << LOCCTR ;
                            myStream >> address ;
                            make_symTable(label,address);
                        }

                        else
                        {
                            if(is_hexa_number(operand))
                            {
                                char* pEnd;
                                long int addr = strtol(operand.c_str(),&pEnd,16);
                                myStream << addr ;
                                myStream >> address ;
                                make_symTable(label,address);
                                writter.write_list(line,warning,-1);
                                continue;
                            }

                            else if(is_in_SYMTAB(operand)>=0)
                            {
                                int addr = is_in_SYMTAB(operand);
                                myStream << addr ;
                                myStream >> address ;
                                make_symTable(label,address);
                                writter.write_list(line,warning,-1);
                                continue;
                            }

                            else
                            {
                                int result = is_expression(operand);
                                if(is_expression(operand)>=0)
                                {
                                    myStream << result ;
                                    myStream >> address ;
                                    make_symTable(label,address);
                                    writter.write_list(line,warning,-1);
                                    continue;
                                }

                                else
                                {
                                    warning = "bad format error in the operand field";
                                }
                            }
                        }
                    }
                }

                else if(OPCode=="org")
                {
                    if(label=="")
                    {
                        if(operand=="")
                        {
                                LOCCTR = prevoiusORG ;
                            }
                        }

                        else
                        {
                            if(is_number(operand))
                            {
                                prevoiusORG = LOCCTR ;
                                LOCCTR = atoi(operand.c_str());
                                writter.write_list(line,warning,-1);
                                continue;
                            }

                            else if(is_in_SYMTAB(operand)>=0)
                            {
                                LOCCTR = is_in_SYMTAB(operand);
                                writter.write_list(line,warning,-1);
                                continue;
                            }

                            else
                            {
                                int addr = is_expression(operand);
                                if(addr>=0)
                                {
                                    prevoiusORG = LOCCTR ;
                                    LOCCTR = addr ;
                                    writter.write_list(line,warning,-1);
                                    continue;
                                }

                                else
                                {
                                    warning = "bad format error in the operand field";
                                }
                            }
                        }
                    }

                    else
                        warning = "bad format error in the label field";
                }

                else
                    warning = "invalid operation code !!" ;



            }



        }

        writter.write_list(line,warning,current_address);


    }
    reader.close();
    writter.close_file();
    writter.close_sym();
}


int Read_File::format(string label)
{
    string labell ;
    labell = label.substr(1,label.length());


    if(label[0] == '+'){
        if (Format.find(labell) != Format.end() && Format[labell] == 3)
            return 4;
         else
            return -1;
    }
    else if (label[0] != '+'){
        if (Format.find(label) != Format.end() && (Format[label]==3 || Format[label]==2) )
            return Format[label];

        else
            return -1 ;
    }


    else
        return -1;
}

vector<string> Read_File::paerser(string line)
{
    vector<string> parser(3) ;

    parser.insert(parser.begin(),line.substr (0,8).c_str());
    if(line.length()<17)
        parser.insert(parser.begin()+1,line.substr (9,line.length()-9));
    else
    {
         parser.insert(parser.begin()+1,line.substr (9,6) );
         if(line.length() < 36)
            parser.insert(parser.begin()+2,line.substr (17,line.length()-17));
        else
            parser.insert(parser.begin()+2,line.substr (17,18));
    }


    return parser ;
}

void Read_File::make_symTable(string label, string address)
{
    if(symTable.find(label) != symTable.end()){
       symTable[label].flag = true;
    }

    else
    {
        Node node;
        node.address = address;
        node.flag = false;
        symTable.emplace(label, node);
    }
}


/*string Read_File::validate(string line)
{
    if(line[0]=='.')
        return "comment";

    if(line.length()<17)
        return "error in the format of the line";

    string label = line.substr (0,8);
    string blank = line.substr(8,1);
    blank += line.substr(15,2);
    string opcode = line.substr (9,6);
    string operand = line.substr (17,line.length()-7);

    label.erase(find_if(label.rbegin(), label.rend(), not1(ptr_fun<int, int>(isspace))).base(), label.end());
    opcode.erase(find_if(opcode.rbegin(), opcode.rend(), not1(ptr_fun<int, int>(isspace))).base(), opcode.end());
    operand.erase(find_if(operand.rbegin(), operand.rend(), not1(ptr_fun<int, int>(isspace))).base(), operand.end());

 //   if(blank != "   ")
   //S     return "error in the format of the line";

    string label_warning = validate_label(label);
    if(label_warning=="")
    {
        return validate_operand(operand,opcode);
    }
    else
    {
        return label_warning ;
    }

}*/

string Read_File::validate_label(string label)
{
    if(label=="")
        return "";
    if(label[0]>='0' && label[0]<='9')
        return "label format error";

    if(! (is_right_symbol(label)) )
        return "label format error";

    return "";
}

string Read_File::validate_operand(string operand,string opcode)
{
    if(operand[0]=='#')
    {
        if(! (is_number(operand.substr(1,operand.length()-1)) || is_right_symbol( operand.substr(1,operand.length()-1) )) )
            return "error in the format of the operand field";
    }

    else if(opcode=="rsub")
    {
        if(operand!="")
            return "error in the format of the operand field";
    }

    else if(operand[0]=='@')
    {
        if(! ( is_right_symbol( operand.substr(1,operand.length()-1) )) )
        {
            return "error in the format of the operand field";
        }
    }

    else if(format(opcode)==3 || format(opcode)==4)
    {
        if(is_number(operand))
            return "error in the format of the operand field";

        else if(! (is_right_symbol(operand) || is_right_indexed(operand))  )
        {
            return "error in the format of the operand field";
        }
    }

    return "" ;

}

bool Read_File::is_right_symbol(string s)
{
    int i ;
    for(int i=0 ;i<s.length() ; i++)
    {
        if ( (s[i]>'9' || s[i]<'0') && (s[i]>'z' || s[i]<'a') )
            return false ;
    }
    return true ;
}

int Read_File::is_expression(string s)
{
    string oper1 = "" , oper2 = "" ;
    int i , stringLength , stringStart=0 , addr = -1;
    for(int i=0 ;i<s.length() ; i++)
    {
        if(s[i]=='+')
        {
            stringLength = i - stringStart ;
            oper1 = s.substr(stringStart , stringLength);
            if(i==s.length()-1)
                return -1;

            oper2 = s.substr(i+1 , s.length()-i-1);

            if( (!is_number(oper1)) && (!is_number(oper2)) )
                return -1 ;

            else
            {
                if(is_number(oper1))
                {
                    addr = is_in_SYMTAB(oper2);
                    if(addr>=0)
                    {
                        return atoi(oper1.c_str()) + addr ;
                    }
                }

                else if(is_number(oper2))
                {
                    addr = is_in_SYMTAB(oper1);
                    if(addr>=0)
                    {
                        return atoi(oper2.c_str()) + addr ;
                    }
                }
            }

            return -1;
        }

        else if(s[i]=='-')
        {
            stringLength = i - stringStart ;
            oper1 = s.substr(stringStart , stringLength);
            if(i==s.length()-1)
                return -1;

            oper2 = s.substr(i+1 , s.length()-i-1);

            if(is_number(oper1))
            {
                addr = is_in_SYMTAB(oper2);
                if(addr<0)
                {
                    if(is_number(oper2))
                    {
                        return atoi(oper1.c_str()) - atoi(oper2.c_str()) ;
                    }
                }
            }

            else if(is_number(oper2))
            {
                addr = is_in_SYMTAB(oper1);
                if(addr>=0)
                {
                    return addr - atoi(oper2.c_str());
                }
                if(is_number(oper1))
                {
                    return atoi(oper1.c_str()) - atoi(oper2.c_str()) ;
                }
            }

            return -1;
        }

        else if(s[i]=='*')
        {
            stringLength = i - stringStart ;
            oper1 = s.substr(stringStart , stringLength);
            if(i==s.length()-1)
                return -1;

            oper2 = s.substr(i+1 , s.length()-i-1);

            if(is_number(oper1) && is_number(oper2))
                return atoi(oper1.c_str()) * atoi(oper2.c_str()) ;

            else
                return -1;
        }

        else if(s[i]=='/')
        {
            stringLength = i - stringStart ;
            oper1 = s.substr(stringStart , stringLength);
            if(i==s.length()-1)
                return -1;

            oper2 = s.substr(i+1 , s.length()-i-1);

            if(is_number(oper1) && is_number(oper2))
            {
                int result = atoi(oper1.c_str()) / atoi(oper2.c_str()) ;
                if(result==0)
                    return -1;
                else
                    return result ;
            }
            else
                return -1;
        }
    }
    return -1 ;
}

int evaluate_expression(string expression)
{

}

int Read_File::is_in_SYMTAB(string operand)
{
    stringstream myStream ;
    int addr = -1 ;
    for( unordered_map<string,Node>::const_iterator it = symTable.begin(); it != symTable.end(); ++it )
    {
        if(operand == (it->first) )
        {
            myStream << (it->second).address ;
            myStream >> addr ;
            return addr ;
        }
    }
    return -1 ;
}

bool Read_File::is_right_indexed(string s)
{
    int i ;
    int comma_number =0 ;
    for(int i=0 ;i<s.length() ; i++)
    {
        if(s[i]==',' && i!=s.length()-1)
        {
            if( ! ( is_register(s.substr(i+1,s.length()-i-1)) ) )
            {
                return false ;
            }
        }

        else if ( (s[i]>'9' || s[i]<'0') && (s[i]>'z' || s[i]<'a') )
            return false ;
    }
    return true ;
}

bool Read_File::is_register(string label)
{
    return label=="a" || label=="b" || label=="l" || label=="s" || label=="x" || label=="f" || label=="t" ;
}

bool Read_File::is_number(string operand)
{
    int i ;
    for(i=0 ; i<operand.length() ; i++)
    {
        if ( operand[i]>'9' || operand[i]<'0' )
            return false ;
    }
    return true ;
}

bool Read_File::is_hexa_number(string operand)
{
    int i ;
    for(i=0 ; i<operand.length() ; i++)
    {
        if ( (operand[i]>'9' || operand[i]<'0') && (operand[i]>'f' || operand[i]<'a') )
            return false ;
    }

    return true ;
}

void Read_File::set_FormatHash()
{
    Format.insert({"add",3});
    Format.insert({"addr",2});
    Format.insert({"clear",2});
    Format.insert({"comp",3});
    Format.insert({"compr",2});
    Format.insert({"div",3});
    Format.insert({"divr",2});
    Format.insert({"add",3});
    Format.insert({"j",3});
    Format.insert({"jeq",3});
    Format.insert({"jgt",3});
    Format.insert({"jlt",3});
    Format.insert({"jsub",3});
    Format.insert({"lda",3});
    Format.insert({"ldb",3});
    Format.insert({"ldch",3});
    Format.insert({"ldl",3});
    Format.insert({"lds",3});
    Format.insert({"ldt",3});
    Format.insert({"ldx",3});
    Format.insert({"mul",3});
    Format.insert({"mulr",2});
    Format.insert({"rd",3});
    Format.insert({"rmo",2});
    Format.insert({"rsub",3});
    Format.insert({"shiftl",2});
    Format.insert({"shiftr",2});
    Format.insert({"sta",3});
    Format.insert({"stb",3});
    Format.insert({"stch",3});
    Format.insert({"stl",3});
    Format.insert({"sts",3});
    Format.insert({"stt",3});
    Format.insert({"stx",3});
    Format.insert({"sub",3});
    Format.insert({"subr",2});
    Format.insert({"td",3});
    Format.insert({"tix",3});
    Format.insert({"tixr",2});
    Format.insert({"wd",3});
}

string Read_File::validate(string input)
{
    cout << "check 1\n";
    string g1 = "-1",g2 = "-1",g3 = "-1",g4 = "-1",x1,x2,x3,check;
    int ch = 0;
    int i = 0;
    cout << "check 2\n";
    regex fourth("([ ]|\\w|[0-9]){0,31}");
        cout << "check 3\n";
        g1 = "-1";
        g2 = "-1";
        g3 = "-1";
        g4 = "\nthere is no comment\n";
        cout << "check 2\n";
        g1 = Read_File1(input);
        if(input.length()>=9){
            cout << "check 3\n";
            x1 = input.substr(9,17);
            g2 = Read_File2(x1);
            cout << "check 4\n";
            check = g2;
            g2.erase(std::remove(g2.begin(), g2.end(), ' '), g2.end());
            if(g2 != "word" && g2 != "byte" && g2 != "resw"&& g2 != "resb")
                ch = format(g2);
            else
                ch = 5;
            g2 = check;
        }
        if(input.length()>=17){
            x2 = input.substr(17,36);
            cout << "check 5\n";
            g3 = Read_File3(x2);
            cout << "check 6\n";
        }
        if(input.length()>=36){
            x3 = input.substr(36,66);
            if(regex_match(x3,fourth)){
                g4 = "\nwith comment\n";
            }
            else{
                g4 = "\nthere is no comment\n";
            }
        }

        if(g1 == "-1")
            return "bad format in the label region";

        if(g1 == "comment")
            return "comment";

         if(g2 == "-1")
                return "bad format in the operation region.";
         if(ch == -1)
            return "there is no a such operation.";
        if(g3 == "-1")
            return "bad format in the operand region.";

        if(g1 != "-1" && g2 != "-1" && g3 != "-1" )
            return "";

}

string Read_File::Read_File1(string s)
{
    string a,b,c,d;
    string answer;
    bool flag = false;
    int i = 0;
    regex t( "^[.]{1}(\\w|\\s|.){0,66}");
    regex first( "^(([a-z]|[A-Z])(\\s|\\w|[0-9]){0,7}([ ]){1})|[ ]{0,9}");

    if(regex_match(s,t)){
        return "comment";
    }

    /************************************************/

    a = s.substr(0, 9);

    i = 0;
    if(regex_match(a,first)){

        while(i < a.length() && a[i] != '\0'){
            if(a[i] == ' '){
                flag = true;
            }
            if(a[i] != ' ' && a[i] != '+' &&flag == true){
                return "-1";
            }
            i++;
        }
    }

    else
        return "-1";

    if(a == "")
        return "-1";

    return a;
}


string Read_File::Read_File2(string s)
{
    string a,b,c,d;
    string answer;
    bool flag = false;
    int i = 0;

    regex first( "([a-z]|[A-Z]|\\+)([ ]|\\w){0,5}([ ]){0,2}");

    a = s.substr(0,8);

    i = 0;
    if(regex_match(a,first)){

        while(i < a.length() && a[i] != '\0'){
            if(a[i] == ' '){
                flag = true;
            }
            if(a[i] != ' ' && a[i] != '+' &&flag == true){
                return "-1";
            }
            i++;
        }
    }

    else
        return "-1";


    return a;
}

string Read_File::Read_File3(string s)
{
    string a,b,c,d;
    string answer;
    bool flag = false;
    int i = 0;

    regex first( "(([a-z]|[A-Z])([ ]|\\w|[0-9]){0,7}([ ]){0,10})|((#|@)([ ]|\\w|[0-9]){0,8}([ ]){0,10})|((\\*){1}[ ]{0,17})|((\\w){1,8}\\,(\\w){1}[ ]{0,15})|((c|C)'((\\s){0,1}(\\w){0,1}){1,15}')|((x|X)'([0-9]{2})')|((x|X)'(([a-f]|[A-F]){1}[0-9]{1})')|((x|X)'(([a-f]|[A-F]){2})')|((x|X)'[0-9]{1}(([a-f]|[A-F]){1})')");

    a = s.substr(0,18);

    i = 0;
    if(regex_match(a,first)){

        while(i < a.length() && a[i] != '\0'){
            if(a[i] == ' '){
                flag = true;
            }
            if(a[i] == '\'' &&(a[i-1] == 'c' ||a[i-1] == 'C')){
                break;
            }
            if(a[i] != ' ' && a[i] != '+' &&flag == true){
                return "-1";
            }
            i++;
        }
        if(a[i+1] == '\'')
            return "-1";
    }

    else
        return "-1";

    return a;
}




class Node
{
    public:
        string address;
        bool flag;
};


