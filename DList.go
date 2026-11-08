package main

import (
	"fmt"
)

type LNode struct {
	data string
	prev *LNode
	next *LNode
}

type LList struct {
	head *LNode
	tail *LNode
	size int
}

func LCREATE() LList {
	return LList{head: nil, tail: nil, size: 0}
}

func LPUSH_HEAD(list *LList, val string) {
	newNode := &LNode{data: val, prev: nil, next: list.head}
	if list.head != nil {
		list.head.prev = newNode
	} else {
		list.tail = newNode
	}
	list.head = newNode
	list.size++
}

func LPUSH_TAIL(list *LList, val string) {
	newNode := &LNode{data: val, prev: list.tail, next: nil}
	if list.tail != nil {
		list.tail.next = newNode
	} else {
		list.head = newNode
	}
	list.tail = newNode
	list.size++
}

func LPUSH_BEFORE(list *LList, val string, target string) {
	curr := list.head
	for curr != nil && curr.data != target {
		curr = curr.next
	}
	if curr != nil {
		if curr == list.head {
			LPUSH_HEAD(list, val)
			return
		}
		newNode := &LNode{data: val, prev: curr.prev, next: curr}
		curr.prev.next = newNode
		curr.prev = newNode
		list.size++
	}
}

func LPUSH_AFTER(list *LList, val string, target string) {
	curr := list.head
	for curr != nil && curr.data != target {
		curr = curr.next
	}
	if curr != nil {
		if curr == list.tail {
			LPUSH_TAIL(list, val)
			return
		}
		newNode := &LNode{data: val, prev: curr, next: curr.next}
		curr.next.prev = newNode
		curr.next = newNode
		list.size++
	}
}

func LDEL_HEAD(list *LList) {
	if list.head == nil {
		return
	}
	list.head = list.head.next
	if list.head != nil {
		list.head.prev = nil
	} else {
		list.tail = nil
	}
	list.size--
}

func LDEL_TAIL(list *LList) {
	if list.tail == nil {
		return
	}
	list.tail = list.tail.prev
	if list.tail != nil {
		list.tail.next = nil
	} else {
		list.head = nil
	}
	list.size--
}

func LDEL_BEFORE(list *LList, target string) {
	curr := list.head
	for curr != nil && curr.data != target {
		curr = curr.next
	}
	if curr != nil && curr.prev != nil {
		targetNode := curr.prev
		if targetNode == list.head {
			LDEL_HEAD(list)
		} else {
			targetNode.prev.next = curr
			curr.prev = targetNode.prev
			list.size--
		}
	}
}

func LDEL_AFTER(list *LList, target string) {
	curr := list.head
	for curr != nil && curr.data != target {
		curr = curr.next
	}
	if curr != nil && curr.next != nil {
		targetNode := curr.next
		if targetNode == list.tail {
			LDEL_TAIL(list)
		} else {
			curr.next = targetNode.next
			targetNode.next.prev = curr
			list.size--
		}
	}
}

func LDEL_VALUE(list *LList, target string) {
	curr := list.head
	for curr != nil && curr.data != target {
		curr = curr.next
	}
	if curr != nil {
		if curr == list.head {
			LDEL_HEAD(list)
		} else if curr == list.tail {
			LDEL_TAIL(list)
		} else {
			curr.prev.next = curr.next
			curr.next.prev = curr.prev
			list.size--
		}
	}
}

func LFIND(list LList, target string) int {
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

func LGET(list LList) {
	curr := list.head
	for curr != nil {
		fmt.Print(curr.data, " ")
		curr = curr.next
	}
	fmt.Println()
}

func LGET_BACK(list LList) {
	curr := list.tail
	for curr != nil {
		fmt.Print(curr.data, " ")
		curr = curr.prev
	}
	fmt.Println()
}

func LDESTROY(list *LList) {
	list.head = nil
	list.tail = nil
	list.size = 0
}