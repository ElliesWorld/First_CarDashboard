#include "window.h"

Window::Window(QWidget *parent)
    : QDialog(parent)
{
    Canvas *canvas = new Canvas(this);
    canvas->resize(800, 600);
    canvas->show();
}
