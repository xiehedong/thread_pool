#include"ThreadPool.h"
#include<iostream>
#include <string>


void printHello(const string& str)
{
    cout << str.c_str() << endl;
}

int main()
{
    ThreadPool::GetInstance()->Enqueue(printHello, "Hello World!");
    return 0;
}