// Задание 7.III п.3: добавление объектов класса в vector, list и set
// (по 4-10 в каждый) и анализ вывода: сколько объектов создано,
// какие конструкторы/деструкторы вызываются.
#include "robot.h"
#include <vector>
#include <list>
#include <set>
#include <iostream>

int main() {
    std::cout << "=== старт: объектов = " << QuadrupedRobot::getObjectCount() << " ===\n\n";

    // ---------- vector ----------
    std::cout << "----- vector -----\n";
    std::vector<QuadrupedRobot> v;
    v.reserve(4);                       // reserve: конструкторы НЕ вызываются
    for (int i = 0; i < 4; ++i)
        v.push_back(QuadrupedRobot("R" + std::to_string(i), "Spot", 80.0, "walk",
                                   static_cast<double>(i), nullptr));
    // Анализ: каждый push_back создаёт временный объект (initialization constructor),
    // затем копирует его в контейнер (copy constructor) и уничтожает временный
    // (destructor). Итого на 4 push_back: 4 init + 4 copy + 4 dtor.

    // ---------- list ----------
    std::cout << "\n----- list -----\n";
    std::list<QuadrupedRobot> l;
    for (int i = 0; i < 5; ++i)
        l.push_back(QuadrupedRobot("L" + std::to_string(i), "Spot", 80.0, "walk",
                                   static_cast<double>(i), nullptr));
    // На 5 push_back: 5 init + 5 copy + 5 dtor.

    // ---------- set ----------
    std::cout << "\n----- set -----\n";
    std::set<QuadrupedRobot> s;         // требуется operator<
    for (int i = 0; i < 4; ++i)
        s.insert(QuadrupedRobot("S" + std::to_string(i), "Spot", 80.0, "walk",
                                static_cast<double>(i), nullptr));
    // На 4 insert: 4 init + 4 copy + 4 dtor.

    std::cout << "\n=== объекты в контейнерах (живых): "
              << QuadrupedRobot::getObjectCount() << " ===\n";

    std::cout << "\n=== очистка контейнеров (деструкторы элементов) ===\n";
    s.clear();
    l.clear();
    v.clear();

    std::cout << "\n=== после очистки: объектов = "
              << QuadrupedRobot::getObjectCount() << " ===\n";
    return 0;
}
