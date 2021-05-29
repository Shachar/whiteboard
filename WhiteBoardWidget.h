#ifndef WHITEBOARDWIDGET_H
#define WHITEBOARDWIDGET_H

#include <QWidget>

class WhiteBoardWidget : public QWidget
{
    Q_OBJECT

    QPixmap underlyingImage, background;
    QPointF lastPoint;
public:
    explicit WhiteBoardWidget(QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    void tabletEvent(QTabletEvent *event) override;
signals:

public slots:
    void clearBoard();
    void clearDrawing();
    void newBackground(QPixmap pixmap);

private:
    void internalClearBackground(QSize size);

    void draw(QPointF pos, qreal pressure);
};

#endif // WHITEBOARDWIDGET_H
