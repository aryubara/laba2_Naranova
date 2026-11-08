#include "utils.h"
#include <iostream>
#include <limits>
using namespace std;

int ReadInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Ошибка: нужно целое число.\n";
    }
}

string ReadString(const string& prompt) {
    string s;
    cout << prompt;
    getline(cin, s);
    return s;
}
