# Задание 9.II — класс с дружественными функциями (доступ к части полей)

Класс `QuadrupedRobot` дополнен приватным полем `model` (название модели).
Класс `Administrator` содержит функции, дружественные `QuadrupedRobot`, — но
доступ получает не весь класс, а **конкретные функции** и **не все поля**.

## Схема доступа

| Метод `Administrator` | друг? | что может менять |
|---|---|---|
| `setModel(r, model)` | да (`friend void Administrator::setModel(...)`) | `model` — одно приватное поле |
| `assignRegistration(r, id, model)` | да | `id` + `model` — несколько приватных полей |
| `renamePublicly(r, name)` | нет | только `name` через публичный `setName()` |
| `report(r)` | нет | ничего, только публичные геттеры |

Недоступно администратору совсем: `uid`, `batteryLevel`, `currentCommand`,
`todayDistance`, `last10`, приватная статика `nextUid`/`objectCount`.

## Как объявлена дружба

```cpp
// admin.h — класс объявлен раньше, иначе friend-объявление метода не скомпилируется
class Administrator {
public:
    void setModel(QuadrupedRobot& r, const std::string& model);
    void assignRegistration(QuadrupedRobot& r, const std::string& id,
                            const std::string& model);
    ...
};

// robot.h
#include "admin.h"          // нужен ПОЛНЫЙ тип Administrator
class QuadrupedRobot {
    std::string model;      // приватное поле
    friend void Administrator::setModel(QuadrupedRobot&, const std::string&);
    friend void Administrator::assignRegistration(QuadrupedRobot&,
                                                 const std::string&,
                                                 const std::string&);
};
```

Тонкости, без которых эта схема не собирается:

1. `friend void Administrator::setModel(...)` — это объявление функции внутри
   класса: имя `Administrator::setModel` должно быть уже объявлено, поэтому
   `admin.h` подключается **до** определения `QuadrupedRobot` (оно в `robot.h`,
   а сам `Administrator` определён в `admin.h`, который `robot.h` включает).
2. Определяться эти методы должны вне `robot.h` (в `admin.cpp`) — иначе получились
   бы определения функций в namespace-области видимости, и они не совпали бы с
   объявлениями класса.
3. Определение метода-друга в `admin.cpp` доступ к `private` наследует от
   `friend`-объявления в классе, а не от того, в каком файле лежит определение.

## Границы: доказательство компилятором

`not_allowed_example.cpp` (в сборку не входит) обращается к приватному из
НЕдружественного метода того же `Administrator`:

```
not_allowed_example.cpp:8:7:  error: 'int QuadrupedRobot::uid' is private within this context
not_allowed_example.cpp:9:7:  error: 'double QuadrupedRobot::batteryLevel' is private within this context
not_allowed_example.cpp:10:7: error: 'double* QuadrupedRobot::last10' is private within this context
not_allowed_example.cpp:11:21: error: 'int QuadrupedRobot::nextUid' is private within this context
```

Проверить: `g++ -std=c++17 -fsyntax-only -I. not_allowed_example.cpp`

При разработке этот же механизм уже поймал реальную оплошность: в
`renamePublicly()` был написан `r.uid` — компилятор отказал, потому что этот
метод не друг. Это и есть смысл дружбы поимённым методам вместо
`friend class`: лишнее недоступно даже «своему» классу.

## Сравнение с 9.I

- **9.I**: `friend class RobotFactory;` — завод может менять **все** поля и
  приватную статику; язык его не ограничивает вовсе.
- **9.II**: дружба двум конкретным методам — минимально необходимый роль
  администратора объём доступа. Это ближе к принципу наименьших привилегий:
  если завтра в `Administrator` добавят новый метод, приватного доступа у него
  не будет, пока его явно не объявят другом.

## Сборка и запуск

```
cmake -S homeworks/Task9/Task9_2 -B homeworks/Task9/Task9_2/build -G "MinGW Makefiles"
cmake --build homeworks/Task9/Task9_2/build
homeworks\Task9\Task9_2\build\robot.exe
```

Собрано MinGW (g++ 6.3.0, `-Wall -Wextra`) без предупреждений.
