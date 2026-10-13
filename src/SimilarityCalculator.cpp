
#include "SimilarityCalculator.h"
#include <set>

double SimilarityCalculator::calculateSimilarity(
    vector<string> gramsA, vector<string> gramsB)
{
    set<string> setA(gramsA.begin(), gramsA.end());
    set<string> setB(gramsB.begin(), gramsB.end());

    if (setA.empty() && setB.empty())
    {
        return 0.0;
    }

    int common = 0;

    for (const string& gram : setA)
    {
        if (setB.find(gram) != setB.end())
        {
            common++;
        }
    }

    set<string> combined = setA;

    for (const string& gram : setB)
    {
        combined.insert(gram);
    }

    return (static_cast<double>(common) / combined.size()) * 100.0;
}
