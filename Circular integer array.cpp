#include <iostream>
#include <vector>
using namespace std;

vector <int> getGreaterNum(vector<int>& nums) {

    int n = nums.size();
    vector <int> result(n, -1);

    for (int i = 0; i < n; i++) {
        for (int j = 1; j < n; j++) {
            int index = (i + j) % n;
            if (nums[index] > nums[i]) {
                result[i] = nums[index];
                break;
            }
        }
    }

    return result;
}

int main() {
    int n;
    cin >> n;

    vector <int> values(n);
    for (int& num : values) {
        cin >> num;
    }

    vector<int> result = getGreaterNum(values);
    for (int num : result) {
        cout << num << " ";
    }

    return 0;
}
