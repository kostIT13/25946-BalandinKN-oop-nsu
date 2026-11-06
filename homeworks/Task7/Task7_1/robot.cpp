#include "robot.h"
#include <stdexcept>

// ---------- нормализация ----------
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

void QuadrupedRobot::checkId(const std::string& id) const {
    if (id.empty())
        throw std::invalid_argument("id must not be empty");
}

// ---------- конструкторы ----------
QuadrupedRobot::QuadrupedRobot()
    : id("R0"), name("Unnamed"), batteryLevel(100.0),
      currentCommand("idle"), todayDistance(0.0), last10(new double[DAYS])
{
    for (int i = 0; i < DAYS; ++i) last10[i] = 0.0;
}

QuadrupedRobot::QuadrupedRobot(const std::string& id, const std::string& name,
                               double battery, const std::string& command,
                               double dist, const double* last10Arr)
    : id(id),
      name(normalizeName(name)),
      batteryLevel(normalizeBattery(battery)),
      currentCommand(command),
      todayDistance(normalizeDistance(dist)),
      last10(new double[DAYS])
{
    checkId(id);
    for (int i = 0; i < DAYS; ++i)
        last10[i] = last10Arr ? normalizeDistance(last10Arr[i]) : 0.0;
}

QuadrupedRobot::QuadrupedRobot(const QuadrupedRobot& o)
    : id(o.id), name(o.name), batteryLevel(o.batteryLevel),
      currentCommand(o.currentCommand), todayDistance(o.todayDistance),
      last10(new double[DAYS])
{
    for (int i = 0; i < DAYS; ++i) last10[i] = o.last10[i];
}

QuadrupedRobot& QuadrupedRobot::operator=(const QuadrupedRobot& o) {
    if (this != &o) {
        const_cast<std::string&>(id) = o.id;
        name = o.name;
        batteryLevel = o.batteryLevel;
        currentCommand = o.currentCommand;
        todayDistance = o.todayDistance;
        for (int i = 0; i < DAYS; ++i) last10[i] = o.last10[i];
    }
    return *this;
}

QuadrupedRobot::~QuadrupedRobot() {
    delete[] last10;
}

// ---------- геттеры (все const) ----------
const std::string& QuadrupedRobot::getId() const { return id; }
const std::string& QuadrupedRobot::getName() const { return name; }
double QuadrupedRobot::getBatteryLevel() const { return batteryLevel; }
const std::string& QuadrupedRobot::getCurrentCommand() const { return currentCommand; }
double QuadrupedRobot::getTodayDistance() const { return todayDistance; }
const double* QuadrupedRobot::getLast10() const { return last10; }
int QuadrupedRobot::getDays() { return DAYS; }

// ---------- сеттеры (с нормализацией) ----------
void QuadrupedRobot::setName(const std::string& n) { name = normalizeName(n); }
void QuadrupedRobot::setBatteryLevel(double b) { batteryLevel = normalizeBattery(b); }
void QuadrupedRobot::setCurrentCommand(const std::string& c) { currentCommand = c; }
void QuadrupedRobot::setTodayDistance(double d) { todayDistance = normalizeDistance(d); }
void QuadrupedRobot::setLast10(const double* arr) {
    for (int i = 0; i < DAYS; ++i)
        last10[i] = arr ? normalizeDistance(arr[i]) : 0.0;
}
