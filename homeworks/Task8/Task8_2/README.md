# Задание 8.II — тесты с использованием fixture

Класс — тот же, что в 8.I (`robot.h` / `robot.cpp`). Тесты: googletest,
`TEST_F` (fixture), 19 тестов в двух fixture-ях.

## п.1. Fixture и зачем он здесь

```cpp
class RobotFixture : public ::testing::Test {
protected:
    void SetUp() override;     // resetStats() + канонический объект src
    void TearDown() override;  // delete src + сверка числа живых буферов
};

class MoveSemanticsFixture : public RobotFixture {   // наследование fixture
protected:
    void SetUp() override;     // RobotFixture::SetUp() + снимок счётчиков
    static Counters snapshot();
};
```

Fixture даёт три вещи, без которых эти тесты были бы невозможны:

1. **Общее начальное состояние.** Канонический робот (`R-001`, `Spot`, 55%,
   `walk`, 7 км, история 1…10) создаётся в `SetUp()` и уничтожается в
   `TearDown()` вокруг **каждого** теста. Тесты независимы: перемещение объекта
   в одном тесте не «отравляет» другой (`SetUpGivesExactlyOneLiveObjectPerTest`,
   `RobotIsPristineDespiteMutationsInEarlierTests`).
2. **Обнуление статических счётчиков** класса. Класс считает вызовы спецметодов
   и аллокации; без `resetStats()` в `SetUp()` проверки вида «аллокаций стало
   ровно на 1 больше» были бы невозможны.
3. **Проверку владения памятью в `TearDown()`**: сравнивается
   `getLiveBuffers()` до и после теста. Тест, оставивший утечку или устроивший
   double free, падает сразу на себе.

Важная деталь: снимок счётчиков берётся **непосредственно перед** проверяемой
операцией (`snapshot()` внутри теста), потому что подготовка теста (создание
объекта-получателя) сама делает аллокацию.

## Что проверяют тесты

| Тест | Смысл |
|---|---|
| `CopyCtorAllocatesOwnBufferAndKeepsSource` | у копии ДРУГОЙ буфер, `+1` аллокация, источник цел |
| `MoveCtorStealsBufferWithoutAllocation` | буфер тот же по адресу, `0` аллокаций, источник `moved-from`, данные доехали |
| `CopyAssignReusesBufferAndKeepsSource` | буфер переиспользован, 0 аллокаций и 0 освобождений |
| `MoveAssignFreesOwnBufferAndStealsSource` | ровно 1 `delete[]` (свой буфер), 0 аллокаций, буфер источника у получателя |
| `OverloadChosenOnlyByValueCategory` | copy/move ctor и copy/move assign выбираются ровно по категории значения (по 1 вызову каждого) |
| `MovedFromObjectIsSafeAndReusable` | `last10 == nullptr`, `getLast10() == nullptr`, объект живёт дальше после `setLast10()` |
| `CopyingMovedFromObjectDoesNotReadFreedMemory` | копирование перемещённого объекта не читает из `nullptr` |
| `SelfMoveAssignmentIsSafe` | `x = std::move(x)` не освобождает собственный буфер |
| `MovingMethodsAreNoexcept` | `is_nothrow_move_constructible` / `is_nothrow_move_assignable` |
| `VectorGrowthMovesInsteadOfCopying` | рост `vector`: 0 копирующих конструкторов, перемещений больше 8 |
| `ReturnedObjectDoesNotCopyTheBuffer` | `return` из функции: 1 аллокация буфера, не 2 |
| `NoLeaksAfterMoveChain` | после цепочки move в контейнере живых буферов ровно столько же, сколько до |
| `StreamMarksMovedFromObject` | `operator<<` помечает `(moved-from)` |
| `EmptyIdThrowsAndLeaksNothing` | **нашёл реальный дефект**: `checkId()` бросал исключение после `new double[10]`, а деструктор при исключении из конструктора не вызывается → утечка. Исправлено порядком инициализации |

## Сборка и запуск

googletest подключается через `FetchContent` (как в Task6). Чтобы не качать его
повторно, указан локальный исходник:

```
cmake -S homeworks/Task8/Task8_2 -B homeworks/Task8/Task8_2/build -G "Visual Studio 18 2026" -DFETCHCONTENT_SOURCE_DIR_GOOGLETEST=<repo>/homeworks/Task6/Task6_3/build/_deps/googletest-src
cmake --build homeworks/Task8/Task8_2/build --config Debug
ctest --test-dir homeworks/Task8/Task8_2/build -C Debug --output-on-failure
```

Результат: `100% tests passed`, 19 tests.

Про генератор: g++ 6.3.0 (MinGW) не собирает сам googletest (`std::mutex`,
`_stricmp` и т. п. в `gtest-port.h`), поэтому тестовая часть собирается под
MSVC / Visual Studio 2026 — так же, как в `Task6`.
