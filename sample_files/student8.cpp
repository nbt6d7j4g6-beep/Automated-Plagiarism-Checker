#include <iostream>
#include <string>
#include <cctype>
using namespace std;

int main() {
    string input, cleaned;
    cout << "Enter a word or phrase: ";
    getline(cin, input);

    for (unsigned char ch : input) {
        if (isalnum(ch))
            cleaned += static_cast<char>(tolower(ch));
    }

    bool palindrome = true;
    int left = 0;
    int right = static_cast<int>(cleaned.size()) - 1;

    while (left < right) {
        if (cleaned[left] != cleaned[right]) {
            palindrome = false;
            break;
        }
        ++left;
        --right;
    }

    if (palindrome)
        cout << "The input is a palindrome.\n";
    else
        cout << "The input is not a palindrome.\n";

    cout << "Normalized input: " << cleaned << '\n';
    return 0;
}
