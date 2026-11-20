#include "robot.h"

#include <stdexcept>
#include <utility>

// ---------- статические поля ----------
int QuadrupedRobot::objectCount = 0;
int QuadrupedRobot::nextUid = 1;
const std::string QuadrupedRobot::class_name = "QuadrupedRobot";

int QuadrupedRobot::copyCtorCalls = 0;
int QuadrupedRobot::moveCtorCalls = 0;
int QuadrupedRobot::copyAssignCalls = 0;
int QuadrupedRobot::moveAssignCalls = 0;
int QuadrupedRobot::heapAllocs = 0;
int QuadrupedRobot::heapFrees = 0;
int QuadrupedRobot::liveBuffers = 0;

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
        ++heapFrees;
        --liveBuffers;
        last10 = nullptr;
    }
}

void QuadrupedRobot::ensureBuffer() {
    if (!last10) {
        last10 = new double[DAYS];
        ++heapAllocs;
        ++liveBuffers;
        for (int i = 0; i < DAYS; ++i) last10[i] = 0.0;
    }
}

// ---------- статические методы ----------
int QuadrupedRobot::getObjectCount() { return objectCount; }
void QuadrupedRobot::print_count() {
    std::cout << "  " << class_name << " count: " << objectCount << "\n";
}

void QuadrupedRobot::resetStats() {
    copyCtorCalls = moveCtorCalls = copyAssignCalls = moveAssignCalls = 0;
    heapAllocs = heapFrees = 0;
}

void QuadrupedRobot::printStats() {
    std::cout << "  copy ctor=" << copyCtorCalls
              << " | move ctor=" << moveCtorCalls
              << " | copy assign=" << copyAssignCalls
              << " | move assign=" << moveAssignCalls
              << " | new double[]=" << heapAllocs
              << " | delete[]=" << heapFrees << "\n";
}

