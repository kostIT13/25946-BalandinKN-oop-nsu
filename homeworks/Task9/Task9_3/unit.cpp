#include "unit.h"

#include <algorithm>
#include <stdexcept>
#include <utility>

// ============================ RobotUnit (композиция) ========================

RobotUnit::RobotUnit(const std::string& name, int count) : name_(name) {
    if (count < MIN_SIZE || count > MAX_SIZE)
        throw std::invalid_argument(
            "unit must contain between 5 and 10 robots");

    std::cout << "  [RobotUnit '" << name_ << "] composing " << count
              << " robots (unit owns them)\n";
    for (int i = 0; i < count; ++i) {
        // робот создаётся ЗДЕСЬ и живёт ровно пока живо отделение
        squad_.emplace_back("CMP-" + std::to_string(i + 1),
                            "Unit-" + std::to_string(i + 1), 100.0, "standby",
                            0.0, nullptr);
    }
}

RobotUnit::~RobotUnit() {
    std::cout << "  [~RobotUnit '" << name_
              << "'] destroying owned robots (they cannot outlive the unit)\n";
    // vector<QuadrupedRobot> уничтожит роботов сам — это и есть композиция
}

RobotUnit::RobotUnit(const RobotUnit& other)
    : name_(other.name_ + "-copy"), squad_(other.squad_) {
    std::cout << "  [RobotUnit '" << name_
              << "'] copy: every robot copied (deep copy)\n";
}

RobotUnit& RobotUnit::operator=(const RobotUnit& other) {
    if (this != &other) {
        name_ = other.name_;
        squad_ = other.squad_;   // присваивание vector копирует роботов
        std::cout << "  [RobotUnit '" << name_ << "'] copy assign\n";
    }
    return *this;
}

RobotUnit::RobotUnit(RobotUnit&& other) noexcept
    : name_(std::move(other.name_)), squad_(std::move(other.squad_)) {
    std::cout << "  [RobotUnit '" << name_
              << "'] move: ownership of all robots transferred, 0 robots copied\n";
}

RobotUnit& RobotUnit::operator=(RobotUnit&& other) noexcept {
    if (this != &other) {
        name_ = std::move(other.name_);
        squad_ = std::move(other.squad_);
        std::cout << "  [RobotUnit '" << name_ << "'] move assign\n";
    }
    return *this;
}

void RobotUnit::rollCall() const {
    std::cout << "  unit '" << name_ << "' (" << squad_.size()
              << " robots, composed):\n";
    for (const auto& r : squad_) std::cout << "    " << r << "\n";
}

QuadrupedRobot RobotUnit::retirementCopy(std::size_t i) const {
    const int originalUid = squad_.at(i).getUid();
    QuadrupedRobot out = squad_.at(i);   // копия: оригинал остаётся владельцу
    std::cout << "  [RobotUnit '" << name_ << "'] retired a COPY of #"
              << originalUid << " (original #" << originalUid
              << " still owned by the unit)\n";
    return out;
}

// ============================ RobotSquad (агрегация) ========================

RobotSquad::RobotSquad(const std::string& name) : name_(name) {
    std::cout << "  [RobotSquad '" << name_
              << "'] created empty (holds references only, owns nothing)\n";
}

bool RobotSquad::enroll(QuadrupedRobot* r) {
    if (!r) return false;
    if (has(r)) {
        std::cout << "  [RobotSquad '" << name_ << "'] robot #" << r->getUid()
                  << " already enrolled — not added twice\n";
        return false;
    }
    members_.push_back(r);
    std::cout << "  [RobotSquad '" << name_ << "'] enrolled #" << r->getUid()
              << " '" << r->getName() << "' (robot still owned by caller)\n";
    return true;
}

bool RobotSquad::discharge(QuadrupedRobot* r) {
    auto it = std::find(members_.begin(), members_.end(), r);
    if (it == members_.end()) return false;
    members_.erase(it);
    std::cout << "  [RobotSquad '" << name_ << "'] discharged #"
              << r->getUid() << " (robot is alive — squad never owned it)\n";
    return true;
}

bool RobotSquad::has(const QuadrupedRobot* r) const {
    return std::find(members_.begin(), members_.end(), r) != members_.end();
}

void RobotSquad::rollCall() const {
    std::cout << "  squad '" << name_ << "' (" << members_.size()
              << " robots, aggregated):\n";
    for (const QuadrupedRobot* r : members_) std::cout << "    " << *r << "\n";
}

void RobotSquad::issueCommand(const std::string& cmd) {
    for (QuadrupedRobot* r : members_) r->setCurrentCommand(cmd);
    std::cout << "  [RobotSquad '" << name_ << "'] command '" << cmd
              << "' issued to " << members_.size() << " robot(s)\n";
}
