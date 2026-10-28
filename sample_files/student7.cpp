
#include <iostream>
using namespace std;

class Stack {
    int a[5], top = -1;

public:
    bool empty() { return top == -1; }
    bool full() { return top == 4; }

    void push(int x) {
        if (full()) cout << "Stack full\n";
        else a[++top] = x;
    }

    void pop() {
        if (empty()) cout << "Stack empty\n";
        else cout << "Popped: " << a[top--] << endl;
    }
};

class Queue {
    int a[5], front = 0, rear = -1;

public:
    bool empty() { return front > rear; }
    bool full() { return rear == 4; }

    void push(int x) {
        if (full()) cout << "Queue full\n";
        else a[++rear] = x;
    }

    void pop() {
        if (empty()) cout << "Queue empty\n";
        else cout << "Removed: " << a[front++] << endl;
    }
};

int main() {
    Stack s;
    Queue q;
    int x;

    cout << "Enter a value for stack: ";
    cin >> x;
    s.push(x);
    s.pop();

    cout << "Enter a value for queue: ";
    cin >> x;
    q.push(x);
    q.pop();

    cout << "Stack empty: " << s.empty() << endl;
    cout << "Queue empty: " << q.empty() << endl;

    return 0;
}
