#include "Task1.h"
#include "Stack.h"
#include <cctype>

using namespace std;

// Определение приоритета логических операций
int getPriority(char op) { 
    if (op == '!') return 3;
    if (op == '&') return 2;
    if (op == '|' || op == '^') return 1;
    return 0;
}

// числа - операции
string toPostfix(const string& expr) { 
    string postfix = "";
    Stack opStack = SCREATE(4);

    for (size_t i = 0; i < expr.length(); ++i) {
        char c = expr[i];
        if (isspace(c)) continue;

        if (c == '0' || c == '1') {
            postfix += c;
        }
        else if (c == '(') {
            string s(1, c);
            SPUSH(opStack, s);
        }
        else if (c == ')') {
            while (opStack.top >= 0 && opStack.data[opStack.top] != "(") {
                postfix += SPOP(opStack);
            }
            if (opStack.top >= 0) SPOP(opStack);
        }
        else if (c == '!' || c == '&' || c == '|' || c == '^') {
            while (opStack.top >= 0) {
                char topOp = opStack.data[opStack.top][0];
                if (getPriority(topOp) >= getPriority(c)) {
                    if (c == '!' && topOp == '!') break;
                    postfix += SPOP(opStack);
                } else {
                    break;
                }
            }
            string s(1, c);
            SPUSH(opStack, s);
        }
    }
    while (opStack.top >= 0) {
        postfix += SPOP(opStack);
    }
    SDESTROY(opStack);
    return postfix;
}

// считаем результат строки, ! послед число,остальные два верхних
int evaluatePostfix(const string& postfix) { 
    Stack valStack = SCREATE(4);

    for (size_t i = 0; i < postfix.length(); ++i) {
        char c = postfix[i];

        if (c == '0' || c == '1') {
            string s(1, c);
            SPUSH(valStack, s);
        } 
        else if (c == '!') {
            string topVal = SPOP(valStack);
            if (topVal.empty()) { SDESTROY(valStack); return 0; }
            int op = topVal[0] - '0';
            string s = to_string(!op);
            SPUSH(valStack, s);
        } 
        else {
            string top2 = SPOP(valStack);
            string top1 = SPOP(valStack);
            if (top1.empty() || top2.empty()) { SDESTROY(valStack); return 0; }
            int op2 = top2[0] - '0';
            int op1 = top1[0] - '0';
            int res = 0;

            if (c == '&') res = op1 && op2;
            else if (c == '|') res = op1 || op2;
            else if (c == '^') res = op1 ^ op2;

            string s = to_string(res);
            SPUSH(valStack, s);
        }
    }
    string finalStr = SPOP(valStack);
    int finalResult = finalStr.empty() ? 0 : (finalStr[0] - '0');
    SDESTROY(valStack);
    return finalResult;
}
