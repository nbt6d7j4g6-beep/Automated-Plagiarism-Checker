
#include <iostream>
#include "SimilarityCalculator.h"
using namespace std;

int main()
{
    SimilarityCalculator calculator;

    vector<string> gramsA = {"int sum", "sum =", "= 10"};
    vector<string> gramsB = {"int sum", "sum =", "= 20"};

    double result = calculator.calculateSimilarity(gramsA, gramsB);

    cout << "Similarity: " << result << "%" << endl;

    return 0;
}
