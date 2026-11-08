package main

import (
	"fmt"
	"strconv"
)

func runLruCache(capacity int, qCount int, operations []string) {
	cacheKeys := MCREATE(capacity)
	cacheValues := MCREATE(capacity)

	i := 0
	processedQueries := 0

	for i < len(operations) && processedQueries < qCount {
		op := operations[i]

		if op == "SET" && i+2 < len(operations) {
			key := operations[i+1]
			val := operations[i+2]
			i += 3
			processedQueries++

			foundIdx := -1
			for k := 0; k < cacheKeys.size; k++ {
				if cacheKeys.data[k] == key {
					foundIdx = k
					break
				}
			}

			if foundIdx != -1 {
				for k := foundIdx; k < cacheKeys.size-1; k++ {
					cacheKeys.data[k] = cacheKeys.data[k+1]
					cacheValues.data[k] = cacheValues.data[k+1]
				}
				cacheKeys.data[cacheKeys.size-1] = key
				cacheValues.data[cacheKeys.size-1] = val
			} else {
				if cacheKeys.size >= capacity {
					for k := 0; k < cacheKeys.size-1; k++ {
						cacheKeys.data[k] = cacheKeys.data[k+1]
						cacheValues.data[k] = cacheValues.data[k+1]
					}
					cacheKeys.size--
					cacheValues.size--
				}
				MPUSH(&cacheKeys, key)
				MPUSH(&cacheValues, val)
			}
		} else if op == "GET" && i+1 < len(operations) {
			key := operations[i+1]
			i += 2
			processedQueries++

			foundIdx := -1
			for k := 0; k < cacheKeys.size; k++ {
				if cacheKeys.data[k] == key {
					foundIdx = k
					break
				}
			}

			if foundIdx != -1 {
				val := cacheValues.data[foundIdx]
				for k := foundIdx; k < cacheKeys.size-1; k++ {
					cacheKeys.data[k] = cacheKeys.data[k+1]
					cacheValues.data[k] = cacheValues.data[k+1]
				}
				cacheKeys.data[cacheKeys.size-1] = key
				cacheValues.data[cacheKeys.size-1] = val
				fmt.Print(val, " ")
			} else {
				fmt.Print("-1 ")
			}
		} else {
			i++
		}
	}
	fmt.Println()
	MDESTROY(&cacheKeys)
	MDESTROY(&cacheValues)
}

func parseCapacityAndQueries(args []string) (int, int, []string) {
	if len(args) < 2 {
		return 0, 0, nil
	}
	cap, _ := strconv.Atoi(args[0])
	qc, _ := strconv.Atoi(args[1])
	var ops []string
	if len(args) > 2 {
		ops = args[2:]
	}
	return cap, qc, ops
}