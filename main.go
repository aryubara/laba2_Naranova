package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

type Args struct {
	file  string
	query string
}

func parseArgs() (Args, bool) {
	var args Args
	argsList := os.Args[1:]
	for i := 0; i < len(argsList); i++ {
		if argsList[i] == "--file" && i+1 < len(argsList) {
			args.file = argsList[i+1]
			i++
		} else if argsList[i] == "--query" && i+1 < len(argsList) {
			args.query = argsList[i+1]
			i++
		} else {
			fmt.Println("Неизвестный аргумент:", argsList[i])
			return args, false
		}
	}
	if args.file == "" || args.query == "" {
		fmt.Println("Использование: go run *.go --file <file> --query '<cmd>'")
		return args, false
	}
	return args, true
}

type Query struct {
	command string
	name    string
	args    []string
}

func parseQuery(str string) Query {
	parts := strings.Fields(str)
	var q Query
	if len(parts) > 0 {
		q.command = parts[0]
	}
	if len(parts) > 1 {
		q.name = parts[1]
	}
	if len(parts) > 2 {
		q.args = parts[2:]
	}
	return q
}

type Entry struct {
	typeName string
	name     string
	values   []string
}

type DB struct {
	filename string
	entries  []Entry
}

func (db *DB) Load() bool {
	file, err := os.Open(db.filename)
	if err != nil {
		return true
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		line := scanner.Text()
		if strings.TrimSpace(line) == "" {
			continue
		}
		headerParts := strings.Fields(line)
		if len(headerParts) < 3 {
			continue
		}
		var entry Entry
		entry.typeName = headerParts[0]
		entry.name = headerParts[1]
		size, _ := strconv.Atoi(headerParts[2])

		for i := 0; i < size && scanner.Scan(); i++ {
			entry.values = append(entry.values, scanner.Text())
		}
		db.entries = append(db.entries, entry)
	}
	return true
}

func (db *DB) Save() bool {
	file, err := os.Create(db.filename)
	if err != nil {
		return false
	}
	defer file.Close()

	for _, entry := range db.entries {
		fmt.Fprintf(file, "%s %s %d\n", entry.typeName, entry.name, len(entry.values))
		for _, val := range entry.values {
			fmt.Fprintln(file, val)
		}
	}
	return true
}

func (db *DB) GetOrCreate(typeName string, name string) *Entry {
	for i := range db.entries {
		if db.entries[i].name == name {
			if db.entries[i].typeName != typeName {
				fmt.Println("Тип не совпадает: ожидается", db.entries[i].typeName)
				return nil
			}
			return &db.entries[i]
		}
	}
	db.entries = append(db.entries, Entry{typeName: typeName, name: name, values: []string{}})
	return &db.entries[len(db.entries)-1]
}

func toArray(vals []string) Array {
	arr := MCREATE(len(vals))
	if len(vals) == 0 {
		arr = MCREATE(4)
	}
	for _, v := range vals {
		MPUSH(&arr, v)
	}
	return arr
}

func fromArray(arr *Array, entry *Entry) {
	entry.values = nil
	for i := 0; i < arr.size; i++ {
		entry.values = append(entry.values, arr.data[i])
	}
	MDESTROY(arr)
}

func toStack(vals []string) Stack {
	stack := SCREATE(4)
	for _, v := range vals {
		SPUSH(&stack, v)
	}
	return stack
}

func fromStack(stack *Stack, entry *Entry) {
	entry.values = nil
	for i := 0; i <= stack.top; i++ {
		entry.values = append(entry.values, stack.data[i])
	}
	SDESTROY(stack)
}

func toTree(vals []string) Tree {
	tree := TCREATE()
	for _, str := range vals {
		item, err := strconv.Atoi(str)
		if err == nil {
			TINSERT(&tree, item)
		} else if str == "null" {
			TINSERT(&tree, -999)
		}
	}
	return tree
}

func fromTree(tree *Tree, entry *Entry) {
	entry.values = nil
	if tree.root == nil {
		return
	}
	q := []*TNode{tree.root}
	for len(q) > 0 {
		curr := q[0]
		q = q[1:]
		entry.values = append(entry.values, strconv.Itoa(curr.key))
		if curr.left != nil {
			q = append(q, curr.left)
		}
		if curr.right != nil {
			q = append(q, curr.right)
		}
	}
	TDESTROY(tree.root)
	tree.root = nil
}

func toSet(vals []string) Set {
	set := SETCREATE(100)
	for _, str := range vals {
		copyStr := str
		SETADD(&set, copyStr)
	}
	return set
}

func fromSet(set *Set, entry *Entry) {
	entry.values = nil
	for i := 0; i < set.capacity; i++ {
		curr := set.buckets[i].head
		for curr != nil {
			entry.values = append(entry.values, curr.data)
			curr = curr.next
		}
	}
	SETDESTROY(set)
}

