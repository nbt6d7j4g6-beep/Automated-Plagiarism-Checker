#include <iostream>
using namespace std;

unsigned long long factorial(int n) {
    unsigned long long answer = 1;
    for (int i = 2; i <= n; ++i) {
        answer *= i;
    }
    return answer;
}

int main() {
    int number;
    cout << "Enter a number from 0 to 20: ";
    cin >> number;

    if (number < 0 || number > 20) {
        cout << "Input is outside the supported range.\n";
        return 1;
    }

    cout << "Factorial of " << number << " = "
         << factorial(number) << '\n';

    cout << "\nFactorials from 0 to 10:\n";
    for (int i = 0; i <= 10; ++i) {
        cout << i << "! = " << factorial(i) << '\n';
    }
    return 0;
}
