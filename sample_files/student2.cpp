#include <iostream>
using namespace std;

int main() {
    float first, second;
    char symbol;

    cout << "First value: ";
    cin >> first;
    cout << "Choose operation (+ - * /): ";
    cin >> symbol;
    cout << "Second value: ";
    cin >> second;

    if (symbol == '+') {
        cout << "Answer = " << first + second << endl;
    } else if (symbol == '-') {
        cout << "Answer = " << first - second << endl;
    } else if (symbol == '*') {
        cout << "Answer = " << first * second << endl;
    } else if (symbol == '/') {
        if (second == 0)
            cout << "Division is not possible.\n";
        else
            cout << "Answer = " << first / second << endl;
    } else {
        cout << "Unknown operation.\n";
    }
    return 0;
}
