#include "Task6.h"
#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;

// проверка на простоту
static bool isPrime(int num) {
    if (num < 2) return false;
    if (num % 2 == 0) return num == 2;
    for (int i = 3; i * i <= num; i += 2) {
        if (num % i == 0) return false;
    }
    return true;
}
//поиск след простого числа
static int nextPrime(int num) {
    while (!isPrime(num)) ++num;
    return num;
}

// Создание и выделение памяти под таблицу
RouteHashTable createRouteTable(int cap) {
    RouteHashTable ht;
    ht.capacity = cap;
    ht.size = 0;
    ht.table = new RouteEdge[cap];
    return ht;
}

void destroyRouteTable(RouteHashTable& ht) {
    delete[] ht.table;
    ht.table = nullptr;
    ht.capacity = 0;
    ht.size = 0;
}

// Первая хеш-функция (DJB2) — стартовый индекс
unsigned long hash_lp(const string& str, int capacity) {
    unsigned long hash = 5381;
    for (char c : str) hash = ((hash << 5) + hash) + (unsigned char)c;
    return hash % capacity;
}

// Вторая хеш-функция (SDBM) — шаг для двойного хеширования
unsigned long hash_dh2(const string& str, int capacity) {
    unsigned long hash = 0;
    for (char c : str) hash = (unsigned char)c + (hash << 6) + (hash << 16) - hash;
    return (hash % (capacity - 2)) + 1;
}

// ЛИНЕЙНОЕ АПРОБИРОВАНИЕ
//вставка
void insertLinear(RouteHashTable& ht, const string& key, int value) {
    int idx = hash_lp(key, ht.capacity);
    int firstDeleted = -1;

    for (int i = 0; i < ht.capacity; ++i) {
        if (ht.table[idx].isOccupied) {
            if (ht.table[idx].key == key) {
                ht.table[idx].value = value; // Обновляем значение, если ключ уже есть
                return;
            }
        } else {
            if (!ht.table[idx].isDeleted) {
                int target = (firstDeleted != -1) ? firstDeleted : idx;
                ht.table[target].key = key;
                ht.table[target].value = value;
                ht.table[target].isOccupied = true;
                ht.table[target].isDeleted = false;
                ht.size++;
                return;
            } else if (firstDeleted == -1) {
                firstDeleted = idx;
            }
        }
        idx = (idx + 1) % ht.capacity;
    }

    if (firstDeleted != -1) {
        ht.table[firstDeleted].key = key;
        ht.table[firstDeleted].value = value;
        ht.table[firstDeleted].isOccupied = true;
        ht.table[firstDeleted].isDeleted = false;
        ht.size++;
    }
}
//поиск
bool searchLinear(const RouteHashTable& ht, const string& key, int& outValue) {
    int idx = hash_lp(key, ht.capacity);

    for (int i = 0; i < ht.capacity; ++i) {
        if (!ht.table[idx].isOccupied && !ht.table[idx].isDeleted) {
            return false;
        }
        if (ht.table[idx].isOccupied && ht.table[idx].key == key) {
            outValue = ht.table[idx].value;
            return true;
        }
        idx = (idx + 1) % ht.capacity;
    }
    return false;
}
//удаление
bool removeLinear(RouteHashTable& ht, const string& key) {
    int idx = hash_lp(key, ht.capacity);

    for (int i = 0; i < ht.capacity; ++i) {
        if (!ht.table[idx].isOccupied && !ht.table[idx].isDeleted) {
            return false;
        }
        if (ht.table[idx].isOccupied && ht.table[idx].key == key) {
            ht.table[idx].isOccupied = false;
            ht.table[idx].isDeleted = true;
            ht.size--;
            return true;
        }
        idx = (idx + 1) % ht.capacity;
    }
    return false;
}

