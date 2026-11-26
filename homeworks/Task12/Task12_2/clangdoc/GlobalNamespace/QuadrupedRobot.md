# class QuadrupedRobot

*Defined at homeworks\Task12\Task12_1\quadruped_robot.h#8*



**brief** Прямой наследник Robot: квадропедальный робот (четыре ноги).

 Первый конкретный класс иерархии — именно здесь появляется реализация move(), отсутствовавшая в абстрактном классе: шаг галопом с расходом заряда.



Inherits from Robot



## Members

private int gaitStrideCm

private double distanceCoveredKm



## Functions

### stepBatteryCost

*protected double stepBatteryCost()*

*Defined at homeworks\Task12\Task12_1\quadruped_robot.cpp#3*



**brief** Расход заряда за один шаг базового квадропеда.

**return** проценты, которые отнимаются у батареи.

### takeStep

*protected void takeStep(double cost)*

*Defined at homeworks\Task12\Task12_1\quadruped_robot.cpp#7*



**brief** Применить последствия шага: пройти дистанцию и разрядиться.

**cost** проценты заряда, затраченные на шаг.

### QuadrupedRobot

*public void QuadrupedRobot(const std::string & name, double battery, int gaitStrideCm)*

*Defined at homeworks\Task12\Task12_1\quadruped_robot.cpp#12*



**name** имя экземпляра.

**battery** заряд в процентах.

**gaitStrideCm** длина шага в сантиметрах.

### move

*public void move()*

*Defined at homeworks\Task12\Task12_1\quadruped_robot.cpp#16*



**brief** Шаг галопом: дистанция растёт, заряд падает на stepBatteryCost().

### className

*public std::string className()*

*Defined at homeworks\Task12\Task12_1\quadruped_robot.cpp#21*



**brief** Имя самого конкретного класса.

### gaitTag

*public std::string gaitTag()*

*Defined at homeworks\Task12\Task12_1\quadruped_robot.cpp#25*



**brief** Подпись походки этого класса; СКРЫВАЕТ Robot::gaitTag() (не virtual).

### getDistanceCoveredKm

*public double getDistanceCoveredKm()*

*Defined at homeworks\Task12\Task12_1\quadruped_robot.h#39*



**brief** Пройденная дистанция в километрах.

### getGaitStrideCm

*public int getGaitStrideCm()*

*Defined at homeworks\Task12\Task12_1\quadruped_robot.h#42*



**brief** Длина шага в сантиметрах.



