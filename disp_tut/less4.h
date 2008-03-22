// Time-stamp: <less4.h - 13:55:50 27.03.2006>
/*****************************************************************
 *** Заголовочный файл четвертого урока туториала по дисперсии ***
 *****************************************************************/

/* code: Наконечный Павел <zorg1331@mail.ru> */

#ifndef LESS4_H
#define LESS4_H

#include <QObject>
#include <QWidget>
#include <QMutex>
#include <QSize>
#include <QThread>
#include <QWaitCondition>

class QLabel;
class QPushButton;
class QHBoxLayout;
class QVBoxLayout;
class QRadioButton;
class QLineEdit;
class QLCDNumber;
class QGroupBox;
class ZChart;
class ZSimpleSeries;
class GLWidget4;

class AnimThread : public QThread
{
  Q_OBJECT

public:
  AnimThread(QObject *parent = 0);
  ~AnimThread();

  void anim();
  float *wave;

protected:
  void run();

private:
  QObject *parentWidget;
  QMutex mutex;
  QWaitCondition condition;
  bool restart;
  bool abort;
};

class Less4 : public QWidget
{
  Q_OBJECT

public:
  Less4(QWidget *);

private slots:
  void drawBegin(bool);
  void drawSpec();
  void drawStep();
  void resetSlot();
  void drawAnim();
  void stopAnimSlot();
  void redrawWaveSlot();
  void draw3DSlot();
  void setPercents(double);
private:
  // Время, которое используется при пошаговом построении волны
  static double t;
  bool stillAnim;

  QRadioButton *chkZeroHour;
  QRadioButton *chkSpecMoment;
  QRadioButton *chkStepByStep;
  QRadioButton *chkAnimation;
  QLabel *specTimeLabel;
  QLabel *intervalLabel;
  QLabel *animSpeedLabel;
  QLineEdit *specTimeEdit;
  QLineEdit *intervalEdit;
  QLineEdit *animSpeedEdit;
  QPushButton *redraw;
  QPushButton *nextStep;
  QPushButton *reset;
  QPushButton *startAnim;
  QPushButton *stopAnim;
  QPushButton *redrawWave;
  QPushButton *draw3DBut;
  QGroupBox *groupBox;
  QVBoxLayout *leftLayout;
  QVBoxLayout *rightLayout;
  QVBoxLayout *groupLayout;
  QHBoxLayout *mainLayout;
  QHBoxLayout *hor1Layout;
  QHBoxLayout *hor2Layout;
  QHBoxLayout *hor3Layout;
  QHBoxLayout *hor4Layout;
  QHBoxLayout *hor5Layout;
  QHBoxLayout *hor6Layout;
  QLCDNumber *indLCD;
  QLabel *indLabel;
  ZChart *chart2D;
  ZSimpleSeries *waveGraph;
  ZSimpleSeries *markWave;
  GLWidget4 *glwidget;
  AnimThread *animThread;
};

#endif
