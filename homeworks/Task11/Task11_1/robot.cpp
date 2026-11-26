#include "robot.h"

#include <stdexcept>

int QuadrupedRobot::liveCount = 0;
int QuadrupedRobot::baseCtorCalls = 0;
const std::string QuadrupedRobot::class_name = "QuadrupedRobot";

std::string QuadrupedRobot::normalizeName(const std::string& value) {
    if (value.empty())
        throw std::invalid_argument("robot name must not be empty");
    return value;
}

void QuadrupedRobot::announce(const std::string& what) const {
    std::cout << "      [" << class_name << "] " << what
              << " name=\"" << name << "\" | alive: " << liveCount << "\n";
}

QuadrupedRobot::QuadrupedRobot(const std::string& name)
    : name(normalizeName(name))
{
    ++liveCount;
    ++baseCtorCalls;
    announce("base constructor");
}

QuadrupedRobot::~QuadrupedRobot() {
    --liveCount;
    std::cout << "      [" << class_name << "] base destructor"
              << " name=\"" << name << "\" | alive: " << liveCount << "\n";
}

void QuadrupedRobot::setName(const std::string& value) {
    name = normalizeName(value);
}
