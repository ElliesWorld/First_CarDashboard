#include "canvas.h"
#include <QPainter>
#include <QRectF>
#include <QPen>
#include <QLinearGradient>
#include <cmath>
#include <QDir>
#include <QtMultimedia/QSoundEffect>

Canvas::Canvas(QWidget *parent)
    : QWidget(parent),
      isConnected(false),    // True to test
      leftArrowOn(false),    // This controls the left turn signal
      rightArrowOn(false),   // This controls the right turn signal
      warningOn(false),      // This controls the warning signal
      warningVisible(false), // This is the warning symbol
      leftArrowVisible(false),
      rightArrowVisible(false),
      temperature(0),
      batteryLevel(50),
      speed(0),
      batteryIcon(QDir::homePath() + "/res/battery_0_bar.png")

{
    setWindowTitle("Client");

    // Connect left and right arrow timers
    leftArrowTimer.setInterval(250); // Blink every 250 ms
    connect(&leftArrowTimer, &QTimer::timeout, this, &Canvas::toggleLeftArrowVisibility);
    leftArrowTimer.start(); // Start the timer

    rightArrowTimer.setInterval(250); // Blink every 250 ms
    connect(&rightArrowTimer, &QTimer::timeout, this, &Canvas::toggleRightArrowVisibility);
    rightArrowTimer.start(); // Start the timer

    // Initialize sound effect
    QString soundPath = QDir::currentPath() + "/../res/car-blinker.wav";
    QString warningSoundPath = QDir::currentPath() + "/../res/squeaky.wav";

    soundEffect.setSource(QUrl::fromLocalFile(soundPath));
    soundEffect.setVolume(0.5f);

    warningSoundEffect.setSource(QUrl::fromLocalFile(warningSoundPath));
    warningSoundEffect.setVolume(0.5f);
}

void Canvas::setLeftArrow(bool value)
{
    leftArrowOn = value;
    update();
}

void Canvas::setRightArrow(bool value)
{
    rightArrowOn = value;
    update();
}

void Canvas::setWarning(bool value)
{
    warningOn = value;
    warningVisible = value;
    update();
}

void Canvas::setTemperature(int temp)
{
    temperature = temp;
    update();
}

void Canvas::playSound()
{
    if (warningOn)
    {
        if (warningSoundEffect.isPlaying())
        {
            warningSoundEffect.stop();
        }
        warningSoundEffect.play();
    }
    else
    {
        if (soundEffect.isPlaying())
        {
            soundEffect.stop();
        }
        soundEffect.play();
    }
}

void Canvas::stopSound()
{
    if (soundEffect.isPlaying())
    {
        soundEffect.stop();
    }
    if (warningSoundEffect.isPlaying())
    {
        warningSoundEffect.stop();
    }
}
void Canvas::updateSound()
{
    if (leftArrowVisible || rightArrowVisible || warningOn)
    {
        playSound();
    }
    else
    {
        stopSound();
    }
}

void Canvas::setSpeed(int newSpeed)
{
    speed = newSpeed;
    update(); // Trigger repaint
}

void Canvas::setBatteryLevel(int level)
{
    batteryLevel = level;
    update();
}

void Canvas::setConnectionStatus(bool connected)
{
    isConnected = connected;
    update();
}

// WARNING
void Canvas::toggleWarning()
{
    if (warningOn)
    {
        toggleLeftArrowVisibility();
        toggleRightArrowVisibility();
    }
    updateSound();
    update();
}

// LEFT & RIGHT ARROWS
void Canvas::toggleLeftArrowVisibility()
{
    if ((isConnected && !warningOn && leftArrowOn) || (warningOn))
    {
        leftArrowVisible = !leftArrowVisible;
    }
    else
    {
        leftArrowVisible = false;
    }

    leftArrowTimer.stop();
    leftArrowTimer.start(500);

    updateSound();
    update();
}

