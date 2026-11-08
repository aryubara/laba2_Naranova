#include "Queue.h"

// распрямляем кольцевой буфер в новый массив нужного размера
static void QRESIZE(Queue& queue, int newCapacity) {
    string* newData = new string[newCapacity];
    for (int i = 0; i < queue.size; ++i) {
        newData[i] = queue.data[(queue.head + i) % queue.capacity];
    }
    delete[] queue.data;
    queue.data = newData;
    queue.head = 0;
    queue.tail = queue.size % newCapacity;
    queue.capacity = newCapacity;
}

// создание - QCREATE
Queue QCREATE(int initCapacity) {
    if (initCapacity <= 0) {
        cout << "Начальная вместимость должна быть > 0. По умолчанию - 4\n";
        initCapacity = 4;
    }
    Queue queue;
    queue.capacity = initCapacity;
    queue.size = 0;
    queue.head = 0;
    queue.tail = 0;
    queue.data = new string[initCapacity];
    return queue;
}

// уничтожение - QDESTROY
void QDESTROY(Queue& queue) {
    delete[] queue.data;
    queue.data = nullptr;
    queue.head = 0;
    queue.tail = 0;
    queue.size = 0;
    queue.capacity = 0;
}

// добавление элемента - QPUSH
void QPUSH(Queue& queue, string value) {
    if (queue.size == queue.capacity) {
        QRESIZE(queue, queue.capacity * 2);
    }
    queue.data[queue.tail] = value;
    queue.tail = (queue.tail + 1) % queue.capacity; // кольцевой сдвиг
    ++queue.size;
}

// удаление элемента - QPOP
string QPOP(Queue& queue) {
    if (queue.size == 0) {
        cout << "Ошибка: Очередь пуста\n";
        return "";
    }
    string deleted = queue.data[queue.head];
    queue.head = (queue.head + 1) % queue.capacity;
    --queue.size;
    if (queue.size > 0 && queue.size <= queue.capacity / 4 && queue.capacity > 4) {
        QRESIZE(queue, queue.capacity / 2);
    }
    return deleted;
}

// чтение - QGET
void QGET(const Queue& queue) {
    if (queue.size == 0) {
        cout << "Очередь пуста\n";
        return;
    }
    for (int i = 0; i < queue.size; ++i) {
        cout << queue.data[(queue.head + i) % queue.capacity] << " ";
    }
    cout << endl;
}

void QGET_BACK(const Queue& queue) {
    if (queue.size == 0) {
        cout << "Очередь пуста\n";
        return;
    }
    for (int i = queue.size - 1; i >= 0; --i)
        cout << queue.data[(queue.head + i) % queue.capacity] << " ";
    cout << endl;
}
