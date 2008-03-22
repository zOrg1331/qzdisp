// Time-stamp: <glwidget5.cpp - 09:15:26 12.04.2006>
/*****************************************************************
 ****      Реализация 3D виджета для 5-го урока по дисперсии  ****
 *****************************************************************/

/* code: Наконечный Павел <zorg1331@mail.ru> */

#include <QtGui>
#include <QtOpenGL>

#include <math.h>

#include "glwidget5.h"
#include "less3.h"
#include "less5.h"

GLWidget5::GLWidget5(QWidget *parent)
  : QGLWidget(parent)
{
  setFocusPolicy(Qt::StrongFocus);

  object = 0;
  xRot = -1000;
  yRot = 0;
  zRot = 0;
  trX = 0;
  trY= 0;
  trZ = -10.0;

  mySand = QColor::fromRgbF(0.8, 0.6, 0.2, 0.0);
}

GLWidget5::~GLWidget5()
{
  makeCurrent();
  glDeleteLists(object, 1);
  glDeleteLists(list, 1);
}

QSize GLWidget5::minimumSizeHint() const
{
  return QSize(50, 50);
}

QSize GLWidget5::sizeHint() const
{
  return QSize(400, 400);
}

void GLWidget5::setXRotation(int angle)
{
  normalizeAngle(&angle);
  if (angle != xRot) {
    xRot = angle;
    emit xRotationChanged(angle);
    updateGL();
  }
}

void GLWidget5::setYRotation(int angle)
{
  normalizeAngle(&angle);
  if (angle != yRot) {
    yRot = angle;
    emit yRotationChanged(angle);
    updateGL();
  }
}

void GLWidget5::setZRotation(int angle)
{
  normalizeAngle(&angle);
  if (angle != zRot) {
    zRot = angle;
    emit zRotationChanged(angle);
    updateGL();
  }
}

void GLWidget5::initializeGL()
{
  GLfloat light_ambient[] = {.8, .8, .8, .7};
  GLfloat light_diffuse[] = {.0, .0, .5, .7};
  GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};

  GLfloat material_ambient[] = {.15, .15, .35, .5};
  GLfloat material_diffuse[] = {.1, .1, .5, .5};
  GLfloat material_specular[] = {1.0, 1.0, 1.0, 1.0};

  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_ambient);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_specular);
  glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 40);

  qglClearColor(mySand);
  glColor3f(1.0, 1.0, 1.0);
  object = makeObject();
  glShadeModel(GL_FLAT);
  glEnable(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE);
  glEnable(GL_AUTO_NORMAL);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GLWidget5::paintGL()
{
  GLfloat light_position[] = {10.0, 10.0, -100.0, 1.0};
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  glTranslated(trX, trY, trZ);
  glRotated(xRot / 16.0, 1.0, 0.0, 0.0);
  glRotated(yRot / 16.0, 0.0, 1.0, 0.0);
  glRotated(zRot / 16.0, 0.0, 0.0, 1.0);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);

  drawSand();
  for (int i = 0; i < 1000; i++) {
    glCallList(object);
    glRotatef(360.0/1000.0, 0, 0, 1);
  }
}

void GLWidget5::resizeGL(int width, int height)
{
  int side = qMin(width, height);
  glViewport(0/*(width - side) / 4*/, (height - side) / 4, width/*side*/, side);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-1.0, 1.0, -1.0, 1.0, -30.0, 30.0);
  glMatrixMode(GL_MODELVIEW);
}

void GLWidget5::mousePressEvent(QMouseEvent *event)
{
  lastPos = event->pos();
}

void GLWidget5::mouseMoveEvent(QMouseEvent *event)
{
  int dx = event->x() - lastPos.x();
  int dy = event->y() - lastPos.y();

  if (event->buttons() & Qt::LeftButton) {
    setXRotation(xRot + 8 * dy);
    setYRotation(yRot + 8 * dx);
  } else if (event->buttons() & Qt::RightButton) {
    setXRotation(xRot + 8 * dy);
    setZRotation(zRot + 8 * dx);
  }
  lastPos = event->pos();
}

