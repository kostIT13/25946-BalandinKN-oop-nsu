// Задание 8.II: тесты с использованием fixture (gtest TEST_F).
//
// Зачем здесь fixture:
//   1) общее начальное состояние — канонический робот с известными данными
//      создаётся в SetUp() и уничтожается в TearDown() вокруг КАЖДОГО теста,
//      поэтому тесты независимы: перемещение объекта в одном тесте не мешает
//      другому;
//   2) класс хранит статические счётчики вызовов спецметодов и аллокаций. Без
//      обнуления в SetUp() проверки вида «аллокаций стало ровно на 1 больше»
//      были бы невозможны — счётчики копились бы через все тесты;
//   3) TearDown() сверяет число живых буферов: тест, оставивший утечку или
//      устроивший double free, падает сам, а не «где-то потом».
//
// Fixture два: RobotFixture (базовое состояние) и MoveSemanticsFixture
// (наследуется от первого и добавляет снимок счётчиков) — переиспользование
// fixture через наследование.
#include <gtest/gtest.h>

#include <sstream>
#include <type_traits>
#include <utility>
#include <vector>

#include "robot.h"

namespace {

constexpr int kDays = 10;

// ---------- базовый fixture ----------
class RobotFixture : public ::testing::Test {
protected:
    void SetUp() override {
        QuadrupedRobot::resetStats();
        buffersBefore = QuadrupedRobot::getLiveBuffers();
        src = new QuadrupedRobot("R-001", "Spot", 55.0, "walk", 7.0, history);
    }

    void TearDown() override {
        const int aliveObjects = QuadrupedRobot::getObjectCount();
        delete src;
        src = nullptr;
        EXPECT_EQ(QuadrupedRobot::getLiveBuffers(), buffersBefore)
            << "буферов стало больше/меньше, чем до теста: утечка или лишнее "
               "освобождение; живых объектов в конце теста: "
            << aliveObjects;
    }

    // история fixture: 1, 2, ... , 10
    void expectHistorySeq(const QuadrupedRobot& r) const {
        ASSERT_NE(r.getLast10(), nullptr);
        for (int i = 0; i < kDays; ++i)
            EXPECT_DOUBLE_EQ(r.getLast10()[i], i + 1.0);
    }

    void expectZeroHistory(const QuadrupedRobot& r) const {
        ASSERT_NE(r.getLast10(), nullptr);
        for (int i = 0; i < kDays; ++i) EXPECT_DOUBLE_EQ(r.getLast10()[i], 0.0);
    }

    QuadrupedRobot makeRobot(const char* id) const {
        return QuadrupedRobot(id, "V", 50.0, "idle", 0.0, history);
    }

    QuadrupedRobot* src = nullptr;
    double history[kDays] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int buffersBefore = 0;
};

// ---------- производный fixture: только про перемещение ----------
class MoveSemanticsFixture : public RobotFixture {
protected:
    struct Counters {
        int copyCtor = 0, moveCtor = 0, copyAssign = 0, moveAssign = 0;
        int allocs = 0, frees = 0;
    };

    void SetUp() override {
        RobotFixture::SetUp();
        afterSetup = snapshot();
    }

    // Снимок берётся НЕПОСРЕДСТВЕННО перед проверяемой операцией: подготовка
    // теста (например, создание объекта-получателя) сама делает аллокации, и
    // сравнивать с ними «до/после» было бы неверно.
    static Counters snapshot() {
        Counters c;
        c.copyCtor = QuadrupedRobot::getCopyCtorCalls();
        c.moveCtor = QuadrupedRobot::getMoveCtorCalls();
        c.copyAssign = QuadrupedRobot::getCopyAssignCalls();
        c.moveAssign = QuadrupedRobot::getMoveAssignCalls();
        c.allocs = QuadrupedRobot::getHeapAllocs();
        c.frees = QuadrupedRobot::getHeapFrees();
        return c;
    }

