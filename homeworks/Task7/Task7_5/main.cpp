// Задание 7.V: демонстрация operator>> и operator<<.
#include "robot.h"
#include <iostream>

int main() {
    std::cout << "Enter robot: <name> <battery> <command> <distance>\n> ";
    QuadrupedRobot r;                 // сначала default (для uid)
    if (std::cin >> r) {              // operator>> : 4 параметра
        std::cout << "\nRead OK, object:\n";
        std::cout << r << std::endl;  // operator<< : полная информация
    } else {
        std::cout << "\nInput failed\n";
    }
    return 0;
}
