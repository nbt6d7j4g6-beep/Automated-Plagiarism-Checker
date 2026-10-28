
#include <iostream>
using namespace std;

class Stack {
    int arr[5], top;

public:
    Stack() {
        top = -1;
    }

    bool empty() {
        return top == -1;
    }

    bool full() {
        return top == 4;
    }

    void push(int x) {
        if (full())
            cout << "Stack is full\n";
        else
            arr[++top] = x;
    }

    void pop() {
        if (empty())
            cout << "Stack is empty\n";
        else
            cout << "Popped: " << arr[top--] << endl;
    }
};

class Queue {
    int arr[5], front, rear;

public:
    Queue() {
        front = 0;
        rear = -1;
    }

    bool empty() {
        return rear < front;
    }

    bool full() {
        return rear == 4;
    }

    void push(int x) {
        if (full())
            cout << "Queue is full\n";
        else
            arr[++rear] = x;
    }

    void pop() {
        if (empty())
            cout << "Queue is empty\n";
        else
            cout << "Removed: " << arr[front++] << endl;
    }
};

int main() {
    Stack s;
    Queue q;

    cout << "STACK\n";
    s.push(10);
    s.push(20);
    s.push(30);
    s.pop();
    cout << "Empty: " << s.empty() << endl;
    cout << "Full: " << s.full() << endl;

    cout << "\nQUEUE\n";
    q.push(10);
    q.push(20);
    q.push(30);
    q.pop();
    cout << "Empty: " << q.empty() << endl;
    cout << "Full: " << q.full() << endl;

    return 0;
}
