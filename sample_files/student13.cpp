#include <iostream>
#include <string>
using namespace std;

class BankAccount {
private:
    string owner;
    double balance;

public:
    BankAccount(string name, double openingBalance)
        : owner(name), balance(openingBalance >= 0 ? openingBalance : 0) {}

    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            cout << "Deposit complete.\n";
        } else {
            cout << "Amount must be positive.\n";
        }
    }

    void withdraw(double amount) {
        if (amount <= 0)
            cout << "Invalid withdrawal.\n";
        else if (amount > balance)
            cout << "Insufficient funds.\n";
        else {
            balance -= amount;
            cout << "Withdrawal complete.\n";
        }
    }

    void show() const {
        cout << "Account owner: " << owner << '\n';
        cout << "Balance: Rs. " << balance << '\n';
    }
};

int main() {
    BankAccount account("Ananya", 2500.0);
    account.show();
    account.deposit(750.0);
    account.withdraw(400.0);
    account.show();
    return 0;
}
