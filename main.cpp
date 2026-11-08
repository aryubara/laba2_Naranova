#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <functional>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <algorithm>

// Подключаем файлы структур данных из первой лабораторной
#include "Array.h"
#include "List.h"
#include "DList.h"
#include "Stack.h"
#include "Queue.h"
#include "Tree.h"
#include "Set.h"

// Подключаем заголовочные файлы вынесенных заданий 
#include "Task1.h"
#include "Task2.h"
#include "Task3.h"
#include "Task4.h"
#include "Task5.h"
#include "Task6.h"
#include "Task7.h"

using namespace std;

// КАРКАС СУБД И ОБЕРТКИ ДЛЯ СТРУКТУР 

enum class Type {
    ARRAY, STACK, QUEUE, TREE, FLIST, DLIST, SET, UNKNOWN
};

struct Args {
    string file;
    string query;
};

static bool ParseArgs(int argc, char** argv, Args& args) {
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "--file" && i + 1 < argc) args.file  = argv[++i];
        else if (arg == "--query" && i + 1 < argc) args.query = argv[++i];
        else {cout << "Неизвестный аргумент: " << arg << endl; return false;}
    }
    if (args.file.empty() || args.query.empty()) {
        cout << "Использование: ./dbms --file <file> --query '<cmd>'\n";
        return false;
    }
    return true;
}

struct Query {
    string command;
    string name;
    vector<string> args;
};

static Query ParseQuery(const string& str) {
    Query query;
    istringstream input(str);
    input >> query.command >> query.name;
    for (string i; input >> i; ) query.args.push_back(i);
    return query;
}

struct Entry {
    string type;
    string name;
    vector<string> values;
};

struct DB {
    string filename;
    vector<Entry> entries;

    bool Load() {
        ifstream file(filename);
        if (!file) return true;
        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;
            istringstream header(line);
            Entry entry;
            int size = 0;
            if (!(header >> entry.type >> entry.name >> size)) return false;
            for (int i = 0; i < size; ++i) {
                string value;
                if (!getline(file, value)) break;
                entry.values.push_back(value);
            }
            entries.push_back(move(entry));
        }
        return true;
    }

    bool Save() {
        ofstream file(filename);
        if (!file) return false;
        for (auto& entry : entries) {
            file << entry.type << " " << entry.name << " " << entry.values.size() << endl;
            for (auto& value : entry.values) file << value << endl;
        }
        return true;
    }

    Entry* GetOrCreate(const string& type, const string& name) {
        for (auto& entry : entries) {
            if (entry.name == name) {
                if (entry.type != type) {
                    cout << "Тип не совпадает: ожидается " << entry.type << endl;
                    return nullptr;
                }
                return &entry;
            }
        }
        entries.push_back({type, name, {}});
        return &entries.back();
    }
};

static bool ParseInt(const string& str, int& out) {
    char* end = nullptr;
    long value = strtol(str.c_str(), &end, 10);
    if (end == str.c_str() || *end != '\0') return false;
    out = (int)value;
    return true;
}

static Array ToArray(vector<string>& vector) {
    Array arr = MCREATE(vector.empty() ? 4 : (int)vector.size());
    for (auto& str : vector) MPUSH(arr, str);
    return arr;
}
static void FromArray(Array& arr, Entry& entry) {
    entry.values.clear();
    for (int i = 0; i < arr.size; ++i) entry.values.push_back(arr.data[i]);
    MDESTROY(arr);
}

static Stack ToStack(vector<string>& vector) {
    Stack stack = SCREATE(4);
    for (auto& item : vector) SPUSH(stack, item);
    return stack;
}
static void FromStack(Stack& stack, Entry& entry) {
    entry.values.clear();
    for (int i = 0; i <= stack.top; ++i) entry.values.push_back(stack.data[i]);
    SDESTROY(stack);
}

static Queue ToQueue(const vector<string>& vector) {
    Queue queue = QCREATE(4);
    for (auto& item : vector) QPUSH(queue, item);
    return queue;
}
static void FromQueue(Queue& queue, Entry& entry) {
    entry.values.clear();
    for (int i = 0; i < queue.size; ++i)
        entry.values.push_back(queue.data[(queue.head + i) % queue.capacity]);
    QDESTROY(queue);
}

