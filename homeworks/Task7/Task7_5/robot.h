#pragma once
#include <string>
#include <iostream>

// Задание 7.V: операторы ввода (>>) и вывода (<<).
// operator<< выводит основную информацию об объекте, включая:
//   - имя класса, уникальный идентификатор (uid), текущее количество объектов
// operator>> строго ограничен 4 параметрами: name battery command distance
class QuadrupedRobot {
private:
    static int objectCount;
    static int nextUid;

    const int uid;
    const std::string id;
    std::string name;
    double batteryLevel;
    std::string currentCommand;
    double todayDistance;

    static const int DAYS = 10;
    double* last10;

    static std::string normalizeName(const std::string& n);
    static double normalizeBattery(double b);
    static double normalizeDistance(double d);
    void checkId(const std::string& id) const;
    void printMsg(const std::string& what) const;

public:
    static const std::string class_name;
    static int getObjectCount();
    static void print_count();

    QuadrupedRobot();
    QuadrupedRobot(const std::string& id, const std::string& name,
                   double battery, const std::string& command,
                   double dist, const double* last10Arr);
    QuadrupedRobot(const QuadrupedRobot& o);
    QuadrupedRobot& operator=(const QuadrupedRobot& o);
    ~QuadrupedRobot();

    void printRobot() const;

    int getUid() const;
    const std::string& getId() const;
    const std::string& getName() const;
    double getBatteryLevel() const;
    const std::string& getCurrentCommand() const;
    double getTodayDistance() const;
    const double* getLast10() const;
    static int getDays();

    void setName(const std::string& n);
    void setBatteryLevel(double b);
    void setCurrentCommand(const std::string& c);
    void setTodayDistance(double d);
    void setLast10(const double* arr);

    // 7.V: операторы ввода/вывода (дружественные)
    friend std::ostream& operator<<(std::ostream& os, const QuadrupedRobot& r);
    friend std::istream& operator>>(std::istream& is, QuadrupedRobot& r);
};
