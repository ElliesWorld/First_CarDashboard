#include <QApplication>
#include "window.h"
#include "canvas.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    Canvas canvas;
    canvas.show(); 

    // Simulate different scenarios based on user input

    canvas.setLeftArrow(false);         // Turn on/off the left arrow
    canvas.setRightArrow(false);        // Turn on/off the right arrow
    canvas.setWarning(false);           // Blink both arrows as a warning with a different warning sound + icon
    canvas.setConnectionStatus(true); // Simulate the connection 

    // Set temperature based on user input
    canvas.setTemperature(60); // Temp changes color, the temp is set to 60 degrees 

    // Set battery level based on user input
    canvas.setBatteryLevel(80); // Battery level will fill the battery, set to 80 currently

    // Set speed based on user input
    canvas.setSpeed(140); // Sets the needle to the speed of 140 km/h

    return app.exec();
}
