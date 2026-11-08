package main

import (
	"strconv"
)

func getPriority(op rune) int {
	if op == '!' {
		return 3
	}
	if op == '&' {
		return 2
	}
	if op == '|' || op == '^' {
		return 1
	}
	return 0
}

func toPostfix(expr string) string {
	postfix := ""
	opStack := SCREATE(4)

	for _, c := range expr {
		if c == ' ' || c == '\t' || c == '\n' || c == '\r' {
			continue
		}
		if c == '0' || c == '1' {
			postfix += string(c)
		} else if c == '(' {
			SPUSH(&opStack, string(c))
		} else if c == ')' {
			for opStack.top >= 0 && opStack.data[opStack.top] != "(" {
				postfix += SPOP(&opStack)
			}
			if opStack.top >= 0 {
				SPOP(&opStack)
			}
		} else if c == '!' || c == '&' || c == '|' || c == '^' {
			for opStack.top >= 0 {
				topOp := rune(opStack.data[opStack.top][0])
				if getPriority(topOp) >= getPriority(c) {
					if c == '!' && topOp == '!' {
						break
					}
					postfix += SPOP(&opStack)
				} else {
					break
				}
			}
			SPUSH(&opStack, string(c))
		}
	}
	for opStack.top >= 0 {
		postfix += SPOP(&opStack)
	}
	SDESTROY(&opStack)
	return postfix
}

func evaluatePostfix(postfix string) int {
	valStack := SCREATE(4)

	for _, c := range postfix {
		if c == '0' || c == '1' {
			SPUSH(&valStack, string(c))
		} else if c == '!' {
			topVal := SPOP(&valStack)
			if topVal == "" {
				SDESTROY(&valStack)
				return 0
			}
			op := int(topVal[0] - '0')
			res := 0
			if op == 0 {
				res = 1
			}
			SPUSH(&valStack, strconv.Itoa(res))
		} else {
			top2 := SPOP(&valStack)
			top1 := SPOP(&valStack)
			if top1 == "" || top2 == "" {
				SDESTROY(&valStack)
				return 0
			}
			op2 := int(top2[0] - '0')
			op1 := int(top1[0] - '0')
			res := 0
			if c == '&' {
				if op1 != 0 && op2 != 0 {
					res = 1
				}
			} else if c == '|' {
				if op1 != 0 || op2 != 0 {
					res = 1
				}
			} else if c == '^' {
				if op1 != op2 {
					res = 1
				}
			}
			SPUSH(&valStack, strconv.Itoa(res))
		}
	}
	finalStr := SPOP(&valStack)
	finalResult := 0
	if finalStr != "" {
		finalResult = int(finalStr[0] - '0')
	}
	SDESTROY(&valStack)
	return finalResult
}