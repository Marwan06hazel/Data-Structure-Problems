#include <iostream>
#include <vector>
using namespace std;

int comparisons = 0; // global counter

// Function to find the pivot index
// Time complexity: O(log n)
int findPivot(vector<int>&arr) {
	// If the array is not rotated
    comparisons++;
    if (arr[0] < arr[arr.size() - 1]) {
        return 0;
    }

	// Binary search to find the pivot point
    int left = 0, right = arr.size() - 1;
    while (left < right) {
        int mid = left + (right - left) / 2;
        comparisons++;
		if (arr[mid] > arr[right]) left = mid + 1; // pivot is in the right half
		else if (arr[mid] < arr[right]) right = mid; // pivot is in the left half (including mid)
        else right--;
    }
	return left; // pivot index (lowest value)
}

// Standard binary search function
// Time complexity: O(log n)
int binarySearch(vector<int>& arr, int target, int left, int right) {
    if (left > right) return -1; 
    int mid = left + (right - left) / 2;

    comparisons++;
    if (arr[mid] == target) return mid;
    if (arr[mid] < target) return binarySearch(arr, target, mid + 1, right);
                           return binarySearch(arr, target, left, mid - 1);
}

// Main function to search in a rotated sorted array
// Time complexity: O(log n), O(n) (target not found).
int searchRotated(vector<int>& arr, int target) {
    int pivot = findPivot(arr);
	if (pivot == 0) return binarySearch(arr, target, 0, arr.size() - 1); // not rotated, search entire array
	if (target >= arr[0]) return binarySearch(arr, target, 0, pivot - 1); // search in the left half [0, pivot - 1] [high values]
	return binarySearch(arr, target, pivot, arr.size() - 1); // search in the right half [pivot, end] [low values]
}

int main() {
    // Test 1: Normal rotated array
    vector<int> arr1 = { 5, 6, 7, 1, 2, 3, 4 };
    cout << "Array: [5, 6, 7, 1, 2, 3, 4]" << endl;
    vector<int> targets1 = { 7, 1, 5, 4, 100 };

    for (int t : targets1) {
        comparisons = 0;
        int result = searchRotated(arr1, t);
        cout << "Target " << t;
        if (result != -1) cout << " >> FOUND at index " << result;
        else cout << " >> NOT FOUND";
        cout << "  |  Comparisons: " << comparisons << endl;
    }

    cout << endl;

    // Test 2: Duplicates
    vector<int> arr2 = { 3, 3, 3, 1, 3 };
    cout << "Array: [3, 3, 3, 1, 3]" << endl;
    vector<int> targets2 = { 1, 3, 100 };

    for (int t : targets2) {
        comparisons = 0;
        int result = searchRotated(arr2, t);
        cout << "Target " << t;
        if (result != -1) cout << " >> FOUND at index " << result;
        else cout << " >> NOT FOUND";
        cout << "  |  Comparisons: " << comparisons << endl;
    }

    cout << endl;

    // Test 3: Not rotated
    vector<int> arr3 = { 1, 2, 3, 4, 5 };
    cout << "Array: [1, 2, 3, 4, 5]" << endl;
    vector<int> targets3 = { 3, 100 };

    for (int t : targets3) {
        comparisons = 0;
        int result = searchRotated(arr3, t);
        cout << "Target " << t;
        if (result != -1) cout << " >> FOUND at index " << result;
        else cout << " >> NOT FOUND";
        cout << "  |  Comparisons: " << comparisons << endl;
    }

    cout << endl;

    // Test 4: Single element
    vector<int> arr4 = { 42 };
    cout << "Array: [42]" << endl;
    vector<int> targets4 = { 42, 100 };

    for (int t : targets4) {
        comparisons = 0;
        int result = searchRotated(arr4, t);
        cout << "Target " << t;
        if (result != -1) cout << " >> FOUND at index " << result;
        else cout << " >> NOT FOUND";
        cout << "  |  Comparisons: " << comparisons << endl;
    }
}

// Stating the complexity for best and worst cases:

// Best case: O(log n) when the target is found at the pivot or in the first comparison of binary search.
// Best case (no rotation): O(1) when the target is found at the first position.

// Worst case: O(log n) when the array is not rotated and the target is not found.
// Worst case (with all numbers are duplicates and no target found): O(n)
