package main

import (
	"strconv"
)

func runBstForks(args []string) {
	var bstRoot *TNode = nil
	for _, arg := range args {
		val, err := strconv.Atoi(arg)
		if err != nil {
			continue
		}
		if val == 0 {
			break
		}
		TINSERT_BST(&bstRoot, val)
	}
	TPRINT_TWO_CHILDREN(bstRoot)
	println()
	TDESTROY(bstRoot)
}