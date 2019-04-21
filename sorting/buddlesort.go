package sorting

func BubbleSort(data []int) {
	n := len(data)

	for i := 0; i < n-1; i++ {
		isSwapped := false
		for j := 0; j < n-1-i; j++ {
			if data[j] > data[j+1] {
				data[j], data[j+1] = data[j+1], data[j]
				isSwapped = true
			}
		}

		if !isSwapped {
			break
		}
	}
}
