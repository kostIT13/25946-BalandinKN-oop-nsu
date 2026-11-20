#pragma once
#include <iostream>
#include <string>

// Задание 8.I: класс QuadrupedRobot дополнен перемещающими методами.
//
// п.1: конструктор перемещения и оператор присваивания с перемещением.
// Класс владеет динамическим массивом last10, поэтому разница между
// копированием и перемещением наблюдаема: копирование выделяет новый буфер
// и переписывает 10 значений, перемещение забирает буфер у источника.
// Счётчики ниже нужны для того, чтобы п.2 можно было измерить, а не «верить
// на слово».
class QuadrupedRobot {
private:
    static int objectCount;
    static int nextUid;

    // 8.I: uid/id больше не const. const-поле запрещает оператор
    // присваивания по умолчанию, и присваивание приходилось вымучивать через
    // const_cast (см. Task7). Здесь запрет на изменение id обеспечен
    // отсутствием сеттера, а не ключевым словом const.
    int uid;
    std::string id;
    std::string name;
    double batteryLevel;
    std::string currentCommand;
    double todayDistance;

    static const int DAYS = 10;
    double* last10;

    // п.1: счётчики работы спецметодов и аллокаций собственного буфера
    static int copyCtorCalls;
    static int moveCtorCalls;
    static int copyAssignCalls;
    static int moveAssignCalls;
    static int heapAllocs;   // new double[DAYS]
    static int heapFrees;    // delete[]
    static int liveBuffers;  // текущее число буферов «в воздухе»

    static std::string normalizeName(const std::string& n);
    static double normalizeBattery(double b);
    static double normalizeDistance(double d);
    void checkId(const std::string& id) const;
    void printMsg(const std::string& what) const;
    void releaseBuffer();
    void ensureBuffer();

public:
    static const std::string class_name;
    static int getObjectCount();
    static void print_count();

    QuadrupedRobot();
    QuadrupedRobot(const std::string& id, const std::string& name,
                   double battery, const std::string& command,
                   double dist, const double* last10Arr);

    // копирование: всегда выделяет СВОЙ буфер
    QuadrupedRobot(const QuadrupedRobot& o);
    QuadrupedRobot& operator=(const QuadrupedRobot& o);

    // 8.I п.1: перемещение — забирает буфер у источника, аллокаций по данным
    // нет. noexcept обязателен: без него std::vector при росте обязан беречь
    // strong guarantee и копирует элементы вместо перемещения.
    QuadrupedRobot(QuadrupedRobot&& o) noexcept;
    QuadrupedRobot& operator=(QuadrupedRobot&& o) noexcept;

    ~QuadrupedRobot();

    // Перемещённый объект: пустой робот без буфера. Можно уничтожить,
    // присвоить (копированием или перемещением) или вернуть ему буфер через
    // setLast10 / operator>>.
    bool isMovedFrom() const { return last10 == nullptr; }
    const double* bufferAddress() const { return last10; }

    void printRobot() const;

    int getUid() const;
    const std::string& getId() const;
    const std::string& getName() const;
    double getBatteryLevel() const;
    const std::string& getCurrentCommand() const;
    double getTodayDistance() const;
    const double* getLast10() const;
    static int getDays();

    void setName(const std::string& n);
    void setBatteryLevel(double b);
    void setCurrentCommand(const std::string& c);
    void setTodayDistance(double d);
    void setLast10(const double* arr);

    // п.1: доступ к счётчикам для демонстрации (п.2) и тестов (8.II)
    static int getCopyCtorCalls()   { return copyCtorCalls; }
    static int getMoveCtorCalls()   { return moveCtorCalls; }
    static int getCopyAssignCalls() { return copyAssignCalls; }
    static int getMoveAssignCalls() { return moveAssignCalls; }
    static int getHeapAllocs()      { return heapAllocs; }
    static int getHeapFrees()       { return heapFrees; }
    static int getLiveBuffers()     { return liveBuffers; }
    static void resetStats();
    static void printStats();

    friend std::ostream& operator<<(std::ostream& os, const QuadrupedRobot& r);
    friend std::istream& operator>>(std::istream& is, QuadrupedRobot& r);
};