//ДВОЙНОЕ ХЕШИРОВАНИЕ
//вставка
void insertDoubleHash(RouteHashTable& ht, const string& key, int value) {
    int idx = hash_lp(key, ht.capacity);
    int step = hash_dh2(key, ht.capacity);
    int firstDeleted = -1;

    for (int i = 0; i < ht.capacity; ++i) {
        if (ht.table[idx].isOccupied) {
            if (ht.table[idx].key == key) {
                ht.table[idx].value = value;
                return;
            }
        } else {
            if (!ht.table[idx].isDeleted) {
                int target = (firstDeleted != -1) ? firstDeleted : idx;
                ht.table[target].key = key;
                ht.table[target].value = value;
                ht.table[target].isOccupied = true;
                ht.table[target].isDeleted = false;
                ht.size++;
                return;
            } else if (firstDeleted == -1) {
                firstDeleted = idx;
            }
        }
        idx = (idx + step) % ht.capacity;
    }

    if (firstDeleted != -1) {
        ht.table[firstDeleted].key = key;
        ht.table[firstDeleted].value = value;
        ht.table[firstDeleted].isOccupied = true;
        ht.table[firstDeleted].isDeleted = false;
        ht.size++;
    }
}
//поиск
bool searchDoubleHash(const RouteHashTable& ht, const string& key, int& outValue) {
    int idx = hash_lp(key, ht.capacity);
    int step = hash_dh2(key, ht.capacity);

    for (int i = 0; i < ht.capacity; ++i) {
        if (!ht.table[idx].isOccupied && !ht.table[idx].isDeleted) {
            return false;
        }
        if (ht.table[idx].isOccupied && ht.table[idx].key == key) {
            outValue = ht.table[idx].value;
            return true;
        }
        idx = (idx + step) % ht.capacity;
    }
    return false;
}
//удаление
bool removeDoubleHash(RouteHashTable& ht, const string& key) {
    int idx = hash_lp(key, ht.capacity);
    int step = hash_dh2(key, ht.capacity);

    for (int i = 0; i < ht.capacity; ++i) {
        if (!ht.table[idx].isOccupied && !ht.table[idx].isDeleted) {
            return false;
        }
        if (ht.table[idx].isOccupied && ht.table[idx].key == key) {
            ht.table[idx].isOccupied = false;
            ht.table[idx].isDeleted = true;
            ht.size--;
            return true;
        }
        idx = (idx + step) % ht.capacity;
    }
    return false;
}

//  ПОДЗАДАНИЕ 6.1: ЭМПИРИЧЕСКИЙ АНАЛИЗ

