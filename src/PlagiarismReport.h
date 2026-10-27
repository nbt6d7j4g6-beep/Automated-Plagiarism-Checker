
#ifndef PLAGIARISMREPORT_H
#define PLAGIARISMREPORT_H

#include <string>
#include <vector>

using namespace std;

struct Result
{
    string file1;
    string file2;
    double similarity;
    string category;
    vector<string> matchingNGrams;
};

class PlagiarismReport
{
private:
    vector<Result> results;

public:
    void addResult(
    string file1, string file2, double similarity,
    vector<string> matchingNGrams);
    void saveReport(string fileName);
    void saveJsonReport(string fileName, int n = 3);
    vector<Result> getResults();
};

#endif
