
#include <iostream>
#include <stack>
#include <string>
#include <cctype>
using namespace std;

class ExpressionConverter {
    stack<char> operators;

    int precedence(char symbol) {
        if (symbol == '*' || symbol == '/') return 2;
        if (symbol == '+' || symbol == '-') return 1;
        return 0;
    }

public:
    void toPostfix(string expression) {
        string output = "";

        for (char ch : expression) {
            if (isalnum(ch))
                output += ch;
            else if (ch == '(')
                operators.push(ch);
            else if (ch == ')') {
                while (!operators.empty() && operators.top() != '(') {
                    output += operators.top();
                    operators.pop();
                }
                if (!operators.empty()) operators.pop();
            }
            else {
                while (!operators.empty() && operators.top() != '(' &&
                       precedence(operators.top()) >= precedence(ch)) {
                    output += operators.top();
                    operators.pop();
                }
                operators.push(ch);
            }
        }

        while (!operators.empty()) {
            output += operators.top();
            operators.pop();
        }

        cout << "Postfix expression: " << output << endl;
    }
};

int main() {
    ExpressionConverter converter;
    string input;

    cout << "Enter expression: ";
    cin >> input;
    converter.toPostfix(input);

    return 0;
}
