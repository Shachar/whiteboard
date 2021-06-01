#ifndef SECONDARYWINDOW_H
#define SECONDARYWINDOW_H

#include "WhiteBoardWidget.h"

#include <QMainWindow>

namespace Ui {
class SecondaryWindow;
}

class SecondaryWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit SecondaryWindow(QWidget *parent, WhiteBoardWidget *board);

signals:

public slots:
    void toggleFullScreen();
    void exitFullScreen();

private:
    void unimportantWidgetsVisibility(bool show);

    Ui::SecondaryWindow *_ui = nullptr;
};

#endif // SECONDARYWINDOW_H
