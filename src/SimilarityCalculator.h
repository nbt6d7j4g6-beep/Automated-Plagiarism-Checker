
#ifndef SIMILARITYCALCULATOR_H
#define SIMILARITYCALCULATOR_H

#include <string>
#include <vector>
using namespace std;

class SimilarityCalculator
{
public:
    double calculateSimilarity(vector<string> gramsA, vector<string> gramsB);
};

#endif
