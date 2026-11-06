#pragma once
#include <string>
#include <iostream>

// Задание 7.III: класс доработан для main из примера (Book):
//  - статический print_count() и публичное статическое class_name
//  - метод printRobot() (аналог printBook) — печать информации об объекте
//  - метод addDistance() (аналог add_bookmark) — добавляет дистанцию за день
//  - оператор << для вывода объекта в поток
class QuadrupedRobot {
private:
    static int objectCount;      // количество живых объектов
    static int nextUid;          // генератор уникальных идентификаторов

    const int uid;               // уникальный числовой идентификатор объекта
    const std::string id;        // текстовый id = "R" + uid
    std::string name;
    double batteryLevel;         // 0..100
    std::string currentCommand;
    double todayDistance;        // >= 0

    static const int DAYS = 10;
    double* last10;              // дистанции за последние 10 дней

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

    // аналог printBook()
    void printRobot() const;

    // аналог add_bookmark(int): добавляет дистанцию к todayDistance
    void addDistance(double d);

    // геттеры
    int getUid() const;
    const std::string& getId() const;
    const std::string& getName() const;
    double getBatteryLevel() const;
    const std::string& getCurrentCommand() const;
    double getTodayDistance() const;
    const double* getLast10() const;
    static int getDays();

    // сеттеры
    void setName(const std::string& n);
    void setBatteryLevel(double b);
    void setCurrentCommand(const std::string& c);
    void setTodayDistance(double d);
    void setLast10(const double* arr);

    friend std::ostream& operator<<(std::ostream& os, const QuadrupedRobot& r);

    // для использования в std::set (сравнение по uid)
    friend bool operator<(const QuadrupedRobot& a, const QuadrupedRobot& b);
};
