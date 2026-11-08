#ifndef LIST_H
#define LIST_H

#include <iostream>
#include <fstream>
using namespace std;

struct FNode {
    string data;
    FNode* next;
};

struct FList {
    int size;
    FNode* head;
};

FList FCREATE();

void FDESTROY(FList& list);

void FPUSH_HEAD(FList& list, string& data);
void FPUSH_TAIL(FList& list, string& data);
void FPUSH_BEFORE(FList& list, string& data, string& value);
void FPUSH_AFTER(FList& list, string& data, string& value);

void FDEL_HEAD(FList& list);
void FDEL_TAIL(FList& list);
void FDEL_BEFORE(FList& list, string& value);
void FDEL_AFTER(FList& list, string& value);

void FDEL_VALUE(FList& list, string& value);

int FLEN(const FList& list);

void FGET_CLASSIC(FList& list);
void FGET_REVERSE(FNode* node);
void FGET_REVERSE(FList& list);

int FFIND(const FList& list,const string& value);

#endif
