#ifndef WHITEBOARDWIDGET_H
#define WHITEBOARDWIDGET_H

#include <QWidget>

class WhiteBoardWidget : public QWidget
{
    Q_OBJECT

    QPixmap underlyingImage, background;
    QPoint lastPoint;
public:
    explicit WhiteBoardWidget(QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
signals:

public slots:
    void clearBoard();
};

#endif // WHITEBOARDWIDGET_H
