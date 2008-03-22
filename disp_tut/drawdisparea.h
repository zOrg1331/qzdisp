// Time-stamp: <drawdisparea.h - 23:09:20 14.12.2005>
/*****************************************************************
 *** Заголовочный файл виджета, позволяющего мышкой нарисовать ***
 ***                 функциональную зависимость                ***
 *****************************************************************/

/* code: Наконечный Павел <zorg1331@gmail.com> */

#ifndef DRAWDISPAREA_H
#define DRAWDISPAREA_H

#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>

class DrawDispArea : public QWidget
{
  Q_OBJECT

public:
  DrawDispArea(QWidget *parent = 0);
  int getYValue(int);

public slots:
  void clearImage();

protected:
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void paintEvent(QPaintEvent *event);
  void resizeEvent(QResizeEvent *event);

private:
  void drawLineTo(const QPoint &endPoint);
  void clearColumn(int point);
  void resizeImage(QImage *image, const QSize &newSize);

  bool scribbling;
  QImage image;
  QPoint lastPoint;
};

#endif
