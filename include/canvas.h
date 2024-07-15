#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QFont>
#include <QTimer>
#include <QSoundEffect>

class Canvas : public QWidget
{

public:
    Canvas(QWidget *parent = nullptr);
    void setConnectionStatus(bool connected);
    void setLeftArrow(bool on);
    void setRightArrow(bool on);
    void setWarning(bool on);
    void setTemperature(int temp);
    void setBatteryLevel(int level);
    void setSpeed(int newSpeed);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    bool isConnected;
    bool leftArrowOn;
    bool rightArrowOn;
    bool warningOn;
    bool warningVisible;
    bool leftArrowVisible;
    bool rightArrowVisible;
    int temperature;
    int batteryLevel;
    int speed;
    int leftArrowToggleCount;

    QFont iconFont;

    void toggleLeftArrowVisibility();
    void toggleRightArrowVisibility();
    void toggleWarning();
    void toggleLeftArrow();
    void toggleRightArrow();
    void drawSpeedometerArc(QPainter &painter, QPoint center, int radius);
    void drawSpeedometerLines(QPainter &painter, QPoint center, int radius);

    QTimer warningTimer;
    QTimer leftArrowTimer;
    QTimer rightArrowTimer;
    QTimer *arrowBlinkTimer;

    QPixmap batteryIcon;
    QSoundEffect soundEffect;
    QSoundEffect warningSoundEffect;

    void playSound();
    void stopSound();
    void updateSound();
};

#endif
