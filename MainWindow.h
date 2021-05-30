#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
    void setPenColor();
    void backgroundType();

private:
    void unimportantWidgetsVisibility(bool show);
    void adoptMenuActions(QAction *action);
    void updateStatusBar();

    Ui::MainWindow *ui = nullptr;
    QLabel *statusPenSize = nullptr;
    QLabel *statusPenColor = nullptr;
};

#endif // MAINWINDOW_H
