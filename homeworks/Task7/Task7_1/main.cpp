// Задание 7.I: демонстрация геттеров и const-корректности.
#include "robot.h"
#include <iostream>

// Функция принимает константную ссылку: объект только читается,
// поэтому доступны только const-методы (геттеры).
void printInfo(const QuadrupedRobot& r) {
    std::cout << "  id=" << r.getId()
              << " name=" << r.getName()
              << " battery=" << r.getBatteryLevel()
              << " cmd=" << r.getCurrentCommand()
              << " today=" << r.getTodayDistance()
              << " last10[0]=" << r.getLast10()[0] << "\n";
}

int main() {
    double arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    QuadrupedRobot r("R1", "Spot", 55.0, "walk", 5.0, arr);

    std::cout << "--- вызов геттеров через const-функцию ---\n";
    printInfo(r);

    // const-объект: можно вызывать ТОЛЬКО const-методы
    std::cout << "--- const-объект ---\n";
    const QuadrupedRobot& cr = r;
    std::cout << "  name=" << cr.getName() << "\n";   // const-геттер: OK
    // cr.setName("X");  // ошибка компиляции: сеттер не const — так и задумано

    std::cout << "--- сеттеры ---\n";
    r.setBatteryLevel(77.0);
    r.setTodayDistance(-3.0);          // нормализуется в 0
    std::cout << "  battery=" << r.getBatteryLevel()
              << " today=" << r.getTodayDistance() << "\n";

    std::cout << "--- статический геттер ---\n";
    std::cout << "  days tracked = " << QuadrupedRobot::getDays() << "\n";
    return 0;
}
