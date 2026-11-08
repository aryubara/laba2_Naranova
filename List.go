package main

import (
	"fmt"
)

type FNode struct {
	data string
	next *FNode
}

type FList struct {
	head *FNode
	size int
}

func FCREATE() FList {
	return FList{head: nil, size: 0}
}

func FPUSH_HEAD(list *FList, val string) {
	newNode := &FNode{data: val, next: list.head}
	list.head = newNode
	list.size++
}

func FPUSH_TAIL(list *FList, val string) {
	newNode := &FNode{data: val, next: nil}
	if list.head == nil {
		list.head = newNode
	} else {
		curr := list.head
		for curr.next != nil {
			curr = curr.next
		}
		curr.next = newNode
	}
	list.size++
}

func FPUSH_BEFORE(list *FList, val string, target string) {
	if list.head == nil {
		return
	}
	if list.head.data == target {
		FPUSH_HEAD(list, val)
		return
	}
	curr := list.head
	for curr.next != nil && curr.next.data != target {
		curr = curr.next
	}
	if curr.next != nil {
		newNode := &FNode{data: val, next: curr.next}
		curr.next = newNode
		list.size++
	}
}

func FPUSH_AFTER(list *FList, val string, target string) {
	curr := list.head
	for curr != nil && curr.data != target {
		curr = curr.next
	}
	if curr != nil {
		newNode := &FNode{data: val, next: curr.next}
		curr.next = newNode
		list.size++
	}
}

func FDEL_HEAD(list *FList) {
	if list.head == nil {
		return
	}
	list.head = list.head.next
	list.size--
}

func FDEL_TAIL(list *FList) {
	if list.head == nil {
		return
	}
	if list.head.next == nil {
		list.head = nil
		list.size = 0
		return
	}
	curr := list.head
	for curr.next.next != nil {
		curr = curr.next
	}
	curr.next = nil
	list.size--
}

func FDEL_BEFORE(list *FList, target string) {
	if list.head == nil || list.head.next == nil {
		return
	}
	if list.head.next.data == target {
		FDEL_HEAD(list)
		return
	}
	curr := list.head
	for curr.next.next != nil && curr.next.next.data != target {
		curr = curr.next
	}
	if curr.next.next != nil {
		curr.next = curr.next.next
		list.size--
	}
}

func FDEL_AFTER(list *FList, target string) {
	curr := list.head
	for curr != nil && curr.data != target {
		curr = curr.next
	}
	if curr != nil && curr.next != nil {
		curr.next = curr.next.next
		list.size--
	}
}

func FDEL_VALUE(list *FList, target string) {
	if list.head == nil {
		return
	}
	if list.head.data == target {
		FDEL_HEAD(list)
		return
	}
	curr := list.head
	for curr.next != nil && curr.next.data != target {
		curr = curr.next
	}
	if curr.next != nil {
		curr.next = curr.next.next
		list.size--
	}
}

func FFIND(list FList, target string) int {
	curr := list.head
	idx := 0
	for curr != nil {
		if curr.data == target {
			return idx
		}
		curr = curr.next
		idx++
	}
	return -1
}

func FLEN(list FList) int {
	return list.size
}

func FGET_CLASSIC(list FList) {
	curr := list.head
	for curr != nil {
		fmt.Print(curr.data, " ")
		curr = curr.next
	}
	fmt.Println()
}

func FGET_REVERSE(list FList) {
	var helper func(node *FNode)
	helper = func(node *FNode) {
		if node == nil {
			return
		}
		helper(node.next)
		fmt.Print(node.data, " ")
	}
	helper(list.head)
	fmt.Println()
}

func FDESTROY(list *FList) {
	list.head = nil
	list.size = 0
}