void Canvas::toggleRightArrowVisibility()
{
    if ((isConnected && !warningOn && rightArrowOn) || (warningOn))
    {
        rightArrowVisible = !rightArrowVisible;
    }
    else
    {
        rightArrowVisible = false;
    }

    rightArrowTimer.stop();
    rightArrowTimer.start(500);

    updateSound();
    update();
}

void Canvas::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);

    // Set the BACKGROUND color
    painter.fillRect(rect(), QColor(30, 0, 30));

    // Apply the new BACKGROUND gradient
    QLinearGradient gradient(0, 0, width(), height());
    gradient.setColorAt(0.0, QColor(0, 0, 230));
    gradient.setColorAt(1.0, QColor(0, 0, 0));
    painter.fillRect(rect(), gradient);

    // Get the size of the widget
    int widgetWidth = width();
    int widgetHeight = height();

    // Define the size of the ARC
    int arcWidth = 500;
    int arcHeight = 500;

    // Calculate the center of the ARC
    QPointF center(widgetWidth / 2, widgetHeight / 2 + 50);

    // Define the rectangle for the ARC
    QRectF rectangle(center.x() - arcWidth / 2, center.y() - arcHeight / 2, arcWidth, arcHeight);

    // Adjust the start and span angles to rotate the ARC
    int startAngle = 330 * 16;
    int spanAngle = 240 * 16;

    // Set a pen for drawing the ARC
    QPen pen(Qt::white);
    pen.setWidth(8); // The width of the pen
    painter.setPen(pen);

    painter.drawArc(rectangle, startAngle, spanAngle);

    // Draw the white DOT at the center of the arc
    QBrush whiteBrush(Qt::gray);
    painter.setBrush(whiteBrush);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(center, 10, 10);

    // Draw the NEEDLE of the speedometer
    QPen needlePen(QColor(0, 128, 128));
    // QPen needlePen(Qt::gray);
    needlePen.setWidth(6);
    painter.setPen(needlePen);

    // Calculate the angle based on the speed (0-240)
    double angle = 210 - (speed / 240.0) * 240;
    double radian = angle * M_PI / 180.0; // Convert the angle to radians

    // TESTING FROM HERE:
    //  Calculate the position of the needle's tip
    int needleLength = 185;
    int needleTipX = center.x() + needleLength * std::cos(radian);
    int needleTipY = center.y() - needleLength * std::sin(radian);

    // Calculate the starting point of the needle closer to the center of the dot
    int needleBaseX = center.x() + 0 * std::cos(radian);
    int needleBaseY = center.y() + 5 * std::sin(radian);

    // Draw the needle with a triangle shape at the tip
    QPolygonF needlePolygon;
    needlePolygon << QPointF(needleBaseX - 3, needleBaseY - 3); // Left corner of the needle base
    needlePolygon << QPointF(needleBaseX + 3, needleBaseY - 3); // Right corner of the needle base
    needlePolygon << QPointF(needleTipX, needleTipY);
    painter.drawPolygon(needlePolygon);

    // Load icon font
    QFont iconFont("FontAwesome");
    iconFont.setPixelSize(50);
    painter.setFont(iconFont);

    // TO HERE

    // Draw the NEEDLE at the calculated angle
    // painter.drawLine(center, QPointF(center.x() + 200 * std::cos(radian), center.y() - 200 * std::sin(radian)));

    // Load icon font
    QFont largeIconFont = iconFont;
    largeIconFont.setPointSize(50); // Icons size

    painter.setFont(largeIconFont);

    // Draw icons
    QPen iconPen(Qt::green);
    painter.setPen(iconPen);

    // LEFT Arrow icon

    if (leftArrowVisible)
    {
        painter.drawText(QRectF(rectangle.left() - 100, rectangle.top() - 80, 100, 80), Qt::AlignCenter, QChar(0xe5c4));
    }

    // RIGHT Arrow icon

    if (rightArrowVisible)
    {
        painter.drawText(QRectF(rectangle.right(), rectangle.top() - 80, 100, 80), Qt::AlignCenter, QChar(0xe5c8));
    }

    // THERMOSTAT. Set the color for the thermometer icon based on the temperature
    if (temperature < 5)
    {
        iconPen.setColor(Qt::white);
    }
    else if (temperature <= 39)
    {
        iconPen.setColor(Qt::blue);
    }
    else
    {
        iconPen.setColor(Qt::red);
    }

    painter.setPen(iconPen);

    // THERMOSTAT icon
    painter.drawText(QRectF(rectangle.right() + 20, rectangle.bottom() - 180, 100, 80), Qt::AlignCenter, QChar(0xe1ff));

    // Draw the TEMPERATURE text
    QFont tempFont("Arial", 12); // Set the font for the temperature text
    painter.setFont(tempFont);
    painter.drawText(QRectF(rectangle.right() + 20, rectangle.bottom() - 120, 100, 40), Qt::AlignCenter, QString::number(temperature) + " °C");

    // Check CONNECTION status to draw the appropriate icon and text
    if (isConnected)
    {
        // SPEED icon size (added to enlarge speed icon)
        QFont speedFont = iconFont;
        speedFont.setPointSize(50); // Ensure the font size is consistent with other icons
        painter.setFont(speedFont);

        // SPEED icon
        painter.setPen(Qt::white);
        painter.drawText(QRectF(center.x() - 40, rectangle.bottom() - 180, 80, 80), Qt::AlignCenter, QChar(0xe9e4));

        // SPEED text
        painter.setFont(QFont("Arial", 18));
        painter.drawText(QRectF(center.x() - 50, rectangle.bottom() - 120, 100, 40), Qt::AlignCenter, QString::number(speed) + " km/h");
    }
    else
    {
        // Connection ERROR icon
        QFont errorFont = iconFont;
        errorFont.setPointSize(50); // Ensure the font size is consistent with other icons
        painter.setFont(errorFont);

        QPen errorPen(Qt::red);
        painter.setPen(errorPen);
        painter.drawText(QRectF(center.x() - 40, rectangle.bottom() - 180, 80, 80), Qt::AlignCenter, QChar(0xe9e4));

        // Connection ERROR text
        QFont errorTextFont("Arial", 20); // Use a larger font for the connection error text if needed
        painter.setFont(errorTextFont);
        painter.drawText(QRectF(center.x() - 100, rectangle.bottom() - 120, 200, 40), Qt::AlignCenter, "Connection Error");
    }

    QFont batteryTextFont("Arial", 12); // Set the font for the BATTERY level text
    painter.setFont(batteryTextFont);
    QPen originalPen = painter.pen();
    QPen textPen(Qt::white);
    painter.setPen(textPen);
    painter.drawText(QRectF(rectangle.right() + 30, rectangle.bottom() - 220, 85, 40), Qt::AlignCenter, QString::number(batteryLevel) + "%");
    painter.setPen(originalPen);

    // Draw the BATTERY icon and level
    if (batteryLevel < 25)
    {
        iconPen.setColor(Qt::red);
    }
    else if (batteryLevel <= 49)
    {
        iconPen.setColor(Qt::yellow);
    }
    else
    {
        iconPen.setColor(Qt::green);
    }

    painter.setPen(iconPen);
    // BATTERY size
    QFont batteryIconFont = iconFont;
    batteryIconFont.setPointSize(70);
    painter.setFont(batteryIconFont);

    // Adjust the BATTERY icon position above the thermostat icon
    QRectF batteryRect(rectangle.right() + 20, rectangle.bottom() - 380, 100, 250);
    painter.drawText(batteryRect, Qt::AlignCenter, QChar(0xebdc));

    // Draw the BATTERY level rectangle inside the battery icon
    int batteryIconHeight = 60; // Assuming the battery icon height is 50 pixels
    int batteryIconWidth = 35;  // Assuming the battery icon width is 40 pixels
    int batteryLevelHeight = batteryIconHeight * batteryLevel / 100;
    QRectF batteryLevelRect(batteryRect.left() + 32, batteryRect.bottom() - 100 - batteryLevelHeight, batteryIconWidth, batteryLevelHeight);
    painter.fillRect(batteryLevelRect, iconPen.color());

    // Draw the battery icon
    int iconWidth = batteryIcon.width();
    int iconHeight = batteryIcon.height();
    painter.drawPixmap(20, 20, batteryIcon);

    // Draw the SPEEDOMETER numbers
    drawSpeedometerArc(painter, center.toPoint(), arcWidth / 2);

    // Draw the lines inside the arc
    drawSpeedometerLines(painter, center.toPoint(), arcWidth / 2);
}

