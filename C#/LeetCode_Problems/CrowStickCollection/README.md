# Crow Stick Collection

### Problem Description
You have a forest represented by an array `forest` of length `n`, where:
* `forest[i] = 0` means that position `i` is empty.
* `forest[i] > 0` represents a stick with length equal to the number at that position.

A crow is initially positioned at `forest[crow]`. Its goal is to collect sticks by moving in the array under the following rules:
1. First, the crow checks up to two positions to the left (`crow - 1`, `crow - 2`) and collects any stick it finds.
2. Then, the crow checks up to two positions to the right (`crow + 1`, `crow + 2`) and collects any stick it finds.
3. The crow repeats this process until the total sum of the collected sticks reaches 100 or more.
4. Return a list of the indices in `forest` where the collected sticks were found.

If in one direction (left or right) there are no more valid positions within the array, the crow only checks the accessible positions.

### Example 1
Input:
```python
forest = [0, 20, 5, 0, 30, 10, 0, 50, 0, 15]
crow = 4
```
Process:

* `crow = 4`, checks left:
	+ `forest[3] = 0` -> empty.
	+ `forest[2] = 5` -> collects 5 at index 2.
* `crow = 4`, checks right:
	+ `forest[5] = 10` -> collects 10 at index 5.
	+ `forest[6] = 0` -> empty.
* `crow = 4`, checks left again:
	+ `forest[1] = 20` -> collects 20 at index 1.
	+ `forest[0] = 0` -> empty.
* `crow = 4`, checks right again:
	+ `forest[7] = 50` -> collects 50 at index 7.
* Total sum of sticks: 5 + 10 + 20 + 50 = 85 (continues searching).
* Last round:
	+ `forest[9] = 15` -> collects 15 at index 9, totaling 100.
* Stops and returns `[2, 5, 1, 7, 9]`.

Output:
```python
[2, 5, 1, 7, 9]
```

### Example 2
Input:
```python
forest = [0, 0, 0, 40, 0, 0, 60, 0, 0, 10]
crow = 3
```
Process:

* `crow = 3`, checks left:
	+ `forest[2] = 0`, `forest[1] = 0` -> empty.
* `crow = 3`, checks right:
	+ `forest[4] = 0`, `forest[5] = 0` -> empty.
* `crow = 3`, checks left:
	+ `forest[0] = 0` -> empty.
* `crow = 3`, checks right:
	+ `forest[6] = 60` -> collects 60 at index 6.
	+ `forest[9] = 10` -> collects 10 at index 9.
* Total sum: 60 + 10 = 70, insufficient.
* `forest[3] = 40` -> collects 40, total 110, ends.

Output:
```python
[6, 9, 3]
```

### Example 3 (Edge Case)
Input:
```python
forest = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
crow = 5
```
Output:
```python
[]
```
Explanation:

There are no sticks to collect, so the response is an empty list.
