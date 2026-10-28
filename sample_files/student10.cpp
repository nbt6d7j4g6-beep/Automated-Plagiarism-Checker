
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
            cout << "Stack is full!\n";
        else {
            arr[++top] = x;
            cout << "Element pushed successfully.\n";
        }
    }

    void pop() {
        if (empty())
            cout << "Stack is empty!\n";
        else
            cout << "Popped element: " << arr[top--] << endl;
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
            cout << "Queue is full!\n";
        else {
            arr[++rear] = x;
            cout << "Element inserted successfully.\n";
        }
    }

    void pop() {
        if (empty())
            cout << "Queue is empty!\n";
        else
            cout << "Removed element: " << arr[front++] << endl;
    }
};

int main() {
    Stack s;
    Queue q;
    int choice, value;

    do {
        cout << "\n1. Stack Push";
        cout << "\n2. Stack Pop";
        cout << "\n3. Check Stack Empty";
        cout << "\n4. Check Stack Full";
        cout << "\n5. Queue Push";
        cout << "\n6. Queue Pop";
        cout << "\n7. Check Queue Empty";
        cout << "\n8. Check Queue Full";
        cout << "\n9. Exit";
        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter value: ";
                cin >> value;
                s.push(value);
                break;

            case 2:
                s.pop();
                break;

            case 3:
                cout << (s.empty() ? "Stack is empty\n" :
                                     "Stack is not empty\n");
                break;

            case 4:
                cout << (s.full() ? "Stack is full\n" :
                                    "Stack is not full\n");
                break;

            case 5:
                cout << "Enter value: ";
                cin >> value;
                q.push(value);
                break;

            case 6:
                q.pop();
                break;

            case 7:
                cout << (q.empty() ? "Queue is empty\n" :
                                     "Queue is not empty\n");
                break;

            case 8:
                cout << (q.full() ? "Queue is full\n" :
                                    "Queue is not full\n");
                break;

            case 9:
                cout << "Exiting program...\n";
                break;

            default:
                cout << "Invalid choice!\n";
        }
    } while (choice != 9);

    return 0;
}
