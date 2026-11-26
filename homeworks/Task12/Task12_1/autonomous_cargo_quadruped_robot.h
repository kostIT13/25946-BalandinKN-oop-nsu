#pragma once
#include "cargo_quadruped_robot.h"

/// \brief Грузовой квадропед с автономным режимом — четвёртый класс цепочки.
///
/// Замыкает последовательную иерархию из четырёх классов (без множественного
/// наследования): Robot -> QuadrupedRobot -> CargoQuadrupedRobot ->
/// AutonomousCargoQuadrupedRobot. Своя реализация move(): робот едет маршем без
/// внешней команды, пока остаток автономного запаса хода не кончится.
class AutonomousCargoQuadrupedRobot : public CargoQuadrupedRobot {
private:
    double autonomousRangeM;

protected:
    /// \brief Расход за автономный шаг: марш расходует меньше галопа с грузом.
    double stepBatteryCost() const override;

public:
    /// \param name имя экземпляра.
    /// \param battery заряд в процентах.
    /// \param cargoWeightKg масса груза в килограммах.
    /// \param autonomousRangeM автономный запас хода в метрах.
    explicit AutonomousCargoQuadrupedRobot(
        const std::string& name = "Unnamed-Auto-Cargo-Quad",
        double battery = 100.0, double cargoWeightKg = 0.0,
        double autonomousRangeM = 0.0);

    /// \brief Автономный марш: своя реализация move() для этого класса.
    ///
    /// Пока автономный запас хода не исчерпан, робот делает шаг без внешней
    /// команды и тратит меньше заряда; после исчерпания — стоит и об этом
    /// сообщает.
    void move() override;

    /// \brief Имя самого конкретного класса.
    std::string className() const override;

    /// \brief Подпись походки этого класса; СКРЫВАЕТ походку предков (не virtual).
    std::string gaitTag() const;

    /// \brief Остаток автономного запаса хода в метрах.
    double getAutonomousRangeM() const { return autonomousRangeM; }
};