static Tree ToTree(const vector<string>& vec) {
    Tree tree = TCREATE();
    for (auto& str : vec) {
        int item = 0;
        if (ParseInt(str, item)) TINSERT(tree, item);
        else if (str == "null") TINSERT(tree, -999);
    }
    return tree;
}
static void FromTree(Tree& tree, Entry& entry) {
    entry.values.clear();
    if (tree.root == nullptr) { tree.root = nullptr; return; }
    queue<TNode*> q;
    q.push(tree.root);
    while (!q.empty()) {
        TNode* curr = q.front(); q.pop();
        entry.values.push_back(to_string(curr->key));
        if (curr->left != nullptr) q.push(curr->left);
        if (curr->right != nullptr) q.push(curr->right);
    }
    TDESTROY(tree.root);
    tree.root = nullptr;
}

static Set ToSet(vector<string>& vec) {
    Set set = SETCREATE(100);
    for (auto& str : vec) { string copy = str; SETADD(set, copy); }
    return set;
}
static void FromSet(Set& set, Entry& entry) {
    entry.values.clear();
    for (int i = 0; i < set.capacity; ++i) {
        FNode* curr = set.buckets[i].head;
        while (curr) { entry.values.push_back(curr->data); curr = curr->next; }
    }
    SETDESTROY(set);
}

static FList ToFList(const vector<string>& vector) {
    FList list = FCREATE();
    FNode* tail = nullptr;
    for (auto& str : vector) {
        FNode* node = new FNode{str, nullptr};
        if (!list.head) list.head = node;
        else tail->next = node;
        tail = node;
        ++list.size;
    }
    return list;
}
static void FromFList(FList& list, Entry& entry) {
    entry.values.clear();
    for (FNode* node = list.head; node; node = node->next) entry.values.push_back(node->data);
    FDESTROY(list);
}

static LList ToDList(const vector<string>& vector) {
    LList list = LCREATE();
    for (auto& str : vector) {
        LNode* node = new LNode{str, list.tail, nullptr};
        if (list.tail) list.tail->next = node;
        else list.head = node;
        list.tail = node;
        ++list.size;
    }
    return list;
}
static void FromDList(LList& list, Entry& entry) {
    entry.values.clear();
    for (LNode* node = list.head; node; node = node->next) entry.values.push_back(node->data);
    LDESTROY(list);
}

static Type TypeByCommand(const string& cmd) {
    if (cmd.rfind("SET", 0) == 0 || cmd == "TURTLES") return Type::SET;
    if (cmd == "LRU_CACHE" || cmd == "HIST" || cmd == "ROUTE_OPT" || cmd == "HASH_TEST") return Type::ARRAY;
    if (cmd.empty()) return Type::UNKNOWN;
    switch (cmd[0]) {
        case 'M': return Type::ARRAY;
        case 'F': return Type::FLIST;
        case 'L': return Type::DLIST;
        case 'S': return Type::STACK;
        case 'Q': return Type::QUEUE;
        case 'T': return Type::TREE;
        default:  return Type::UNKNOWN;
    }
}

static const char* TypeName(Type type) {
    switch (type) {
        case Type::SET:   return "SET";
        case Type::ARRAY: return "ARRAY";
        case Type::STACK: return "STACK";
        case Type::QUEUE: return "QUEUE";
        case Type::TREE:  return "TREE";
        case Type::FLIST: return "FLIST";
        case Type::DLIST: return "DLIST";
        default:          return "UNKNOWN";
    }
}

using Handler = function<bool(Entry&, const Query&)>;

