package sorting

import "fmt"

func QuickSort(data []int) {
	if len(data) <= 1 {
		return
	}

	pivot := data[0]
	head, tail := 0, len(data)-1
	for i := 1; i <= tail; {
		if data[i] > pivot {
			data[i], data[tail] = data[tail], data[i]
			tail--
		} else {
			data[i], data[head] = data[head], data[i]
			head++
			i++
		}
	}

	QuickSort(data[:head])
	QuickSort(data[head+1:])
}

func QuickSortVerb(data []int) {
	if len(data) <= 1 {
		fmt.Printf("One data %v\n", data)
		return
	}

	pivot := data[0]
	fmt.Printf("pivot = %d\n", pivot)
	head, tail := 0, len(data)-1
	for i := 1; i <= tail; {
		if data[i] > pivot {
			fmt.Printf("i = %d, tail = %d, data[%d] > pivot, change data[%d] and data[%d], then tail--\n", i, tail, i, i, tail)
			data[i], data[tail] = data[tail], data[i]
			tail--
		} else {
			fmt.Printf("i = %d, head = %d, data[%d] < pivot, change data[%d] and data[%d], then head++, i++\n", i, head, i, i, head)
			data[i], data[head] = data[head], data[i]
			head++
			i++
		}
	}

	QuickSort(data[:head])
	QuickSort(data[head+1:])
}
