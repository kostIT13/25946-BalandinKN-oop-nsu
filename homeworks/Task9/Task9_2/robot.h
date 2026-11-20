#pragma once
#include "admin.h"   // нужен полный тип: friend объявляется методу Administrator::...

#include <iostream>
#include <string>

// Задание 9.II: класс Administrator содержит функции, дружественные к
// QuadrupedRobot. Дружба объявлена ПО КОНКРЕТНЫМ МЕТОДАМ (не классу целиком),
// поэтому доступ к приватному получают только они — и только к части полей.
//
//   Administrator::setModel()          -> приватное поле model
//   Administrator::assignRegistration() -> поля model + id (несколько, но НЕ все)
//
// Остальные методы администратора (report, renamePublicly) видят только
// public-часть: они объявлены без friend.
class QuadrupedRobot {
private:
    static int objectCount;
    static int nextUid;

    int uid;
    std::string id;              // публичного сеттера нет
    std::string name;
    double batteryLevel;
    std::string currentCommand;
    double todayDistance;

    static const int DAYS = 10;
    double* last10;

    // 9.II: название модели — приватное поле класса
    std::string model;

    static std::string normalizeName(const std::string& n);
    static std::string normalizeModel(const std::string& m);
    static double normalizeBattery(double b);
    static double normalizeDistance(double d);
    void checkId(const std::string& value) const;
    void printMsg(const std::string& what) const;
    void releaseBuffer();
    void ensureBuffer();

public:
    static const std::string class_name;
    static int getObjectCount();
    static int getDays();

    QuadrupedRobot();
    QuadrupedRobot(const std::string& id, const std::string& name,
                   double battery, const std::string& command,
                   double dist, const double* last10Arr);

    QuadrupedRobot(const QuadrupedRobot& o);
    QuadrupedRobot& operator=(const QuadrupedRobot& o);
    QuadrupedRobot(QuadrupedRobot&& o) noexcept;
    QuadrupedRobot& operator=(QuadrupedRobot&& o) noexcept;
    ~QuadrupedRobot();

    void printRobot() const;

    int getUid() const { return uid; }
    const std::string& getId() const { return id; }
    const std::string& getName() const { return name; }
    double getBatteryLevel() const { return batteryLevel; }
    const std::string& getCurrentCommand() const { return currentCommand; }
    double getTodayDistance() const { return todayDistance; }
    const double* getLast10() const { return last10; }
    const std::string& getModel() const { return model; }

    void setName(const std::string& n);
    void setBatteryLevel(double b);
    void setCurrentCommand(const std::string& c);
    void setTodayDistance(double d);
    void setLast10(const double* arr);

    // 9.II: дружба объявлена конкретным методам класса Administrator.
    // Доступ к приватному получает только они, а не весь класс.
    friend void Administrator::setModel(QuadrupedRobot& r,
                                        const std::string& model);
    friend void Administrator::assignRegistration(QuadrupedRobot& r,
                                                  const std::string& id,
                                                  const std::string& model);

    friend std::ostream& operator<<(std::ostream& os, const QuadrupedRobot& r);
};
