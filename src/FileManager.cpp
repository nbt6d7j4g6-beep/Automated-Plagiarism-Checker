#include "FileManager.h"
#include <fstream>
#include <set>
#include <algorithm>
#include <cctype> 
#include <stdexcept>

vector<string> FileManager::findFiles(string folderPath)
{
    vector<string> files;

    set<string> extensions = {
        ".txt", ".cpp", ".h", ".hpp",
        ".c", ".cc", ".cxx",
        ".py", ".java", ".js", ".ts",
        ".cs", ".php", ".html", ".css"
    };

    try
    {
        for (const auto& entry : filesystem::directory_iterator(folderPath))
        {
            if (entry.is_regular_file())
            {
                string extension = entry.path().extension().string();

                    transform(extension.begin(), extension.end(),
                    extension.begin(),
                      [](unsigned char c) { return tolower(c); });

                if (extensions.find(extension) != extensions.end())
                {
                    files.push_back(entry.path().string());
                }
            }
        }
    }
    catch (const filesystem::filesystem_error&)
    {
        return files;
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
