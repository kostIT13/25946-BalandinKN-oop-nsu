#pragma once
#include <iostream>
#include <string>

/// \brief Вершина иерархии: абстрактный класс «робот».
///
/// move() объявлена чистой виртуальной, поэтому класс нельзя создать напрямую
/// (Robot r; — ошибка компиляции): перемещение обязан описать каждый конкретный
/// потомок. Поля приватные, наследники видят их только через protected-часть.
class Robot {
private:
    std::string name;
    double batteryLevel;

protected:
    /// \brief Нормализация заряда в диапазон [0; 100] процентов.
    /// \param value заряд в процентах.
    /// \return приведённое значение.
    static double normalizeBattery(double value);

    /// \brief Строка состояния с именем самого конкретного класса.
    /// \param what текст о вызываемом действии.
    void logState(const std::string& what) const;

public:
    /// \brief Создание робота.
    /// \param name имя экземпляра, пустое имя недопустимо.
    /// \param battery заряд в процентах, нормализуется в [0; 100].
    /// \throws std::invalid_argument при пустом имени.
    explicit Robot(const std::string& name = "Unnamed", double battery = 100.0);

    virtual ~Robot();

    /// \brief Переместить робота на один шаг.
    ///
    /// Чистая виртуальная функция: реализации здесь нет, конкретный класс обязан
    /// определить свою.
    virtual void move() = 0;

    /// \brief Имя экземпляра.
    const std::string& getName() const { return name; }

    /// \brief Текущий заряд в процентах.
    double getBatteryLevel() const { return batteryLevel; }

    /// \brief Задать имя. \throws std::invalid_argument при пустом имени.
    void setName(const std::string& value);

    /// \brief Задать заряд (нормализуется в [0; 100]).
    void setBatteryLevel(double value);

    /// \brief Имя самого конкретного класса — для печатаемых сообщений.
    virtual std::string className() const;

    /// \brief Подпись походки. НЕ виртуальная — для контраста с move():
    /// вызов выбирается по статическому типу указателя, а не по объекту.
    std::string gaitTag() const;

    /// \brief Краткая строка состояния «Класс(имя, battery=…)».
    std::string statusLine() const;
};
