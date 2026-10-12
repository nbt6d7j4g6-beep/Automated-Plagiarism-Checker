
#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <vector>
#include <filesystem>
using namespace std;

class FileManager
{
public:
    vector<string> findFiles(string folderPath);
    string readFile(string filePath);
};

#endif
