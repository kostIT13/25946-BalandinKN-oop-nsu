#include "robot.h"
#include <iostream>
#include <vector>

int main() {
    std::cout << "--- 1) default ---\n";
    QuadrupedRobot a;

    std::cout << "\n--- 2) initialization ---\n";
    QuadrupedRobot b("R1", "Spot", 55.0, "walk", 5.0,
                     std::vector<double>(10, 1.0));

    std::cout << "\n--- 3) copy constructor (создание нового объекта) ---\n";
    QuadrupedRobot c(b);            // вызывается КОНСТРУКТОР КОПИРОВАНИЯ

    std::cout << "\n--- 4) assignment operator (уже существующий объект) ---\n";
    a = b;                          // вызывается ОПЕРАТОР ПРИСВАИВАНИЯ

    std::cout << "\n--- нормализация значений ---\n";
    QuadrupedRobot d("R2", "", -50.0, "stand", -3.0,
                     std::vector<double>({-1.0, 2.0, -3.0, 4.0}));
    std::cout << "name=" << d.getName()
              << " battery=" << d.getBatteryLevel()
              << " todayDist=" << d.getTodayDistance()
              << " last10.size=" << d.getLast10().size() << "\n";

    return 0;
}