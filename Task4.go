package main

import (
	"fmt"
)

func printHistogram(text string) {
	freq := make([]int, 256)
	maxHeight := 0

	for i := 0; i < len(text); i++ {
		c := text[i]
		if c == ' ' || c == '\n' || c == '\r' || c == '\t' {
			continue
		}
		uc := int(c)
		freq[uc]++
		if freq[uc] > maxHeight {
			maxHeight = freq[uc]
		}
	}

	uniqueChars := MCREATE(4)
	for i := 0; i < 256; i++ {
		if freq[i] > 0 {
			MPUSH(&uniqueChars, string(byte(i)))
		}
	}

	for h := maxHeight; h >= 1; h-- {
		for i := 0; i < uniqueChars.size; i++ {
			uc := int(uniqueChars.data[i][0])
			if freq[uc] >= h {
				fmt.Print("#")
			} else {
				fmt.Print(" ")
			}
		}
		fmt.Println()
	}

	for i := 0; i < uniqueChars.size; i++ {
		fmt.Print(uniqueChars.data[i])
	}
	fmt.Println()

	MDESTROY(&uniqueChars)
}