static Handler wrapStack(function<bool(Stack&, const Query&)> f) { return [f](Entry& e, const Query& q) { Stack s = ToStack(e.values); bool ok = f(s, q); FromStack(s, e); return ok; }; }
static Handler wrapArray(function<bool(Array&, const Query&)> f) { return [f](Entry& e, const Query& q) { Array a = ToArray(e.values); bool ok = f(a, q); FromArray(a, e); return ok; }; }
static Handler wrapQueue(function<bool(Queue&, const Query&)> f) { return [f](Entry& e, const Query& q) { Queue qe = ToQueue(e.values); bool ok = f(qe, q); FromQueue(qe, e); return ok; }; }
static Handler wrapTree(function<bool(Tree&, const Query&)> f) { return [f](Entry& e, const Query& q) { Tree t = ToTree(e.values); bool ok = f(t, q); FromTree(t, e); return ok; }; }
static Handler wrapFList(function<bool(FList&, const Query&)> f) { return [f](Entry& e, const Query& q) { FList l = ToFList(e.values); bool ok = f(l, q); FromFList(l, e); return ok; }; }
static Handler wrapDList(function<bool(LList&, const Query&)> f) { return [f](Entry& e, const Query& q) { LList l = ToDList(e.values); bool ok = f(l, q); FromDList(l, e); return ok; }; }
static Handler wrapSet(function<bool(Set&, const Query&)> f) { return [f](Entry& e, const Query& q) { Set s = ToSet(e.values); bool ok = f(s, q); FromSet(s, e); return ok; }; }

struct Command {
    int argc;
    Handler handler;
};

