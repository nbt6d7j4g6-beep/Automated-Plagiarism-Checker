
#include "NGramGenerator.h"
#include <sstream>

vector<string> NGramGenerator::generateNGrams(string text, int n)
{
    vector<string> words;
    vector<string> ngrams;

    stringstream ss(text);
    string word;

    while (ss >> word)
    {
        words.push_back(word);
    }

    if (n <= 0 || words.size() < n)
    {
        return ngrams;
    }

    for (int i = 0; i <= static_cast<int>(words.size()) - n; i++)
    {
        string gram = "";

        for (int j = 0; j < n; j++)
        {
            if (j > 0)
            {
                gram += " ";
            }

            gram += words[i + j];
        }

        ngrams.push_back(gram);
    }

    return ngrams;
}
