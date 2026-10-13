
#include <iostream>
#include "FileManager.h"
#include "TextProcessor.h"
#include "NGramGenerator.h"
#include "SimilarityCalculator.h"
using namespace std;

int main()
{
    FileManager manager;
    TextProcessor processor;
    NGramGenerator generator;
    SimilarityCalculator calculator;

    string file1 = "sample_files/student1.cpp";
    string file2 = "sample_files/student2.cpp";

    string text1 = manager.readFile(file1);
    string text2 = manager.readFile(file2);

    if (text1.empty() || text2.empty())
    {
        cout << "Error reading files." << endl;
        return 1;
    }

    text1 = processor.processText(text1);
    text2 = processor.processText(text2);

    int n = 3;

    vector<string> grams1 = generator.generateNGrams(text1, n);
    vector<string> grams2 = generator.generateNGrams(text2, n);

    double similarity = calculator.calculateSimilarity(grams1, grams2);

    cout << "File 1: " << file1 << endl;
    cout << "File 2: " << file2 << endl;
    cout << "N-Gram size: " << n << endl;
    cout << "Similarity: " << similarity << "%" << endl;

    return 0;
}
