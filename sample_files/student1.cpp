#include <iostream>
using namespace std;

int main() {
    double a, b;
    char op;

    cout << "Enter first number: ";
    cin >> a;
    cout << "Enter operator (+, -, *, /): ";
    cin >> op;
    cout << "Enter second number: ";
    cin >> b;

    switch (op) {
        case '+':
            cout << "Result: " << a + b << '\n';
            break;
        case '-':
            cout << "Result: " << a - b << '\n';
            break;
        case '*':
            cout << "Result: " << a * b << '\n';
            break;
        case '/':
            if (b != 0)
                cout << "Result: " << a / b << '\n';
            else
                cout << "Cannot divide by zero.\n";
            break;
        default:
            cout << "Invalid operator.\n";
    }
    return 0;
}
