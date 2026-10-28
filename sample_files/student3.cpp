#include <iostream>
#include <iomanip>
using namespace std;

int main() {
    string studentName;
    double marks[5], total = 0;

    cout << "Enter student name: ";
    getline(cin, studentName);

    for (int i = 0; i < 5; ++i) {
        cout << "Enter mark for subject " << i + 1 << ": ";
        cin >> marks[i];
        if (marks[i] < 0 || marks[i] > 100) {
            cout << "Marks must be between 0 and 100.\n";
            return 1;
        }
        total += marks[i];
    }

    double average = total / 5;
    cout << fixed << setprecision(2);
    cout << "\nStudent: " << studentName << '\n';
    cout << "Total: " << total << '\n';
    cout << "Average: " << average << '\n';

    if (average >= 90) cout << "Grade: A\n";
    else if (average >= 75) cout << "Grade: B\n";
    else if (average >= 60) cout << "Grade: C\n";
    else if (average >= 40) cout << "Grade: D\n";
    else cout << "Grade: F\n";

    return 0;
}
