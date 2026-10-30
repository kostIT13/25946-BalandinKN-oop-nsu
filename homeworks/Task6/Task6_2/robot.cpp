#include "robot.h"
#include <iostream>
#include <stdexcept>
#include <algorithm>

std::string QuadrupedRobot::normalizeName(const std::string& n) {
    return n.empty() ? "Unnamed" : n;
}

double QuadrupedRobot::normalizeBattery(double b) {
    if (b < 0.0) return 0.0;
    if (b > 100.0) return 100.0;
    return b;
}

double QuadrupedRobot::normalizeDistance(double d) {
    return d < 0.0 ? 0.0 : d;
}

std::vector<double> QuadrupedRobot::normalizeLast10(const std::vector<double>& v) {
    std::vector<double> r = v;
    for (double& x : r) if (x < 0.0) x = 0.0;
    if (r.size() < 10) r.resize(10, 0.0);        // дополнить нулями
    else if (r.size() > 10) r.resize(10);        // обрезать
    return r;
}

void QuadrupedRobot::checkId(const std::string& id) {
    if (id.empty())
        throw std::invalid_argument("id must not be empty");
}

// ---------- Конструкторы ----------
QuadrupedRobot::QuadrupedRobot()
    : id("R0"),
      name(normalizeName("Unnamed")),
      batteryLevel(normalizeBattery(100.0)),
      currentCommand("idle"),
      todayDistance(normalizeDistance(0.0)),
      last10(normalizeLast10(std::vector<double>(10, 0.0)))
{
    std::cout << "default constructor\n";
}

QuadrupedRobot::QuadrupedRobot(const std::string& id, const std::string& name,
                               double battery, const std::string& command,
                               double dist, const std::vector<double>& last10)
    : id(id),
      name(normalizeName(name)),
      batteryLevel(normalizeBattery(battery)),
      currentCommand(command),
      todayDistance(normalizeDistance(dist)),
      last10(normalizeLast10(last10))
{
    checkId(id);
    std::cout << "initialization constructor\n";
}

QuadrupedRobot::QuadrupedRobot(const QuadrupedRobot& o)
    : id(o.id),
      name(o.name),
      batteryLevel(o.batteryLevel),
      currentCommand(o.currentCommand),
      todayDistance(o.todayDistance),
      last10(o.last10)
{
    std::cout << "copy constructor\n";
}

// ---------- Оператор присваивания ----------
QuadrupedRobot& QuadrupedRobot::operator=(const QuadrupedRobot& o) {
    std::cout << "assignment operator\n";
    if (this != &o) {
        const_cast<std::string&>(id) = o.id;
        name = o.name;
        batteryLevel = o.batteryLevel;
        currentCommand = o.currentCommand;
        todayDistance = o.todayDistance;
        last10 = o.last10;
    }
    return *this;
}

// ---------- Геттеры ----------
const std::string& QuadrupedRobot::getId() const { return id; }
const std::string& QuadrupedRobot::getName() const { return name; }
double QuadrupedRobot::getBatteryLevel() const { return batteryLevel; }
const std::string& QuadrupedRobot::getCurrentCommand() const { return currentCommand; }
double QuadrupedRobot::getTodayDistance() const { return todayDistance; }
const std::vector<double>& QuadrupedRobot::getLast10() const { return last10; }

// ---------- Сеттеры (с нормализацией) ----------
void QuadrupedRobot::setName(const std::string& n) {
    name = normalizeName(n);
}
void QuadrupedRobot::setBatteryLevel(double b) {
    batteryLevel = normalizeBattery(b);
}
void QuadrupedRobot::setCurrentCommand(const std::string& c) {
    currentCommand = c;
}
void QuadrupedRobot::setTodayDistance(double d) {
    todayDistance = normalizeDistance(d);
}
void QuadrupedRobot::setLast10(const std::vector<double>& v) {
    last10 = normalizeLast10(v);
}