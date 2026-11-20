#pragma once
#include <string>

class QuadrupedRobot;

// Задание 9.II: класс-администратор. Часть его методов — друзья
// QuadrupedRobot (дружба объявлена в robot.h поимённо), поэтому доступ к
// приватным полям получают ЭТИ ФУНКЦИИ, а не класс целиком.
//
// Разрешено менять одно/несколько приватных полей, но НЕ все:
//   setModel()            -> model                      (одно поле)
//   assignRegistration()  -> id + model                 (несколько полей)
// Закрыто для администратора и остаётся недоступным:
//   uid, batteryLevel, currentCommand, todayDistance, last10, nextUid,
//   objectCount — ни один его метод эти поля менять не может.
class Administrator {
private:
    std::string login_;
    int changesCount_ = 0;

public:
    explicit Administrator(const std::string& login);

    // ---- дружественные QuadrupedRobot методы (реализация в admin.cpp) ----
    void setModel(QuadrupedRobot& r, const std::string& model);
    void assignRegistration(QuadrupedRobot& r, const std::string& id,
                            const std::string& model);

    // ---- НЕ дружественные методы: только public API ----
    // Компилируются, потому что работают через геттеры/сеттеры публичной части.
    void renamePublicly(QuadrupedRobot& r, const std::string& newName);
    std::string report(const QuadrupedRobot& r) const;

    int changesCount() const { return changesCount_; }
    const std::string& login() const { return login_; }

    // Демонстрация границ доступа для README: НЕ друг, определения нет,
    // вызывается только в not_allowed_example.cpp (ожидаемая ошибка сборки).
    void forbiddenDemo(QuadrupedRobot& r);
};
