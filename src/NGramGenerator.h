
#ifndef NGRAMGENERATOR_H
#define NGRAMGENERATOR_H

#include <string>
#include <vector>
using namespace std;

class NGramGenerator
{
public:
    vector<string> generateNGrams(string text, int n);
};

#endif
