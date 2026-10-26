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
};

class PlagiarismReport
{
private:
    vector<Result> results;

public:
    void addResult(string file1, string file2, double similarity);
    void saveReport(string fileName);
    void saveJsonReport(string fileName);
    vector<Result> getResults();
};

#endif