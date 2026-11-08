package main

import (
	"fmt"
	"strings"
	"time"
)

type RouteEdge struct {
	key        string
	value      int
	isOccupied bool
	isDeleted  bool
}

type RouteHashTable struct {
	table    []RouteEdge
	capacity int
	size     int
}

func isPrime(num int) bool {
	if num < 2 {
		return false
	}
	if num%2 == 0 {
		return num == 2
	}
	for i := 3; i*i <= num; i += 2 {
		if num%i == 0 {
			return false
		}
	}
	return true
}

func nextPrime(num int) int {
	for !isPrime(num) {
		num++
	}
	return num
}

func createRouteTable(cap int) RouteHashTable {
	return RouteHashTable{
		table:    make([]RouteEdge, cap),
		capacity: cap,
		size:     0,
	}
}

func destroyRouteTable(ht *RouteHashTable) {
	ht.table = nil
	ht.capacity = 0
	ht.size = 0
}

func hash_lp(str string, capacity int) int {
	var hash uint64 = 5381
	for i := 0; i < len(str); i++ {
		hash = ((hash << 5) + hash) + uint64(str[i])
	}
	return int(hash % uint64(capacity))
}

func hash_dh2(str string, capacity int) int {
	var hash uint64 = 0
	for i := 0; i < len(str); i++ {
		hash = uint64(str[i]) + (hash << 6) + (hash << 16) - hash
	}
	return int((hash % uint64(capacity-2)) + 1)
}

func insertLinear(ht *RouteHashTable, key string, value int) {
	idx := hash_lp(key, ht.capacity)
	firstDeleted := -1

	for i := 0; i < ht.capacity; i++ {
		if ht.table[idx].isOccupied {
			if ht.table[idx].key == key {
				ht.table[idx].value = value
				return
			}
		} else {
			if !ht.table[idx].isDeleted {
				target := idx
				if firstDeleted != -1 {
					target = firstDeleted
				}
				ht.table[target].key = key
				ht.table[target].value = value
				ht.table[target].isOccupied = true
				ht.table[target].isDeleted = false
				ht.size++
				return
			} else if firstDeleted == -1 {
				firstDeleted = idx
			}
		}
		idx = (idx + 1) % ht.capacity
	}

	if firstDeleted != -1 {
		ht.table[firstDeleted].key = key
		ht.table[firstDeleted].value = value
		ht.table[firstDeleted].isOccupied = true
		ht.table[firstDeleted].isDeleted = false
		ht.size++
	}
}

func searchLinear(ht RouteHashTable, key string) (int, bool) {
	idx := hash_lp(key, ht.capacity)
	for i := 0; i < ht.capacity; i++ {
		if !ht.table[idx].isOccupied && !ht.table[idx].isDeleted {
			return 0, false
		}
		if ht.table[idx].isOccupied && ht.table[idx].key == key {
			return ht.table[idx].value, true
		}
		idx = (idx + 1) % ht.capacity
	}
	return 0, false
}

func removeLinear(ht *RouteHashTable, key string) bool {
	idx := hash_lp(key, ht.capacity)
	for i := 0; i < ht.capacity; i++ {
		if !ht.table[idx].isOccupied && !ht.table[idx].isDeleted {
			return false
		}
		if ht.table[idx].isOccupied && ht.table[idx].key == key {
			ht.table[idx].isOccupied = false
			ht.table[idx].isDeleted = true
			ht.size--
			return true
		}
		idx = (idx + 1) % ht.capacity
	}
	return false
}

