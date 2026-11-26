#pragma once
#include <iostream>
#include <string>

class QuadrupedRobot {
private:
    std::string name;

    static int liveCount;
    static int baseCtorCalls;

    static std::string normalizeName(const std::string& value);
    void announce(const std::string& what) const;

public:
    static const std::string class_name;

    explicit QuadrupedRobot(const std::string& name = "Unnamed");
    ~QuadrupedRobot();

    const std::string& getName() const { return name; }
    void setName(const std::string& value);

    static int getLiveCount() { return liveCount; }
    static int getBaseCtorCalls() { return baseCtorCalls; }
    static void resetBaseCtorCalls() { baseCtorCalls = 0; }
};
