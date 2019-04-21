package sorting

func InsertSort(data []int) {
	n := len(data)
	if n < 2 {
		return
	}

	for i := 1; i < n; i++ {
		for j := i - 1; j >= 0; j-- {
			if data[j] > data[j+1] {
				data[j], data[j+1] = data[j+1], data[j]
			} else {
				break
			}
		}
	}
}
