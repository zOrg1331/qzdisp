// Time-stamp: <glwidget4.cpp - 18:22:08 16.04.2006>
/*****************************************************************
 ****      Реализация 3D виджета для 4-го урока по дисперсии  ****
 *****************************************************************/

/* code: Наконечный Павел <zorg1331@gmail.com> */

#include <QtGui>
#include <QtOpenGL>

#include <math.h>

#include "glwidget4.h"
#include "wave_setup.h"
#include "disp_setup.h"

// {{{ конструктор и деструктор

GLWidget4::GLWidget4(QWidget *parent)
  : QGLWidget(parent)
{
  setFocusPolicy(Qt::StrongFocus);

  object = 0;
  xRot = 460;
  yRot = 90;
  zRot = 0;
  trX = 0;
  trY= 0;
  trZ = 0;

  lightX = 150.0;
  lightY = 150.0;

  trolltechGreen = QColor::fromCmykF(0.40, 0.0, 1.0, 0.0);
  trolltechPurple = QColor::fromCmykF(0.0, 0.39, 0.0, 0.0);
}

GLWidget4::~GLWidget4()
{
  makeCurrent();
  glDeleteLists(object, 1);
  glDeleteLists(list, 1);
}

// }}}

QSize GLWidget4::minimumSizeHint() const
{
  return QSize(50, 50);
}

QSize GLWidget4::sizeHint() const
{
  return QSize(400, 400);
}

// {{{ устанавливаем углы поворота

void GLWidget4::setXRotation(int angle)
{
  normalizeAngle(&angle);
  if (angle != xRot) {
    xRot = angle;
    emit xRotationChanged(angle);
    updateGL();
  }
}

void GLWidget4::setYRotation(int angle)
{
  normalizeAngle(&angle);
  if (angle != yRot) {
    yRot = angle;
    emit yRotationChanged(angle);
    updateGL();
  }
}

void GLWidget4::setZRotation(int angle)
{
  normalizeAngle(&angle);
  if (angle != zRot) {
    zRot = angle;
    emit zRotationChanged(angle);
    updateGL();
  }
}

// }}}

// {{{ задаем начальные параметры OpenGL

void GLWidget4::initializeGL()
{
  //  GLfloat light_position[] = {.0, .0, -100.0, 1.0};

  GLfloat light_ambient[] = {.8, .8, .8, 1.0};
  GLfloat light_diffuse[] = {.0, .0, .5, 1.0};
  GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};

  GLfloat material_ambient[] = {.2, .2, .4, 1.0};
  GLfloat material_diffuse[] = {.2, .2, .4, 1.0};
  GLfloat material_specular[] = {1.0, 1.0, 1.0, 1.0};

  //  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_ambient);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
  glMateriali(GL_FRONT, GL_SHININESS, 16);

  qglClearColor(trolltechPurple.dark());
  glColor3f(1.0, 1.0, 1.0);
  object = makeObject();
  glShadeModel(GL_FLAT);
  glEnable(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE);
  glEnable(GL_AUTO_NORMAL);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
}

// }}}

void GLWidget4::paintGL()
{
  GLfloat light_position[] = {lightX, lightY, 10.0, 1.0};
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  glTranslated(trX, trY, trZ);
  glRotated(xRot / 16.0, 1.0, 0.0, 0.0);
  glRotated(yRot / 16.0, 0.0, 1.0, 0.0);
  glRotated(zRot / 16.0, 0.0, 0.0, 1.0);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glCallList(object);
}

void GLWidget4::resizeGL(int width, int height)
{
  int side = qMin(width, height);
  glViewport(0/*(width - side) / 4*/, (height - side) / 4, width/*side*/, side);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-1.0, 6.0, -4.0, 2.0, -100.0, 100.0);
  glMatrixMode(GL_MODELVIEW);
}

void GLWidget4::mousePressEvent(QMouseEvent *event)
{
  lastPos = event->pos();
}

