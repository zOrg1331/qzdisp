// Time-stamp: <drawdisparea.cpp - 22:42:07 14.12.2005>
/*************************************************************
 ***          Виджет предоставляет возможность             ***
 ***      рисовать мышкой функциональную зависимость       ***
 *************************************************************/

/* code: Наконечный Павел <zorg1331@gmail.com> */

#include <QtGui>

#include "drawdisparea.h"

DrawDispArea::DrawDispArea(QWidget *parent)
  : QWidget(parent)
{
  setAttribute(Qt::WA_StaticContents);
  setMinimumSize(500, 150);
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  scribbling = false;
}

void DrawDispArea::clearImage()
{
  image.fill(qRgb(204, 204, 204));
  update();
}

void DrawDispArea::mousePressEvent(QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton) {
    lastPoint = event->pos();
    scribbling = true;
  }
}

void DrawDispArea::mouseMoveEvent(QMouseEvent *event)
{
  if ((event->buttons() & Qt::LeftButton) && scribbling) {
    clearColumn(event->x());
    drawLineTo(event->pos());
  }
}

void DrawDispArea::mouseReleaseEvent(QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton && scribbling) {
    drawLineTo(event->pos());
    scribbling = false;
  }
}

void DrawDispArea::paintEvent(QPaintEvent * /* event */)
{
  QPainter painter(this);
  painter.drawImage(QPoint(0, 0), image);
}

void DrawDispArea::resizeEvent(QResizeEvent *event)
{
  if (width() > image.width() || height() > image.height()) {
    int newWidth = qMax(width() + 128, image.width());
    int newHeight = qMax(height() + 128, image.height());
    resizeImage(&image, QSize(newWidth, newHeight));
    update();
  }
  QWidget::resizeEvent(event);
}

void DrawDispArea::clearColumn(int point)
{
  QPainter painter(&image);
  painter.setPen(QPen(QColor(204, 204, 204), 2, Qt::SolidLine, Qt::RoundCap,
		      Qt::RoundJoin));
  painter.drawLine(point, 0, point, height());

  int rad = 2 / 2;
  update(QRect(point, 0, point, height()).normalized()
	 .adjusted(-rad, -rad, +rad, +rad));
}

void DrawDispArea::drawLineTo(const QPoint &endPoint)
{
  QPainter painter(&image);
  painter.setPen(QPen(Qt::blue, 2, Qt::SolidLine, Qt::RoundCap,
		      Qt::RoundJoin));
  painter.drawLine(lastPoint, endPoint);

  int rad = 2 / 2;
  update(QRect(lastPoint, endPoint).normalized()
	 .adjusted(-rad, -rad, +rad, +rad));
  lastPoint = endPoint;
}

void DrawDispArea::resizeImage(QImage *image, const QSize &newSize)
{
  if (image->size() == newSize)
    return;

  QImage newImage(newSize, QImage::Format_RGB32);
  newImage.fill(qRgb(204, 204, 204));
  QPainter painter(&newImage);
  painter.drawImage(QPoint(0, 0), *image);
  *image = newImage;
}

int DrawDispArea::getYValue(int x)
{
 for (int y = 0; y <= height(); y++) {
    uint *ptr = reinterpret_cast<uint *>(image.scanLine(y)) + x;
    if (*ptr == qRgb(0, 0, 255)) {
      return y;
    }
  }
}
