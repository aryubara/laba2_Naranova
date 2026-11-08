#ifndef TASK1_H
#define TASK1_H

#include <string>

// Функция перевода инфиксного логического выражения в постфиксную форму (обратную польскую нотацию)
std::string toPostfix(const std::string& expr);

// Функция вычисления значения выражения, уже переведенного в постфиксную форму
int evaluatePostfix(const std::string& postfix);

#endif
