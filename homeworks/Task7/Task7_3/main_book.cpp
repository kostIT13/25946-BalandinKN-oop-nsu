// Задание 7.III п.1-2: main из примера (класс Book), модифицированный
// под класс QuadrupedRobot. Структура примера сохранена один в один:
//   Book::print_count()          -> QuadrupedRobot::print_count()
//   Book::class_name             -> QuadrupedRobot::class_name
//   Book my_book("author", ...)  -> конструктор с параметрами
//   my_book.printBook()          -> my_robot.printRobot()
//   my_book.add_bookmark(n)      -> my_robot.addDistance(d)
//   std::cout << my_book         -> std::cout << my_robot
//
// Соответствие количества объектов и переменных:
//   my_first_robot  - объект #1 (живёт до конца main)
//   my_second_robot - объект #2 (копия #1, живёт до конца main)
//   во внутреннем блоке {} - объект #3 (копия #1, уничтожается в конце блока)
//   итого в разные моменты: 3 создано, 3 уничтожено (счётчик это показывает)
#include "robot.h"
#include <iostream>

int main() {
    std::cout << "static values: " << std::endl;
    QuadrupedRobot::print_count();
    std::cout << QuadrupedRobot::class_name << std::endl;
    std::cout << std::endl;

    std::cout << "Robot creation: " << std::endl;
    double arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    QuadrupedRobot my_first_robot("R1", "Spot", 80.0, "walk", 5.0, arr);
    my_first_robot.printRobot();
    QuadrupedRobot::print_count();
    std::cout << my_first_robot << std::endl << std::endl;

    std::cout << "Second robot: " << std::endl;
    QuadrupedRobot my_second_robot{my_first_robot};   // конструктор копирования
    std::cout << my_second_robot << std::endl;
    my_second_robot.addDistance(2.0);                 // меняет только копию
    my_first_robot.addDistance(7.0);                  // меняет только оригинал
    my_second_robot.printRobot();

    {
        std::cout << " inside {} " << std::endl;
        QuadrupedRobot my_third_robot{my_first_robot}; // ещё одна копия
        std::cout << my_third_robot << std::endl;
        my_third_robot.addDistance(3.0);
        my_first_robot.addDistance(4.0);
        my_third_robot.printRobot();
    }   // деструктор третьего робота (счётчик уменьшится)

    std::cout << " outside {} " << std::endl;
    my_second_robot.printRobot();   // его данные не изменились из блока {}
    QuadrupedRobot::print_count();

    return 0;
}
