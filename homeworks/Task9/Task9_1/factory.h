#pragma once
#include "robot.h"

#include <string>

// Задание 9.I п.2: дружественный класс — завод-производитель.
//
// RobotFactory имеет доступ ко ВСЕМ приватным полям QuadrupedRobot:
// id, uid, name, batteryLevel, currentCommand, todayDistance, last10,
// manufacturer, а также к приватной статике (objectCount, nextUid) и к
// приватным методам (normalizeXxx, ensureBuffer, checkId).
//
// Отличие от дружественной функции (п.1): дружба с классом распространяется
// на все его методы и сразу даёт доступ ко всему приватному интерфейсу,
// поэтому контроль «что именно можно менять» здесь уже ответственность
// самого завода, а языка.
class RobotFactory {
private:
    std::string plantName_;

public:
    explicit RobotFactory(const std::string& plantName = "UnknownPlant");

    // Сборка робота «с нуля»: заполняет приватные поля напрямую, минуя
    // публичные сеттеры и их нормализацию-ограничения.
    QuadrupedRobot produce(const std::string& id, const std::string& name,
                           double battery, const std::string& command,
                           double dist, const double* last10Arr) const;

    // Перебивает ВСЕ приватные поля уже готового объекта — то, что снаружи
    // невозможно ни одним публичным способом (id и uid не имеют сеттеров).
    void repurpose(QuadrupedRobot& r, const std::string& newId,
                   const std::string& newName, double battery,
                   const std::string& command, double dist,
                   const std::string& manufacturer) const;

    // Доступ к приватной статике класса: счётчик объектов и следующий uid.
    void audit(const QuadrupedRobot& r) const;
    void setNextUid(int value);
    int  nextUidPreview() const;

    const std::string& plantName() const { return plantName_; }
};