    Counters afterSetup;
};

// ==================== RobotFixture: состояние и базовое поведение =========

TEST_F(RobotFixture, FixtureBuildsRobotWithKnownData) {
    EXPECT_EQ(src->getId(), "R-001");
    EXPECT_EQ(src->getName(), "Spot");
    EXPECT_DOUBLE_EQ(src->getBatteryLevel(), 55.0);
    EXPECT_EQ(src->getCurrentCommand(), "walk");
    EXPECT_DOUBLE_EQ(src->getTodayDistance(), 7.0);
    expectHistorySeq(*src);
}

TEST_F(RobotFixture, SetUpGivesExactlyOneLiveObjectPerTest) {
    // Все объекты предыдущих тестов уже уничтожены TearDown-ом: в начале
    // теста жив ровно один объект — src.
    EXPECT_EQ(QuadrupedRobot::getObjectCount(), 1);
    src->setName("Mutated");
    EXPECT_EQ(src->getName(), "Mutated");
}

TEST_F(RobotFixture, RobotIsPristineDespiteMutationsInEarlierTests) {
    // Предыдущий тест менял имя своего объекта — здесь снова канонические
    // данные, т.к. fixture пересоздаёт объект на каждый тест.
    EXPECT_EQ(src->getName(), "Spot");
    EXPECT_DOUBLE_EQ(src->getBatteryLevel(), 55.0);
    expectHistorySeq(*src);
}

TEST_F(RobotFixture, SettersNormalizeValues) {
    src->setName("");
    EXPECT_EQ(src->getName(), "Unnamed");
    src->setBatteryLevel(-10.0);
    EXPECT_DOUBLE_EQ(src->getBatteryLevel(), 0.0);
    src->setBatteryLevel(250.0);
    EXPECT_DOUBLE_EQ(src->getBatteryLevel(), 100.0);
    src->setTodayDistance(-5.0);
    EXPECT_DOUBLE_EQ(src->getTodayDistance(), 0.0);
    src->setLast10(nullptr);
    expectZeroHistory(*src);
}

// Пустой id запрещён. Отдельно проверяется, что неудачная конструкция не
// оставляет утечку: деструктор при исключении из тела конструктора НЕ
// вызывается, поэтому массив должен выделяться ПОСЛЕ проверки.
TEST_F(RobotFixture, EmptyIdThrowsAndLeaksNothing) {
    const int buffersBeforeAttempt = QuadrupedRobot::getLiveBuffers();
    EXPECT_THROW(QuadrupedRobot("", "Spot", 10.0, "idle", 0.0, history),
                 std::invalid_argument);
    EXPECT_EQ(QuadrupedRobot::getLiveBuffers(), buffersBeforeAttempt);
}

TEST_F(RobotFixture, StreamOperatorsRoundTrip) {
    std::istringstream in("Tombot 40 run 12.5");
    QuadrupedRobot r;
    ASSERT_TRUE(static_cast<bool>(in >> r));
    EXPECT_EQ(r.getName(), "Tombot");
    EXPECT_DOUBLE_EQ(r.getBatteryLevel(), 40.0);
    EXPECT_EQ(r.getCurrentCommand(), "run");
    EXPECT_DOUBLE_EQ(r.getTodayDistance(), 12.5);

    std::ostringstream out;
    out << r;
    EXPECT_NE(out.str().find("Tombot"), std::string::npos);
    EXPECT_NE(out.str().find(QuadrupedRobot::class_name), std::string::npos);
}

// ==================== MoveSemanticsFixture: 8.I ============================

TEST_F(MoveSemanticsFixture, CopyCtorAllocatesOwnBufferAndKeepsSource) {
    const Counters before = snapshot();
    const void* srcAddr = src->bufferAddress();

    QuadrupedRobot copy(*src);

    EXPECT_NE(copy.getLast10(), nullptr);
    EXPECT_NE(copy.bufferAddress(), srcAddr) << "копия владеет своим буфером";
    EXPECT_FALSE(src->isMovedFrom()) << "источник при копировании не меняется";
    EXPECT_EQ(copy.getId(), src->getId());
    expectHistorySeq(copy);

    EXPECT_EQ(QuadrupedRobot::getCopyCtorCalls(), before.copyCtor + 1);
    EXPECT_EQ(QuadrupedRobot::getMoveCtorCalls(), before.moveCtor);
    EXPECT_EQ(QuadrupedRobot::getHeapAllocs(), before.allocs + 1)
        << "копирование = ровно одна новая аллокация буфера";
}

TEST_F(MoveSemanticsFixture, MoveCtorStealsBufferWithoutAllocation) {
    const Counters before = snapshot();
    const void* srcAddr = src->bufferAddress();

    QuadrupedRobot moved(std::move(*src));

    EXPECT_EQ(moved.bufferAddress(), srcAddr) << "буфер передан, а не скопирован";
    EXPECT_TRUE(src->isMovedFrom()) << "источник обязан отдать владение";
    EXPECT_EQ(moved.getId(), "R-001");
    EXPECT_EQ(moved.getName(), "Spot");
    EXPECT_DOUBLE_EQ(moved.getBatteryLevel(), 55.0);
    EXPECT_DOUBLE_EQ(moved.getTodayDistance(), 7.0);
    // данные доехали целыми
    expectHistorySeq(moved);

    EXPECT_EQ(QuadrupedRobot::getMoveCtorCalls(), before.moveCtor + 1);
    EXPECT_EQ(QuadrupedRobot::getCopyCtorCalls(), before.copyCtor)
        << "копирование вызвано быть не должно";
    EXPECT_EQ(QuadrupedRobot::getHeapAllocs(), before.allocs)
        << "перемещение не аллоцирует буфер данных";
}

TEST_F(MoveSemanticsFixture, CopyAssignReusesBufferAndKeepsSource) {
    QuadrupedRobot target("R-target", "Target", 10.0, "idle", 1.0, history);
    const Counters before = snapshot();
    const void* targetAddr = target.bufferAddress();

    target = *src;   // l-value -> копирующее присваивание

    EXPECT_EQ(target.bufferAddress(), targetAddr) << "свой буфер переиспользован";
    EXPECT_FALSE(src->isMovedFrom());
    EXPECT_EQ(target.getName(), "Spot");
    EXPECT_DOUBLE_EQ(target.getBatteryLevel(), 55.0);
    expectHistorySeq(target);

    EXPECT_EQ(QuadrupedRobot::getCopyAssignCalls(), before.copyAssign + 1);
    EXPECT_EQ(QuadrupedRobot::getMoveAssignCalls(), before.moveAssign);
    EXPECT_EQ(QuadrupedRobot::getHeapAllocs(), before.allocs)
        << "присваивание в существующий буфер не аллоцирует";
    EXPECT_EQ(QuadrupedRobot::getHeapFrees(), before.frees);
}

TEST_F(MoveSemanticsFixture, MoveAssignFreesOwnBufferAndStealsSource) {
    QuadrupedRobot target("R-target", "Target", 10.0, "idle", 1.0, history);
    const Counters before = snapshot();
    const void* targetAddr = target.bufferAddress();
    const void* srcAddr = src->bufferAddress();

    target = std::move(*src);

    EXPECT_EQ(target.bufferAddress(), srcAddr) << "взял буфер источника";
    EXPECT_NE(target.bufferAddress(), targetAddr) << "прежний буфер сброшен";
    EXPECT_TRUE(src->isMovedFrom());
    EXPECT_EQ(target.getName(), "Spot");
    EXPECT_DOUBLE_EQ(target.getBatteryLevel(), 55.0);
    expectHistorySeq(target);

    EXPECT_EQ(QuadrupedRobot::getMoveAssignCalls(), before.moveAssign + 1);
    EXPECT_EQ(QuadrupedRobot::getCopyAssignCalls(), before.copyAssign);
    EXPECT_EQ(QuadrupedRobot::getHeapAllocs(), before.allocs)
        << "перемещающее присваивание не аллоцирует";
    EXPECT_EQ(QuadrupedRobot::getHeapFrees(), before.frees + 1)
        << "освобождён ровно прежний буфер получателя";
}

TEST_F(MoveSemanticsFixture, OverloadChosenOnlyByValueCategory) {
    const Counters before = snapshot();
    QuadrupedRobot a("R-a", "A", 20.0, "idle", 2.0, history);
    QuadrupedRobot b(a);                // l-value -> copy ctor
    QuadrupedRobot c(std::move(a));     // x-value -> move ctor
    b = a;                              // a ещё жив, но это l-value -> copy assign
    c = std::move(b);                   // x-value -> move assign

    EXPECT_EQ(QuadrupedRobot::getCopyCtorCalls(), before.copyCtor + 1);
    EXPECT_EQ(QuadrupedRobot::getMoveCtorCalls(), before.moveCtor + 1);
    EXPECT_EQ(QuadrupedRobot::getCopyAssignCalls(), before.copyAssign + 1);
    EXPECT_EQ(QuadrupedRobot::getMoveAssignCalls(), before.moveAssign + 1);
}

TEST_F(MoveSemanticsFixture, MovedFromObjectIsSafeAndReusable) {
    QuadrupedRobot moved(std::move(*src));
    (void)moved;

    EXPECT_TRUE(src->isMovedFrom());
    EXPECT_EQ(src->getLast10(), nullptr);
    // Уничтожение перемещённого объекта (в TearDown) не должно быть double free.

    // Перемещённый объект возвращаем в рабочее состояние: буфер создаётся
    // лениво при первой записи.
    src->setLast10(history);
    ASSERT_NE(src->getLast10(), nullptr);
    expectHistorySeq(*src);
    src->setName("Reborn");
    EXPECT_EQ(src->getName(), "Reborn");
}

TEST_F(MoveSemanticsFixture, CopyingMovedFromObjectDoesNotReadFreedMemory) {
    QuadrupedRobot moved(std::move(*src));
    (void)moved;
    ASSERT_TRUE(src->isMovedFrom());

    QuadrupedRobot copy(*src);
    ASSERT_NE(copy.getLast10(), nullptr);
    expectZeroHistory(copy);

    QuadrupedRobot assigned;
    assigned = *src;
    ASSERT_NE(assigned.getLast10(), nullptr);
    expectZeroHistory(assigned);
    EXPECT_TRUE(src->isMovedFrom());
}

TEST_F(MoveSemanticsFixture, SelfMoveAssignmentIsSafe) {
    QuadrupedRobot r("R-self", "Self", 33.0, "run", 4.0, history);
    const void* addr = r.bufferAddress();

    r = std::move(r);

    EXPECT_EQ(r.bufferAddress(), addr) << "самоперемещение не освобождает буфер";
    EXPECT_FALSE(r.isMovedFrom());
    EXPECT_EQ(r.getName(), "Self");
    EXPECT_DOUBLE_EQ(r.getBatteryLevel(), 33.0);
    expectHistorySeq(r);
}

TEST_F(MoveSemanticsFixture, MovingMethodsAreNoexcept) {
    // Без noexcept std::vector при релокации обязан давать strong guarantee и
    // копирует элементы вместо перемещения (сравни демо 8.I, шаг 6).
    EXPECT_TRUE(std::is_nothrow_move_constructible<QuadrupedRobot>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<QuadrupedRobot>::value);
}

TEST_F(MoveSemanticsFixture, VectorGrowthMovesInsteadOfCopying) {
    const Counters before = snapshot();
    std::vector<QuadrupedRobot> v;
    for (int i = 0; i < 8; ++i) v.push_back(makeRobot("R-v"));

    EXPECT_EQ(QuadrupedRobot::getCopyCtorCalls(), before.copyCtor)
        << "при росте vector элементы должны перемещаться, а не копироваться";
    // 8 перемещений при push_back + переносы существующих элементов при
    // релокации внутреннего буфера
    EXPECT_GT(QuadrupedRobot::getMoveCtorCalls(), before.moveCtor + 8);
    EXPECT_EQ(v.size(), 8u);
    for (const auto& r : v) {
        EXPECT_EQ(r.getName(), "V");
        EXPECT_DOUBLE_EQ(r.getBatteryLevel(), 50.0);
        EXPECT_NE(r.getLast10(), nullptr);
    }
}

TEST_F(MoveSemanticsFixture, ReturnedObjectDoesNotCopyTheBuffer) {
    const Counters before = snapshot();

    QuadrupedRobot r = makeRobot("R-f");

    EXPECT_EQ(r.getName(), "V");
    EXPECT_EQ(QuadrupedRobot::getCopyCtorCalls(), before.copyCtor);
    EXPECT_LE(QuadrupedRobot::getHeapAllocs(), before.allocs + 1)
        << "аллокация буфера ровно одна (NRVO либо перемещение), не две";
}

TEST_F(MoveSemanticsFixture, NoLeaksAfterMoveChain) {
    const int buffersAtStart = QuadrupedRobot::getLiveBuffers();
    const Counters before = snapshot();
    {
        std::vector<QuadrupedRobot> v;
        for (int i = 0; i < 5; ++i) {
            QuadrupedRobot tmp("R-c", "Chain", 60.0, "run", 6.0, history);
            v.push_back(std::move(tmp));
            v.back().setBatteryLevel(70.0);
        }
        QuadrupedRobot acc;
        for (auto& r : v) acc = std::move(r);
        EXPECT_EQ(acc.getBatteryLevel(), 70.0);
        EXPECT_EQ(QuadrupedRobot::getMoveAssignCalls(), before.moveAssign + 5);
        EXPECT_EQ(QuadrupedRobot::getCopyAssignCalls(), before.copyAssign);
    }
    EXPECT_EQ(QuadrupedRobot::getLiveBuffers(), buffersAtStart)
        << "после цепочки перемещений живых буферов ровно столько же, сколько до";
}

TEST_F(MoveSemanticsFixture, StreamMarksMovedFromObject) {
    QuadrupedRobot moved(std::move(*src));

    std::ostringstream movedFrom;
    movedFrom << *src;
    EXPECT_NE(movedFrom.str().find("moved-from"), std::string::npos);

    std::ostringstream owner;
    owner << moved;
    EXPECT_EQ(owner.str().find("moved-from"), std::string::npos);
    EXPECT_NE(owner.str().find("Spot"), std::string::npos);
}

}  // namespace
