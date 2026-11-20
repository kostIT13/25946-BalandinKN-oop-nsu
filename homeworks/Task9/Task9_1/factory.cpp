#include "factory.h"

RobotFactory::RobotFactory(const std::string& plantName)
    : plantName_(plantName.empty() ? "UnknownPlant" : plantName) {}

// Дружество даёт доступ к приватным полям, поэтому конструктор по умолчанию
// + прямая запись: ни один публичный сеттер здесь не нужен.
QuadrupedRobot RobotFactory::produce(const std::string& id,
                                     const std::string& name, double battery,
                                     const std::string& command, double dist,
                                     const double* last10Arr) const {
    QuadrupedRobot r;                       // default ctor (id/uid автогенерацией)
    r.id = id;                              // публичного сеттера id нет
    r.name = QuadrupedRobot::normalizeName(name);
    r.batteryLevel = battery;               // без нормализации: завод отвечает за данные
    r.currentCommand = command;
    r.todayDistance = dist;
    r.manufacturer = plantName_;
    r.ensureBuffer();
    for (int i = 0; i < QuadrupedRobot::DAYS; ++i)
        r.last10[i] = last10Arr ? last10Arr[i] : 0.0;

    std::cout << "  [friend class " << plantName_ << "] produced #" << r.uid
              << " id=" << r.id << " | objects: "
              << QuadrupedRobot::objectCount << "\n";
    return r;
}

void RobotFactory::repurpose(QuadrupedRobot& r, const std::string& newId,
                             const std::string& newName, double battery,
                             const std::string& command, double dist,
                             const std::string& manufacturer) const {
    r.id = newId;
    r.name = newName;
    r.batteryLevel = battery;
    r.currentCommand = command;
    r.todayDistance = dist;
    r.manufacturer = manufacturer;
    std::cout << "  [friend class " << plantName_ << "] repurpose(#" << r.uid
              << "): id/name/battery/command/distance/vendor rewritten\n";
}

void RobotFactory::audit(const QuadrupedRobot& r) const {
    std::cout << "  [friend class " << plantName_ << "] audit #" << r.uid << ": "
              << "uid=" << r.uid
              << " id=" << r.id
              << " name=" << r.name
              << " battery=" << r.batteryLevel
              << " cmd=" << r.currentCommand
              << " today=" << r.todayDistance
              << " vendor=" << r.manufacturer
              << " last10[0]=" << (r.last10 ? r.last10[0] : -1.0)
              << " | live objects=" << QuadrupedRobot::objectCount
              << " | next uid=" << QuadrupedRobot::nextUid << "\n";
}

void RobotFactory::setNextUid(int value) {
    QuadrupedRobot::nextUid = value;
    std::cout << "  [friend class " << plantName_
              << "] nextUid := " << QuadrupedRobot::nextUid
              << " (приватная статика класса)\n";
}

int RobotFactory::nextUidPreview() const { return QuadrupedRobot::nextUid; }