func insertDoubleHash(ht *RouteHashTable, key string, value int) {
	idx := hash_lp(key, ht.capacity)
	step := hash_dh2(key, ht.capacity)
	firstDeleted := -1

	for i := 0; i < ht.capacity; i++ {
		if ht.table[idx].isOccupied {
			if ht.table[idx].key == key {
				ht.table[idx].value = value
				return
			}
		} else {
			if !ht.table[idx].isDeleted {
				target := idx
				if firstDeleted != -1 {
					target = firstDeleted
				}
				ht.table[target].key = key
				ht.table[target].value = value
				ht.table[target].isOccupied = true
				ht.table[target].isDeleted = false
				ht.size++
				return
			} else if firstDeleted == -1 {
				firstDeleted = idx
			}
		}
		idx = (idx + step) % ht.capacity
	}

	if firstDeleted != -1 {
		ht.table[firstDeleted].key = key
		ht.table[firstDeleted].value = value
		ht.table[firstDeleted].isOccupied = true
		ht.table[firstDeleted].isDeleted = false
		ht.size++
	}
}

func searchDoubleHash(ht RouteHashTable, key string) (int, bool) {
	idx := hash_lp(key, ht.capacity)
	step := hash_dh2(key, ht.capacity)
	for i := 0; i < ht.capacity; i++ {
		if !ht.table[idx].isOccupied && !ht.table[idx].isDeleted {
			return 0, false
		}
		if ht.table[idx].isOccupied && ht.table[idx].key == key {
			return ht.table[idx].value, true
		}
		idx = (idx + step) % ht.capacity
	}
	return 0, false
}

func removeDoubleHash(ht *RouteHashTable, key string) bool {
	idx := hash_lp(key, ht.capacity)
	step := hash_dh2(key, ht.capacity)
	for i := 0; i < ht.capacity; i++ {
		if !ht.table[idx].isOccupied && !ht.table[idx].isDeleted {
			return false
		}
		if ht.table[idx].isOccupied && ht.table[idx].key == key {
			ht.table[idx].isOccupied = false
			ht.table[idx].isDeleted = true
			ht.size--
			return true
		}
		idx = (idx + step) % ht.capacity
	}
	return false
}

func runEmpiricalAnalysis(n int) {
	if n <= 0 {
		return
	}
	cap := nextPrime(n*2 + 1)
	keys := make([]string, n)
	for i := 0; i < n; i++ {
		keys[i] = fmt.Sprintf("Key_%d", i)
	}

	htLinear := createRouteTable(cap)
	htDouble := createRouteTable(cap)

	startLin := time.Now()
	for i := 0; i < n; i++ {
		insertLinear(&htLinear, keys[i], i)
	}
	for i := 0; i < n; i++ {
		searchLinear(htLinear, keys[i])
	}
	for i := 0; i < n; i++ {
		removeLinear(&htLinear, keys[i])
	}
	timeLin := time.Since(startLin).Microseconds()

	startDbl := time.Now()
	for i := 0; i < n; i++ {
		insertDoubleHash(&htDouble, keys[i], i)
	}
	for i := 0; i < n; i++ {
		searchDoubleHash(htDouble, keys[i])
	}
	for i := 0; i < n; i++ {
		removeDoubleHash(&htDouble, keys[i])
	}
	timeDbl := time.Since(startDbl).Microseconds()

	fmt.Printf("Линейное апробирование (%d эл.): %d мкс\n", n, timeLin)
	fmt.Printf("Двойное хеширование (%d эл.): %d мкс\n", n, timeDbl)

	destroyRouteTable(&htLinear)
	destroyRouteTable(&htDouble)
}

func findMinDelay(ht RouteHashTable, current string, dest string, currentDelay int, currentPath string, minDelay *int, bestPath *string, visited map[string]bool) {
	if current == dest {
		if currentDelay < *minDelay {
			*minDelay = currentDelay
			*bestPath = currentPath
		}
		return
	}

	visited[current] = true
	for i := 0; i < ht.capacity; i++ {
		if ht.table[i].isOccupied {
			key := ht.table[i].key
			pos := strings.Index(key, "->")
			if pos == -1 {
				continue
			}
			from := key[:pos]
			to := key[pos+2:]
			if from == current && !visited[to] {
				findMinDelay(ht, to, dest, currentDelay+ht.table[i].value, currentPath+" -> "+to, minDelay, bestPath, visited)
			}
		}
	}
	visited[current] = false
}