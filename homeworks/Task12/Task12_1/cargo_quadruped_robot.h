#pragma once
#include "quadruped_robot.h"

/// \brief Грузовой квадропедальный робот — прямой наследник QuadrupedRobot.
///
/// Своя реализация move(): тот же шаг, но расход заряда зависит от массы груза,
/// поэтому грузовой робот ходит «тяжелее» и дальше разряжается за тот же шаг.
class CargoQuadrupedRobot : public QuadrupedRobot {
private:
    double cargoWeightKg;
    int extraConsumptionPerHundredKg;

protected:
    /// \brief Расход заряда за шаг с учётом груза.
    ///
    /// Переопределение protected-хука базы: реализация move() из базы сама
    /// спрашивает стоимость шага у самого конкретного класса.
    /// \return базовый расход плюс надбавка за каждые 100 кг груза.
    double stepBatteryCost() const override;

public:
    /// \param name имя экземпляра.
    /// \param battery заряд в процентах.
    /// \param cargoWeightKg масса перевозимого груза в килограммах.
    /// \param extraConsumptionPerHundredKg дополнительная трата заряда за 100 кг.
    explicit CargoQuadrupedRobot(const std::string& name = "Unnamed-Cargo-Quad",
                                 double battery = 100.0,
                                 double cargoWeightKg = 0.0,
                                 int extraConsumptionPerHundredKg = 3);

    /// \brief Шаг с грузом: реализация move() этого класса.
    void move() override;

    /// \brief Имя самого конкретного класса.
    std::string className() const override;

    /// \brief Подпись походки этого класса; СКРЫВАЕТ походку предков (не virtual).
    std::string gaitTag() const;

    /// \brief Масса груза в килограммах.
    double getCargoWeightKg() const { return cargoWeightKg; }
};
