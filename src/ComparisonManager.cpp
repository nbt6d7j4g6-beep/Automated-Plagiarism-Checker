#include <iostream>
#include <string>
#include <vector>

#include "ComparisonManager.h"
#include "FileManager.h"
#include "TextProcessor.h"
#include "NGramGenerator.h"
#include "SimilarityCalculator.h"
#include "PlagiarismReport.h"

using namespace std;
vector<Result> ComparisonManager::compareFiles(vector<string> files)
{
    FileManager manager;
    TextProcessor processor;
    NGramGenerator generator;
    SimilarityCalculator calculator;
    PlagiarismReport report;

    int n = 3;

    for (int i = 0; i < files.size(); i++)
    {
        for (int j = i + 1; j < files.size(); j++)
        {
            string text1 = manager.readFile(files[i]);
            string text2 = manager.readFile(files[j]);

            if (text1.empty() || text2.empty())
                continue;

            text1 = processor.processText(text1);
            text2 = processor.processText(text2);

            vector<string> grams1 = generator.generateNGrams(text1, n);
            vector<string> grams2 = generator.generateNGrams(text2, n);

            double similarity =
                calculator.calculateSimilarity(grams1, grams2);

            report.addResult(files[i], files[j], similarity);
        }
    }

    report.saveReport("plagiarism_report.txt");
    report.saveJsonReport("plagiarism_report.json");

    return report.getResults();
}
