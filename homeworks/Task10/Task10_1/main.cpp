#include "autonomous_cargo_robot.h"
#include "cargo_robot.h"
#include "robot.h"

#include <iostream>
#include <stdexcept>
#include <string>

namespace {

void sep(const std::string& title) {
    const size_t dashes = title.size() + 2 < 64 ? 64 - title.size() - 2 : 2;
    std::cout << "\n=== " << title << " " << std::string(dashes, '-') << "\n";
}

void alive(const std::string& where) {
    std::cout << "  >>> alive QuadrupedRobot objects: "
              << QuadrupedRobot::getObjectCount() << "   (" << where << ")\n";
}

}

int main() {
    sep("0. BASE CLASS: no friends, no template, plain class");
    {
        QuadrupedRobot base("BASE-1", "Atlas", 90.0, "standby", 0.0, nullptr);
        base.printRobot();
        std::cout << "  private fields are reachable only through public API:\n";
        base.setName("Atlas-II");
        base.setBatteryLevel(45.0);
        base.printRobot();
        std::cout << "  no friend function / friend class is declared in robot.h"
                     " (see not_allowed_example.cpp)\n";
    }

    sep("1. CargoRobot : public QuadrupedRobot  (+ private cargo field)");
    {
        double last10[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        CargoRobot cargo("CRG-1", "Mule", 80.0, "haul", 12.0, last10, 30.0);
        cargo.printRobot();
        cargo.show_weight();
        std::cout << "  inherited from the base class and still available: "
                  << cargo.getName() << ", battery=" << cargo.getBatteryLevel()
                  << "\n";

        sep("1.1 cargo limit is an invariant of the class");
        cargo.setCargoWeight(49.5);
        cargo.show_weight();
        try {
            cargo.setCargoWeight(120.0);
        } catch (const std::overflow_error& e) {
            std::cout << "  120 kg -> " << e.what() << "\n";
        }
        cargo.show_weight();
        try {
            CargoRobot bad("CRG-X", "Broken", 50.0, "haul", 0.0, nullptr,
                           80.0, 50.0);
        } catch (const std::overflow_error& e) {
            std::cout << "  80 kg into a 50 kg robot -> " << e.what() << "\n";
        }

        sep("1.2 base part is built by the base constructor");
        CargoRobot copy(cargo);
        copy.show_weight();
        alive("base + cargo + copy are alive");
    }
    alive("cargo objects destroyed");

    sep("2. AutonomousCargoRobot final : private CargoRobot");
    {
        AutonomousCargoRobot auto_(
            "ACR-1", "Ghost", 70.0, "patrol", 3.0, nullptr, 18.0);
        std::cout << "  reportMode() prints in ~half of the calls:\n";
        for (int i = 0; i < 6; ++i) {
            std::cout << "    call " << (i + 1) << ": ";
            auto_.reportMode();
        }
        auto_.setAutonomousMode(true);
        std::cout << "  mode forced to autonomous: ";
        auto_.reportMode();
        auto_.printAutonomousRobot();

        sep("2.1 private inheritance hides the base interface from users");
        std::cout << "  visible by choice (using-declarations): show_weight /"
                     " getCargoWeight /\n"
                     "      setCargoWeight; own members: reportMode /"
                     " isAutonomousMode / modeName\n"
                  << "  cargo = " << auto_.getCargoWeight() << " kg\n";
        std::cout << "  hidden: CargoRobot& and QuadrupedRobot& conversions,"
                     " printRobot(),\n"
                     "      printCargoRobot(), getMaxCargoWeight(), getId()"
                     " — the object is NOT a\n"
                     "      CargoRobot for the outside code"
                     " (not_allowed_example.cpp)\n";

        sep("2.2 final: the chain stops here");
        std::cout << "  class Derived : public AutonomousCargoRobot"
                  " — does not compile\n";

        sep("3. show_weight() IS CALLABLE on the class of p.2");
        auto_.show_weight();
        auto_.setCargoWeight(25.0);
        std::cout << "  after setCargoWeight(25): ";
        auto_.show_weight();
        std::cout << "  reachable because of 'using CargoRobot::show_weight;'"
                     " in the\n"
                     "      public section — it lifts the access of an existing"
                     " method,\n"
                     "      it does not override or redefine anything\n";
        alive("autonomous robot alive");
    }
    alive("end of main");
    return 0;
}
