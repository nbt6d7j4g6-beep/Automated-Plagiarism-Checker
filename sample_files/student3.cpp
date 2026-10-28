
#include <iostream>
#include <stack>
#include <string>
using namespace std;

class Conversion {
    stack<char> s;

    int priority(char op) {
        if (op == '^') return 3;
        if (op == '*' || op == '/') return 2;
        if (op == '+' || op == '-') return 1;
        return 0;
    }

public:
    void convert(string infix) {
        string postfix = "";

        for (char c : infix) {
            if (isalnum(c)) {
                postfix += c;
            }
            else if (c == '(') {
                s.push(c);
            }
            else if (c == ')') {
                while (!s.empty() && s.top() != '(') {
                    postfix += s.top();
                    s.pop();
                }
                if (!s.empty()) s.pop();
            }
            else {
                while (!s.empty() && s.top() != '(' &&
                       (priority(s.top()) > priority(c) ||
                       (priority(s.top()) == priority(c) && c != '^'))) {
                    postfix += s.top();
                    s.pop();
                }
                s.push(c);
            }
        }

        while (!s.empty()) {
            postfix += s.top();
            s.pop();
        }

        cout << "Postfix expression: " << postfix << endl;
    }
};

int main() {
    Conversion obj;
    string infix;

    cout << "Enter infix expression: ";
    cin >> infix;

    obj.convert(infix);

    return 0;
}
