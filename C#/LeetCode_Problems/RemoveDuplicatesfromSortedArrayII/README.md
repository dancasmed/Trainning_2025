# 80. Remove Duplicates from Sorted Array II

### Problem Description
Given an integer array `nums` sorted in non-decreasing order, remove some duplicates in-place such that each unique element appears at most twice. The relative order of the
elements should be kept the same.

#### Constraints
* `1 <= nums.length <= 3 * 10^4`
* `-10^4 <= nums[i] <= 10^4`
* `nums` is sorted in non-decreasing order

### Task
Return `k` after placing the final result in the first `k` slots of `nums`. Do not allocate extra space for another array. You must do this by modifying the input array
in-place with O(1) extra memory.

### Example Use Cases

#### Example 1
* Input: `nums = [1,1,1,2,2,3]`
* Output: `5`, `nums = [1,1,2,2,3,_]`

#### Example 2
* Input: `nums = [0,0,1,1,1,1,2,3,3]`
* Output: `7`, `nums = [0,0,1,1,2,3,3,_,_]`

### Custom Judge
The judge will test your solution with the following code:
```python
int[] nums = [...]; // Input array
int[] expectedNums = [...]; // The expected answer with correct length

int k = removeDuplicates(nums); // Calls your implementation

assert k == expectedNums.length;
for (int i = 0; i < k; i++) {
    assert nums[i] == expectedNums[i];
}
```
If all assertions pass, then your solution will be accepted.