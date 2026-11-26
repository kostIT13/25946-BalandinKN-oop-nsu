# Задание 11 (docs/shared.pdf, 13-11-2026) — иерархия классов роботов

Каждая часть — самостоятельный проект; каждый класс — отдельный модуль (`.h` + `.cpp`).

| Часть | Папка | Что сделано |
|---|---|---|
| 11.I п.1 | `Task11_1/robot.h/cpp` | вершина иерархии `QuadrupedRobot`: имя экземпляра задаётся в конструкторе (пустое имя → исключение) |
| 11.I п.2 | `Task11_1/cargo_robot.h/cpp` | `CargoRobot : public virtual QuadrupedRobot` — приватный вес перевозимого груза |
| 11.I п.3 | `Task11_1/autonomous_robot.h/cpp` | `AutonomousRobot : public virtual QuadrupedRobot` — приватный запас хода в автономном режиме |
| 11.I п.4 | `Task11_1/cargo_autonomous_robot.h/cpp` | `CargoAutonomousRobot : CargoRobot, AutonomousRobot` — конструктор использует родительские для груза, запаса хода и имени |
| 11.I п.5 | `Task11_1/diamond_problem.*` + `main.cpp` | алмазная проблема (тот же ромб без `virtual`: 2 подобъекта базы, два разных имени, `ambiguous`) и её решение (`virtual`: 1 подобъект) |
| 11.II | `Task11_2/` | UML-диаграмма иерархии: `uml_virtual.svg`, `uml_diamond_problem.svg`, исходник `hierarchy.mmd` |

## Сборка и запуск

```
cmake -S homeworks/Task11/Task11_1 -B homeworks/Task11/Task11_1/build -G "MinGW Makefiles"
cmake --build homeworks/Task11/Task11_1/build
homeworks\Task11\Task11_1\build\robot.exe
```

Собрано MinGW (g++ 6.3.0, `-Wall -Wextra`) без предупреждений.

Файл `Task11_1/diamond_ambiguity.cpp` в сборку не входит: он показывает
неоднозначность `getName`/`setName` и приведения к базе в ромбе без `virtual` и
обязан не компилироваться
(`g++ -std=c++17 -fsyntax-only -I. diamond_ambiguity.cpp`).

Подробности — в [`Task11_1/README.md`](Task11_1/README.md) и
[`Task11_2/README.md`](Task11_2/README.md).
