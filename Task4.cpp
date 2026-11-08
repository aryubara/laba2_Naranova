#include "Task4.h"
#include "Array.h"
#include <iostream>

using namespace std;

void printHistogram(const string& text) {
    int freq[256] = {0}; //массив счетчиков по количеству возможных кодов аски
    int maxHeight = 0; //макс частота

    for (char c : text) { //проходимся по всем символам
        if (c == ' ' || c == '\n' || c == '\r' || c == '\t') continue; //переход на друг строку игнор
        unsigned char uc = (unsigned char)c; // код использ симв преобраз
        freq[uc]++;
        if (freq[uc] > maxHeight) {
            maxHeight = freq[uc];
        }
    }

    Array uniqueChars = MCREATE(4); // сбор тех символов которые встрелись в строке исходной
    for (int i = 0; i < 256; ++i) {
        if (freq[i] > 0) {
            string s(1, (char)i);
            MPUSH(uniqueChars, s);
        }
    }

    cout << "Вывод:" << endl;
    for (int h = maxHeight; h >= 1; --h) {
        for (int i = 0; i < uniqueChars.size; ++i) { //скан гистограммы строками
            unsigned char uc = (unsigned char)uniqueChars.data[i][0];
            if (freq[uc] >= h) cout << "#"; //если частота больше или равна текущ высоте то #
            else cout << " ";
        }
        cout << endl;
    }

    for (int i = 0; i < uniqueChars.size; ++i) { //вывод самих символов
        cout << uniqueChars.data[i];
    }
    cout << endl;

    MDESTROY(uniqueChars);
}
