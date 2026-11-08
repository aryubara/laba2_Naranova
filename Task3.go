package main

import (
	"strconv"
)

func solveTurtles(n int, pairs []string) int {
	if len(pairs) == 0 || n <= 0 {
		return 0
	}

	uniquePositions := SETCREATE(n + 10)
	truthfulCount := 0

	for i := 0; i+1 < len(pairs); i += 2 {
		a, err1 := strconv.Atoi(pairs[i])
		b, err2 := strconv.Atoi(pairs[i+1])
		if err1 != nil || err2 != nil {
			continue
		}

		if a < 0 || b < 0 || a+b+1 != n || a >= n {
			continue
		}

		posKey := "pos_" + strconv.Itoa(a)
		if SET_AT(uniquePositions, posKey) {
			continue
		}

		SETADD(&uniquePositions, posKey)
		truthfulCount++
	}

	SETDESTROY(&uniquePositions)
	return truthfulCount
}