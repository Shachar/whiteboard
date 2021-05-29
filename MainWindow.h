#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private:
    void unimportantWidgetsVisibility(bool show);
    void adoptMenuActions(QAction *action);

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
