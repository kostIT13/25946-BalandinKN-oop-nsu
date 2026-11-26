#pragma once
#include <iostream>
#include <string>

class QuadrupedRobot {
private:
    static int objectCount;
    static int nextUid;

    int uid;
    std::string id;
    std::string name;
    double batteryLevel;
    std::string currentCommand;
    double todayDistance;

    static const int DAYS = 10;
    double* last10;

    static std::string normalizeName(const std::string& n);
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

    void setName(const std::string& n);
    void setBatteryLevel(double b);
    void setCurrentCommand(const std::string& c);
    void setTodayDistance(double d);
    void setLast10(const double* arr);
};
