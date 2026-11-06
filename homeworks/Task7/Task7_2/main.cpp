// Задание 7.II: счётчик объектов, уникальные идентификаторы,
// имя класса; сообщения с текущим количеством объектов.
#include "robot.h"
#include <iostream>

static void sep(const std::string& title) {
    std::cout << "---------- " << title << " ----------\n";
}

int main() {
    sep("static values");
    QuadrupedRobot::print_count();                      // объектов: 0
    std::cout << "  class name: " << QuadrupedRobot::class_name << "\n\n";

    sep("creation");
    double arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    QuadrupedRobot a("R1", "Spot", 55.0, "walk", 5.0, arr);   // #1
    QuadrupedRobot b;                                          // #2
    QuadrupedRobot::print_count();                             // объектов: 2

    sep("copy constructor");
    QuadrupedRobot c(a);                                       // #3

    sep("assignment operator");
    b = a;                                                     // объекты: 2 (кол-во не меняется)

    sep("scope {} : создание и уничтожение");
    {
        QuadrupedRobot d(b);                                   // #4
        QuadrupedRobot::print_count();                         // объектов: 4
    }                                                          // деструктор #4

    sep("after scope");
    QuadrupedRobot::print_count();                             // объектов: 3

    sep("end of main: деструкторы c, b, a");
    return 0;
}
