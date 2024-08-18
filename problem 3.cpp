#include <iostream>
using namespace std;


int main() {
    int arr[500], n;
    cin >> n;

    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }
    int temp = -1;
    for (int i = 0; i < n; i++) {
        if (arr[i] > temp) {
            temp = arr[i];
        }
    }
    cout << temp << endl;
    return 0;
}