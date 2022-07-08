#include <iostream>
#include <string>
#include "Json.h"
#include <fstream>

using namespace std;



int main()
{
    json ele;
    Json test;

    char c = (char)30;

    string str = "";

    ifstream ifs;
    ifs.open("test4.txt", ios::in);

    if (!ifs.is_open())
    {
        cout << "文件打开失败" << endl;
        return 0;
    }

    string buf;
    while (getline(ifs, buf))
    {
        str += buf;
    }

    ifs.close();

    cout << str << endl;


    str = test.parseJson(ele, str);

    test.printJson(ele, "");

    

    

    
}

