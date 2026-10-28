#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

int main() {
    string text;
    cout << "Enter a sentence: ";
    getline(cin, text);

    string reversed = text;
    reverse(reversed.begin(), reversed.end());

    cout << "Original text: " << text << '\n';
    cout << "Reversed text: " << reversed << '\n';

    int vowels = 0;
    for (char ch : text) {
        char lower = static_cast<char>(tolower(
            static_cast<unsigned char>(ch)));
        if (lower == 'a' || lower == 'e' || lower == 'i' ||
            lower == 'o' || lower == 'u') {
            ++vowels;
        }
    }

    cout << "Vowel count: " << vowels << '\n';
    cout << "Character count: " << text.size() << '\n';
    return 0;
}
