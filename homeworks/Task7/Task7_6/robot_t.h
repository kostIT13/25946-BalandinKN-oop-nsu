#pragma once
#include <string>
#include <iostream>
#include <stdexcept>

// Задание 7.VI: шаблонная реализация класса.
// Шаблонные параметры:
//   IdType    - тип уникального идентификатора (std::string, int, ...)
//   DistType  - тип дистанций (double, float, int, ...)
// Определения методов ВЫНЕСЕНЫ за пределы класса (обязательное требование).
// Все методы определены в заголовочном файле (шаблоны требуют этого
// при раздельной компиляции в один .cpp).
template <typename IdType, typename DistType>
class QuadrupedRobotT {
private:
    IdType id;                    // уникальный идентификатор (тип = IdType)
    std::string name;
    double batteryLevel;
    std::string currentCommand;
    DistType todayDistance;       // тип = DistType

    static const int DAYS = 10;
    DistType* last10;             // массив DistType (владеемый)

    static std::string normalizeName(const std::string& n);
    static double normalizeBattery(double b);
    static DistType normalizeDistance(DistType d);

public:
    static const std::string class_name;

    QuadrupedRobotT();
    QuadrupedRobotT(const IdType& id, const std::string& name,
                    double battery, const std::string& command,
                    DistType dist, const DistType* last10Arr);
    QuadrupedRobotT(const QuadrupedRobotT& o);
    QuadrupedRobotT& operator=(const QuadrupedRobotT& o);
    ~QuadrupedRobotT();

    void printRobot() const;

    // объявления методов; определения вне класса
    const IdType& getId() const;
    const std::string& getName() const;
    double getBatteryLevel() const;
    const std::string& getCurrentCommand() const;
    DistType getTodayDistance() const;
    const DistType* getLast10() const;
    static int getDays();

    void setName(const std::string& n);
    void setBatteryLevel(double b);
    void setCurrentCommand(const std::string& c);
    void setTodayDistance(DistType d);
    void setLast10(const DistType* arr);
};

// ---------- определения методов ВНЕ класса ----------

template <typename IdType, typename DistType>
const std::string QuadrupedRobotT<IdType, DistType>::class_name = "QuadrupedRobotT";

template <typename IdType, typename DistType>
std::string QuadrupedRobotT<IdType, DistType>::normalizeName(const std::string& n) {
    return n.empty() ? "Unnamed" : n;
}

template <typename IdType, typename DistType>
double QuadrupedRobotT<IdType, DistType>::normalizeBattery(double b) {
    if (b < 0.0) return 0.0;
    if (b > 100.0) return 100.0;
    return b;
}

template <typename IdType, typename DistType>
DistType QuadrupedRobotT<IdType, DistType>::normalizeDistance(DistType d) {
    return d < static_cast<DistType>(0) ? static_cast<DistType>(0) : d;
}

template <typename IdType, typename DistType>
QuadrupedRobotT<IdType, DistType>::QuadrupedRobotT()
    : id(), name("Unnamed"), batteryLevel(100.0),
      currentCommand("idle"), todayDistance(static_cast<DistType>(0)),
      last10(new DistType[DAYS])
{
    for (int i = 0; i < DAYS; ++i) last10[i] = static_cast<DistType>(0);
    std::cout << "  [" << class_name << "] default constructor\n";
}

template <typename IdType, typename DistType>
QuadrupedRobotT<IdType, DistType>::QuadrupedRobotT(const IdType& id,
                                                   const std::string& name,
                                                   double battery,
                                                   const std::string& command,
                                                   DistType dist,
                                                   const DistType* last10Arr)
    : id(id),
      name(normalizeName(name)),
      batteryLevel(normalizeBattery(battery)),
      currentCommand(command),
      todayDistance(normalizeDistance(dist)),
      last10(new DistType[DAYS])
{
    if (this->id == IdType())
        throw std::invalid_argument("id must not be empty");
    for (int i = 0; i < DAYS; ++i)
        last10[i] = last10Arr ? normalizeDistance(last10Arr[i]) : static_cast<DistType>(0);
    std::cout << "  [" << class_name << "] initialization constructor\n";
}

template <typename IdType, typename DistType>
QuadrupedRobotT<IdType, DistType>::QuadrupedRobotT(const QuadrupedRobotT& o)
    : id(o.id), name(o.name), batteryLevel(o.batteryLevel),
      currentCommand(o.currentCommand), todayDistance(o.todayDistance),
      last10(new DistType[DAYS])
{
    for (int i = 0; i < DAYS; ++i) last10[i] = o.last10[i];
    std::cout << "  [" << class_name << "] copy constructor\n";
}

template <typename IdType, typename DistType>
QuadrupedRobotT<IdType, DistType>&
QuadrupedRobotT<IdType, DistType>::operator=(const QuadrupedRobotT& o) {
    std::cout << "  [" << class_name << "] assignment operator\n";
    if (this != &o) {
        id = o.id;
        name = o.name;
        batteryLevel = o.batteryLevel;
        currentCommand = o.currentCommand;
        todayDistance = o.todayDistance;
        for (int i = 0; i < DAYS; ++i) last10[i] = o.last10[i];
    }
    return *this;
}

template <typename IdType, typename DistType>
QuadrupedRobotT<IdType, DistType>::~QuadrupedRobotT() {
    delete[] last10;
    std::cout << "  [" << class_name << "] destructor\n";
}

template <typename IdType, typename DistType>
void QuadrupedRobotT<IdType, DistType>::printRobot() const {
    std::cout << "  QuadrupedRobotT: id=" << id << " name=" << name
              << " battery=" << batteryLevel
              << " command=" << currentCommand
              << " today=" << todayDistance << "\n";
}

// геттеры
template <typename IdType, typename DistType>
const IdType& QuadrupedRobotT<IdType, DistType>::getId() const { return id; }

template <typename IdType, typename DistType>
const std::string& QuadrupedRobotT<IdType, DistType>::getName() const { return name; }

template <typename IdType, typename DistType>
double QuadrupedRobotT<IdType, DistType>::getBatteryLevel() const { return batteryLevel; }

template <typename IdType, typename DistType>
const std::string& QuadrupedRobotT<IdType, DistType>::getCurrentCommand() const { return currentCommand; }

template <typename IdType, typename DistType>
DistType QuadrupedRobotT<IdType, DistType>::getTodayDistance() const { return todayDistance; }

template <typename IdType, typename DistType>
const DistType* QuadrupedRobotT<IdType, DistType>::getLast10() const { return last10; }

template <typename IdType, typename DistType>
int QuadrupedRobotT<IdType, DistType>::getDays() { return DAYS; }

// сеттеры
template <typename IdType, typename DistType>
void QuadrupedRobotT<IdType, DistType>::setName(const std::string& n) {
    name = normalizeName(n);
}

template <typename IdType, typename DistType>
void QuadrupedRobotT<IdType, DistType>::setBatteryLevel(double b) {
    batteryLevel = normalizeBattery(b);
}

template <typename IdType, typename DistType>
void QuadrupedRobotT<IdType, DistType>::setCurrentCommand(const std::string& c) {
    currentCommand = c;
}

template <typename IdType, typename DistType>
void QuadrupedRobotT<IdType, DistType>::setTodayDistance(DistType d) {
    todayDistance = normalizeDistance(d);
}

template <typename IdType, typename DistType>
void QuadrupedRobotT<IdType, DistType>::setLast10(const DistType* arr) {
    for (int i = 0; i < DAYS; ++i)
        last10[i] = arr ? normalizeDistance(arr[i]) : static_cast<DistType>(0);
}
