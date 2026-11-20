// Проверка для README: доступ к приватному из НЕдружественного метода
// класса Administrator. Ожидается ошибка компиляции — дружба в robot.h
// объявлена только методам setModel() и assignRegistration().
#include "admin.h"
#include "robot.h"

void Administrator::forbiddenDemo(QuadrupedRobot& r) {
    r.uid = 1;                          // private, метод не друг
    r.batteryLevel = 0.0;               // private, метод не друг
    r.last10[0] = 99.0;                 // private, метод не друг
    QuadrupedRobot::nextUid = 42;       // private static, метод не друг
}

int main() { return 0; }
