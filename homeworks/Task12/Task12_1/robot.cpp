#include "robot.h"

#include <stdexcept>

double Robot::normalizeBattery(double value) {
    if (value < 0.0) return 0.0;
    if (value > 100.0) return 100.0;
    return value;
}

Robot::Robot(const std::string& name, double battery)
    : name(name), batteryLevel(normalizeBattery(battery))
{
    if (name.empty())
        throw std::invalid_argument("robot name must not be empty");
}

Robot::~Robot() = default;

void Robot::logState(const std::string& what) const {
    std::cout << "      [" << className() << "] " << what
              << " name=\"" << name << "\" battery=" << batteryLevel << "\n";
}

void Robot::setName(const std::string& value) {
    if (value.empty())
        throw std::invalid_argument("robot name must not be empty");
    name = value;
}

void Robot::setBatteryLevel(double value) {
    batteryLevel = normalizeBattery(value);
}

std::string Robot::className() const {
    return "Robot";
}

std::string Robot::gaitTag() const {
    return "base-gait";
}

std::string Robot::statusLine() const {
    return className() + "(" + name + ", battery=" +
           std::to_string(batteryLevel) + ")";
}
