
#ifndef COMPARISONMANAGER_H
#define COMPARISONMANAGER_H

#include <string>
#include <vector>
#include <functional>
#include "PlagiarismReport.h"

using namespace std;

class ComparisonManager
{
public:
    vector<Result> compareFiles(
        vector<string> files,
        int n = 3,
        function<void(int, int)> progressCallback = nullptr
    );
};

#endif
