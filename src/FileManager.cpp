#include "FileManager.h"
#include <fstream>

vector<string> FileManager::findFiles(string folderPath)
{
    vector<string> files;

    for (const auto& entry : filesystem::directory_iterator(folderPath))
    {
        if (entry.is_regular_file())
        {
            string extension = entry.path().extension().string();

            if (extension == ".txt" ||
                extension == ".cpp" ||
                extension == ".h" ||
                extension == ".hpp")
            {
                files.push_back(entry.path().string());
            }
        }
    }

    return files;
}

string FileManager::readFile(string filePath)
{
    ifstream file(filePath);

    if (!file.is_open())
    {
        return "";
    }

    string content;
    string line;

    while (getline(file, line))
    {
        content += line + "\n";
    }

    file.close();

    return content;
}
