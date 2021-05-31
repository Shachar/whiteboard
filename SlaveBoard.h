#ifndef SLAVEBOARD_H
#define SLAVEBOARD_H

#include "WhiteBoardWidget.h"

#include <QWidget>

class SlaveBoard : public QWidget
{
    Q_OBJECT
public:
    explicit SlaveBoard(QWidget *parent = nullptr);

    void setMaster(WhiteBoardWidget *master);

    void paintEvent(QPaintEvent *event) override;
signals:

public slots:
    void refreshImage();

private:
    WhiteBoardWidget *_master = nullptr;
};

#endif // SLAVEBOARD_H
