// Задание 7.IV: при некорректных данных нормализация кидает исключение,
// main ловит его и даёт пользователю возможность исправить данные вручную
// (повторный ввод).
#include "robot.h"
#include <iostream>
#include <limits>
#include <string>

int main() {
    std::cout << "Enter robot data: <name> <battery 0..100> <command> <distance >=0>\n";

    std::string name, command;
    double battery, dist;
    bool ok = false;

    while (!ok) {
        std::cout << "> ";
        if (!(std::cin >> name >> battery >> command >> dist)) {
            std::cout << "  Input error: expected 4 values (string double string double)\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        // Локальная проверка данных через нормализацию ДО создания объекта:
        // если данные плохие, исключение перехватим и попросим ввести заново.
        try {
            // допустим int-значения заряда/дистанции: 55.0 и т.п. — ок
            QuadrupedRobot::validateArgs(name, battery, dist);
            ok = true;
        } catch (const std::exception& e) {
            std::cout << "  Invalid data: " << e.what() << "\n"
                      << "  Please fix and re-enter.\n";
        }
    }

    // Данные корректны — создаём объект (исключений уже не будет)
    QuadrupedRobot r("R1", name, battery, command, dist, nullptr);
    std::cout << "Robot created:\n";
    r.printRobot();
    return 0;
}
