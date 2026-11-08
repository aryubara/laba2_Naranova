#include "List.h"

//создание и удаление
FList FCREATE() {
    FList list;
    list.head = nullptr;
    list.size = 0;
    return list;
}

void FDESTROY(FList& list) {
    while (list.head) {
        FNode* deleted = list.head;
        list.head = list.head->next;
        delete deleted;
    }
    list.size = 0;
}

// добавлеине 4 способа(до, после, голова, хвост)
void FPUSH_HEAD(FList& list, string& data) {
    FNode* node = new FNode{data, list.head};
    list.head = node;
    ++list.size;
    //cout << "Элемент успешно добавлен в начало\n";
}

void FPUSH_TAIL(FList& list, string& data) {
    FNode* node = new FNode{data, nullptr};
    if (!list.head) {
        list.head = node;
    } else {
        FNode* current = list.head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = node;
    }
    ++list.size;
    cout << "Элемент успешно добавлен в конец\n";
}

void FPUSH_BEFORE(FList& list, string& data, string& value) {
// проверки
    if (!list.head) {
        cout << "Список пуст\n";
        return;
    } 
    if (list.head->data == value) {
        FPUSH_HEAD(list, data);
        return;
    }
    FNode* prev = list.head;
    while (prev->next && prev->next->data != value) {
        prev = prev->next;
    }
    if (!prev->next) {
        cout << "Узел не найден\n";
        return;
    }
    FNode* node = new FNode{data, prev->next};
    prev->next = node;
    ++list.size;
    cout << "Элемент успешно добавлен до значения " << value << endl;
}

void FPUSH_AFTER(FList& list, string& data, string& value) {
    FNode* current = list.head;
    while (current && current->data != value) {
        current = current->next;
    }
    if (!current) {
        cout << "Узел не найден\n";
        return;
    }
    FNode* node = new FNode{data, current->next};
    current->next = node;
    ++list.size;
    cout << "Элемент успешно добавлен после " << value << endl;
}

// удаление элемента 4 способа (до, после, голова, хвост)
void FDEL_HEAD(FList& list) {
    if (!list.head) {
        cout << "Список пуст\n";
        return;
    }
    FNode* deleted = list.head;
    list.head = list.head->next;
    delete deleted;
    --list.size;
    cout << "Элемент успешно удален \n";
}

void FDEL_TAIL(FList& list) {
    if (!list.head) {
        cout << "Список пуст\n";
        return;
    }
    if (!list.head->next) {
        FDEL_HEAD(list);
        return;
    }
    FNode* current = list.head;
    while (current->next->next != nullptr) {
        current = current->next;
    }
    delete current->next;
    current->next = nullptr;
    --list.size;
    cout << "Элемент успешно удален\n";
}

void FDEL_BEFORE(FList& list, string& value) {
    if (!list.head) {
        cout << "Список пуст\n";
        return;
    } 
    if (list.head->data == value) {
        cout << "Ошибка: удалить невозможно!\n";
        return;
    }
    if (list.head->next && list.head->next->data == value) {
        FDEL_HEAD(list);
        return;
    }
    FNode* prev = list.head;
    while (prev->next && prev->next->next && prev->next->next->data != value) {
        prev = prev->next;
    }
    if (!prev->next || !prev->next->next) {
        cout << "Ошибка: значение не найдено\n";
        return;
    }
    FNode* deleted = prev->next;
    prev->next = deleted->next;
    delete deleted;
    --list.size;
    cout << "Элемент успешно удален\n";
}

void FDEL_AFTER(FList& list, string& value) {
    FNode* current = list.head;
    while (current && current->data != value) {
        current = current->next;
    }
    if (!current || !current->next) {
        cout << "Ошибка: значение не найдено\n";
        return;
    }
    FNode* deleted = current->next;
    current->next = deleted->next;
    delete deleted;
    --list.size;
    cout << "Элемент успешно удален\n";
}

// удаление элемента по значению
void FDEL_VALUE(FList& list, string& value) {
    if (!list.head) {
        cout << "Список пуст\n";
        return;
    } 
    if (list.head->data == value) { 
        FDEL_HEAD(list); 
        return;
    }
    FNode* prev = list.head;
    while (prev->next && prev->next->data != value) {
        prev = prev->next;
    }
    if (!prev->next) {
        cout << "Ошибка: значение не найдено\n";
        return;
    } 
    FNode* deleted = prev->next;
    prev->next = deleted->next;
    delete deleted;
    --list.size;
    cout << "Элемент успешно удален\n";
}

// длина 
int FLEN(const FList& list) {
    return list.size;
}

// чтение (разное, несколько способов)
void FGET_CLASSIC(FList& list) {
    if (!list.head) {
        cout << "Список пуст\n";
        return;
    }
    FNode* node = list.head;
    while (node != nullptr) {
        cout << node->data << " ";
        node = node->next;
    }
    cout << endl;
}

void FGET_REVERSE(FNode* node) {
    if (node == nullptr) return;
    FGET_REVERSE(node->next);
    cout << node->data << " ";
}

void FGET_REVERSE(FList& list) {
    if (!list.head) {
        cout << "Список пуст\n";
        return;
    }
    FGET_REVERSE(list.head);
    cout << endl;
}

// поиск элемента по значению.
int FFIND(const FList& list,const string& value) {
    int index = 0;
    FNode* current = list.head;
    while (current != nullptr) {
        if (current->data == value) return index;
        else {
            current = current->next;
            ++index;
        }
    }
    return -1;
}
