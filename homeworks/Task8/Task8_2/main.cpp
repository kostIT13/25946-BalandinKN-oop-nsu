// Минимальное приложение части 8.II: вся полезная нагрузка здесь — tests.exe.
// Этот файл нужен, чтобы target `robot` собирался вместе с тестами.
#include "robot.h"
#include <iostream>
#include <utility>
#include <vector>

int main() {
    std::vector<double> hist;
    for (int i = 1; i <= 10; ++i) hist.push_back(i);
    QuadrupedRobot src("R1", "Spot", 55.0, "walk", 7.0, hist.data());

    QuadrupedRobot moved(std::move(src));   // move ctor
    std::cout << src << "\n" << moved << "\n";

    QuadrupedRobot target;
    target = std::move(moved);              // move assign
    std::cout << target << "\n";
    QuadrupedRobot::printStats();
    return 0;
}