void GLWidget4::mouseMoveEvent(QMouseEvent *event)
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

// {{{ рисуем объект

void GLWidget4::normal(point3 a, point3 b, point3 c, point3 n)
{
  n[0] = (b[1]-a[1])*(c[2]-a[2])-(b[2]-a[2])*(c[1]-a[1]);
  n[1] = (b[2]-a[2])*(c[0]-a[0])-(b[0]-a[0])*(c[2]-a[2]);
  n[2] = (b[0]-a[0])*(c[1]-a[1])-(b[1]-a[1])*(c[0]-a[0]);
  n[0] = -n[0];
  n[1] = -n[1];
  n[2] = -n[2];
}

GLuint GLWidget4::makeObject()
{
  // Заполняем двумерный массив данными
  double z[250][250];
  double L = 5, T = 5, x = 0, time = 0, accuracy = 245.0;
  int i = 0, j = 0, N = WaveSetup::N;
  double disp[N], spektrAmpl[N], spektrPhase[N];
  // Копируем статические компоненты в локальные (может скорость больше будет?)
  for (int i = 0; i < N/2; i++) {
    disp[i] = DispSetup::disp[i];
    spektrAmpl[i] = WaveSetup::spektrAmpl[i];
    spektrPhase[i] = WaveSetup::spektrPhase[i];
  }
  for (time = 0, i = 0; time <= T; time += T/accuracy, i++) {
    // Пробегаем по оси расстояний
    for (x = 0, j = 0; x <= L; x += L/accuracy, j++) {
      // Отклонение от нуля
      z[i][j] = 0;
      // Проходим все частоты
      for (int wi = 0; wi < N/2; wi++ ) {
	// z=A*cos(w*t-k*x+f), w=2*pi/T, k=k(w)
	double k = 2*M_PI*wi/L;
	double w = disp[wi];
	z[i][j] += spektrAmpl[wi]*cos(w*time+k*x+spektrPhase[wi]);
      }
    }
    emit complete(100*time/T);
  }
  emit complete(100);
  int im = i, jm = j;

  list = glGenLists(1);
  glNewList(list, GL_COMPILE);

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  double y = 0;
  for (j = 0, y = 0; j < (jm-1); j++, y += L/accuracy) {
    glBegin(GL_TRIANGLE_STRIP);
    for (i = 0, x = 0; i < (im-1); i++, x += T/accuracy) {
      point3 a = {x, z[i][j], y};
      point3 b = {x+T/accuracy, z[i+1][j], y};
      point3 c = {x+T/accuracy, z[i+1][j+1], y+L/accuracy};
      point3 d = {x, z[i][j+1], y+L/accuracy};
      point3 n;
      normal(a, b, c, n);

      glNormal3fv(n);
      glVertex3fv(a);
      //glVertex3fv(b);
      //glVertex3fv(c);
      glVertex3fv(d);
    }
    glEnd();
  }

  glEndList();
  return list;
}

// }}}

void GLWidget4::normalizeAngle(int *angle)
{
  while (*angle < 0)
    *angle += 360 * 16;
  while (*angle > 360 * 16)
    *angle -= 360 * 16;
}

void GLWidget4::keyPressEvent(QKeyEvent *event)
{
  switch (event->key()) {

  case Qt::Key_Left:
    trX -= 0.5;
    //lightX -= 1.0;
    updateGL();
    break;
  case Qt::Key_Right:
    trX += 0.5;
    //lightX += 1.0;
    qDebug() << "lX = " << lightX;
    updateGL();
    break;
  case Qt::Key_Down:
    trY -= 0.5;
    //lightY -= 1.0;
    updateGL();
    break;
  case Qt::Key_Up:
    trY += 0.5;
    //lightY += 1.0;
    updateGL();
    break;
  default:
    QWidget::keyPressEvent(event);

  }
}