// ТАБЛИЦА КОМАНД 
static const map<string, Command>& Table() {
    static const map<string, Command> table = {
        // --- ЛАБОРАТОРНАЯ РАБОТА 2 (ОЛИМПИАДНЫЕ/ПРИКЛАДНЫЕ СЦЕНАРИИ) ---
        {"SEVAL", {1, wrapStack([](Stack& stack, const Query& query) -> bool {
            string expr = query.args[0];
            string postfix = toPostfix(expr); 
            int res = evaluatePostfix(postfix); 
            cout << "-> " << res << endl;
            string strRes = to_string(res);
            SPUSH(stack, strRes);
            return true;
        })}},
        {"SETADD", {1, wrapSet([](Set& set, const Query& q) -> bool {
            return handleSetAdd(set, q.args[0]); 
        })}},
        {"SETDEL", {1, wrapSet([](Set& set, const Query& q) -> bool {
            return handleSetDel(set, q.args[0]); 
        })}},
        {"SET_AT", {1, wrapSet([](Set& set, const Query& q) -> bool {
            return handleSetAt(set, q.args[0]);  
        })}},
        {"TURTLES", {2, wrapSet([](Set& set, const Query& q) -> bool {
            int n = stoi(q.args[0]); 
            vector<string> pairs;
            for (size_t i = 1; i < q.args.size(); ++i) pairs.push_back(q.args[i]);
            cout << "-> " << solveTurtles(n, pairs) << endl; 
            return true;
        })}},
        {"HIST", {1, wrapArray([](Array& arr, const Query& q) -> bool {
            string text = "";
            for (const string& s : q.args) {
                if (!text.empty()) text += " ";
                text += s;
            }
            printHistogram(text); 
            return true;
        })}},
        {"BST_FORKS", {1, wrapTree([](Tree& tree, const Query& q) -> bool {
            runBstForks(q.args); 
            return true;
        })}},
        {"HASH_TEST", {1, wrapArray([](Array& arr, const Query& q) -> bool {
            // Принимаем одно или несколько чисел (например: 100, 1000 или 10000) прямо при запуске
            for (const string& arg : q.args) {
                int n = 0;
                if (ParseInt(arg, n) && n > 0) {
                    runEmpiricalAnalysis(n);
                }
            }
            return true;
        })}},
        {"ROUTE_OPT", {1, wrapArray([](Array& arr, const Query& q) -> bool {
            // Если при запуске ROUTE_OPT первым аргументом передали число (например 100, 1000 или 10000),
            // выполняем эмпирический анализ для введенного количества элементов:
            int n = 0;
            if (ParseInt(q.args[0], n)) {
                for (const string& arg : q.args) {
                    if (ParseInt(arg, n) && n > 0) {
                        runEmpiricalAnalysis(n);
                    }
                }
                return true;
            }

            // Иначе решаем задачу оптимизации маршрутов (Подзадание 6.2)
            if (q.args.size() < 2) return false;
            string startServer = q.args[0]; 
            string endServer = q.args[1];   
            RouteHashTable htRoutes = createRouteTable(199);

            for (size_t i = 2; i + 2 < q.args.size(); i += 3) {
                string key = q.args[i] + "->" + q.args[i+1]; // Ключ маршрута
                int delay = stoi(q.args[i+2]);               // Значение задержки
                insertDoubleHash(htRoutes, key, delay);      // Передаем пару (ключ, значение)
            }
            
            int minDelay = 999999;
            string bestPath = "";
            map<string, bool> visited;
            findMinDelay(htRoutes, startServer, endServer, 0, startServer, minDelay, bestPath, visited);

            if (minDelay == 999999) cout << "Вывод: Маршрут не найден" << endl;
            else cout << "Вывод: минимальная задержка = " << minDelay << " (маршрут \"" << bestPath << "\")" << endl;

            destroyRouteTable(htRoutes);
            return true;
        })}},
        {"LRU_CACHE", {2, wrapArray([](Array& arr, const Query& q) -> bool {
            if (q.args.size() < 2) return true;
            int capacity = stoi(q.args[0]); 
            int qCount = stoi(q.args[1]);   
            vector<string> operations;
            for (size_t i = 2; i < q.args.size(); ++i) operations.push_back(q.args[i]);
            runLruCache(capacity, qCount, operations); 
            return true;
        })}},

        // --- ЛАБОРАТОРНАЯ РАБОТА 1 (СТАНДАРТНЫЕ МЕТОДЫ СТРУКТУР ДАННЫХ) ---
        {"MPUSH", {1, wrapArray([](Array& arr, const Query& q) -> bool { string val = q.args[0]; MPUSH(arr, val); return true; })}},
        {"MPUSH_INDEX", {2, wrapArray([](Array& arr, const Query& q) -> bool { int idx; if(!ParseInt(q.args[1], idx)) return false; string val = q.args[0]; MPUSH_INDEX(arr, val, idx); return true; })}},
        {"MDEL", {1, wrapArray([](Array& arr, const Query& q) -> bool { int idx; if(!ParseInt(q.args[0], idx)) return false; cout << MDEL(arr, idx) << endl; return true; })}},
        {"MREPLACE", {2, wrapArray([](Array& arr, const Query& q) -> bool { int idx; if(!ParseInt(q.args[1], idx)) return false; string val = q.args[0]; MREPLACE(arr, val, idx); return true; })}},
        {"MRECEIVE", {1, wrapArray([](Array& arr, const Query& q) -> bool { int idx; if(!ParseInt(q.args[0], idx)) return false; cout << MRECEIVE(arr, idx) << endl; return true; })}},
        {"MLEN", {0, wrapArray([](Array& arr, const Query&) -> bool { cout << MLEN(arr) << endl; return true; })}},
        {"MGET", {0, wrapArray([](Array& arr, const Query&) -> bool { MGET(arr); return true; })}},

        {"SPUSH", {1, wrapStack([](Stack& stack, const Query& q) -> bool { string val = q.args[0]; SPUSH(stack, val); cout << val << endl; return true; })}},
        {"SPOP", {0, wrapStack([](Stack& stack, const Query&) -> bool { cout << SPOP(stack) << endl; return true; })}},
        {"SGET", {0, wrapStack([](Stack& stack, const Query&) -> bool { SGET(stack); return true; })}},
        {"SGET_REVERSE", {0, wrapStack([](Stack& stack, const Query&) -> bool { SGET_REVERSE(stack); return true; })}},

        {"QPUSH", {1, wrapQueue([](Queue& queue, const Query& q) -> bool { string val = q.args[0]; QPUSH(queue, val); cout << val << endl; return true; })}},
        {"QPOP", {0, wrapQueue([](Queue& queue, const Query&) -> bool { cout << QPOP(queue) << endl; return true; })}},
        {"QGET", {0, wrapQueue([](Queue& queue, const Query&) -> bool { QGET(queue); return true; })}},
        {"QGET_BACK", {0, wrapQueue([](Queue& queue, const Query&) -> bool { QGET_BACK(queue); return true; })}},

        {"TINSERT", {1, wrapTree([](Tree& tree, const Query& q) -> bool { int num; if(!ParseInt(q.args[0], num)) return false; if (tree.size > 0 && TFIND(tree, num)) return false; TINSERT(tree, num); return true; })}},
        {"TDEL", {1, wrapTree([](Tree& tree, const Query& q) -> bool { int num; if(!ParseInt(q.args[0], num)) return false; TDEL(tree, num); return true; })}},
        {"TFIND", {1, wrapTree([](Tree& tree, const Query& q) -> bool { int num; if(!ParseInt(q.args[0], num)) return false; if(!tree.root) { cout << "Дерево пустое" << endl; return true; } cout << (TFIND(tree, num) ? "TRUE" : "FALSE") << endl; return true; })}},
        {"TGET_PREORDER", {0, wrapTree([](Tree& tree, const Query&) -> bool { cout << "Дерево полное: " << (TIS_COMPLETE(tree) ? "TRUE" : "FALSE") << endl; if(tree.size) TGET_PREORDER(tree.root); else cout << "Нет значений"; cout << endl; return true; })}},
        {"TGET_INORDER", {0, wrapTree([](Tree& tree, const Query&) -> bool { cout << "Дерево полное: " << (TIS_COMPLETE(tree) ? "TRUE" : "FALSE") << endl; if(tree.size) TGET_INORDER(tree.root); else cout << "Нет значений"; cout << endl; return true; })}},
        {"TGET_POSTORDER", {0, wrapTree([](Tree& tree, const Query&) -> bool { cout << "Дерево полное: " << (TIS_COMPLETE(tree) ? "TRUE" : "FALSE") << endl; if(tree.size) TGET_POSTORDER(tree.root); else cout << "Нет значений"; cout << endl; return true; })}},
        {"TGET_LEVELORDER", {0, wrapTree([](Tree& tree, const Query&) -> bool { cout << "Дерево полное: " << (TIS_COMPLETE(tree) ? "TRUE" : "FALSE") << endl; if(tree.size) TGET_LEVELORDER(tree); else cout << "Нет значений"; cout << endl; return true; })}},

        {"FPUSH_HEAD", {1, wrapFList([](FList& list, const Query& q) -> bool { string v = q.args[0]; FPUSH_HEAD(list, v); return true; })}},
        {"FPUSH_TAIL", {1, wrapFList([](FList& list, const Query& q) -> bool { string v = q.args[0]; FPUSH_TAIL(list, v); return true; })}},
        {"FPUSH_BEFORE", {2, wrapFList([](FList& list, const Query& q) -> bool { string v = q.args[0], t = q.args[1]; FPUSH_BEFORE(list, v, t); return true; })}},
        {"FPUSH_AFTER", {2, wrapFList([](FList& list, const Query& q) -> bool { string v = q.args[0], t = q.args[1]; FPUSH_AFTER(list, v, t); return true; })}},
        {"FDEL_HEAD", {0, wrapFList([](FList& list, const Query&) -> bool { FDEL_HEAD(list); return true; })}},
        {"FDEL_TAIL", {0, wrapFList([](FList& list, const Query&) -> bool { FDEL_TAIL(list); return true; })}},
        {"FDEL_BEFORE", {1, wrapFList([](FList& list, const Query& q) -> bool { string t = q.args[0]; FDEL_BEFORE(list, t); return true; })}},
        {"FDEL_AFTER", {1, wrapFList([](FList& list, const Query& q) -> bool { string t = q.args[0]; FDEL_AFTER(list, t); return true; })}},
        {"FDEL_VALUE", {1, wrapFList([](FList& list, const Query& q) -> bool { string t = q.args[0]; FDEL_VALUE(list, t); return true; })}},
        {"FFIND", {1, wrapFList([](FList& list, const Query& q) -> bool { string t = q.args[0]; int idx = FFIND(list, t); cout << (idx < 0 ? "Нет такого значения" : to_string(idx)) << endl; return true; })}},
        {"FLEN", {0, wrapFList([](FList& list, const Query&) -> bool { cout << FLEN(list) << endl; return true; })}},
        {"FGET", {0, wrapFList([](FList& list, const Query&) -> bool { FGET_CLASSIC(list); return true; })}},
        {"FGET_REVERSE", {0, wrapFList([](FList& list, const Query&) -> bool { FGET_REVERSE(list); return true; })}},

        {"LPUSH_HEAD", {1, wrapDList([](LList& list, const Query& q) -> bool { string v = q.args[0]; LPUSH_HEAD(list, v); return true; })}},
        {"LPUSH_TAIL", {1, wrapDList([](LList& list, const Query& q) -> bool { string v = q.args[0]; LPUSH_TAIL(list, v); return true; })}},
        {"LPUSH_BEFORE", {2, wrapDList([](LList& list, const Query& q) -> bool { string v = q.args[0], t = q.args[1]; LPUSH_BEFORE(list, v, t); return true; })}},
        {"LPUSH_AFTER", {2, wrapDList([](LList& list, const Query& q) -> bool { string v = q.args[0], t = q.args[1]; LPUSH_AFTER(list, v, t); return true; })}},
        {"LDEL_HEAD", {0, wrapDList([](LList& list, const Query&) -> bool { LDEL_HEAD(list); return true; })}},
        {"LDEL_TAIL", {0, wrapDList([](LList& list, const Query&) -> bool { LDEL_TAIL(list); return true; })}},
        {"LDEL_BEFORE", {1, wrapDList([](LList& list, const Query& q) -> bool { string t = q.args[0]; LDEL_BEFORE(list, t); return true; })}},
        {"LDEL_AFTER", {1, wrapDList([](LList& list, const Query& q) -> bool { string t = q.args[0]; LDEL_AFTER(list, t); return true; })}},
        {"LDEL_VALUE", {1, wrapDList([](LList& list, const Query& q) -> bool { string t = q.args[0]; LDEL_VALUE(list, t); return true; })}},
        {"LFIND", {1, wrapDList([](LList& list, const Query& q) -> bool { string t = q.args[0]; int idx = LFIND(list, t); cout << (idx < 0 ? "Нет такого значения" : to_string(idx)) << endl; return true; })}},
        {"LGET", {0, wrapDList([](LList& list, const Query&) -> bool { LGET(list); return true; })}},
        {"LGET_BACK", {0, wrapDList([](LList& list, const Query&) -> bool { LGET_BACK(list); return true; })}}
    };
    return table;
}

