#ifndef STACK_H
#define STACK_H

#include <iostream>
#include <fstream>

using namespace std;

struct Stack {
    string* data;
    int top; // индекс вершины
    int capacity;
};

Stack SCREATE(int initCapacity = 4);
void SDESTROY(Stack& stack);

void SPUSH(Stack& stack, string& value);
string SPOP(Stack& stack);

void SGET(const Stack& stack);
void SGET_REVERSE(const Stack& stack);

#endif
