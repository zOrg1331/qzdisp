// Time-stamp: <glwidget4.h - 23:37:53 14.12.2005>
/*****************************************************************
 *** Заголовочный файл 3D виджета для 4-го урока по дисперсии  ***
 *****************************************************************/

/* code: Наконечный Павел <zorg1331@gmail.com> */

#ifndef GLWIDGET4_H
#define GLWIDGET4_H

#include <QGLWidget>

class GLWidget4 : public QGLWidget
{
  Q_OBJECT

public:
  GLWidget4(QWidget *parent = 0);
  ~GLWidget4();

  QSize minimumSizeHint() const;
  QSize sizeHint() const;
  int xRotation() const { return xRot; }
  int yRotation() const { return yRot; }
  int zRotation() const { return zRot; }

  void initializeGL();

public slots:
  void setXRotation(int angle);
  void setYRotation(int angle);
  void setZRotation(int angle);

signals:
  void xRotationChanged(int angle);
  void yRotationChanged(int angle);
  void zRotationChanged(int angle);
  void complete(double);

protected:
  void paintGL();
  void resizeGL(int width, int height);
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void keyPressEvent(QKeyEvent *event);

private:
  typedef GLfloat point3[3];
  void normal(point3 x, point3 y, point3 z, point3 n);
  GLuint list;
  GLuint makeObject();
  void normalizeAngle(int *angle);

  GLuint object;
  int xRot;
  int yRot;
  int zRot;
  double trX, trY, trZ, lightX, lightY;
  QPoint lastPos;
  QColor trolltechGreen;
  QColor trolltechPurple;
};

#endif