int main(int argc, char** argv) {
    Args args;
    if (!ParseArgs(argc, argv, args)) return 1;

    Query query = ParseQuery(args.query);
    if (query.command.empty() || query.name.empty()) {
        cout << "Неверный запрос\n";
        return 1;
    }

    // Изолированная обработка LRU_CACHE
    if (query.command == "LRU_CACHE") {
        if (query.args.size() >= 2) {
            int capacity = stoi(query.args[0]);
            int qCount = stoi(query.args[1]);
            vector<string> operations;
            for (size_t i = 2; i < query.args.size(); ++i) operations.push_back(query.args[i]);
            runLruCache(capacity, qCount, operations);
        } else {
            cout << "Ошибка: недостаточно аргументов для LRU_CACHE" << endl;
        }
        return 0;
    }

    auto& tbl = Table();
    auto it = tbl.find(query.command);
    if (it == tbl.end()) {
        cout << "Неизвестная команда: " << query.command << "\n";
        return 1;
    }
    const Command& cmd = it->second;
    if ((int)query.args.size() < cmd.argc) {
        cout << "Команда " << query.command << " требует минимум " << cmd.argc << " аргумент(ов)\n";
        return 1;
    }
    Type type = TypeByCommand(query.command);
    string requiredType = TypeName(type);
    DB db;
    db.filename = args.file;
    if (!db.Load()) return 1;
    Entry* entry = db.GetOrCreate(requiredType, query.name);
    if (!entry) return 1;
    if (!cmd.handler(*entry, query)) return 1;
    if (!db.Save()) return 1;
    return 0;
}

