#ifndef ARRAY_H
#define ARRAY_H

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

struct Array {
    string* data;
    int size;
    int capacity;
};

Array MCREATE(int initialCapacity = 4);
void  MDESTROY(Array& arr);

void MPUSH_INDEX(Array& arr, string& value, int index);
void MPUSH(Array& arr, string& value);

string MRECEIVE(const Array& arr, int index);

string MDEL(Array& arr, int index);
void MREPLACE(Array& arr, string& value, int index);
int MLEN(const Array& arr);

void MGET(const Array& arr);

#endif
