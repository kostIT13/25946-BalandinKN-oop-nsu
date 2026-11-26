# class AutonomousCargoQuadrupedRobot

*Defined at homeworks\Task12\Task12_1\autonomous_cargo_quadruped_robot.h#10*



**brief** Грузовой квадропед с автономным режимом — четвёртый класс цепочки.

 Замыкает последовательную иерархию из четырёх классов (без множественного наследования): Robot -> QuadrupedRobot -> CargoQuadrupedRobot -> AutonomousCargoQuadrupedRobot. Своя реализация move(): робот едет маршем без внешней команды, пока остаток автономного запаса хода не кончится.



Inherits from CargoQuadrupedRobot



## Members

private double autonomousRangeM



## Functions

### stepBatteryCost

*protected double stepBatteryCost()*

*Defined at homeworks\Task12\Task12_1\autonomous_cargo_quadruped_robot.cpp#3*



**brief** Расход за автономный шаг: марш расходует меньше галопа с грузом.

### AutonomousCargoQuadrupedRobot

*public void AutonomousCargoQuadrupedRobot(const std::string & name, double battery, double cargoWeightKg, double autonomousRangeM)*

*Defined at homeworks\Task12\Task12_1\autonomous_cargo_quadruped_robot.cpp#7*



**name** имя экземпляра.

**battery** заряд в процентах.

**cargoWeightKg** масса груза в килограммах.

**autonomousRangeM** автономный запас хода в метрах.

### move

*public void move()*

*Defined at homeworks\Task12\Task12_1\autonomous_cargo_quadruped_robot.cpp#13*



**brief** Автономный марш: своя реализация move() для этого класса.

 Пока автономный запас хода не исчерпан, робот делает шаг без внешней команды и тратит меньше заряда; после исчерпания — стоит и об этом сообщает.

### className

*public std::string className()*

*Defined at homeworks\Task12\Task12_1\autonomous_cargo_quadruped_robot.cpp#26*



**brief** Имя самого конкретного класса.

### gaitTag

*public std::string gaitTag()*

*Defined at homeworks\Task12\Task12_1\autonomous_cargo_quadruped_robot.cpp#30*



**brief** Подпись походки этого класса; СКРЫВАЕТ походку предков (не virtual).

### getAutonomousRangeM

*public double getAutonomousRangeM()*

*Defined at homeworks\Task12\Task12_1\autonomous_cargo_quadruped_robot.h#42*



**brief** Остаток автономного запаса хода в метрах.



