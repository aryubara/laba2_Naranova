#include "Task3.h"
#include "Set.h"

using namespace std;

int solveTurtles(int n, const vector<string>& pairs) {
    // Если черепах вообще нет в файле
    if (pairs.empty() || n <= 0) return 0;

    Set uniquePositions = SETCREATE(n + 10); // координаты которые заняли черепахи
    int truthfulCount = 0; // честн
    int totalStatements = (int)pairs.size() / 2; // Сколько всего черепах высказалось

    for (size_t i = 0; i + 1 < pairs.size(); i += 2) { //создаются парв спереди и сзади
        int a = stoi(pairs[i]);
        int b = stoi(pairs[i+1]);

        // 1. проверка математической корректности фразы
        if (a < 0 || b < 0 || a + b + 1 != n || a >= n) {
            continue;
        }

        // 2. номер позиции черпахи 
        string posKey = "pos_" + to_string(a);
        
        // Сначала проверяем, есть ли уже этот ключ в множестве
        if (SET_AT(uniquePositions, posKey)) {
            // Если место занято другой честной черепахой, текущая врет
            continue;
        }

        // Добавляем позицию в множество честных и увеличиваем счетчик
        SETADD(uniquePositions, posKey);
        truthfulCount++;
    }

    SETDESTROY(uniquePositions);

    // Финальная проверка на физический максимум
    if (truthfulCount > n) truthfulCount = n;
    if (truthfulCount > totalStatements) truthfulCount = totalStatements;

    return truthfulCount;
}
