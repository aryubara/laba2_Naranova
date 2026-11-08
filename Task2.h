#ifndef TASK2_H
#define TASK2_H

#include "Set.h"
#include <string>

// добавление элемента в множество через команду СУБД
bool handleSetAdd(Set& set, const std::string& val);

// удаление элемента из множества через команду СУБД
bool handleSetDel(Set& set, const std::string& val);

// проверка наличия элемента в множестве через команду СУБД
bool handleSetAt(const Set& set, const std::string& val);

#endif
