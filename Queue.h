#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

struct Queue {
    string* data;
    int head;
    int tail;
    int size;
    int capacity;
};

Queue QCREATE(int initCapacity = 4);
void QDESTROY(Queue& queue);

void QPUSH(Queue& queue, string value);
string QPOP(Queue& queue);

void QGET(const Queue& queue);
void QGET_BACK(const Queue& q);

#endif
