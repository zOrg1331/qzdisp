// Time-stamp: <glwidget5.h - 00:29:32 29.11.2005>
/*****************************************************************
 *** Заголовочный файл 3D виджета для 5-го урока по дисперсии  ***
 *****************************************************************/

/* code: Наконечный Павел <zorg1331@mail.ru> */

#ifndef GLWIDGET5_H
#define GLWIDGET5_H

#include <QGLWidget>

class GLWidget5 : public QGLWidget
{
  Q_OBJECT

public:
  GLWidget5(QWidget *parent = 0);
  ~GLWidget5();

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

protected:
  void paintGL();
  void resizeGL(int width, int height);
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void keyPressEvent(QKeyEvent *event);

private:
  typedef GLfloat point3[3];
  void normal(point3 x, point3 y, point3 z, point3 n);
  GLuint makeObject();
  GLuint list;
  void normalizeAngle(int *angle);
  void drawSand();
  /* GLUnurbsObj *pNurb; */

  GLuint object;
  int xRot;
  int yRot;
  int zRot;
  float trX, trY, trZ;
  QPoint lastPos;
  QColor trolltechGreen;
  QColor trolltechPurple;
  QColor mySand;
};

#endif
