
#include <fstream>
#include "PlagiarismReport.h"
using namespace std;

void PlagiarismReport::addResult(string result)
{
    results.push_back(result);
}

void PlagiarismReport::saveReport(string fileName)
{
    ofstream file(fileName);

    if (!file.is_open())
    {
        return;
    }

    file << "AUTOMATED PLAGIARISM CHECKER REPORT\n\n";

    for (int i = 0; i < results.size(); i++)
    {
        file << results[i] << "\n\n";
    }

    file.close();
}
