#include "robot.h"

#include <stdexcept>
#include <utility>

// ---------- статические поля ----------
int QuadrupedRobot::objectCount = 0;
int QuadrupedRobot::nextUid = 1;
const std::string QuadrupedRobot::class_name = "QuadrupedRobot";

// ---------- нормализация ----------
std::string QuadrupedRobot::normalizeName(const std::string& n) {
    return n.empty() ? "Unnamed" : n;
}

std::string QuadrupedRobot::normalizeModel(const std::string& m) {
    return m.empty() ? "NoName-000" : m;
}

double QuadrupedRobot::normalizeBattery(double b) {
    if (b < 0.0) return 0.0;
    if (b > 100.0) return 100.0;
    return b;
}

double QuadrupedRobot::normalizeDistance(double d) {
    return d < 0.0 ? 0.0 : d;
}

void QuadrupedRobot::checkId(const std::string& value) const {
    if (value.empty())
        throw std::invalid_argument("id must not be empty");
}

void QuadrupedRobot::printMsg(const std::string& what) const {
    std::cout << "  [" << class_name << " #" << uid << "] " << what
              << " | objects: " << objectCount << "\n";
}

// ---------- владение буфером ----------
void QuadrupedRobot::releaseBuffer() {
    if (last10) {
        delete[] last10;
        last10 = nullptr;
    }
}

void QuadrupedRobot::ensureBuffer() {
    if (!last10) {
        last10 = new double[DAYS];
        for (int i = 0; i < DAYS; ++i) last10[i] = 0.0;
    }
}

// ---------- статические методы ----------
int QuadrupedRobot::getObjectCount() { return objectCount; }
int QuadrupedRobot::getDays() { return DAYS; }

// ---------- конструкторы / спецметоды ----------
QuadrupedRobot::QuadrupedRobot()
    : uid(nextUid++), id("R" + std::to_string(uid)),
      name("Unnamed"), batteryLevel(100.0),
      currentCommand("idle"), todayDistance(0.0), last10(nullptr),
      model("NoName-000")
{
    ++objectCount;
    ensureBuffer();
    printMsg("default constructor");
}

QuadrupedRobot::QuadrupedRobot(const std::string& id, const std::string& name,
                               double battery, const std::string& command,
                               double dist, const double* last10Arr)
    : uid(nextUid++), id(id),
      name(normalizeName(name)),
      batteryLevel(normalizeBattery(battery)),
      currentCommand(command),
      todayDistance(normalizeDistance(dist)),
      last10(nullptr),
      model("NoName-000")
{
    // Проверка до аллокации: при исключении из тела конструктора деструктор
    // не вызывается, иначе выделенный массив утёк бы.
    checkId(id);
    ++objectCount;
    ensureBuffer();
    for (int i = 0; i < DAYS; ++i)
        last10[i] = last10Arr ? normalizeDistance(last10Arr[i]) : 0.0;
    printMsg("initialization constructor");
}

QuadrupedRobot::QuadrupedRobot(const QuadrupedRobot& o)
    : uid(nextUid++), id(o.id), name(o.name), batteryLevel(o.batteryLevel),
      currentCommand(o.currentCommand), todayDistance(o.todayDistance),
      last10(nullptr), model(o.model)
{
    ++objectCount;
    ensureBuffer();
    for (int i = 0; i < DAYS; ++i) last10[i] = o.last10 ? o.last10[i] : 0.0;
    printMsg("copy constructor (from #" + std::to_string(o.uid) + ")");
}

QuadrupedRobot& QuadrupedRobot::operator=(const QuadrupedRobot& o) {
    if (this != &o) {
        id = o.id;
        name = o.name;
        batteryLevel = o.batteryLevel;
        currentCommand = o.currentCommand;
        todayDistance = o.todayDistance;
        model = o.model;
        ensureBuffer();
        for (int i = 0; i < DAYS; ++i)
            last10[i] = o.last10 ? o.last10[i] : 0.0;
    }
    printMsg("assignment operator (from #" + std::to_string(o.uid) + ")");
    return *this;
}

QuadrupedRobot::QuadrupedRobot(QuadrupedRobot&& o) noexcept
    : uid(nextUid++), id(std::move(o.id)), name(std::move(o.name)),
      batteryLevel(o.batteryLevel), currentCommand(std::move(o.currentCommand)),
      todayDistance(o.todayDistance), last10(o.last10), model(std::move(o.model))
{
    o.last10 = nullptr;
    ++objectCount;
    printMsg("move constructor (from #" + std::to_string(o.uid) + ")");
}

QuadrupedRobot& QuadrupedRobot::operator=(QuadrupedRobot&& o) noexcept {
    if (this != &o) {
        releaseBuffer();
        id = std::move(o.id);
        name = std::move(o.name);
        batteryLevel = o.batteryLevel;
        currentCommand = std::move(o.currentCommand);
        todayDistance = o.todayDistance;
        model = std::move(o.model);
        last10 = o.last10;
        o.last10 = nullptr;
        printMsg("move assignment (from #" + std::to_string(o.uid) + ")");
    }
    return *this;
}

QuadrupedRobot::~QuadrupedRobot() {
    --objectCount;
    releaseBuffer();
    printMsg("destructor");
}

// ---------- печать ----------
void QuadrupedRobot::printRobot() const {
    std::cout << "  " << class_name << " #" << uid
              << ": id=" << id << " name=" << name
              << " model=" << model
              << " battery=" << batteryLevel
              << " command=" << currentCommand
              << " today=" << todayDistance << " km\n";
}

// ---------- сеттеры публичной части ----------
void QuadrupedRobot::setName(const std::string& n) { name = normalizeName(n); }
void QuadrupedRobot::setBatteryLevel(double b) { batteryLevel = normalizeBattery(b); }
void QuadrupedRobot::setCurrentCommand(const std::string& c) { currentCommand = c; }
void QuadrupedRobot::setTodayDistance(double d) { todayDistance = normalizeDistance(d); }
void QuadrupedRobot::setLast10(const double* arr) {
    ensureBuffer();
    for (int i = 0; i < DAYS; ++i)
        last10[i] = arr ? normalizeDistance(arr[i]) : 0.0;
}

// ---------- вывод ----------
std::ostream& operator<<(std::ostream& os, const QuadrupedRobot& r) {
    return os << QuadrupedRobot::class_name << " #" << r.uid
              << "{id=" << r.id
              << ", name=" << r.name
              << ", model=" << r.model
              << ", battery=" << r.batteryLevel
              << ", cmd=" << r.currentCommand
              << ", today=" << r.todayDistance << "}";
}
