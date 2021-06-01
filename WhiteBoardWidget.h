#ifndef WHITEBOARDWIDGET_H
#define WHITEBOARDWIDGET_H

#include "BoardBackground.h"

#include <QWidget>

class WhiteBoardWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WhiteBoardWidget(QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    void tabletEvent(QTabletEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

    QColor getPenColor() const;
    qreal getPenSize() const {
        return penWidth;
    }

    void drawOnMe( QPainter &painter, QRect regionOfInterest );

signals:
    void imageUpdated();
    void penSizeChanged(qreal penSize);

public slots:
    void clearBoard();
    void clearDrawing();
    void newBackground(QPixmap pixmap);
    void newBackground(BoardBackground background);
    void setPenColor(QColor color);

private:
    enum class DrawType {
        Pen,
        Highlighter,
        Eraser,
    };
    void internalClearBackground(QSize size);

    void draw(QPointF pos, qreal pressure, DrawType drawType);
    void drawingChanged();

    QPixmap underlyingImage, backgroundImage;
    qreal penWidth = 2;
    QColor penColor = Qt::black;
    QPointF lastPoint;
    BoardBackground backgroundType;
};

#endif // WHITEBOARDWIDGET_H
