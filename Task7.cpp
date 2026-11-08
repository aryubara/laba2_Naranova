#include "Task7.h"
#include "Array.h"
#include <iostream>

using namespace std;

// (управление памятью которая удаляет старые ненужные данные)
void runLruCache(int capacity, int qCount, const vector<string>& operations) {
    
    Array cacheKeys = MCREATE(capacity); //ключи
    Array cacheValues = MCREATE(capacity); //значения

    size_t i = 0;              // Указатель текущей позиции в векторе считанных команд
    int processedQueries = 0;  // Псчетчик для контроля лимита запросов 

    // крутится, пока есть команды и не исчерпан лимит
    while (i < operations.size() && processedQueries < qCount) {
        string op = operations[i]; // Извлекаем имя текущей операции (SET или GET)

        //  Обработка команды "SET" (Запись / Обновление данных)
        if (op == "SET" && i + 2 < operations.size()) {
            string key = operations[i+1];  // Считываем ключ из параметров
            string val = operations[i+2];  // Считываем значение из параметров
            i += 3;                        // Сдвигаем указатель вектора на 3 позиции вперед
            processedQueries++;            // Фиксируем выполнение очередного запроса

            // Линейный поиск ключа в текущем кэше
            int foundIdx = -1;
            for (int k = 0; k < cacheKeys.size; ++k) {
                if (cacheKeys.data[k] == key) { foundIdx = k; break; }
            }

            // Элемент уже был в кэше  обновляем значение и "свежесть"
            if (foundIdx != -1) {
                // Сдвигаем все последующие элементы влево, чтобы освободить место в самом конце
                for (int k = foundIdx; k < cacheKeys.size - 1; ++k) {
                    cacheKeys.data[k] = cacheKeys.data[k+1];
                    cacheValues.data[k] = cacheValues.data[k+1];
                }
                // Помещаем обновленный элемент в хвост массива (теперь он "самый свежий")
                cacheKeys.data[cacheKeys.size - 1] = key;
                cacheValues.data[cacheValues.size - 1] = val;
            } 
            // Элемент новый вставляем его с проверкой на переполнение
            else {
                if (cacheKeys.size >= capacity) { //кеш переполнен
                    // сдвигаем абсолютно весь массив влево на один шаг, стирая индекс 0
                    for (int k = 0; k < cacheKeys.size - 1; ++k) {
                        cacheKeys.data[k] = cacheKeys.data[k+1];
                        cacheValues.data[k] = cacheValues.data[k+1];
                    }
                    cacheKeys.size--; // Уменьшаем логический размер, освобождая ячейку в конце
                    cacheValues.size--;
                }
                // добавл новую запись в освободившийся хвост массива
                MPUSH(cacheKeys, key);
                MPUSH(cacheValues, val);
            }
        } 

        //  Обработка команды "GET" (Чтение данных)
        else if (op == "GET" && i + 1 < operations.size()) {
            string key = operations[i+1]; // Считываем ключ, который запросил пользователь
            i += 2;                       // Перематываем указатель вектора на 2 позиции вперед
            processedQueries++;           // Фиксируем выполнение очередного запроса

            // Линейный поиск ключа в массиве
            int foundIdx = -1;
            for (int k = 0; k < cacheKeys.size; ++k) {
                if (cacheKeys.data[k] == key) { foundIdx = k; break; }
            }

            //  Элемент найден
            if (foundIdx != -1) {
                string val = cacheValues.data[foundIdx]; // Запоминаем его значение для вывода
                
                // двигаем элемент из середины в самый конец массива
                for (int k = foundIdx; k < cacheKeys.size - 1; ++k) {
                    cacheKeys.data[k] = cacheKeys.data[k+1];
                    cacheValues.data[k] = cacheValues.data[k+1];
                }
                cacheKeys.data[cacheKeys.size - 1] = key;
                cacheValues.data[cacheValues.size - 1] = val;
                
                cout << val << " " << flush;
            } 
            // Элемента в памяти нет
            else {
                cout << "-1 " << flush; 
            }
        } 
        
        // сдвиг указателя, если в файле встретилась некорректная команда
        else {
            i++; 
        }
    }
    cout << endl; 

    MDESTROY(cacheKeys);
    MDESTROY(cacheValues);
}
