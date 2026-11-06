# Задание 7 (docs/Задания OOP_07.pdf) — класс QuadrupedRobot

Каждая часть — самостоятельный cmake-проект. Сборка (пример):
```
cmake -S homeworks/Task7/Task7_1 -B homeworks/Task7/Task7_1/build -G "MinGW Makefiles"
cmake --build homeworks/Task7/Task7_1/build
```

| Часть | Папка | Что сделано |
|---|---|---|
| 7.I | `Task7_1` | Геттеры для всех информационных полей; const-корректность (все читающие методы const, геттеры возвращают const-ссылки/указатели). Объяснение — в `README.md` части. Демо: const-объект + const-методы |
| 7.II | `Task7_2` | Статические поля: `objectCount` (счётчик живых объектов), `uid` (уникальный числовой идентификатор, id = "R"+uid), `class_name` (имя класса). Все сообщения конструкторов/деструктора/`operator=` содержат текущее количество объектов, вывод с разделителями |
| 7.III | `Task7_3` | `robot_book.exe` — main из примера (Book), переписанный под наш класс (`print_count()`, `class_name`, `printRobot()`, `addDistance()`, `operator<<`); `robot_containers.exe` — объекты в vector/list/set + анализ вывода (`README.md`) |
| 7.IV | `Task7_4` | Функции нормализации генерируют исключения (`std::invalid_argument`, `std::out_of_range`); main ловит исключения и даёт повторно ввести данные вручную |
| 7.V | `Task7_5` | `operator<<` (полная информация: имя класса, uid, количество объектов) и `operator>>` (строго 4 параметра: name battery command distance, с нормализацией) |
| 7.VI | `Task7_6` | Шаблонный класс `QuadrupedRobotT<IdType, DistType>` — типы id и дистанций задаются шаблоном; определения методов вынесены за пределы класса. Демо: `<std::string,double>` и `<int,int>` |

Пример запуска части с вводом (7.4/7.5):
```
echo Spot 55 walk 12 | homeworks\Task7\Task7_5\build\robot.exe
```
