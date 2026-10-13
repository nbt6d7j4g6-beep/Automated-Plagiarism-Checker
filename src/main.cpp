
#include <iostream>
#include "FileManager.h"
#include "ComparisonManager.h"
using namespace std;

int main()
{
    FileManager manager;
    ComparisonManager comparison;

    vector<string> files = manager.findFiles("sample_files");

    if (files.size() < 2)
    {
        cout << "Add at least two supported files to sample_files."
             << endl;
        return 1;
    }

    cout << "Files found: " << files.size() << endl;

    comparison.compareFiles(files);

    return 0;
}
