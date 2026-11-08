#ifndef DLIST_H
#define DLIST_H

#include <iostream>
#include <fstream>

using namespace std;

struct LNode {
    string data;
    LNode* prev;
    LNode* next;
};

struct LList {
    LNode* head;
    LNode* tail;
    int size;
};

LList LCREATE();
void LDESTROY(LList& list);

void LPUSH_HEAD(LList& list, string& data);
void LPUSH_TAIL(LList& list, string& data);
void LPUSH_BEFORE(LList& list, string& data, string& value);
void LPUSH_AFTER(LList& list, string& data, string& value);

void LDEL_HEAD(LList& list);
void LDEL_TAIL(LList& list);
void LDEL_BEFORE(LList& list, string& value);
void LDEL_AFTER(LList& list, string& value);
void LDEL_VALUE(LList& list, string& value);

void LGET(LList& list);
void LGET_BACK(LList& list);

int LFIND(const LList& list, const string& value);

#endif
