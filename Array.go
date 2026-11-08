package main

import (
	"fmt"
	"os"
)

type Array struct {
	data     []string
	size     int
	capacity int
}

func MCREATE(initCapacity int) Array {
	if initCapacity <= 0 {
		initCapacity = 4
	}
	return Array{
		data:     make([]string, initCapacity),
		size:     0,
		capacity: initCapacity,
	}
}

func MDISPLAY(arr Array) {
	for i := 0; i < arr.size; i++ {
		fmt.Print(arr.data[i], " ")
	}
	fmt.Println()
}

func MPUSH(arr *Array, val string) {
	if arr.size >= arr.capacity {
		arr.capacity *= 2
		newData := make([]string, arr.capacity)
		copy(newData, arr.data)
		arr.data = newData
	}
	arr.data[arr.size] = val
	arr.size++
}

func MPUSH_INDEX(arr *Array, val string, index int) {
	if index < 0 || index > arr.size {
		return
	}
	if arr.size >= arr.capacity {
		arr.capacity *= 2
		newData := make([]string, arr.capacity)
		copy(newData, arr.data)
		arr.data = newData
	}
	for i := arr.size; i > index; i-- {
		arr.data[i] = arr.data[i-1]
	}
	arr.data[index] = val
	arr.size++
}

func MDEL(arr *Array, index int) string {
	if index < 0 || index >= arr.size {
		return ""
	}
	val := arr.data[index]
	for i := index; i < arr.size-1; i++ {
		arr.data[i] = arr.data[i+1]
	}
	arr.size--
	return val
}

func MREPLACE(arr *Array, val string, index int) {
	if index >= 0 && index < arr.size {
		arr.data[index] = val
	}
}

func MRECEIVE(arr Array, index int) string {
	if index < 0 || index >= arr.size {
		return ""
	}
	return arr.data[index]
}

func MLEN(arr Array) int {
	return arr.size
}

func MGET(arr Array) {
	MDISPLAY(arr)
}

func MGET_REVERSE(arr Array) {
	for i := arr.size - 1; i >= 0; i-- {
		fmt.Print(arr.data[i], " ")
	}
	fmt.Println()
}

func MDESTROY(arr *Array) {
	arr.data = nil
	arr.size = 0
	arr.capacity = 0
}

func MSAVE(arr Array, filename string) {
	file, err := os.Create(filename)
	if err != nil {
		return
	}
	defer file.Close()
	for i := 0; i < arr.size; i++ {
		file.WriteString(arr.data[i] + "\n")
	}
}