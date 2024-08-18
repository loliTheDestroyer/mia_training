#include <iostream>

#include <vector>
using namespace std;


int main() {


    int r, c;


    cin >> r >> c;

    auto villans = vector<vector<int>>(r, vector<int>(c));
    auto heroes = vector<vector<int>>(r, vector<int>(c));

    for (int i = 0; i < c; i++) {
        for (int j = 0; j < r; j++) {
            int temp;
            cin >> temp;
            heroes[i][j] = temp;
        }
    }

    for (int i = 0; i < c; i++) {
        for (int j = 0; j < r; j++) {
            int temp;
            cin >> temp;
            villans[i][j] = temp;
        }
    }

    int v_win_count = 0;

    for (int i = 0; i < r; i++)
        for (int j = 0; j < c; j++)
            if (villans[i][j] > heroes[i][j])
                v_win_count++;
            else if (villans[i][j] < heroes[i][j])
                v_win_count--;

    string winner;
    if (v_win_count > 0)
        winner = "Villains";
    else if (v_win_count == 0)
        winner = "Tie";
    else
        winner = "Justice League";

    cout << winner << endl;

    return 0;
}

