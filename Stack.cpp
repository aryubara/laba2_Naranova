#include "Stack.h"

static void SRESIZE(Stack& stack, int newCapacity) { // новый массив увелич размера
    string* newData = new string[newCapacity];
    for (int i = 0; i <= stack.top; ++i)
        newData[i] = stack.data[i];
    delete[] stack.data;
    stack.data = newData;
    stack.capacity = newCapacity;
}

Stack SCREATE(int initCapacity) {
    if (initCapacity <= 0) {
        cout << "Начальная вместимость должна быть > 0. По умолчанию - 4\n";
        initCapacity = 4;
    }
    Stack stack;    
    stack.capacity = initCapacity;
    stack.top = -1;
    stack.data = new string[initCapacity];
    return stack;
}

void SDESTROY(Stack& stack) {
    delete[] stack.data;
    stack.data = nullptr;
    stack.top = -1;
    stack.capacity = 0;
}

// добавление  - SPUSH
void SPUSH(Stack& stack, string& value) {
    if (stack.top + 1 == stack.capacity) {
        SRESIZE(stack, stack.capacity * 2);
    }
    stack.data[++stack.top] = value;
}

// удаление - SPOP
string SPOP(Stack& stack) {
    if (stack.top < 0) {
        cout << "Стек пуст\n";
        return "";
    }
    return stack.data[stack.top--];
}

// чтение - SGET сверху вниз
void SGET(const Stack& stack) {
    if (stack.top < 0) {
        cout << "Стек пуст\n";
        return;
    }
    for (int i = stack.top; i >= 0; --i) {
        cout << stack.data[i] << " ";
    }
    cout << endl;
}

void SGET_REVERSE(const Stack& stack) { // снизу вверх
    if (stack.top < 0) {
        cout << "Стек пуст\n";
        return;
    }
    for (int i = 0; i <= stack.top; ++i) {
        cout << stack.data[i] << " ";
    }
    cout << endl;
}
