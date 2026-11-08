package main

type Set struct {
	buckets  []FList
	capacity int
	size     int
}

func hash_func(str string, capacity int) int {
	var hash uint64 = 5381
	for i := 0; i < len(str); i++ {
		hash = ((hash << 5) + hash) + uint64(str[i])
	}
	return int(hash % uint64(capacity))
}

func SETCREATE(initCapacity int) Set {
	buckets := make([]FList, initCapacity)
	for i := 0; i < initCapacity; i++ {
		buckets[i] = FCREATE()
	}
	return Set{
		buckets:  buckets,
		capacity: initCapacity,
		size:     0,
	}
}

func SET_AT(set Set, value string) bool {
	index := hash_func(value, set.capacity)
	return FFIND(set.buckets[index], value) != -1
}

func SETADD(set *Set, value string) bool {
	if SET_AT(*set, value) {
		return false
	}
	index := hash_func(value, set.capacity)
	FPUSH_HEAD(&set.buckets[index], value)
	set.size++
	return true
}

func SETDEL(set *Set, value string) bool {
	index := hash_func(value, set.capacity)
	if FFIND(set.buckets[index], value) != -1 {
		FDEL_VALUE(&set.buckets[index], value)
		set.size--
		return true
	}
	return false
}

func SETDESTROY(set *Set) {
	for i := 0; i < set.capacity; i++ {
		FDESTROY(&set.buckets[i])
	}
	set.buckets = nil
	set.capacity = 0
	set.size = 0
}