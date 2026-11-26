# class Robot

*Defined at homeworks\Task12\Task12_1\robot.h#10*



**brief** Вершина иерархии: абстрактный класс «робот».

 move() объявлена чистой виртуальной, поэтому класс нельзя создать напрямую (Robot r; — ошибка компиляции): перемещение обязан описать каждый конкретный потомок. Поля приватные, наследники видят их только через protected-часть.



## Members

private basic_string name

private double batteryLevel



## Functions

### normalizeBattery

*protected static double normalizeBattery(double value)*

*Defined at homeworks\Task12\Task12_1\robot.cpp#5*



**brief** Нормализация заряда в диапазон [0; 100] процентов.

**value** заряд в процентах.

**return** приведённое значение.

### logState

*protected void logState(const std::string & what)*

*Defined at homeworks\Task12\Task12_1\robot.cpp#20*



**brief** Строка состояния с именем самого конкретного класса.

**what** текст о вызываемом действии.

### Robot

*public void Robot(const std::string & name, double battery)*

*Defined at homeworks\Task12\Task12_1\robot.cpp#11*



**brief** Создание робота.

**name** имя экземпляра, пустое имя недопустимо.

**battery** заряд в процентах, нормализуется в [0; 100].

**throws**при пустом имени.

### ~Robot

*public void ~Robot()*

*Defined at homeworks\Task12\Task12_1\robot.cpp#18*

### getName

*public const std::string & getName()*

*Defined at homeworks\Task12\Task12_1\robot.h#41*



**brief** Имя экземпляра.

### getBatteryLevel

*public double getBatteryLevel()*

*Defined at homeworks\Task12\Task12_1\robot.h#44*



**brief** Текущий заряд в процентах.

### move

*public void move()*



**brief** Переместить робота на один шаг.

 Чистая виртуальная функция: реализации здесь нет, конкретный класс обязан определить свою.

### setName

*public void setName(const std::string & value)*

*Defined at homeworks\Task12\Task12_1\robot.cpp#25*



**brief** Задать имя. 

**throws**при пустом имени.

### setBatteryLevel

*public void setBatteryLevel(double value)*

*Defined at homeworks\Task12\Task12_1\robot.cpp#31*



**brief** Задать заряд (нормализуется в [0; 100]).

### className

*public std::string className()*

*Defined at homeworks\Task12\Task12_1\robot.cpp#35*



**brief** Имя самого конкретного класса — для печатаемых сообщений.

### gaitTag

*public std::string gaitTag()*

*Defined at homeworks\Task12\Task12_1\robot.cpp#39*



**brief** Подпись походки. НЕ виртуальная — для контраста с move(): вызов выбирается по статическому типу указателя, а не по объекту.

### statusLine

*public std::string statusLine()*

*Defined at homeworks\Task12\Task12_1\robot.cpp#43*



**brief** Краткая строка состояния «Класс(имя, battery=…)».



