#include "DList.h"


LList LCREATE() {
    LList list;
    list.head = nullptr;
    list.tail = nullptr;
    list.size = 0;
    return list;
}

void LDESTROY(LList& list) {
    LNode* cur = list.head;
    while (cur) { LNode* n = cur->next; delete cur; cur = n; }
    list.head = list.tail = nullptr;
    list.size = 0;
}
// добавление 4 способа(до, после, голова, хвост)
void LPUSH_HEAD(LList& list, string& data) {
    LNode* node = new LNode{data, nullptr, list.head}; // data, prev, next
    if (list.head) { // если сущ. голова
        list.head->prev = node; 
    } else list.tail = node; // значит список был пуст
    list.head = node;
    ++list.size;
    //cout << "Элемент успешно добавлен в начало\n";
}

void LPUSH_TAIL(LList& list, string& data) {
    LNode* node = new LNode{data, list.tail, nullptr}; // data, prev, next
    if (list.tail) {
        list.tail->next = node;
    } else list.head = node; 
    list.tail = node;
    ++list.size;
    cout << "Элемент успешно добавлен в конец\n";
}

void LPUSH_BEFORE(LList& list, string& data, string& value) {
    LNode* current = list.head;
    while (current && current->data != value) {
        current = current->next;
    }
    if (!current || current->data != value) {
        cout << "Ошибка: значение не найдено\n";
        return;
    }
    LNode* node = new LNode{data, current->prev, current};
    if (current->prev) current->prev->next = node;
    else list.head = node;
    current->prev = node;
    ++list.size;
    cout << "Элемент успешно добавлен до " << value << endl;
}

void LPUSH_AFTER(LList& list, string& data, string& value) {
    LNode* current = list.head;
    while (current && current->data != value) {
        current = current->next;
    }
    if (!current || current->data != value) {
        cout << "Ошибка: значение не найдено\n";
        return;
    }
    LNode* node = new LNode{data, current, current->next};
    if (current->next) current->next->prev = node;
    else list.tail = node;
    current->next = node;
    ++list.size;
    cout << "Элемент успешно добавлен после " << value << endl; 
}

// удаление элемента 4 способа (до после голова хвост)
void LDEL_HEAD(LList& list) {
    if (!list.head) {
        cout << "Список пуст\n";
        return;
    } 
    LNode* deleted = list.head;
    list.head = deleted->next;
    if (list.head) list.head->prev = nullptr;
    else list.tail = nullptr;
    delete deleted;
    --list.size;
    cout << "Элемент успешно удален\n";
}

void LDEL_TAIL(LList& list) {
    if (!list.tail) {
        cout << "Список пуст\n";
        return;
    } 
    LNode* deleted = list.tail;
    list.tail = deleted->prev;
    if (list.tail) list.tail->next = nullptr;
    else list.head = nullptr;
    delete deleted;
    --list.size;
    cout << "Элемент успешно удален\n";
}

void LDEL_BEFORE(LList& list, string& value) {
    LNode* current = list.head;
    while (current && current->data != value) {
        current = current->next;
    }
    if (!current || !current->prev) {
        cout << "Ошибка: Нечего удалять!\n";
        return;
    }
    LNode* deleted = current->prev;
    current->prev = deleted->prev;
    if (deleted->prev) deleted->prev->next = current;
    else list.head = current;
    delete deleted;
    --list.size;
    cout << "Элемент успешно удален\n";
}

void LDEL_AFTER(LList& list, string& value) {
    LNode* current = list.head;
    while (current && current->data != value) {
        current = current->next;
    }
    if (!current || !current->next) {
        cout << "Ошибка: Нечего удалять!\n";
        return;
    }
    LNode* deleted = current->next;
    current->next = deleted->next;
    if (deleted->next) deleted->next->prev = current;
    else list.tail = current;
    delete deleted;
    --list.size;
    cout << "Элемент успешно удален\n";
}

// удаление элемента по значению 
void LDEL_VALUE(LList& list, string& value) {
    LNode* current = list.head;
    while (current && current->data != value) {
        current = current->next;
    }
    if (!current) {
        cout << "Ошибка: значение не найдено!\n";
        return;
    }
    if (current->prev) current->prev->next = current->next;
    else list.head = current->next;
    if (current->next) current->next->prev = current->prev;
    else list.tail = current->prev;
    delete current;
    --list.size;
    cout << "Элемент успешно удален\n";
}

// чтение (разное, несколько способов)
void LGET(LList& list) {
    if (!list.head) {
        cout << "Список пуст\n";
        return;
    }
    LNode* current = list.head;
    while (current != nullptr) {
        cout << current->data << " ";
        current = current->next;
    }
    cout << endl;
}

void LGET_BACK(LList& list) {
    if (!list.head) {
        cout << "Список пуст\n";
        return;
    }
    LNode* current = list.tail;
    while (current != nullptr) {
        cout << current->data << " ";
        current = current->prev;
    }
    cout << endl;
}

// поиск элемента по значению
int LFIND(const LList& list, const string& value) {
    int index = 0;
    LNode* current = list.head;
    while (current != nullptr) {
        if (current->data == value) return index;
        else {
            current = current->next;
            ++index;
        }
    }
    return -1;
}
