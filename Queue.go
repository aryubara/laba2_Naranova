package main

import (
	"fmt"
)

type Queue struct {
	data     []string
	head     int
	tail     int
	size     int
	capacity int
}

func QCREATE(initCapacity int) Queue {
	if initCapacity <= 0 {
		initCapacity = 4
	}
	return Queue{
		data:     make([]string, initCapacity),
		head:     0,
		tail:     0,
		size:     0,
		capacity: initCapacity,
	}
}

func QPUSH(queue *Queue, val string) {
	if queue.size >= queue.capacity {
		newCap := queue.capacity * 2
		newData := make([]string, newCap)
		for i := 0; i < queue.size; i++ {
			newData[i] = queue.data[(queue.head+i)%queue.capacity]
		}
		queue.data = newData
		queue.head = 0
		queue.tail = queue.size
		queue.capacity = newCap
	}
	queue.data[queue.tail] = val
	queue.tail = (queue.tail + 1) % queue.capacity
	queue.size++
}

func QPOP(queue *Queue) string {
	if queue.size <= 0 {
		return ""
	}
	val := queue.data[queue.head]
	queue.head = (queue.head + 1) % queue.capacity
	queue.size--
	return val
}

func QGET(queue Queue) {
	for i := 0; i < queue.size; i++ {
		fmt.Print(queue.data[(queue.head+i)%queue.capacity], " ")
	}
	fmt.Println()
}

func QGET_BACK(queue Queue) {
	for i := queue.size - 1; i >= 0; i-- {
		fmt.Print(queue.data[(queue.head+i)%queue.capacity], " ")
	}
	fmt.Println()
}

func QDESTROY(queue *Queue) {
	queue.data = nil
	queue.head = 0
	queue.tail = 0
	queue.size = 0
	queue.capacity = 0
}