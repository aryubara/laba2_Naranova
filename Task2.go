package main

import (
	"fmt"
)

func handleSetAdd(set *Set, val string) bool {
	if SETADD(set, val) {
		fmt.Println("-> OK")
		return true
	}
	fmt.Println("-> Element already exists")
	return false
}

func handleSetDel(set *Set, val string) bool {
	if SETDEL(set, val) {
		fmt.Println("-> OK")
		return true
	}
	fmt.Println("-> Element not found")
	return false
}

func handleSetAt(set Set, val string) bool {
	if SET_AT(set, val) {
		fmt.Println("-> TRUE")
		return true
	}
	fmt.Println("-> FALSE")
	return false
}