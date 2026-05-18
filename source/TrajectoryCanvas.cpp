#include "TrajectoryCanvas.hpp"
#include <cmath>
#include <QPainter>
#include <QPen>
#include <QBrush>

TrajectoryCanvas::TrajectoryCanvas(QWidget *parent) : QWidget(parent) {
    setMinimumHeight(300);
    connect(&m_timer, &QTimer::timeout, this, &TrajectoryCanvas::updateAnimation);
}

void TrajectoryCanvas::setTrajectory(double v0, double angleRad, double targetMin, double targetMax) {
    m_v0 = v0;
    m_angleRad = angleRad;
    m_targetMin = targetMin;
    m_targetMax = targetMax;
    calculateFlightTime();
    stopAnimation();
    update();
}

void TrajectoryCanvas::calculateFlightTime() {
    double vy = m_v0 * std::sin(m_angleRad);
    m_flightTime = 2.0 * vy / m_g;
    if (m_flightTime < 0) m_flightTime = 0;
}

void TrajectoryCanvas::startAnimation() {
    if (m_flightTime <= 0) return;
    m_currentTime = 0.0;
    m_animating = true;
    m_timer.start(50);
}

void TrajectoryCanvas::stopAnimation() {
    m_animating = false;
    m_timer.stop();
    update();
}

void TrajectoryCanvas::updateAnimation() {
    if (!m_animating) return;
    m_currentTime += 0.05;
    if (m_currentTime >= m_flightTime) {
        m_currentTime = m_flightTime;
        stopAnimation();
    }
    update();
}

void TrajectoryCanvas::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int w = width();
    int h = height();

    const double maxX = 100.0;
    const double maxY = 30.0;

    int originX = 50;
    int originY = h - 50;
    double scaleX = (w - 100) / maxX;
    double scaleY = (h - 100) / maxY;

    drawAxes(painter, w, h, maxX, maxY);
    drawTargetArea(painter, maxX, maxY, originX, originY, scaleX, scaleY);
    drawTrajectory(painter, maxX, maxY, originX, originY, scaleX, scaleY);
    drawProjectile(painter, maxX, maxY, originX, originY, scaleX, scaleY);
}

void TrajectoryCanvas::drawAxes(QPainter &painter, int w, int h, double maxX, double maxY) {
    painter.setPen(QPen(Qt::black, 1));
    int originX = 50;
    int originY = h - 50;
    painter.drawLine(originX, originY, w - 20, originY);
    painter.drawLine(originX, 20, originX, originY);
    painter.drawText(w - 30, originY + 15, "X (м)");
    painter.drawText(originX - 15, 20, "Y (м)");

    for (int x = 0; x <= maxX; x += 10) {
        int px = originX + int(x * (w - 100) / maxX);
        painter.drawLine(px, originY - 3, px, originY + 3);
        painter.drawText(px - 5, originY + 15, QString::number(x));
    }
    for (int y = 0; y <= maxY; y += 5) {
        int py = originY - int(y * (h - 100) / maxY);
        painter.drawLine(originX - 3, py, originX + 3, py);
        painter.drawText(originX - 25, py + 3, QString::number(y));
    }
}

void TrajectoryCanvas::drawTargetArea(QPainter &painter, double maxX, double maxY, int originX, int originY, double scaleX, double scaleY) {
    if (m_targetMin >= m_targetMax) return;
    double targetMin = m_targetMin;
    double targetMax = m_targetMax;
    if (targetMin < 0) targetMin = 0;
    if (targetMax > maxX) targetMax = maxX;

    int xMin = originX + static_cast<int>(targetMin * scaleX);
    int xMax = originX + static_cast<int>(targetMax * scaleX);
    int groundY = originY;

    painter.fillRect(xMin, groundY - 5, xMax - xMin, 10, QBrush(QColor(255, 200, 200)));
    painter.setPen(QPen(Qt::red, 2));
    painter.drawLine(xMin, groundY, xMin, groundY - 8);
    painter.drawLine(xMax, groundY, xMax, groundY - 8);
}

void TrajectoryCanvas::drawTrajectory(QPainter &painter, double maxX, double maxY, int originX, int originY, double scaleX, double scaleY) {
    if (m_v0 == 0) return;
    QVector<QPointF> points;
    double vx = m_v0 * std::cos(m_angleRad);
    double vy = m_v0 * std::sin(m_angleRad);
    int steps = 200;
    for (int i = 0; i <= steps; ++i) {
        double t = m_flightTime * i / steps;
        double x = vx * t;
        double y = vy * t - 0.5 * m_g * t * t;
        if (y < 0) y = 0;
        if (x > maxX) break;
        int px = originX + static_cast<int>(x * scaleX);
        int py = originY - static_cast<int>(y * scaleY);
        points.append(QPointF(px, py));
    }
    painter.setPen(QPen(Qt::blue, 2));
    for (int i = 0; i < points.size() - 1; ++i)
        painter.drawLine(points[i].toPoint(), points[i+1].toPoint());
}

void TrajectoryCanvas::drawProjectile(QPainter &painter, double maxX, double maxY, int originX, int originY, double scaleX, double scaleY) {
    if (!m_animating && m_currentTime == 0) return;
    double vx = m_v0 * std::cos(m_angleRad);
    double vy = m_v0 * std::sin(m_angleRad);
    double x = vx * m_currentTime;
    double y = vy * m_currentTime - 0.5 * m_g * m_currentTime * m_currentTime;
    if (y < 0) y = 0;
    if (x > maxX) return;
    int px = originX + static_cast<int>(x * scaleX);
    int py = originY - static_cast<int>(y * scaleY);

    painter.setBrush(QBrush(Qt::red));
    painter.setPen(QPen(Qt::darkRed, 1));
    painter.drawEllipse(px - 5, py - 5, 10, 10);
}