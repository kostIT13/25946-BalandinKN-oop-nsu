# Задание 11.I — иерархия классов роботов и «алмазная проблема»

Четыре класса, каждый — отдельный модуль (`.h` + `.cpp`):

| Пункт | Класс | Наследование | Что добавляет |
|---|---|---|---|
| п.1 | `QuadrupedRobot` (`robot`) | — | вершина иерархии, в конструкторе задаёт имя экземпляра |
| п.2 | `CargoRobot` (`cargo_robot`) | `public virtual QuadrupedRobot` | приватный вес перевозимого груза |
| п.3 | `AutonomousRobot` (`autonomous_robot`) | `public virtual QuadrupedRobot` | приватный запас хода в автономном режиме |
| п.4 | `CargoAutonomousRobot` (`cargo_autonomous_robot`) | `public CargoRobot, public AutonomousRobot` | обе ветки сразу (множественное наследование) |
| п.5 | `DiamondCargo/DiamondAuto/DiamondBoth` (`diamond_problem`) | то же, но **без** `virtual` | та же ромбовидная схема, чтобы показать проблему |

Схема:

```
            QuadrupedRobot            (вершина: имя экземпляра)
             /        \
   (virtual) /          \ (virtual)
   CargoRobot          AutonomousRobot
             \        /
        CargoAutonomousRobot          (п.4 — множественное наследование)
```

## п.1 — база задаёт имя

```cpp
class QuadrupedRobot {
private:
    std::string name;
public:
    explicit QuadrupedRobot(const std::string& name = "Unnamed");
};
```

Имя проверяется в самом конструкторе базы: пустое → `std::invalid_argument`.
Проверка живёт в одном месте и потому работает для всех четырёх классов —
наследник не может создать экземпляр без имени, даже если его собственный
конструктор об этом «забудет».

Плюс счётчик живых **подобъектов** базы (`liveCount`) и счётчик вызовов
конструктора базы (`baseCtorCalls`) — на них держится вся демонстрация п.5:
сколько раз реально сконструирован `QuadrupedRobot` при создании одного объекта.

## п.2–п.3 — две ветки ромба

```cpp
class CargoRobot : public virtual QuadrupedRobot {
private:
    double cargoWeight;              // вес перевозимого груза
};

class AutonomousRobot : public virtual QuadrupedRobot {
private:
    double autonomousRangeKm;        // текущий запас хода в автономном режиме
};
```

Каждая ветка инициализирует базу своим именем (`CargoRobot(35.0, "Mule")` →
база получает `"Mule"`). Пока класс один, `virtual` ни на что не влияет —
он нужен ради п.4.

## п.4 — общий потомок

```cpp
CargoAutonomousRobot::CargoAutonomousRobot(double cargoWeight,
                                           double autonomousRangeKm,
                                           const std::string& name)
    : QuadrupedRobot(name),            // ← инициализация виртуальной базы
      CargoRobot(cargoWeight, name),
      AutonomousRobot(autonomousRangeKm, name)
{}
```

Требование задания «в конструкторе используются конструкторы родительских
классов для установки массы груза, запаса хода и имени» выполнено, но с
важной особенностью виртуального базового класса: **виртуальную базу
конструирует самый производный класс напрямую**. Поэтому `QuadrupedRobot(name)`
стоит в списке инициализаторов `CargoAutonomousRobot`, а аргументы имени внутри
`CargoRobot(...)`/`AutonomousRobot(...)` при создании `CargoAutonomousRobot`
игнорируются (они работают, только когда объект создаётся как `CargoRobot` или
`AutonomousRobot` — см. п.2/п.3 вывода демо).

Порядок инициализации в выводе: база → `CargoRobot` → `AutonomousRobot` →
сам класс (по порядку обхода графа, а не по порядку в списке инициализаторов);
деструкторы — строго в обратном порядке.

## п.5 — алмазная проблема и её решение

### Проблема: тот же ромб без `virtual` (`diamond_problem.h`)

```
[QuadrupedRobot] base constructor name="Argus" | alive: 1
[QuadrupedRobot] base constructor name="Argus" | alive: 2   ← ДВА подобъекта базы
>>> live QuadrupedRobot subobjects: 2   (ONE DiamondBoth object)
```

Один объект содержит две независимые копии базы, и это видно по поведению:

```
setName through one branch only:
   DiamondCargo="Renamed-By-Cargo"  DiamondAuto="Argus"
```

Объект одного класса сообщает **два разных имени** — единый идентификатор
экземпляра перестал быть единым. Дополнительно (`sizeof`): ромб без `virtual` —
64 байта, с `virtual` — 56 (одна база вместо двух, но появился указатель на
виртуальную базу, поэтому разница меньше, чем «минус целая копия»).

Тот же ромб ломает и самый обычный код — `diamond_ambiguity.cpp` (в сборку не входит):

```
error: request for member 'getName' is ambiguous
error: request for member 'setName' is ambiguous
error: 'QuadrupedRobot' is an ambiguous base of 'DiamondBoth'
```

Проверить: `g++ -std=c++17 -fsyntax-only -I. diamond_ambiguity.cpp`

### Решение: `virtual`-наследование

```cpp
class CargoRobot      : public virtual QuadrupedRobot {};
class AutonomousRobot : public virtual QuadrupedRobot {};
```

```
base-class constructors called while building it: 1  (1 virtual base, not 2)
>>> live QuadrupedRobot subobjects: 1   (one CargoAutonomousRobot)
the same object seen through both branches: "Argus" / "Argus"
```

Что даёт решение:

- в производном объекте ровно **один** подобъект базы → одно имя, один набор
  инвариантов, нет расхождения копий;
- неквалифицированное `r.getName()` снова однозначено, приведение
  `QuadrupedRobot&` к объекту ромба законно;
- «is-a» работает для обеих веток одновременно: `CargoAutonomousRobot` — это и
  грузовик, и автономный робот, и оба взгляда видят один и тот же базовый объект.

Чем расплачиваются:

- подобъект виртуальной базы конструирует самый производный класс — промежуточные
  классы не могут сами задать его аргументы (легко упустить, если забыть
  `QuadrupedRobot(name)` в списке инициализаторов: соберётся с аргументом по
  умолчанию);
- доступ к виртуальной базе идёт через косвенную адресацию (отсюда лишние байты
  и небольшой штраф на каждое обращение);
- `virtual` нужно писать во **всех** ветках ромба; одна забытая ветка — и
  два подобъекта возвращаются.

## Сборка и запуск

```
cmake -S homeworks/Task11/Task11_1 -B homeworks/Task11/Task11_1/build -G "MinGW Makefiles"
cmake --build homeworks/Task11/Task11_1/build
homeworks\Task11\Task11_1\build\robot.exe
```

Собрано MinGW (g++ 6.3.0, `-Wall -Wextra`) без предупреждений.

`diamond_ambiguity.cpp` в `add_executable` не входит: он обязан не компилироваться,
иначе нечего было бы показывать в п.5.
