
#include <iostream>
#include <stack>
#include <string>
#include <cctype>
using namespace std;

class InfixPostfix {
    stack<char> st;

    int precedence(char op) {
        if (op == '^') return 3;
        if (op == '*' || op == '/') return 2;
        if (op == '+' || op == '-') return 1;
        return 0;
    }

public:
    void convertExpression(string exp) {
        string result = "";

        for (char ch : exp) {
            if (isalnum(ch)) {
                result += ch;
            }
            else if (ch == '(') {
                st.push(ch);
            }
            else if (ch == ')') {
                while (!st.empty() && st.top() != '(') {
                    result += st.top();
                    st.pop();
                }
                if (!st.empty())
                    st.pop();
            }
            else {
                while (!st.empty() && st.top() != '(' &&
                       (precedence(st.top()) > precedence(ch) ||
                       (precedence(st.top()) == precedence(ch) &&
                        ch != '^'))) {
                    result += st.top();
                    st.pop();
                }
                st.push(ch);
            }
        }

        while (!st.empty()) {
            result += st.top();
            st.pop();
        }

        cout << "Converted postfix: " << result << endl;
    }
};

int main() {
    InfixPostfix converter;
    string expression;

    cout << "Enter an infix expression: ";
    cin >> expression;

    converter.convertExpression(expression);

    return 0;
}
