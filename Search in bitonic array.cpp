#include <iostream>
#include <vector>
using namespace std;

int comparisons = 0;
int swaps = 0;

// Function to divide an array into two halves
pair<vector<int>, vector<int>> divideArray(vector<int>& arr) {
    int mid = arr.size() / 2;

    vector<int> left(arr.begin(), arr.begin() + mid); // Create left vector from the first half of the array
    vector<int> right(arr.begin() + mid, arr.end()); // Create right vector from the second half of the array

    return { left, right };
}

// Function to sort an array in ascending order using [bubble sort]
vector<int> BubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            comparisons++;
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                swaps++;
            }
        }
    }
    return arr;
}

// Function to sort an array in descending order using [selection sort]
vector<int> SelectionSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        int maxIndex = i;
        for (int j = i + 1; j < n; j++) {
            comparisons++;
            if (arr[j] > arr[maxIndex]) { // Change to descending order
                maxIndex = j;
            }
        }
        if (maxIndex != i) {
            swap(arr[i], arr[maxIndex]);
            swaps++;
        }
    }
    return arr;
}

// Function to perform linear search on an array
int linearSearch(vector<int>& arr, int target) {
    for (size_t i = 0; i < arr.size(); i++) {
        if (arr[i] == target) {
            return i; // Return the index of the target
        }
    }
    return -1;
}

int main() {
    // ---------- Test Case 1 ----------
	// Target at the bitonic point
    vector<int> arr1 = { 7, 2, 11, 5, 3, 9 };
    int target1 = 11;

    // ---------- Test Case 2 ----------
	// Target in the left half (ascending portion)
    vector<int> arr2 = { 4, 1, 6, 3, 2, 5 };
    int target2 = 3;

    // ---------- Test Case 3 ----------
	// Target in the right half (descending portion)
    vector<int> arr3 = { 8, 2, 10, 7, 6, 1 };
    int target3 = 6;

    // ---------- Test Case 4 ----------
	// Target not present in the array
    vector<int> arr4 = { 9, 3, 12, 8, 4, 1 };
    int target4 = 100;

    // ---------- Test Case 5 ----------
    // Array with duplicate elements
    vector<int> arr5 = { 5, 2, 5, 8, 1, 8, 3 };
    int target5 = 5;

    vector<vector<int>> tests = { arr1, arr2, arr3, arr4, arr5 };
    vector<int> targets = { target1, target2, target3, target4, target5 };

    for (int t = 0; t < tests.size(); t++) {

        cout << "\nTest Case " << t + 1 << endl;

        comparisons = 0;
        swaps = 0;

        // Divide array
        pair<vector<int>, vector<int>> result = divideArray(tests[t]);
        vector<int> left = result.first;
        vector<int> right = result.second;

        // Sort halves
        vector<int> sortedLeft = BubbleSort(left);
        vector<int> sortedRight = SelectionSort(right);

        // Build bitonic array
        vector<int> bitonicArray;
        for (int x : sortedLeft) bitonicArray.push_back(x);
        for (int x : sortedRight) bitonicArray.push_back(x);

        cout << "Bitonic Array: ";
        for (int x : bitonicArray) cout << x << " ";
        cout << endl;

        // Search target
        int index = linearSearch(bitonicArray, targets[t]);

        if (index != -1)
            cout << "Target " << targets[t] << " found at index " << index << endl;
        else
            cout << "Target " << targets[t] << " NOT found" << endl;

        cout << "Total Comparisons: " << comparisons << endl;
        cout << "Total Swaps: " << swaps << endl;
    }

    return 0;
}
