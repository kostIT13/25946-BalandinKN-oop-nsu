#pragma once
#include "robot.h"

#include <string>
#include <vector>

// ============================================================
// КОМПОЗИЦИЯ: «сильное» владение.
//
// Роботы живут ВНУТРИ отделения (std::vector<QuadrupedRobot> по значению):
//   - отделение само их создаёт в конструкторе и само уничтожает в деструкторе;
//   - время жизни робота строго внутри времени жизни отделения;
//   - робота нельзя отдать наружу «как объект», чтобы он пережил отделение:
//     указатель/ссылка на элемент vector после выхода из блока — висячий;
//   - одно и то же животное (тот же uid) не может состоять в двух отделениях:
//     при добавлении во второе происходит копия;
//   - при копировании отделения копируются и все роботы (глубокая копия),
//     при перемещении — переходят целиком (владение уезжает вместе с отделением).
// ============================================================
class RobotUnit {
private:
    std::string name_;
    std::vector<QuadrupedRobot> squad_;   // владеет объектами

    static const int MIN_SIZE = 5;
    static const int MAX_SIZE = 10;

public:
    // Отделение обязано набрать 5..10 роботов — роботы создаются здесь же.
    RobotUnit(const std::string& name, int count);
    ~RobotUnit();

    // Копирование: глубокая копия (вместе с роботами) — композиции принадлежит
    // своя копия каждого робота.
    RobotUnit(const RobotUnit& other);
    RobotUnit& operator=(const RobotUnit& other);

    // Перемещение: владение роботами уезжает целиком, роботы не копируются.
    RobotUnit(RobotUnit&& other) noexcept;
    RobotUnit& operator=(RobotUnit&& other) noexcept;

    // Публичный доступ только на чтение: наружу не отдаём ни владение,
    // ни изменяемый объект.
    std::size_t size() const { return squad_.size(); }
    const std::string& name() const { return name_; }
    const QuadrupedRobot& at(std::size_t i) const { return squad_.at(i); }
    void rollCall() const;

    // «Списать» робота: копия уходит наружу, оригинал уничтожается вместе с
    // отделением. Наружу никогда не отдаётся сам объект-владелец.
    QuadrupedRobot retirementCopy(std::size_t i) const;

    static int maxSize() { return MAX_SIZE; }
};

// ============================================================
// АГРЕГАЦИЯ: «слабое» владение.
//
// Отделение только ССЫЛАЕТСЯ на роботов (std::vector<QuadrupedRobot*>),
// которые созданы и принадлежат внешнему коду:
//   - отделение может быть пустым и может содержать любое число роботов;
//   - уничтожение отделения НЕ уничтожает роботов;
//   - один и тот же робот может состоять сразу в двух отделениях (тот же uid);
//   - робот может переехать из одного отделения в другое;
//   - цена свободы: отделение обязано следить, чтобы робот пережил его.
//     Умрёт внешний робот раньше — указатель станет висячим (демонстрация в
//     main.cpp закомментирована, потому что читать его — UB).
// ============================================================
class RobotSquad {
private:
    std::string name_;
    std::vector<QuadrupedRobot*> members_;   // НЕ владеет

public:
    explicit RobotSquad(const std::string& name);

    bool enroll(QuadrupedRobot* r);          // false, если робот уже в составе
    bool discharge(QuadrupedRobot* r);
    std::size_t size() const { return members_.size(); }
    const std::string& name() const { return name_; }
    bool has(const QuadrupedRobot* r) const;
    void rollCall() const;

    // Агрегация позволяет менять общее: робот один, а изменений видно из
    // обоих отделений.
    void issueCommand(const std::string& cmd);
};
