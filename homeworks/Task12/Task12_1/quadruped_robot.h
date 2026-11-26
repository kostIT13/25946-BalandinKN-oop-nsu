#pragma once
#include "robot.h"

/// \brief Прямой наследник Robot: квадропедальный робот (четыре ноги).
///
/// Первый конкретный класс иерархии — именно здесь появляется реализация
/// move(), отсутствовавшая в абстрактном классе: шаг галопом с расходом заряда.
class QuadrupedRobot : public Robot {
private:
    int gaitStrideCm;
    double distanceCoveredKm;

protected:
    /// \brief Расход заряда за один шаг базового квадропеда.
    /// \return проценты, которые отнимаются у батареи.
    virtual double stepBatteryCost() const;

    /// \brief Применить последствия шага: пройти дистанцию и разрядиться.
    /// \param cost проценты заряда, затраченные на шаг.
    void takeStep(double cost);

public:
    /// \param name имя экземпляра.
    /// \param battery заряд в процентах.
    /// \param gaitStrideCm длина шага в сантиметрах.
    explicit QuadrupedRobot(const std::string& name = "Unnamed-Quad",
                            double battery = 100.0, int gaitStrideCm = 60);

    /// \brief Шаг галопом: дистанция растёт, заряд падает на stepBatteryCost().
    void move() override;

    /// \brief Имя самого конкретного класса.
    std::string className() const override;

    /// \brief Подпись походки этого класса; СКРЫВАЕТ Robot::gaitTag() (не virtual).
    std::string gaitTag() const;

    /// \brief Пройденная дистанция в километрах.
    double getDistanceCoveredKm() const { return distanceCoveredKm; }

    /// \brief Длина шага в сантиметрах.
    int getGaitStrideCm() const { return gaitStrideCm; }
};
