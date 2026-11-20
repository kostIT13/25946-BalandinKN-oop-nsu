// Проверка для README: попытка обратиться к приватному полю из НЕдружественной
// функции. Ожидается ошибка компиляции.
#include "robot.h"

void notAFriend(QuadrupedRobot& r) {
    r.manufacturer = "Hacked";
    r.id = "stolen-id";
    QuadrupedRobot::objectCount = 0;
}

int main() { return 0; }
