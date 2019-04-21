package main

import (
	"fmt"
	"github.com/mimiasd/algorithm/sorting"
)

func main() {
	data1 := []int{9, 8, 11, 2, 7, 15}
	data2 := []int{9, 8, 11, 2, 7, 15}
	data3 := []int{9, 8, 11, 2, 7, 15}

	sorting.QuickSort(data1)
	fmt.Println("QuickSort: ", data1)

	sorting.BubbleSort(data2)
	fmt.Println("BubbleSort: ", data2)

	sorting.InsertSort(data3)
	fmt.Println("InsertSort: ", data3)
}
