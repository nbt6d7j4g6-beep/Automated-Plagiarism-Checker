#include <iostream>
#include <iomanip>
using namespace std;

int main() {
    double scores[5], sum = 0.0;
    string name;

    cout << "Student full name: ";
    getline(cin, name);

    for (int subject = 0; subject < 5; subject++) {
        cout << "Score " << subject + 1 << ": ";
        cin >> scores[subject];
        if (scores[subject] > 100 || scores[subject] < 0) {
            cout << "Invalid score entered.\n";
            return 1;
        }
        sum = sum + scores[subject];
    }

    double mean = sum / 5.0;
    cout << fixed << setprecision(2);
    cout << "\nName: " << name << endl;
    cout << "Sum of marks: " << sum << endl;
    cout << "Mean score: " << mean << endl;

    if (mean >= 90) cout << "Grade A\n";
    else if (mean >= 75) cout << "Grade B\n";
    else if (mean >= 60) cout << "Grade C\n";
    else if (mean >= 40) cout << "Grade D\n";
    else cout << "Grade F\n";

    return 0;
}
