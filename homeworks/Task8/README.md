# Задание 8 (docs/Задания OOP_08.pdf) — перемещающие методы и тесты на fixture

Каждая часть — самостоятельный cmake-проект (своя копия `robot.h`/`robot.cpp`,
класс тот же, что развивался в Task6/Task7).

| Часть | Папка | Что сделано |
|---|---|---|
| 8.I | `Task8_1` | п.1 — `QuadrupedRobot(QuadrupedRobot&&) noexcept` и `operator=(QuadrupedRobot&&) noexcept`; п.2 — демо, где отличия от копирования измеримы: адрес буфера, счётчики copy/move ctor и assign, счётчик `new double[]`/`delete[]`, состояние moved-from, выбор перегрузки по категории значения, `vector`/`return` |
| 8.II | `Task8_2` | Тесты googletest на fixture (`TEST_F`): базовый `RobotFixture` (SetUp/TearDown, обнуление счётчиков, сверка живых буферов) и наследующий от него `MoveSemanticsFixture`. 19 тестов, все проходят |

## Сборка

8.I (MinGW или MSVC):
```
cmake -S homeworks/Task8/Task8_1 -B homeworks/Task8/Task8_1/build -G "MinGW Makefiles"
cmake --build homeworks/Task8/Task8_1/build
homeworks\Task8\Task8_1\build\robot.exe
```

8.II (тесты — под MSVC: g++ 6.3.0 не собирает googletest; локальный исходник gtest
из Task6, чтобы не качать заново):
```
cmake -S homeworks/Task8/Task8_2 -B homeworks/Task8/Task8_2/build -G "Visual Studio 18 2026" -DFETCHCONTENT_SOURCE_DIR_GOOGLETEST=homeworks/Task6/Task6_3/build/_deps/googletest-src
cmake --build homeworks/Task8/Task8_2/build --config Debug
ctest --test-dir homeworks/Task8/Task8_2/build -C Debug --output-on-failure
```

Подробности по решениям и разбор вывода — в `Task8_1/README.md` и `Task8_2/README.md`.
