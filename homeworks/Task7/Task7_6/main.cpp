// Задание 7.VI: демонстрация шаблонной реализации класса.
// Типы: id - std::string или int, дистанции - double или int.
#include "robot_t.h"

int main() {
    // Вариант 1: id - строка, дистанции - double (как в основном классе)
    double d[10] = {1.5, 2.5, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0};
    QuadrupedRobotT<std::string, double> a("R1", "Spot", 85.0, "walk", 7.5, d);
    a.printRobot();
    std::cout << "  id=" << a.getId()
              << " today=" << a.getTodayDistance()
              << " days=" << QuadrupedRobotT<std::string, double>::getDays() << "\n";

    // Вариант 2: id - целое число, дистанции - int (шаги)
    int steps[10] = {1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000};
    QuadrupedRobotT<int, int> b(42, "Atlas", 60.0, "run", 12000, steps);
    b.printRobot();

    // Копирование и присваивание шаблонных объектов
    QuadrupedRobotT<int, int> c(b);
    QuadrupedRobotT<int, int> e;
    e = b;
    e.printRobot();
    return 0;
}
