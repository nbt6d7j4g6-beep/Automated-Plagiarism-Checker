#include <iostream>
using namespace std;

bool isPrime(int number) {
    if (number < 2)
        return false;

    for (int divisor = 2; divisor * divisor <= number; ++divisor) {
        if (number % divisor == 0)
            return false;
    }
    return true;
}

int main() {
    int value;
    cout << "Enter an integer: ";
    cin >> value;

    if (isPrime(value))
        cout << value << " is prime.\n";
    else
        cout << value << " is not prime.\n";

    cout << "Prime numbers from 2 to 50:\n";
    for (int n = 2; n <= 50; ++n) {
        if (isPrime(n))
            cout << n << ' ';
    }
    cout << '\n';
    return 0;
}
