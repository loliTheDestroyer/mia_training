#include <iostream>
#include <stack>
#include <vector>
#include <string>

using namespace std;

struct Queue {
private:
    stack<string> in;
    stack<string> out;

    void fill_out() {
        while (!in.empty()) {
            out.push(in.top());
            in.pop();
        }
    }

public:
    string out_queue() {
        fill_out();
        if (out.empty()) {
            return ""; // Return an empty string if the queue is empty
        }
        string temp = out.top();
        out.pop();
        return temp;
    }

    void in_queue(const string& temp) {
        in.push(temp);
    }

    [[nodiscard]] bool is_empty() const {
        return in.empty() && out.empty(); // Check both stacks to determine if the queue is empty
    }
} queue;

enum Commands { enter = 0, remove_item = 1 , remove_all = 2, Exit = 5 };

int main() {
    cout << "Commands: " << endl;
    cout << "Add item: \t0" << endl;
    cout << "Display next: \t1" << endl;
    cout << "Display all: \t2" << endl;
    cout << "Exit program: \t5" << endl;

    bool state = true;
    while (state) {
        int command;
        string item;

        cout << "Enter command: ";
        cin >> command;
        cin.ignore();

        switch (command) {
            case Commands::enter:
                getline(cin, item);
                queue.in_queue(item);
                break;
            case Commands::remove_item:
                if (queue.is_empty()) {
                    cout << "Queue is empty" << endl;
                } else {
                    item = queue.out_queue();
                    cout << item << endl;
                }
                break;
            case Commands::remove_all:
                if (queue.is_empty()) {
                    cout << "Queue is empty" << endl;
                } else {
                    while (!queue.is_empty()) {
                        cout << queue.out_queue() << endl;
                    }
                }
                break;
            case Commands::Exit:
                state = false;
                break;
        }
    }

    return 0;
}
