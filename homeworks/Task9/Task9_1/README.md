# Задание 9.I — дружественная функция и дружественный класс

Класс `QuadrupedRobot` дополнен приватным полем `manufacturer` (название
производителя) и двумя способами дать к нему доступ извне.

## п.1. Дружественная СВОБОДНАЯ функция — одно поле

```cpp
class QuadrupedRobot {
    std::string manufacturer;            // приватное поле
    friend void setManufacturer(QuadrupedRobot& r, const std::string& m);
};

void setManufacturer(QuadrupedRobot& r, const std::string& m) {
    r.manufacturer = QuadrupedRobot::normalizeManufacturer(m);   // доступ есть
}
```

Публичного сеттера для `manufacturer` нет, поэтому эта функция — единственный
путь задать производителя снаружи. Она видит **только** `manufacturer` (и
публичную часть): `r.id = "..."` внутри неё уже не компилируется.

## п.2. Дружественный КЛАСС — все поля

```cpp
class RobotFactory {
    QuadrupedRobot produce(...) const;              // собирает робота напрямую в private-поля
    void repurpose(QuadrupedRobot&, ...) const;     // перебивает ВСЕ поля готового объекта
    void audit(const QuadrupedRobot&) const;        // читает все поля + приватную статику
    void setNextUid(int);                           // пишет в приватную статику
};
// внутри QuadrupedRobot:
friend class RobotFactory;
```

Дружба с классом даёт заводу доступ ко всему приватному интерфейсу: `id`, `uid`,
`name`, `batteryLevel`, `currentCommand`, `todayDistance`, `last10`,
`manufacturer`, к приватной статике (`objectCount`, `nextUid`) и к приватным
методам (`normalizeName`, `ensureBuffer`, `checkId`). `repurpose()` меняет и
`id`, и `uid` — публичных сеттеров для них не существует в принципе.

## Отличия и последствия

| | дружественная функция | дружественный класс |
|---|---|---|
| Объём доступа | ровно то, что разрешено сигнатурой и телом одной функции | все private-поля + private-статика + private-методы |
| Контроль инварианта | обеспечивает язык: лишнее недоступно | обеспечивает сам завод — язык уже не ограничивает |
| Область | одна конкретная функция | каждый метод класса, включая будущие |
| Где уместно | «разрешить одно действие» | «отдать подсистему-владелец» (сборка, сериализация, тест-обёртка) |

Что важно помнить про `friend`:

- дружба **не наследуется** (дружба с базовым классом не даёт доступа наследникам)
  и **не транзитивна** (друг моего друга — мне не друг);
- дружба **не симметрична**: `RobotFactory` видит private робота, робот про завод
  ничего не знает;
- `friend` — не нарушает инкапсуляцию формально (поле остаётся `private`, имя и
  тип по-прежнему скрыты от остальных), но снимает с друга ответственность
  соблюдать инвариант через публичный API — см. `produce()`, которая пишет
  `batteryLevel` без нормализации;
- наружу поля как были, так и остались доступны **только на чтение**
  (`getManufacturer()`, `getId()` возвращают `const std::string&`).

## Доказательство, что доступ даёт именно дружба

Файл `not_allowed_example.cpp` (в сборку не входит) — те же обращения из обычной
функции:

```
not_allowed_example.cpp:6:7: error: 'std::string QuadrupedRobot::manufacturer' is private within this context
not_allowed_example.cpp:7:7: error: 'std::string QuadrupedRobot::id' is private within this context
not_allowed_example.cpp:8:21: error: 'int QuadrupedRobot::objectCount' is private within this context
```

Проверить: `g++ -std=c++17 -fsyntax-only -I. not_allowed_example.cpp`

## Сборка и запуск

```
cmake -S homeworks/Task9/Task9_1 -B homeworks/Task9/Task9_1/build -G "MinGW Makefiles"
cmake --build homeworks/Task9/Task9_1/build
homeworks\Task9\Task9_1\build\robot.exe
```

Собрано MinGW (g++ 6.3.0, `-Wall -Wextra`) без предупреждений.
