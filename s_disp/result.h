// Time-stamp: <result.h - 18:24:49 16.04.2006>
/****************************************************************
 ***    Заголовочный файл виджета, отображающего результат    ***
 ***           прохождения волны в среде с дисперсией         ***
 ****************************************************************/

/* code: Наконечный Павел <zorg1331@mail.ru> */

#ifndef RESULT_H
#define RESULT_H

#include <QObject>
#include <QWidget>
#include <QSize>

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

class Result : public QWidget
{
  Q_OBJECT

public:
  Result();

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
};

#endif
