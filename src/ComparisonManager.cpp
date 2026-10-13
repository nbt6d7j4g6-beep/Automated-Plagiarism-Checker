
#include <iostream>
#include "ComparisonManager.h"
#include "FileManager.h"
#include "TextProcessor.h"
#include "NGramGenerator.h"
#include "SimilarityCalculator.h"
using namespace std;

void ComparisonManager::compareFiles(vector<string> files)
{
    FileManager manager;
    TextProcessor processor;
    NGramGenerator generator;
    SimilarityCalculator calculator;

    int n = 3;

    for (int i = 0; i < files.size(); i++)
    {
        for (int j = i + 1; j < files.size(); j++)
        {
            string text1 = manager.readFile(files[i]);
            string text2 = manager.readFile(files[j]);

            if (text1.empty() || text2.empty())
            {
                cout << "Could not read a file." << endl;
                continue;
            }

            text1 = processor.processText(text1);
            text2 = processor.processText(text2);

            vector<string> grams1 = generator.generateNGrams(text1, n);
            vector<string> grams2 = generator.generateNGrams(text2, n);

            double similarity =
                calculator.calculateSimilarity(grams1, grams2);

            cout << "\nFile 1: " << files[i] << endl;
            cout << "File 2: " << files[j] << endl;
            cout << "Similarity: " << similarity << "%" << endl;
        }
    }
}
