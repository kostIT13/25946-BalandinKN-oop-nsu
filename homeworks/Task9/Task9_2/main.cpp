// Задание 9.II: класс Administrator, чьи функции дружественны классу
// QuadrupedRobot и могут менять одно или несколько приватных полей (не всех).
//
// Схема доступности:
//   Administrator::setModel()           -> model                  (1 поле, друг)
//   Administrator::assignRegistration() -> id + model             (2 поля, друг)
//   Administrator::renamePublicly()     -> name через public API  (не друг)
//   недоступно администратору полностью: uid, batteryLevel, currentCommand,
//                                        last10, static nextUid/objectCount
//
// Дружба объявлена в robot.h поимённо: friend void Administrator::setModel(...).
// Именно поэтому приватный доступ получают две функции, а не весь класс.
#include "admin.h"
#include "robot.h"

#include <iostream>
#include <vector>

namespace {

void sep(const std::string& title) {
    const size_t dashes = title.size() + 2 < 62 ? 62 - title.size() - 2 : 2;
    std::cout << "\n=== " << title << " " << std::string(dashes, '-') << "\n";
}

std::vector<double> history() {
    std::vector<double> h;
    for (int i = 1; i <= 10; ++i) h.push_back(i);
    return h;
}

}  // namespace

int main() {
    std::vector<double> h = history();

    sep("0. Модель не задана: поле model приватное, сеттера нет");
    QuadrupedRobot robot("R-501", "Spot", 65.0, "patrol", 3.0, h.data());
    std::cout << "  " << robot << "\n";

    Administrator admin("admin.ivannov");

    sep("9.II: дружественный метод ставит ОДНО приватное поле");
    admin.setModel(robot, "Unitree-Go2");
    std::cout << "  " << robot << "\n";
    std::cout << "  id не изменился (метод его не трогает): " << robot.getId()
              << "\n";

    sep("9.II: дружественный метод ставит НЕСКОЛЬКО приватных полей");
    admin.assignRegistration(robot, "R-501-REG", "Unitree-Go2-Pro");
    std::cout << "  " << robot << "\n";
    std::cout << "  изменены id + model; поля, недоступные администратору,"
                 " целы: battery=" << robot.getBatteryLevel()
              << ", cmd=" << robot.getCurrentCommand()
              << ", today=" << robot.getTodayDistance()
              << ", uid=" << robot.getUid() << "\n";

    sep("Методы НЕ-друзья работают только через public API");
    admin.renamePublicly(robot, "Spot-Renamed");
    std::cout << "  " << admin.report(robot) << "\n";
    std::cout << "  uid администратор не менял: " << robot.getUid()
              << " (доступен только геттер getUid())\n";

    sep("Итог доступа");
    std::cout << "  разрешено администратору : model (setModel),"
                 " id+model (assignRegistration)\n";
    std::cout << "  недоступно администратору: uid, batteryLevel,"
                 " currentCommand, todayDistance, last10, static nextUid/"
                 "objectCount\n";
    std::cout << "  сравнение с 9.I: там дружба была классу RobotFactory"
                 " целиком (friend class) и завод мог менять всё;\n"
                 "  здесь дружба объявлена ДВУМ методам — доступ минимален"
                 " ровно на столько, сколько нужно роли администратора\n";
    std::cout << "  изменений администратором: " << admin.changesCount() << "\n";
    return 0;
}