void GLWidget5::normal(point3 a, point3 b, point3 c, point3 n)
{
  n[0] = (b[1]-a[1])*(c[2]-a[2])-(b[2]-a[2])*(c[1]-a[1]);
  n[1] = (b[2]-a[2])*(c[0]-a[0])-(b[0]-a[0])*(c[2]-a[2]);
  n[2] = (b[0]-a[0])*(c[1]-a[1])-(b[1]-a[1])*(c[0]-a[0]);
  n[0] = -n[0];
  n[1] = -n[1];
  n[2] = -n[2];
}

void GLWidget5::drawSand()
{
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  // рисуем песочек
  GLfloat sand[] = {0.8, 0.6, 0.2, 1.0};
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, sand);

  glBegin(GL_QUADS);

  point3 a = {-3, 3, -Less5::depth};
  point3 b = {3, 3, -Less5::depth};
  point3 c = {3, -3, -Less5::depth};
  point3 d = {-3, -3, -Less5::depth};
  point3 n;
  normal(a, b, c, n);

  glDisable(GL_BLEND);

  glNormal3fv(n);
  glVertex3fv(a);
  glVertex3fv(b);
  glVertex3fv(c);
  glVertex3fv(d);

  glEnable(GL_BLEND);

  glEnd();
}

GLuint GLWidget5::makeObject()
{
  // Заполняем массив данными
  int accuracy = 300;
  double z[accuracy];
  for (int i = 0; i < accuracy; i++) z[i] = 0;
  double L = 3.0, dpth = Less5::depth, x = 0;
  int j = 0, N = Less3::N;
  double disp[N], spektrAmpl[N], spektrPhase[N];
  // Копируем статические компоненты в локальные (может скорость больше будет?)
  for (int i = 0; i < N/2; i++) {
    double k = 2.0*M_PI*i/L;
    disp[i] = sqrt((k*9.8142 + 0.0729*k*k*k/1000.0)*tanh(dpth*k));
    spektrAmpl[i] = Less5::spektrAmpl[i];
    spektrPhase[i] = Less5::spektrPhase[i];
  }
  // Строим сначала только один профиль
  // Пробегаем по оси расстояний
  for (x = 0, j = accuracy; x <= L; x += (double)L/accuracy, j--) {
    // Отклонение от нуля
    z[j] = 0;
    // Проходим все частоты
    for (int wi = 0; wi < N/2; wi++ ) {
      // z=A*cos(w*t-k*x+f), w=2*pi/T, k=k(w)
      double k = 2.0*M_PI*wi/L;
      double w = disp[wi];
      z[j] += spektrAmpl[wi]*cos(w*Less5::time+k*x+spektrPhase[wi]);
    }
  }

  list = glGenLists(1);
  glNewList(list, GL_COMPILE);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  GLfloat material_ambient[] = {.15, .15, .35, .7};
  GLfloat material_diffuse[] = {.1, .1, .5, .7};
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_ambient);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_diffuse);

  glBegin(GL_TRIANGLE_STRIP);
  // рисуем воду
  double AlphaR = 2.0*M_PI/1000.0;
  for (int i = 0; i <= accuracy; i++) {
    double j = i*L/accuracy;
    point3 a = {j, 0, z[i]};
    point3 b = {(i+1)*L/accuracy, 0, z[i+1]};
    point3 c = {((i+1)*L/accuracy) * cos(AlphaR),
		((i+1)*L/accuracy) * sin(AlphaR), z[i+1]};
    point3 d = {j * cos(AlphaR),
		j * sin(AlphaR), z[i]};
    point3 n;
    normal(a, b, c, n);

    glNormal3fv(n);
    glVertex3fv(a);
    //    glVertex3fv(b);
    //    glVertex3fv(c);
    glVertex3fv(d);
  }
  glEnd();

  glEndList();

  return list;
}

void GLWidget5::normalizeAngle(int *angle)
{
  while (*angle < 0)
    *angle += 360 * 16;
  while (*angle > 360 * 16)
    *angle -= 360 * 16;
}

void GLWidget5::keyPressEvent(QKeyEvent *event)
{
  switch (event->key()) {

  case Qt::Key_Left:
    trX -= 0.5;
    updateGL();
    break;
  case Qt::Key_Right:
    trX += 0.5;
    updateGL();
    break;
  case Qt::Key_Down:
    trY -= 0.5;
    updateGL();
    break;
  case Qt::Key_Up:
    trY += 0.5;
    updateGL();
    break;
  default:
    QWidget::keyPressEvent(event);

  }
}
