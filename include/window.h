#ifndef WINDOW_H
#define WINDOW_H

#include <QDialog>
#include "canvas.h"

class Window : public QDialog
{
public:
    Window(QWidget *parent = nullptr);
};

#endif
