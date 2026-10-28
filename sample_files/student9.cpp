#include <iostream>
using namespace std;

int main() {
    int numbers[8] = {42, 12, 7, 19, 33, 5, 27, 16};
    int size = 8;

    cout << "Before sorting: ";
    for (int i = 0; i < size; ++i)
        cout << numbers[i] << ' ';
    cout << '\n';

    for (int pass = 0; pass < size - 1; ++pass) {
        bool changed = false;

        for (int j = 0; j < size - pass - 1; ++j) {
            if (numbers[j] > numbers[j + 1]) {
                int temp = numbers[j];
                numbers[j] = numbers[j + 1];
                numbers[j + 1] = temp;
                changed = true;
            }
        }

        if (!changed)
            break;
    }

    cout << "After sorting: ";
    for (int i = 0; i < size; ++i)
        cout << numbers[i] << ' ';
    cout << '\n';

    return 0;
}