void Canvas::drawSpeedometerArc(QPainter &painter, QPoint center, int radius)
{
    QPen pen(Qt::white, 6); // White pen with width 6
    painter.setPen(pen);    // Set the pen for the painter

    QFont font("Arial", 15);   // Set the font for the speed numbers
    painter.setFont(font);     // Apply the font
    painter.setPen(Qt::white); // Set the pen color to white

    // Adjust the angles to correctly place the numbers
    for (int i = 0; i <= 12; ++i)
    {
        int speed = i * 20;
        double angle = 210 - (i / 12.0) * 240;                                                    // Calculate the angle for each speed number in the opposite direction
        double radian = angle * M_PI / 180.0;                                                     // Convert the angle to radians
        int x = center.x() + (radius - 45) * cos(radian);                                         // Calculate the x position
        int y = center.y() - (radius - 45) * sin(radian);                                         // Calculate the y position
        painter.drawText(QRect(x - 15, y - 10, 30, 20), Qt::AlignCenter, QString::number(speed)); // Draw the speed number
    }
}

void Canvas::drawSpeedometerLines(QPainter &painter, QPoint center, int radius)
{
    // Draw the main lines
    QPen pen(Qt::white, 4); // White pen with width 4
    painter.setPen(pen);    // Set the pen for the painter

    for (int i = 0; i <= 12; ++i)
    {
        double angle = 210 - (i / 12.0) * 240; // Calculate the angle for each line in the opposite direction
        double radian = angle * M_PI / 180.0;  // Convert the angle to radians

        int xStart = center.x() + (radius - 25) * cos(radian); // Adjust the inward starting point (original: radius - 20)
        int yStart = center.y() - (radius - 25) * sin(radian); // Adjust the inward starting point (original: radius - 20)
        int xEnd = center.x() + (radius - 10) * cos(radian);   // Adjusted inward end point
        int yEnd = center.y() - (radius - 10) * sin(radian);   // Adjusted inward end point

        painter.drawLine(xStart, yStart, xEnd, yEnd); // Draw the line
    }

    // Draw the small lines
    QPen smallPen(Qt::white, 2); // White pen with width 2 for smaller lines
    QPen boldPen(Qt::white, 4);  // White pen with width 4 for the middle line
    painter.setPen(smallPen);    // Start with the smaller pen

    for (int i = 0; i <= 48; ++i) // 48 small lines in total
    {
        if (i % 4 != 0) // Draw only the lines not at the positions of the main lines
        {
            double angle = 210 - (i / 48.0) * 240; // Calculate the angle for each small line in the opposite direction
            double radian = angle * M_PI / 180.0;  // Convert the angle to radians

            int xStart = center.x() + (radius - 20) * cos(radian); // Adjust the inward starting point for smaller lines (original: radius - 15)
            int yStart = center.y() - (radius - 20) * sin(radian); // Adjust the inward starting point for smaller lines (original: radius - 15)
            int xEnd = center.x() + (radius - 10) * cos(radian);   // Adjusted inward end point for smaller lines
            int yEnd = center.y() - (radius - 10) * sin(radian);   // Adjusted inward end point for smaller lines

            // Check if this is the middle line (every third line)
            if (i % 2 == 0)
                painter.setPen(boldPen); // Use the bold pen for the middle line
            else
                painter.setPen(smallPen); // Use the smaller pen for other lines

            painter.drawLine(xStart, yStart, xEnd, yEnd); // Draw the line
        }
    }
}
