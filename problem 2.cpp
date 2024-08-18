#include <iostream>
#include <vector>
using namespace std;


int main() {
    int  n, key;
    cin >> n;
    auto arr = vector<int>(n);

    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    cin >> key;
    for (int i = 0; i < n; i++) {
        if (arr[i] == key) {
            cout << i;
            return 0;
        }
    }
    cout << "-1";
    return 0;

}