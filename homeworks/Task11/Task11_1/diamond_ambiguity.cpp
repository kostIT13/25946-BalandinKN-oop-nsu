#include "diamond_problem.h"

void ambiguousName() {
    DiamondBoth r(10.0, 5.0, "Argus");
    r.getName();
}

void ambiguousSetter() {
    DiamondBoth r(10.0, 5.0, "Argus");
    r.setName("X");
}

void baseConversionIsAmbiguous() {
    DiamondBoth r(10.0, 5.0, "Argus");
    QuadrupedRobot& base = r;
    (void)base;
}
