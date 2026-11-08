package main

import (
	"fmt"
)

type Stack struct {
	data     []string
	top      int
	capacity int
}

func SCREATE(initCapacity int) Stack {
	if initCapacity <= 0 {
		initCapacity = 4
	}
	return Stack{
		data:     make([]string, initCapacity),
		top:      -1,
		capacity: initCapacity,
	}
}

func SPUSH(stack *Stack, val string) {
	if stack.top+1 >= stack.capacity {
		stack.capacity *= 2
		newData := make([]string, stack.capacity)
		copy(newData, stack.data)
		stack.data = newData
	}
	stack.top++
	stack.data[stack.top] = val
}

func SPOP(stack *Stack) string {
	if stack.top < 0 {
		return ""
	}
	val := stack.data[stack.top]
	stack.top--
	return val
}

func SGET(stack Stack) {
	for i := stack.top; i >= 0; i-- {
		fmt.Print(stack.data[i], " ")
	}
	fmt.Println()
}

func SGET_REVERSE(stack Stack) {
	for i := 0; i <= stack.top; i++ {
		fmt.Print(stack.data[i], " ")
	}
	fmt.Println()
}

func SDESTROY(stack *Stack) {
	stack.data = nil
	stack.top = -1
	stack.capacity = 0
}