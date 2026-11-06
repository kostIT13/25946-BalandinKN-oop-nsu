#pragma once
#include <string>

// Задание 7.I: класс с геттерами и const-корректностью.
// Все методы, которые не изменяют объект, объявлены const.
class QuadrupedRobot {
private:
    const std::string id;          // уникальный, неизменяемый (нет сеттера)
    std::string name;              // непустое
    double batteryLevel;           // 0..100
    std::string currentCommand;    // текущая команда
    double todayDistance;          // >= 0

    static const int DAYS = 10;
    double* last10;                // дистанция за последние 10 дней, >= 0

    // Нормализация (не изменяют объект => static)
    static std::string normalizeName(const std::string& n);
    static double normalizeBattery(double b);
    static double normalizeDistance(double d);

    // Проверка "жёсткого" инварианта (id не должен быть пустым)
    void checkId(const std::string& id) const;

public:
    QuadrupedRobot();
    QuadrupedRobot(const std::string& id, const std::string& name,
                   double battery, const std::string& command,
                   double dist, const double* last10Arr);
    QuadrupedRobot(const QuadrupedRobot& o);
    QuadrupedRobot& operator=(const QuadrupedRobot& o);
    ~QuadrupedRobot();

    // ---------- Геттеры (7.I) ----------
    // Все геттеры const: объект-источник информации не должен изменяться.
    const std::string& getId() const;
    const std::string& getName() const;
    double getBatteryLevel() const;
    const std::string& getCurrentCommand() const;
    double getTodayDistance() const;

    // Возвращаем const double*: наружу отдаём доступ только на чтение,
    // чтобы внешний код не мог испортить инвариант (отрицательные дистанции).
    const double* getLast10() const;
    static int getDays();

    // ---------- Сеттеры (id — без сеттера: неизменяем по условию) ----------
    void setName(const std::string& n);
    void setBatteryLevel(double b);
    void setCurrentCommand(const std::string& c);
    void setTodayDistance(double d);
    void setLast10(const double* arr);
};
