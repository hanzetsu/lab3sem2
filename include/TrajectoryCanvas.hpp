#pragma once

#include <QWidget>
#include <QTimer>
#include <QPainter>
#include "Constants.hpp"

class TrajectoryCanvas : public QWidget {
    Q_OBJECT
public:
    explicit TrajectoryCanvas(QWidget *parent = nullptr);

    void setTrajectory(double v0, double angleRad, double targetMin, double targetMax);
    void startAnimation();
    void stopAnimation();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void updateAnimation();

private:
    double m_v0 = 0.0;
    double m_angleRad = 0.0;
    double m_targetMin = 0.0;
    double m_targetMax = 0.0;
    double m_g = Constants::G;

    QTimer m_timer;
    double m_currentTime = 0.0;
    double m_flightTime = 0.0;
    bool m_animating = false;

    void calculateFlightTime();
    void drawAxes(QPainter &painter, int w, int h, double maxX, double maxY);
    void drawTargetArea(QPainter &painter, double maxX, double maxY, int originX, int originY, double scaleX, double scaleY);
    void drawTrajectory(QPainter &painter, double maxX, double maxY, int originX, int originY, double scaleX, double scaleY);
    void drawProjectile(QPainter &painter, double maxX, double maxY, int originX, int originY, double scaleX, double scaleY);
};