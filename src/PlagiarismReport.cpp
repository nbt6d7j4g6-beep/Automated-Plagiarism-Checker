
#include <fstream>
#include <iomanip>
#include "PlagiarismReport.h"

using namespace std;

string escapeJson(string text)
{
    string escaped = "";

    for (int i = 0; i < text.length(); i++)
    {
        if (text[i] == '"')
            escaped += "\\\"";
        else if (text[i] == '\\')
            escaped += "\\\\";
        else if (text[i] == '\n')
            escaped += "\\n";
        else if (text[i] == '\r')
            escaped += "\\r";
        else if (text[i] == '\t')
            escaped += "\\t";
        else
            escaped += text[i];
    }

    return escaped;
}

void PlagiarismReport::addResult(
    string file1, string file2, double similarity,
    vector<string> matchingNGrams)
{
    Result result;

    result.file1 = file1;
    result.file2 = file2;
    result.similarity = similarity;
    result.matchingNGrams = matchingNGrams;

    if (similarity < 30)
        result.category = "Low";
    else if (similarity < 60)
        result.category = "Moderate";
    else if (similarity < 80)
        result.category = "High";
    else
        result.category = "Very High";

    results.push_back(result);
}

void PlagiarismReport::saveReport(string fileName)
{
    ofstream file(fileName);

    if (!file.is_open())
        return;

    file << "AUTOMATED PLAGIARISM CHECKER REPORT\n\n";

    for (int i = 0; i < results.size(); i++)
    {
        file << "File 1: " << results[i].file1 << "\n";
        file << "File 2: " << results[i].file2 << "\n";

        file << fixed << setprecision(2);
        file << "Similarity: " << results[i].similarity << "%\n";
        file << "Category: " << results[i].category << "\n";

        file << "Matching Text Fragments:\n";

        if (results[i].matchingNGrams.empty())
        {
            file << "None found\n";
        }
        else
        {
            for (const string& gram : results[i].matchingNGrams)
                file << "- " << gram << "\n";
        }

        file << "\n";
    }

    file.close();
}

void PlagiarismReport::saveJsonReport(string fileName, int n)
{
    ofstream file(fileName);

    if (!file.is_open())
        return;

    file << fixed << setprecision(2);
    file << "{\n";
    file << "  \"project\": \"Automated Plagiarism Checker\",\n";
    file << "  \"n_gram_size\": " << n << ",\n";
    file << "  \"results\": [\n";

    for (int i = 0; i < results.size(); i++)
    {
        file << "    {\n";
        file << "      \"file1\": \""
             << escapeJson(results[i].file1) << "\",\n";
        file << "      \"file2\": \""
             << escapeJson(results[i].file2) << "\",\n";
        file << "      \"similarity\": "
             << results[i].similarity << ",\n";
        file << "      \"category\": \""
             << escapeJson(results[i].category) << "\",\n";
        file << "      \"matching_fragments\": [";

        for (int j = 0; j < results[i].matchingNGrams.size(); j++)
        {
            if (j > 0)
                file << ", ";

            file << "\"" 
                 << escapeJson(results[i].matchingNGrams[j])
                 << "\"";
        }

        file << "]\n";
        file << "    }";

        if (i < results.size() - 1)
            file << ",";

        file << "\n";
    }

    file << "  ]\n";
    file << "}\n";

    file.close();
}

vector<Result> PlagiarismReport::getResults()
{
    return results;
}
