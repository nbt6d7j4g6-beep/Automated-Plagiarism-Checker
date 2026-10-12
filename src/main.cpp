
#include <iostream>
#include "TextProcessor.h"
using namespace std;

int main()
{
    TextProcessor processor;

    string text = "INT   Sum = 10;";

    cout << "Original text: " << text << endl;
    cout << "Processed text: "
         << processor.processText(text) << endl;

    return 0;
}
