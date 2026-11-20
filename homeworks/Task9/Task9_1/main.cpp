// Задание 9.I: дружественная функция (одно поле) и дружественный класс
// (все поля).
//
// п.1: setManufacturer(r, m) — свободная функция-друг. Доступ ровно к одному
//      приватному полю (manufacturer). Всё остальное для неё закрыто.
// п.2: RobotFactory — класс-друг. Доступ ко всем приватным полям и к
//      приватной статике (objectCount, nextUid).
//
// Дополнительно в README показана строка, которая НЕ компилируется: та же
// запись из обычной функции даёт ошибку компиляции — то есть доступ реально
// даёт дружба, а не «всё вокруг приватно на бумаге».
#include "factory.h"
#include "robot.h"

#include <iostream>
#include <vector>

namespace {

void sep(const std::string& title) {
    const size_t dashes = title.size() + 2 < 62 ? 62 - title.size() - 2 : 2;
    std::cout << "\n=== " << title << " " << std::string(dashes, '-') << "\n";
}

// Обычная (не дружественная) функция: видит только public-часть.
void showPublicApiOnly(const QuadrupedRobot& r) {
    // r.manufacturer = "Hacked";   // ошибка компиляции: private
    // r.setId("X");               // ошибка компиляции: сеттера id нет
    std::cout << "  [outside class] доступен только public API: name="
              << r.getName() << ", vendor(геттер)=" << r.getManufacturer()
              << "\n";
}

std::vector<double> history() {
    std::vector<double> h;
    for (int i = 1; i <= 10; ++i) h.push_back(i);
    return h;
}

}  // namespace

int main() {
    sep("0. Объект создан: manufacturer скрыт за private");
    std::vector<double> h = history();
    QuadrupedRobot robot("R-777", "Spot", 55.0, "walk", 7.0, h.data());
    std::cout << "  " << robot << "\n";
    showPublicApiOnly(robot);

    sep("9.I п.1: дружественная ФУНКЦИЯ ставит одно приватное поле");
    setManufacturer(robot, "Boston Dynamics");
    std::cout << "  после setManufacturer(): " << robot << "\n";
    std::cout << "  проверить извне: getManufacturer() = "
              << robot.getManufacturer() << "\n";
    std::cout << "  остальные поля функция не трогала: name="
              << robot.getName() << ", battery=" << robot.getBatteryLevel()
              << ", id=" << robot.getId() << "\n";
    setManufacturer(robot, "");   // нормализация внутри друга
    std::cout << "  setManufacturer(\"\") -> '" << robot.getManufacturer()
              << "' (друг отвечает за инвариант поля)\n";

    sep("9.I п.2: дружественный КЛАСС меняет все приватные поля");
    RobotFactory plant("ExpoRobotics");

    // (а) завод собирает робота напрямую в приватные поля
    QuadrupedRobot fromLine = plant.produce("R-100", "Tombot", 90.0, "patrol",
                                            0.0, h.data());
    std::cout << "  " << fromLine << "\n";

    // (б) завод перебивает всё в уже готовом объекте, включая id и uid —
    //     публичных сеттеров для них не существует вообще
    plant.repurpose(robot, "R-777-UPG", "Spot-II", 42.0, "run", 12.5,
                    "ExpoRobotics");
    std::cout << "  " << robot << "\n";

    // (в) доступ к приватной статике класса
    plant.audit(robot);
    plant.audit(fromLine);
    std::cout << "  nextUid изнутри завода: " << plant.nextUidPreview() << "\n";
    plant.setNextUid(500);
    std::cout << "  nextUid после setNextUid(500): " << plant.nextUidPreview()
              << "\n";

    sep("Разница п.1 и п.2");
    std::cout << "  дружественная функция : доступ к 1 полю, объём доступа"
                 " задаётся сигнатурой\n";
    std::cout << "  дружественный класс   : доступ ко всем полям + приватная"
                 " статика + приватные методы\n";
    std::cout << "  в обоих случаях дружба НЕ наследуется и НЕ"
                 " передаётся по наследникам, и наружу поля по-прежнему"
                 " только читаются\n";
    return 0;
}
