#pragma once
#include "robot.h"

class AutonomousRobot : public virtual QuadrupedRobot {
private:
    double autonomousRangeKm;

    static double normalizeRange(double value);
    void announce(const std::string& what) const;

public:
    static const std::string class_name;

    explicit AutonomousRobot(double autonomousRangeKm = 0.0,
                             const std::string& name = "Unnamed-Autonomous");
    ~AutonomousRobot();

    double getAutonomousRangeKm() const { return autonomousRangeKm; }
    void setAutonomousRangeKm(double value) {
        autonomousRangeKm = normalizeRange(value);
    }
};
