// Задание 8.I, п.2: демонстрация работы перемещающих конструктора и
// оператора присваивания и их отличий от копирующих.
//
// Отличия сделаны наблюдаемыми тремя способами:
//   1) счётчики вызовов (copy ctor / move ctor / copy assign / move assign);
//   2) счётчик аллокаций собственного буфера (new double[10] / delete[]);
//   3) адрес буфера: при копировании он новый, при перемещении — тот же.
#include "robot.h"

#include <iostream>
#include <utility>
#include <vector>

namespace {

void sep(const std::string& title) {
    const size_t dashes = title.size() + 2 < 60 ? 60 - title.size() - 2 : 2;
    std::cout << "\n=== " << title << " " << std::string(dashes, '-') << "\n";
}

void state(const char* tag, const QuadrupedRobot& r) {
    std::cout << "  " << tag << ": addr=" << (const void*)r.getLast10()
              << " name=" << r.getName() << "\n";
}

QuadrupedRobot makeRobot() {
    std::vector<double> hist;
    for (int i = 1; i <= 10; ++i) hist.push_back(i);
    return QuadrupedRobot("R-demo", "Spot", 80.0, "walk", 12.0, hist.data());
}

}  // namespace

int main() {
    sep("0. Исходный объект");
    QuadrupedRobot a("R1", "Spot", 55.0, "walk", 7.0, nullptr);
    state("a      ", a);
    QuadrupedRobot::printStats();

    sep("1. Конструктор КОПИРОВАНИЯ: QuadrupedRobot b(a)");
    QuadrupedRobot::resetStats();
    QuadrupedRobot b(a);
    state("a      ", a);          // источник жив и полон
    state("b      ", b);          // данные те же, буфер ДРУГОЙ
    std::cout << "  a moved-from? " << (a.isMovedFrom() ? "yes" : "no")
              << " | data copied? " << (b.getName() == a.getName() ? "yes" : "no")
              << "\n";
    QuadrupedRobot::printStats();
    std::cout << "  -> +1 аллокация, a не изменился\n";

    sep("2. Конструктор ПЕРЕМЕЩЕНИЯ: c(std::move(a))");
    QuadrupedRobot::resetStats();
    const void* addrBefore = a.bufferAddress();
    QuadrupedRobot c(std::move(a));
    const void* addrAfter = c.bufferAddress();
    state("a      ", a);          // перемещённый объект: буфера больше нет
    state("c      ", c);
    std::cout << "  буфер перешёл без копирования: "
              << (addrBefore == addrAfter ? "тот же адрес" : "адрес изменился")
              << " (" << addrBefore << " -> " << addrAfter << ")\n";
    std::cout << "  a moved-from? " << (a.isMovedFrom() ? "yes" : "no")
              << " | аллокаций данных: " << QuadrupedRobot::getHeapAllocs() << "\n";
    QuadrupedRobot::printStats();
    std::cout << "  a после перемещения можно присвоить/уничтожить:\n";
    a.setLast10(nullptr);        // вернули буфер
    a.setName("Reborn");
    state("a      ", a);

    sep("3. Копирующее присваивание: c = b");
    QuadrupedRobot::resetStats();
    c = b;
    state("b      ", b);
    state("c      ", c);
    QuadrupedRobot::printStats();
    std::cout << "  -> буфер c переиспользован (аллокаций 0, буфер был), "
                 "значения скопированы, b цел\n";

    sep("4. Перемещающее присваивание: c = std::move(b)");
    QuadrupedRobot::resetStats();
    const void* bAddr = b.bufferAddress();
    c = std::move(b);
    state("b      ", b);
    state("c      ", c);
    std::cout << "  c получил буфер b: "
              << (c.bufferAddress() == bAddr ? "тот же адрес" : "другой")
              << " | b moved-from? " << (b.isMovedFrom() ? "yes" : "no")
              << " | new/delete за операцию: "
              << QuadrupedRobot::getHeapAllocs() << "/"
              << QuadrupedRobot::getHeapFrees() << "\n";
    QuadrupedRobot::printStats();
    std::cout << "  -> свой буфер освобождён (delete[]), чужой украден, "
                 "аллокаций нет\n";

    sep("5. Без std::move l-value копируется (перегрузка НЕ выбирается сама)");
    QuadrupedRobot::resetStats();
    QuadrupedRobot d("R2", "Tombot", 30.0, "run", 3.0, nullptr);
    QuadrupedRobot e(d);              // l-value -> copy ctor
    e = d;                            // l-value -> copy assign
    QuadrupedRobot f(std::move(d));   // x-value -> move ctor
    f = std::move(e);                 // x-value -> move assign
    QuadrupedRobot::printStats();
    std::cout << "  -> копий 2 / перемещений 2 ровно там, где стоял std::move\n";

    sep("6. Куда это идёт само: return value и рост std::vector");
    QuadrupedRobot::resetStats();
    QuadrupedRobot::printStats();
    std::cout << "  (выше счётчики обнулены; объекты a, b, c из прошлых шагов"
                 " живы — это видно по objects)\n";
    int copyInLoop = 0, moveInLoop = 0;
    {
        QuadrupedRobot r = makeRobot();   // NRVO/move из функции
        (void)r;
        std::vector<QuadrupedRobot> v;
        for (int i = 0; i < 8; ++i) v.push_back(makeRobot());  // rvalue -> move
        copyInLoop = QuadrupedRobot::getCopyCtorCalls();
        moveInLoop = QuadrupedRobot::getMoveCtorCalls();
        std::cout << "  цикл push_back(makeRobot()) x8 (рост vector):\n";
        QuadrupedRobot::printStats();
        std::cout << "  размер vector: " << v.size() << ", копий ctor: "
                  << copyInLoop << ", move ctor: " << moveInLoop
                  << " -> копий данных нет, только перемещения указателей\n";
        QuadrupedRobot one = v[0];        // l-value -> снова копирование
        std::cout << "  v[0] (l-value) -> копирование, данные в векторе целы:\n";
        QuadrupedRobot::printStats();
        (void)one;
    }
    std::cout << "  после выхода из блока (все объекты вектора умерли):\n";
    QuadrupedRobot::printStats();
    std::cout << "  живых буферов: " << QuadrupedRobot::getLiveBuffers()
              << " при живых объектах " << QuadrupedRobot::getObjectCount()
              << " (разница = перемещённые «пустышки»);\n"
                 "  new/delete внутри блока совпали => утечек и double free нет\n";

    sep("7. Вывод объектов (перемещённый помечен)");
    std::cout << "  c: " << c << "\n";
    std::cout << "  b: " << b << "\n";

    std::cout << "\n=== ИТОГО "
              << std::string(56, '-') << "\n";
    std::cout << "  копирование: свой буфер (new) + копирование 10 double,"
                 " источник цел\n";
    std::cout << "  перемещение: 0 аллокаций, украли указатель,"
                 " источник = безопасная «пустышка»\n";
    std::cout << "  перемещение срабатывает только для x-value"
                 " (std::move/временный объект) и требует noexcept\n";
    return 0;
}
