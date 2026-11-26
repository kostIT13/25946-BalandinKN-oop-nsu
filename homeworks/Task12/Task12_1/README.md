# Задание 12.I — абстрактный класс и полиморфизм

Последовательная иерархия из **четырёх** классов, без множественного
наследования; каждый класс — отдельный модуль (`.h` + `.cpp`).

| Пункт | Класс | Наследует | Реализация `move()` |
|---|---|---|---|
| п.1 | `Robot` (`robot`) | — | `virtual void move() = 0;` — чистая виртуальная |
| п.2 | `QuadrupedRobot` (`quadruped_robot`) | `Robot` | шаг галопом, расход 1% |
| п.3 | `CargoQuadrupedRobot` (`cargo_quadruped_robot`) | `QuadrupedRobot` | шаг с грузом, расход зависит от массы |
| п.4 | `AutonomousCargoQuadrupedRobot` (`autonomous_cargo_quadruped_robot`) | `CargoQuadrupedRobot` | автономный марш, пока не кончится запас хода |

```
Robot  (абстрактный: move() = 0)
  └─ QuadrupedRobot                       галоп,  шаг 60 см, расход 1%
       └─ CargoQuadrupedRobot             шаг с грузом, расход 1% + 3% / 100 кг
            └─ AutonomousCargoQuadrupedRobot  автономный марш, расход 0,5% + 1% / 100 кг
```

## п.1 — абстрактный класс

```cpp
class Robot {
private:
    std::string name;
    double batteryLevel;
protected:
    static double normalizeBattery(double value);
    void logState(const std::string& what) const;
public:
    virtual void move() = 0;
    virtual std::string className() const;
    virtual ~Robot();
    /* геттеры/сеттеры */
};
```

- `move() = 0` делает класс абстрактным: `Robot r;` и `new Robot(...)` — ошибка
  компиляции (`cannot declare variable 'r' to be of abstract type 'Robot'`);
- **любой** потомок, не переопределивший `move()`, сам остаётся абстрактным —
  в `not_allowed_example.cpp` есть `SilentRobot : public Robot` без `move()`, и
  его создание не собирается. То есть контракт «уметь двигаться» язык требует
  соблюдать, а не оставляет на словах;
- деструктор виртуальный: иначе `delete` через `Robot*` не вызвал бы деструктор
  потомка;
- поля приватные, наследникам отданы только protected-помощники
  (`normalizeBattery`, `logState`), наружу — геттеры.

## п.2–п.4 — свои реализации `move()`

Расход заряда описан protected-хуком `stepBatteryCost()`, который каждая ступень
переопределяет, а `takeStep()` (общий для базы) применяет последствия шага. Так
«своя уникальная реализация» не превращается в копипасту: у класса своя логика
только в том, что реально отличается.

```cpp
void CargoQuadrupedRobot::move() {
    const double cost = stepBatteryCost();   // 1% + 3% за каждые 100 кг
    takeStep(cost);
    logState("move: loaded step, ...");
}
```

`AutonomousCargoQuadrupedRobot::move()` добавляет своё состояние — остаток
автономного запаса хода: пока он больше нуля робот идёт маршем дешевле, после
исчерпания — стоит и сообщает об этом.

## п.5 — массив представителей: полиморфно и нет

В массиве `std::vector<Robot*>` по представителю каждого **конкретного** класса
(`Robot` представителем быть не может — класс абстрактный, и это единственный
способ вообще иметь «элемента типа Robot» в массиве).

### Полиморфное поведение

```
  --- step 1 ---
      [QuadrupedRobot] move: gallop step name="Quad" battery=99
      [CargoQuadrupedRobot] move: loaded step, cost=7.0 per step, cargo=200 kg battery=93
      [AutonomousCargoQuadrupedRobot] move: autonomous march, left=59.5 m, cost=2.5 battery=97.5
```

Одна строка исходника `r->move()` дала три разные реализации: выбор идёт по
**динамическому** типу объекта (таблица vtable), а не по типу переменной.

### Не-полиморфное поведение — три способа его получить

1. **невиртуальный метод.** В каждом классе есть невиртуальная `gaitTag()`,
   скрывающая одноимённую функцию базы. В том же цикле по
   `Robot*`:

   ```
     element points at QuadrupedRobot,            r->gaitTag() = base-gait
     element points at CargoQuadrupedRobot,       r->gaitTag() = base-gait
     element points at AutonomousCargoQuadrupedRobot, r->gaitTag() = base-gait
   ```

   те же объекты через свой тип: `gallop` / `loaded-trot` / `autonomous-march`.
   Связывание произошло на этапе компиляции по **статическому** типу — объект не
   спросили.

2. **массив по значению → срезка объекта (object slicing):**

   ```
     typed[1] was built FROM a CargoQuadrupedRobot,
     after the copy it is a QuadrupedRobot: only the QuadrupedRobot part survived
     [QuadrupedRobot] move: gallop step name="CargoQuad" battery=85
   ```

   вызвался галоп с расходом 1%, а не «грузовой» шаг: вместе с лишней частью
   объекта пропала и его `move()` (таблица vtable стала таблицей `QuadrupedRobot`).

3. **свои методы не видны через `Robot*`:** `r->getCargoWeightKg()` в цикле по
   `Robot*` не компилируется — статический тип знает только интерфейс `Robot`.
   Вернуть конкретный тип можно `dynamic_cast`, и демо печатает результат для
   каждого элемента (`nullptr` для того, кто не грузовой).

## Сборка и запуск

```
cmake -S homeworks/Task12/Task12_1 -B homeworks/Task12/Task12_1/build -G "MinGW Makefiles"
cmake --build homeworks/Task12/Task12_1/build
homeworks\Task12\Task12_1\build\robot.exe
```

Собрано MinGW (g++ 6.3.0, `-Wall -Wextra`) без предупреждений.

`not_allowed_example.cpp` в сборку не входит — он обязан падать:

```
g++ -std=c++17 -fsyntax-only -I. not_allowed_example.cpp
cannot declare variable 'r' to be of abstract type 'Robot'
cannot declare variable 'silent' to be of abstract type 'SilentRobot'
'static double Robot::normalizeBattery(double)' is protected within this context
'double Robot::batteryLevel' is private within this context
```

Комментарии Doxygen-вида в заголовках нужны не для красоты: по ним построен
[`../Task12_2`](../Task12_2/README.md) — документация, сгенерированная из кода
(пункт 12.II).