func typeByName(cmd string) string {
	if strings.HasPrefix(cmd, "SET") || cmd == "TURTLES" {
		return "SET"
	}
	if cmd == "LRU_CACHE" || cmd == "HIST" || cmd == "ROUTE_OPT" || cmd == "HASH_TEST" {
		return "ARRAY"
	}
	if len(cmd) == 0 {
		return "UNKNOWN"
	}
	switch cmd[0] {
	case 'M':
		return "ARRAY"
	case 'F':
		return "FLIST"
	case 'L':
		return "DLIST"
	case 'S':
		return "STACK"
	case 'Q':
		return "QUEUE"
	case 'T':
		return "TREE"
	default:
		return "UNKNOWN"
	}
}

func main() {
	args, ok := parseArgs()
	if !ok {
		os.Exit(1)
	}

	query := parseQuery(args.query)
	if query.command == "" || query.name == "" {
		fmt.Println("Неверный запрос")
		os.Exit(1)
	}

	if query.command == "LRU_CACHE" {
		cap, qc, ops := parseCapacityAndQueries(query.args)
		runLruCache(cap, qc, ops)
		return
	}

	typeName := typeByName(query.command)
	db := DB{filename: args.file}
	if !db.Load() {
		os.Exit(1)
	}
	entry := db.GetOrCreate(typeName, query.name)
	if entry == nil {
		os.Exit(1)
	}

	switch query.command {
	case "SEVAL":
		s := toStack(entry.values)
		expr := query.args[0]
		postfix := toPostfix(expr)
		res := evaluatePostfix(postfix)
		fmt.Println("->", res)
		SPUSH(&s, strconv.Itoa(res))
		fromStack(&s, entry)
	case "SETADD":
		s := toSet(entry.values)
		handleSetAdd(&s, query.args[0])
		fromSet(&s, entry)
	case "SETDEL":
		s := toSet(entry.values)
		handleSetDel(&s, query.args[0])
		fromSet(&s, entry)
	case "SET_AT":
		s := toSet(entry.values)
		handleSetAt(s, query.args[0])
		fromSet(&s, entry)
	case "TURTLES":
		s := toSet(entry.values)
		n, _ := strconv.Atoi(query.args[0])
		fmt.Println("->", solveTurtles(n, query.args[1:]))
		fromSet(&s, entry)
	case "HIST":
		arr := toArray(entry.values)
		text := strings.Join(query.args, " ")
		printHistogram(text)
		fromArray(&arr, entry)
	case "BST_FORKS":
		t := toTree(entry.values)
		runBstForks(query.args)
		fromTree(&t, entry)
	case "HASH_TEST":
		arr := toArray(entry.values)
		for _, arg := range query.args {
			n, err := strconv.Atoi(arg)
			if err == nil && n > 0 {
				runEmpiricalAnalysis(n)
			}
		}
		fromArray(&arr, entry)
	case "ROUTE_OPT":
		arr := toArray(entry.values)
		var n int
		if len(query.args) > 0 {
			if val, err := strconv.Atoi(query.args[0]); err == nil {
				n = val
			}
		}
		if n > 0 {
			for _, arg := range query.args {
				val, err := strconv.Atoi(arg)
				if err == nil && val > 0 {
					runEmpiricalAnalysis(val)
				}
			}
		} else {
			if len(query.args) >= 2 {
				startServer := query.args[0]
				endServer := query.args[1]
				htRoutes := createRouteTable(199)
				for i := 2; i+2 < len(query.args); i += 3 {
					key := query.args[i] + "->" + query.args[i+1]
					delay, _ := strconv.Atoi(query.args[i+2])
					insertDoubleHash(&htRoutes, key, delay)
				}
				minDelay := 999999
				bestPath := ""
				visited := make(map[string]bool)
				findMinDelay(htRoutes, startServer, endServer, 0, startServer, &minDelay, &bestPath, visited)
				if minDelay == 999999 {
					fmt.Println("Вывод: Маршрут не найден")
				} else {
					fmt.Printf("Вывод: минимальная задержка = %d (маршрут \"%s\")\n", minDelay, bestPath)
				}
				destroyRouteTable(&htRoutes)
			}
		}
		fromArray(&arr, entry)
	case "MPUSH":
		arr := toArray(entry.values)
		MPUSH(&arr, query.args[0])
		fromArray(&arr, entry)
	case "SPUSH":
		st := toStack(entry.values)
		SPUSH(&st, query.args[0])
		fmt.Println(query.args[0])
		fromStack(&st, entry)
	case "SPOP":
		st := toStack(entry.values)
		fmt.Println(SPOP(&st))
		fromStack(&st, entry)
	default:
		fmt.Println("Неизвестная команда:", query.command)
		os.Exit(1)
	}

	if !db.Save() {
		os.Exit(1)
	}
}