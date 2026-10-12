
#include "TextProcessor.h"
#include <cctype>

string TextProcessor::processText(string text)
{
    string result;
    bool previousSpace = false;

    for (char ch : text)
    {
        if (isspace(static_cast<unsigned char>(ch)))
        {
            if (!previousSpace)
            {
                result += ' ';
                previousSpace = true;
            }
        }
        else
        {
            result += static_cast<char>(
                tolower(static_cast<unsigned char>(ch))
            );
            previousSpace = false;
        }
    }

    return result;
}
