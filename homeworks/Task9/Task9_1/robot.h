#pragma once
#include <iostream>
#include <string>

class RobotFactory;

// Задание 9.I: два способа дать другому коду доступ к приватному полю.
//
//   п.1 — дружественная СВОБОДНАЯ функция setManufacturer(): доступ ровно к
//         одному полю (manufacturer);
//   п.2 — дружественный КЛАСС RobotFactory(): доступ ко всем приватным полям,
//         включая id, uid и приватную статику.
//
// Публичный API при этом остаётся закрытым для этих полей: сеттеров нет,
// наружу поля отдаются только на чтение.
class QuadrupedRobot {
private:
    static int objectCount;
    static int nextUid;   // приватная статика: дружба даёт доступ и к ней

    int uid;
    std::string id;               // публичного сеттера нет
    std::string name;
    double batteryLevel;
    std::string currentCommand;
    double todayDistance;

    static const int DAYS = 10;
    double* last10;

    // 9.I п.1: название производителя — приватное поле класса
    std::string manufacturer;

    static std::string normalizeName(const std::string& n);
    static std::string normalizeManufacturer(const std::string& m);
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

    // ---- публичные геттеры: всё читается ----
    int getUid() const { return uid; }
    const std::string& getId() const { return id; }
    const std::string& getName() const { return name; }
    double getBatteryLevel() const { return batteryLevel; }
    const std::string& getCurrentCommand() const { return currentCommand; }
    double getTodayDistance() const { return todayDistance; }
    const double* getLast10() const { return last10; }
    const std::string& getManufacturer() const { return manufacturer; }

    // ---- публичные сеттеры: id, uid и manufacturer среди них НЕТ ----
    void setName(const std::string& n);
    void setBatteryLevel(double b);
    void setCurrentCommand(const std::string& c);
    void setTodayDistance(double d);
    void setLast10(const double* arr);

    // 9.I п.1: свободная функция-друг — одно поле
    friend void setManufacturer(QuadrupedRobot& r, const std::string& m);

    // 9.I п.2: класс-друг — все приватные поля и приватная статика
    friend class RobotFactory;

    friend std::ostream& operator<<(std::ostream& os, const QuadrupedRobot& r);
};

// Реализация — в robot.cpp. Вне класса объявлена для обычной видимости имени.
void setManufacturer(QuadrupedRobot& r, const std::string& m);
