#include "pass2.h"
#include <iostream>
#include <bits/stdc++.h>
#include <fstream>
#include <string>
#include <unordered_map>

pass2::pass2(unordered_map <string, Node> sym_table,unordered_map <string, int> format_table)
{
    pass2::formatTable = format_table ;
    pass2::symTable = sym_table ;
}
