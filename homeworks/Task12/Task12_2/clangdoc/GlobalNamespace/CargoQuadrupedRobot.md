# class CargoQuadrupedRobot

*Defined at homeworks\Task12\Task12_1\cargo_quadruped_robot.h#8*



**brief** Грузовой квадропедальный робот — прямой наследник QuadrupedRobot.

 Своя реализация move(): тот же шаг, но расход заряда зависит от массы груза, поэтому грузовой робот ходит «тяжелее» и дальше разряжается за тот же шаг.



Inherits from QuadrupedRobot



## Members

private double cargoWeightKg

private int extraConsumptionPerHundredKg



## Functions

### stepBatteryCost

*protected double stepBatteryCost()*

*Defined at homeworks\Task12\Task12_1\cargo_quadruped_robot.cpp#3*



**brief** Расход заряда за шаг с учётом груза.

 Переопределение protected-хука базы: реализация move() из базы сама спрашивает стоимость шага у самого конкретного класса.

**return** базовый расход плюс надбавка за каждые 100 кг груза.

### CargoQuadrupedRobot

*public void CargoQuadrupedRobot(const std::string & name, double battery, double cargoWeightKg, int extraConsumptionPerHundredKg)*

*Defined at homeworks\Task12\Task12_1\cargo_quadruped_robot.cpp#7*



**name** имя экземпляра.

**battery** заряд в процентах.

**cargoWeightKg** масса перевозимого груза в килограммах.

**extraConsumptionPerHundredKg** дополнительная трата заряда за 100 кг.

### move

*public void move()*

*Defined at homeworks\Task12\Task12_1\cargo_quadruped_robot.cpp#13*



**brief** Шаг с грузом: реализация move() этого класса.

### className

*public std::string className()*

*Defined at homeworks\Task12\Task12_1\cargo_quadruped_robot.cpp#20*



**brief** Имя самого конкретного класса.

### gaitTag

*public std::string gaitTag()*

*Defined at homeworks\Task12\Task12_1\cargo_quadruped_robot.cpp#24*



**brief** Подпись походки этого класса; СКРЫВАЕТ походку предков (не virtual).

### getCargoWeightKg

*public double getCargoWeightKg()*

*Defined at homeworks\Task12\Task12_1\cargo_quadruped_robot.h#41*



**brief** Масса груза в килограммах.