void runEmpiricalAnalysis(int n) { // принимает число элементов
    if (n <= 0) return;

    // Выбираем размер таблицы (простое число >= 2*n, чтобы коэффициент заполнения был ~0.5)
    int cap = nextPrime(n * 2 + 1);

    // Заранее генерируем n ключей, чтобы замерять только работу самой хеш-таблицы
    vector<string> keys(n);
    for (int i = 0; i < n; ++i) {
        keys[i] = "Key_" + to_string(i);
    }

    RouteHashTable htLinear = createRouteTable(cap);
    RouteHashTable htDouble = createRouteTable(cap);
    int foundVal = 0;

    // ЛИНЕЙНОЕ АПРОБИРОВАНИЕ
    
    // Замер добавления (Insertion)
    auto startLinIns = high_resolution_clock::now();
    for (int i = 0; i < n; ++i) {
        insertLinear(htLinear, keys[i], i);
    }
    auto endLinIns = high_resolution_clock::now();
    auto timeLinIns = duration_cast<microseconds>(endLinIns - startLinIns).count();

    // Замер поиска (Search)
    auto startLinSch = high_resolution_clock::now();
    for (int i = 0; i < n; ++i) {
        searchLinear(htLinear, keys[i], foundVal);
    }
    auto endLinSch = high_resolution_clock::now();
    auto timeLinSch = duration_cast<microseconds>(endLinSch - startLinSch).count();

    // Замер удаления (Deletion)
    auto startLinRem = high_resolution_clock::now();
    for (int i = 0; i < n; ++i) {
        removeLinear(htLinear, keys[i]);
    }
    auto endLinRem = high_resolution_clock::now();
    auto timeLinRem = duration_cast<microseconds>(endLinRem - startLinRem).count();

    //ДВОЙНОЕ ХЕШИРОВАНИЕ

    // Замер добавления (Insertion)
    auto startDblIns = high_resolution_clock::now();
    for (int i = 0; i < n; ++i) {
        insertDoubleHash(htDouble, keys[i], i);
    }
    auto endDblIns = high_resolution_clock::now();
    auto timeDblIns = duration_cast<microseconds>(endDblIns - startDblIns).count();

    // Замер поиска (Search)
    auto startDblSch = high_resolution_clock::now();
    for (int i = 0; i < n; ++i) {
        searchDoubleHash(htDouble, keys[i], foundVal);
    }
    auto endDblSch = high_resolution_clock::now();
    auto timeDblSch = duration_cast<microseconds>(endDblSch - startDblSch).count();

    // Замер удаления (Deletion)
    auto startDblRem = high_resolution_clock::now();
    for (int i = 0; i < n; ++i) {
        removeDoubleHash(htDouble, keys[i]);
    }
    auto endDblRem = high_resolution_clock::now();
    auto timeDblRem = duration_cast<microseconds>(endDblRem - startDblRem).count();

    // Выводим результаты по каждой операции отдельно
    cout << "Линейное апробирование (" << n << " эл.)" << endl;
    cout << "Добавление: " << timeLinIns << " мкс" << endl;
    cout << "Поиск:      " << timeLinSch << " мкс" << endl;
    cout << "Удаление:   " << timeLinRem << " мкс" << endl;

    cout << "\n Двойное хеширование (" << n << " эл.)" << endl;
    cout << "Добавление: " << timeDblIns << " мкс" << endl;
    cout << "Поиск:      " << timeDblSch << " мкс" << endl;
    cout << "Удаление:   " << timeDblRem << " мкс" << endl;

    destroyRouteTable(htLinear);
    destroyRouteTable(htDouble);
}

//  ПОДЗАДАНИЕ 6.2: ПОИСК МИНИМАЛЬНОЙ ЗАДЕРЖКИ 

void findMinDelay(const RouteHashTable& ht, const string& current, const string& dest, 
                  int currentDelay, string currentPath, int& minDelay, string& bestPath, 
                  map<string, bool>& visited) {
    
    //Если мы приехали в конечный точку
    if (current == dest) {
        // Если время текущего пути меньше, чем наш прошлый лучший рекорд
        if (currentDelay < minDelay) {
            minDelay = currentDelay; // Запоминаем новое рекордное (минимальное) время
            bestPath = currentPath;  // Сохраняем текстовую карту этого лучшего маршрута
        }
        return; 
        }

    // СТАВИМ ФЛАЖОК: Отмечаем текущ точку как посещенную, чтобы не ходить кругами
    visited[current] = true;

    // Перебираем всю хеш-таблицу, чтобы найти выезды из текущей точки
    for (int i = 0; i < ht.capacity; ++i) {
        // Если ячейка таблицы не пустая (в ней записан какой-то маршрут)
        if (ht.table[i].isOccupied) {
            
            // 4. Разрезаем ключ на две точки
            string key = ht.table[i].key;
            size_t pos = key.find("->");
            if (pos == string::npos) continue; 
            
            string from = key.substr(0, pos);   // точка отправления 
            string to = key.substr(pos + 2);    // точка прибытия 

            // 5. Если дорога ведет из нашей точки и в следующем точке мы еще не были
            if (from == current && !visited[to]) {
                // Запускаем поиск дальше (в глубину) уже из нового города "to"
                findMinDelay(ht, to, dest, 
                             currentDelay + ht.table[i].value, // Прибавляем время пробок этой дороги
                             currentPath + " -> " + to,        // Дописываем новый точку в ниточку пути
                             minDelay, bestPath, visited);
            }
        }
    }

    // Снимаем флажок посещенности, чтобы точка была доступен для других вариантов маршрута
    visited[current] = false;
}
