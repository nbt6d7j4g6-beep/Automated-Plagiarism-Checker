
#include <iostream>
#include "NGramGenerator.h"
using namespace std;

int main()
{
    NGramGenerator generator;

    string text = "int sum = 10 return sum";
    int n = 2;

    vector<string> ngrams = generator.generateNGrams(text, n);

    cout << "Generated N-Grams:" << endl;

    for (const string& gram : ngrams)
    {
        cout << gram << endl;
    }

    return 0;
}