// ---------- конструкторы ----------
QuadrupedRobot::QuadrupedRobot()
    : uid(nextUid++), id("R" + std::to_string(uid)),
      name("Unnamed"), batteryLevel(100.0),
      currentCommand("idle"), todayDistance(0.0), last10(new double[DAYS])
{
    ++heapAllocs;
    ++liveBuffers;
    ++objectCount;
    for (int i = 0; i < DAYS; ++i) last10[i] = 0.0;
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
      last10(nullptr)
{
    // Проверка id ДО аллокации: исключение из тела конструктора не вызывает
    // деструктор, поэтому уже выделенный массив при обратном порядке утекал.
    checkId(id);
    ensureBuffer();
    ++objectCount;
    for (int i = 0; i < DAYS; ++i)
        last10[i] = last10Arr ? normalizeDistance(last10Arr[i]) : 0.0;
    printMsg("initialization constructor");
}

// Копирование: источник остаётся полностью рабочим, у копии СВОЙ буфер
// (ещё одна аллокация + переписывание 10 значений).
QuadrupedRobot::QuadrupedRobot(const QuadrupedRobot& o)
    : uid(nextUid++), id(o.id), name(o.name), batteryLevel(o.batteryLevel),
      currentCommand(o.currentCommand), todayDistance(o.todayDistance),
      last10(new double[DAYS])
{
    ++heapAllocs;
    ++liveBuffers;
    ++copyCtorCalls;
    ++objectCount;
    // Копирование перемещённого объекта законно: своего буфера у источника
    // нет, поэтому копируем «пустоту» (нули), а не читаем из nullptr.
    for (int i = 0; i < DAYS; ++i) last10[i] = o.last10 ? o.last10[i] : 0.0;
    printMsg("COPY ctor (from #" + std::to_string(o.uid) + ")");
}

QuadrupedRobot& QuadrupedRobot::operator=(const QuadrupedRobot& o) {
    if (this != &o) {
        ++copyAssignCalls;
        id = o.id;
        name = o.name;
        batteryLevel = o.batteryLevel;
        currentCommand = o.currentCommand;
        todayDistance = o.todayDistance;
        ensureBuffer();
        for (int i = 0; i < DAYS; ++i)
            last10[i] = o.last10 ? o.last10[i] : 0.0;
    }
    printMsg("COPY assign (from #" + std::to_string(o.uid) + ")");
    return *this;
}

// Перемещение конструктора: только забираем указатель (3-4 присваивания),
// ни одной аллокации и ни одного cudaMemcpy-подобного переписывания массива.
// Источник оставляем в «безопасном пустом» состоянии (last10 = nullptr),
// иначе деструктор источника удалит тот же буфер второй раз (double free).
QuadrupedRobot::QuadrupedRobot(QuadrupedRobot&& o) noexcept
    : uid(nextUid++), id(std::move(o.id)), name(std::move(o.name)),
      batteryLevel(o.batteryLevel), currentCommand(std::move(o.currentCommand)),
      todayDistance(o.todayDistance), last10(o.last10)
{
    o.last10 = nullptr;               // источник больше не владеет буфером
    ++moveCtorCalls;
    ++objectCount;
    printMsg("MOVE ctor (from #" + std::to_string(o.uid) + ", buffer stolen)");
}

// Перемещающее присваивание: свой буфер освобождаем, чужой забираем.
// Самоприсваивание (x = std::move(x)) должно оставаться корректным.
QuadrupedRobot& QuadrupedRobot::operator=(QuadrupedRobot&& o) noexcept {
    if (this != &o) {
        ++moveAssignCalls;
        releaseBuffer();
        id = std::move(o.id);
        name = std::move(o.name);
        batteryLevel = o.batteryLevel;
        currentCommand = std::move(o.currentCommand);
        todayDistance = o.todayDistance;
        last10 = o.last10;
        o.last10 = nullptr;
        printMsg("MOVE assign (from #" + std::to_string(o.uid) +
                 ", own buffer freed, buffer stolen)");
    } else {
        printMsg("MOVE assign (self, ignored)");
    }
    return *this;
}

QuadrupedRobot::~QuadrupedRobot() {
    --objectCount;
    releaseBuffer();   // safe для перемещённого объекта: буфера уже нет
    printMsg("destructor");
}

// ---------- печать ----------
void QuadrupedRobot::printRobot() const {
    std::cout << "  QuadrupedRobot: id=" << id << " name=" << name
              << " battery=" << batteryLevel
              << " command=" << currentCommand
              << " today=" << todayDistance << " km";
    if (isMovedFrom()) std::cout << " [MOVED-FROM, no buffer]";
    std::cout << "\n";
}

// ---------- геттеры ----------
int QuadrupedRobot::getUid() const { return uid; }
const std::string& QuadrupedRobot::getId() const { return id; }
const std::string& QuadrupedRobot::getName() const { return name; }
double QuadrupedRobot::getBatteryLevel() const { return batteryLevel; }
const std::string& QuadrupedRobot::getCurrentCommand() const { return currentCommand; }
double QuadrupedRobot::getTodayDistance() const { return todayDistance; }
// Для перемещённого объекта возвращает nullptr: данные ушли новому владельцу.
const double* QuadrupedRobot::getLast10() const { return last10; }
int QuadrupedRobot::getDays() { return DAYS; }

// ---------- сеттеры ----------
void QuadrupedRobot::setName(const std::string& n) { name = normalizeName(n); }
void QuadrupedRobot::setBatteryLevel(double b) { batteryLevel = normalizeBattery(b); }
void QuadrupedRobot::setCurrentCommand(const std::string& c) { currentCommand = c; }
void QuadrupedRobot::setTodayDistance(double d) { todayDistance = normalizeDistance(d); }
void QuadrupedRobot::setLast10(const double* arr) {
    ensureBuffer();
    for (int i = 0; i < DAYS; ++i)
        last10[i] = arr ? normalizeDistance(arr[i]) : 0.0;
}

// ---------- оператор вывода ----------
std::ostream& operator<<(std::ostream& os, const QuadrupedRobot& r) {
    os << "QuadrupedRobot#" << r.uid
       << " {id=" << r.id
       << ", name=" << r.name
       << ", battery=" << r.batteryLevel
       << ", cmd=" << r.currentCommand
       << ", today=" << r.todayDistance << "}";
    if (r.isMovedFrom()) os << " (moved-from)";
    return os << " [" << QuadrupedRobot::class_name
              << ", alive=" << QuadrupedRobot::objectCount << "]";
}

// ---------- оператор ввода ----------
// Строго ограничен 4 параметрами: name battery command distance
std::istream& operator>>(std::istream& is, QuadrupedRobot& r) {
    std::string name, command;
    double battery, dist;
    if (!(is >> name))    return is;
    if (!(is >> battery)) return is;
    if (!(is >> command)) return is;
    if (!(is >> dist))    return is;

    r.name = QuadrupedRobot::normalizeName(name);
    r.batteryLevel = QuadrupedRobot::normalizeBattery(battery);
    r.currentCommand = command;
    r.todayDistance = QuadrupedRobot::normalizeDistance(dist);
    r.ensureBuffer();
    for (int i = 0; i < QuadrupedRobot::DAYS; ++i) r.last10[i] = 0.0;
    return is;
}
