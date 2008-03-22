// Time-stamp: <less5.h - 09:46:47 17.04.2006>
/*****************************************************************
 ***  Заголовочный файл пятого урока туториала по дисперсии   ***
 *****************************************************************/

/* code: Наконечный Павел <zorg1331@mail.ru> */

#ifndef LESS5_H
#define LESS5_H

#include <QObject>
#include <QWidget>

#include <knz_fft_cpp.h>

class QLabel;
class QPushButton;
class QGridLayout;
class QVBoxLayout;
class QHBoxLayout;
class QRadioButton;
class QLineEdit;
class QLCDNumber;
class QGroupBox;
class ZChart;
class ZSimpleSeries;
class GLWidget5;

class Less5 : public QWidget
{
  Q_OBJECT
public:
  Less5(QWidget *);
  // Амплитудный спектр
  static double spektrAmpl[4096];
  // Фазовый спектр
  static double spektrPhase[4096];
  // Глубина жидкости
  static double depth;
  // Время, в которое рисовать волны
  static double time;

signals:
  void timeChanged(double);
private slots:
  void drawWater();
  void applySett();
  void animation();
  void draw3DSlot();
  void timeChangedSlot(double);
private:
  // Раскладываем воду на гармоники
  void doFFT();
  // Собственно, дискретизированные данные
  KNZComplex data[4096], *a;
  double s[4096];

  QGroupBox *group1;
  QGroupBox *group2;
  QGroupBox *group3;
  QPushButton *water1But;
  QPushButton *water2But;
  QPushButton *water3But;
  QPushButton *water4But;
  QHBoxLayout *mainLayout;
  QVBoxLayout *leftLayout;
  ZChart *chart;
  ZSimpleSeries *waterGraph;
  QVBoxLayout *middleLayout;
  QVBoxLayout *rightLayout;
  QVBoxLayout *group1Layout;
  QVBoxLayout *group3Layout;
  QLineEdit *surfEdit;
  QLabel *surfLabel;
  QLabel *depthLabel;
  QLabel *param1Label;
  QLabel *param2Label;
  QLabel *param3Label;
  QLineEdit *depthEdit;
  QLabel *radiusLabel;
  QLineEdit *radiusEdit;
  QLabel *heightLabel;
  QLineEdit *heightEdit;
  QGridLayout *analLayout;
  QPushButton *apply;
  QPushButton *drawAnim;
  QPushButton *draw3DBut;
  QLCDNumber *currTimeLCD;
  QLabel *currTimeLabel;
  QHBoxLayout *currTimeLayout;
  GLWidget5 *glwidget;
};

#endif
