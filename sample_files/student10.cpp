#include <iostream>
#include <sstream>
#include <string>
#include <set>
#include <cctype>
using namespace std;

set<string> getWords(const string& text) {
    set<string> words;
    stringstream stream(text);
    string word;

    while (stream >> word) {
        string cleaned;
        for (unsigned char ch : word) {
            if (isalnum(ch))
                cleaned += static_cast<char>(tolower(ch));
        }
        if (!cleaned.empty())
            words.insert(cleaned);
    }
    return words;
}

int main() {
    string first, second;

    cout << "Enter first document in one line:\n";
    getline(cin, first);
    cout << "Enter second document in one line:\n";
    getline(cin, second);

    set<string> a = getWords(first);
    set<string> b = getWords(second);
    int intersection = 0;

    for (const string& word : a) {
        if (b.count(word))
            ++intersection;
    }

    int unionSize = static_cast<int>(a.size() + b.size()) - intersection;
    double score = unionSize == 0 ? 0.0 :
        100.0 * intersection / unionSize;

    cout << "Unique words in first: " << a.size() << '\n';
    cout << "Unique words in second: " << b.size() << '\n';
    cout << "Shared unique words: " << intersection << '\n';
    cout << "Jaccard similarity: " << score << "%\n";
    cout << "Note: similarity alone does not prove plagiarism.\n";

    return 0;
}
