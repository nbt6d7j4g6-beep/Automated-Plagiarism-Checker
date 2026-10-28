
#include <iostream>
#include <stack>
#include <string>
#include <cctype>
using namespace std;

class Converter {
    stack<char> s;

    int priority(char op) {
        if (op == '*' || op == '/') return 2;
        if (op == '+' || op == '-') return 1;
        return 0;
    }

public:
    void convert(string exp) {
        string post = "";

        for (char c : exp) {
            if (isalnum(c))
                post += c;
            else if (c == '(')
                s.push(c);
            else if (c == ')') {
                while (!s.empty() && s.top() != '(') {
                    post += s.top();
                    s.pop();
                }
                if (!s.empty()) s.pop();
            }
            else {
                while (!s.empty() && s.top() != '(' &&
                       priority(s.top()) >= priority(c)) {
                    post += s.top();
                    s.pop();
                }
                s.push(c);
            }
        }

        while (!s.empty()) {
            post += s.top();
            s.pop();
        }

        cout << "Postfix: " << post << endl;
    }
};

int main() {
    Converter obj;
    string exp;

    cout << "Enter infix: ";
    cin >> exp;
    obj.convert(exp);

    return 0;
}
