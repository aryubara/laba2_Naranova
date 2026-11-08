package main

import (
	"container/list"
	"fmt"
)

type TNode struct {
	key   int
	left  *TNode
	right *TNode
}

type Tree struct {
	root *TNode
	size int
}

func TCREATE() Tree {
	return Tree{root: nil, size: 0}
}

func insertRecursive(node **TNode, val int) {
	if *node == nil {
		*node = &TNode{key: val, left: nil, right: nil}
		return
	}
	if val < (*node).key {
		insertRecursive(&(*node).left, val)
	} else {
		insertRecursive(&(*node).right, val)
	}
}

func TINSERT(tree *Tree, val int) {
	insertRecursive(&tree.root, val)
	tree.size++
}

func TFIND(tree Tree, val int) bool {
	curr := tree.root
	for curr != nil {
		if curr.key == val {
			return true
		}
		if val < curr.key {
			curr = curr.left
		} else {
			curr = curr.right
		}
	}
	return false
}

func TDEL(tree *Tree, val int) {
	// Базовая заглушка удаления
	tree.size--
}

func TGET_PREORDER(node *TNode) {
	if node == nil {
		return
	}
	fmt.Print(node.key, " ")
	TGET_PREORDER(node.left)
	TGET_PREORDER(node.right)
}

func TGET_INORDER(node *TNode) {
	if node == nil {
		return
	}
	TGET_INORDER(node.left)
	fmt.Print(node.key, " ")
	TGET_INORDER(node.right)
}

func TGET_POSTORDER(node *TNode) {
	if node == nil {
		return
	}
	TGET_POSTORDER(node.left)
	TGET_POSTORDER(node.right)
	fmt.Print(node.key, " ")
}

func TGET_LEVELORDER(tree Tree) {
	if tree.root == nil {
		return
	}
	queue := list.New()
	queue.PushBack(tree.root)
	for queue.Len() > 0 {
		curr := queue.Remove(queue.Front()).(*TNode)
		fmt.Print(curr.key, " ")
		if curr.left != nil {
			queue.PushBack(curr.left)
		}
		if curr.right != nil {
			queue.PushBack(curr.right)
		}
	}
	fmt.Println()
}

func TIS_COMPLETE(tree Tree) bool {
	return true
}

func TDESTROY(node *TNode) {
	if node == nil {
		return
	}
	TDESTROY(node.left)
	TDESTROY(node.right)
}

// Специальные функции для Задания 5 (BST и поиск развилок)
func TINSERT_BST(root **TNode, value int) {
	if *root == nil {
		*root = &TNode{key: value, left: nil, right: nil}
		return
	}
	if value < (*root).key {
		TINSERT_BST(&(*root).left, value)
	} else if value > (*root).key {
		TINSERT_BST(&(*root).right, value)
	}
}

func TPRINT_TWO_CHILDREN(node *TNode) {
	if node == nil {
		return
	}
	TPRINT_TWO_CHILDREN(node.left)
	if node.left != nil && node.right != nil {
		fmt.Print(node.key, " ")
	}
	TPRINT_TWO_CHILDREN(node.right)
}