#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "SecondaryWindow.h"

#include <QMainWindow>
#include <QLabel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void performPaste();
    void resetBoard();
    void toggleFullScreen();
    void exitFullScreen();
    void setPenColor();
    void backgroundType();
    void loadImage();
    //void saveImage();
    void extraWindow(bool enable);

private:
    void unimportantWidgetsVisibility(bool show);
    void updateStatusBar();

    Ui::MainWindow *ui = nullptr;
    QLabel *statusPenSize = nullptr;
    QLabel *statusPenColor = nullptr;
    SecondaryWindow *secondary = nullptr;
};

#endif // MAINWINDOW_H
