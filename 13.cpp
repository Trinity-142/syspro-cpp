#include <iostream>

template <typename Checker, typename... Args>
int getIndexOfFirstMatch(Checker check, Args&&... args) {
    int i = 0;
    auto lambda = [&check, &i]<typename T>(T&& arg) {
        if (check(std::forward<T>(arg))) {
            return true;
        }
        i++;
        return false;
    };
    if ((... || lambda(std::forward<Args>(args))) == false) {
        return -1;
    }
    return i;
}

bool even(const int a) {
    return a % 2 == 0;
}

int main() {
    std::cout << getIndexOfFirstMatch(even, 1, 3, 8, 2.321, 0e-19) << std::endl;
}
