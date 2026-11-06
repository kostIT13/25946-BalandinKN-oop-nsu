#pragma once
#include <string>

// Задание 7.IV: функции нормализации больше НЕ "исправляют" данные,
// а генерируют исключения, если значения выходят за рабочий диапазон.
//  - пустое имя            -> std::invalid_argument
//  - заряд вне [0..100]    -> std::out_of_range
//  - отрицательная дистанция -> std::invalid_argument
//  - пустой id             -> std::invalid_argument
// В main данные вводятся пользователем; при исключении ввод повторяется.
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

    // нормализация с генерацией исключений
    static std::string normalizeName(const std::string& n);
    static double normalizeBattery(double b);
    static double normalizeDistance(double d);
    void checkId(const std::string& id) const;

    void printMsg(const std::string& what) const;

public:
    static const std::string class_name;
    static int getObjectCount();

    QuadrupedRobot();
    QuadrupedRobot(const std::string& id, const std::string& name,
                   double battery, const std::string& command,
                   double dist, const double* last10Arr);
    QuadrupedRobot(const QuadrupedRobot& o);
    QuadrupedRobot& operator=(const QuadrupedRobot& o);
    ~QuadrupedRobot();

    // публичная проверка данных без создания объекта (для повторного ввода в main)
    static void validateArgs(const std::string& name, double battery, double dist);

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
};
