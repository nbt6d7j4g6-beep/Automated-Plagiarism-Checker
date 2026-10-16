
#ifndef PLAGIARISMREPORT_H
#define PLAGIARISMREPORT_H

#include <string>
#include <vector>
using namespace std;

class PlagiarismReport
{
private:
    vector<string> results;

public:
    void addResult(string result);
    void saveReport(string fileName);
};

#endif
