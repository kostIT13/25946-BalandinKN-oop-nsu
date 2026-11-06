#pragma once
#include <string>

// Задание 7.II: специальные статические поля класса.
//  a) objectCount        - количество живых объектов класса
//  b) uid (у каждого)    - уникальный числовой идентификатор объекта;
//                          текстовый id теперь генерируется как "R" + uid
//  c) class_name         - имя класса (строка)
// Все сообщения конструкторов/деструктора/оператора присваивания содержат
// текущее количество объектов; вывод оформлен разделителями.
class QuadrupedRobot {
private:
    static int objectCount;      // a) счётчик живых объектов
    static int nextUid;          // b) генератор уникальных идентификаторов

    const int uid;               // b) уникальный идентификатор ЭТОГО объекта
    const std::string id;        // текстовый id = "R" + uid
    std::string name;
    double batteryLevel;         // 0..100
    std::string currentCommand;
    double todayDistance;        // >= 0

    static const int DAYS = 10;
    double* last10;

    static std::string normalizeName(const std::string& n);
    static double normalizeBattery(double b);
    static double normalizeDistance(double d);
    void checkId(const std::string& id) const;

    // короткое читаемое сообщение + текущее количество объектов
    void printMsg(const std::string& what) const;

public:
    static const std::string class_name;   // c) имя класса (как в примере Book)

    static int getObjectCount();           // a) текущее количество
    static void print_count();             // печать количества (как print_count в примере)

    QuadrupedRobot();
    QuadrupedRobot(const std::string& id, const std::string& name,
                   double battery, const std::string& command,
                   double dist, const double* last10Arr);
    QuadrupedRobot(const QuadrupedRobot& o);
    QuadrupedRobot& operator=(const QuadrupedRobot& o);
    ~QuadrupedRobot();

    // геттеры
    int getUid() const;
    const std::string& getId() const;
    const std::string& getName() const;
    double getBatteryLevel() const;
    const std::string& getCurrentCommand() const;
    double getTodayDistance() const;
    const double* getLast10() const;
    static int getDays();

    // сеттеры (id не изменяется)
    void setName(const std::string& n);
    void setBatteryLevel(double b);
    void setCurrentCommand(const std::string& c);
    void setTodayDistance(double d);
    void setLast10(const double* arr);
};
