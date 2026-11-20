// Реализация методов Administrator. robot.h содержит friend-объявления этих
// конкретных методов, поэтому их определения имеют доступ к private-части
// QuadrupedRobot. Остальные методы класса доступ к private НЕ имеют.
#include "admin.h"
#include "robot.h"

#include <sstream>

Administrator::Administrator(const std::string& login)
    : login_(login.empty() ? "guest" : login) {}

// ---- друг #1: одно приватное поле (model) ----
void Administrator::setModel(QuadrupedRobot& r, const std::string& model) {
    r.model = QuadrupedRobot::normalizeModel(model);   // private — разрешено дружбой
    ++changesCount_;
    std::cout << "  [admin " << login_ << "::setModel] #" << r.uid
              << " model := '" << r.model << "'\n";
}

// ---- друг #2: несколько приватных полей (id + model), но не все ----
void Administrator::assignRegistration(QuadrupedRobot& r,
                                       const std::string& id,
                                       const std::string& model) {
    r.id = id;                                        // private: публичного сеттера нет
    r.model = QuadrupedRobot::normalizeModel(model);   // private
    ++changesCount_;
    std::cout << "  [admin " << login_ << "::assignRegistration] #" << r.uid
              << " id := '" << r.id << "', model := '" << r.model << "'\n";
}

// ---- НЕ друг: работает только через public API ----
// r.uid здесь недоступен (не друг) — компилятор это ловит; только getUid().
void Administrator::renamePublicly(QuadrupedRobot& r,
                                   const std::string& newName) {
    r.setName(newName);        // public-сеттер
    std::cout << "  [admin " << login_ << "::renamePublicly] #" << r.getUid()
              << " name := '" << r.getName() << "' (через public API)\n";
}

std::string Administrator::report(const QuadrupedRobot& r) const {
    std::ostringstream os;
    os << "report by " << login_ << ": uid=" << r.getUid()
       << " id=" << r.getId()
       << " name=" << r.getName()
       << " model=" << r.getModel()
       << " battery=" << r.getBatteryLevel();
    return os.str();
}
