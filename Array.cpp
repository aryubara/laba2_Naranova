#include "Array.h"
#include "utils.h"

// изменение емкости
static void MRESIZE(Array& arr, int newCapacity) {
    string* newData = new string[newCapacity];
    for (int i = 0; i < arr.size; ++i)
        newData[i] = arr.data[i];
    delete[] arr.data;
    arr.data = newData;
    arr.capacity = newCapacity;
}

// создание - MCREATE и уничтожение - ADESTROY
Array MCREATE(int initCapacity) {
    if (initCapacity <= 0) {
        cout << "Начальная вместимость должна быть > 0. По умолчанию - 4\n";
        initCapacity = 4;
    }
    Array arr;
    arr.capacity = initCapacity;
    arr.size = 0;
    arr.data = new string[initCapacity];
    return arr;
}

void MDESTROY(Array& arr) {
    delete[] arr.data;
    arr.data = nullptr;
    arr.size = arr.capacity = 0;
}

// добавление элемента по индексу - MPUSH, если передано значение индекса
void MPUSH_INDEX(Array& arr, string& value, int index) {
    while (index < 0 || index > arr.size) {
        cout << "Некорректный индекс. Допустимо от 0 до " << arr.size<< endl;
        index = ReadInt("Повторите ввод индекса: ");
    }
    if (arr.size == arr.capacity) MRESIZE(arr, arr.capacity * 2);
    for (int i = arr.size; i > index; --i) {
        arr.data[i] = arr.data[i-1];
    }
    arr.data[index] = value;
    ++arr.size;
}

// добавление элемента в конец - MPUSH
void MPUSH(Array& arr, string& value) {
    if (arr.size == arr.capacity) {
        MRESIZE(arr, arr.capacity * 2);
    }
    arr.data[arr.size] = value;
    ++arr.size;
}

// получение элемента по индексу - MRECEIVE
string MRECEIVE(const Array& arr, int index) {
    if (arr.size == 0) {
        cout << "Массив пуст, действие невозможно\n";
        return "";
    }
    while (index < 0 || index >= arr.size) {
        cout << "Некорректный индекс. Допустимо от 0 до " << arr.size - 1 << endl;
        index = ReadInt("Повторите ввод индекса: ");
    }
    return arr.data[index];
}

// удаление элемента по индексу - MDEL
string MDEL(Array& arr, int index) {
    if (arr.size == 0) {
        cout << "Массив пуст, действие невозможно\n";
        return "";
    }
    while (index < 0 || index > arr.size) {
        cout << "Некорректный индекс. Допустимо от 0 до " << arr.size - 1 << endl;
        index = ReadInt("Повторите ввод индекса: ");
    }
    string deleteInd = arr.data[index];
    for (int i = index; i < arr.size - 1; ++i) {
        arr.data[i] = arr.data[i + 1];
    }
    --arr.size;

    if (arr.size > 0 && arr.size <= arr.capacity / 4 && arr.capacity > 4) {
        MRESIZE(arr, arr.capacity / 2);
    }
    return deleteInd;
}

// замена элемента по индексу - MREPLACE
void MREPLACE(Array& arr, string& value, int index) {
    if (arr.size == 0) {
        cout << "Массив пуст, действие невозможно\n";
        return;
    }
    while (index < 0 || index >= arr.size) {
        cout << "Некорректный индекс. Допустимо от 0 до " << arr.size - 1 << endl;
        index = ReadInt("Повторите ввод индекса: ");
    }
    arr.data[index] = value;
}

// длина массива - MLEN
int MLEN(const Array& arr) {
    return arr.size;
}

// чтение - MGET
void MGET(const Array& arr) {
    if (arr.size == 0) {
        cout << "Массив пуст\n";
        return; 
    }
    int len = arr.size;
    for (int i = 0; i < len; ++i) {
        cout << arr.data[i] << " ";
    }
    cout << endl;
}

