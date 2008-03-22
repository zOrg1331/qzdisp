// Time-stamp: <disp_setup.h - 18:23:20 16.04.2006>
/**************************************************************
 *** Заголовочный файл виджета по настройке дисперсии среды ***
 **************************************************************/

/* code: Наконечный Павел <zorg1331@mail.ru> */

#ifndef DISPSETUP_H
#define DISPSETUP_H

#include <QObject>
#include <QWidget>

class QLabel;
class QPushButton;
class QHBoxLayout;
class QVBoxLayout;
class QGridLayout;
class QLCDNumber;
class QLineEdit;
class QGroupBox;
class ZChart;
class ZSimpleSeries;

class DispSetup : public QWidget
{
  Q_OBJECT
public:
  DispSetup();
  // Этот массив содержит значения к из дисперсионной характеристики w=w(k)
  // для частот от 0 до 2*M_PI/Less3::T
  static float disp[4096];
  // Для пущей информативности...
  static QString dispString;
private slots:
  void drawDisp();
  void drawVelocities(double);
  void applySett();
private:
  QGroupBox *gBox1;
  QGroupBox *gBox2;
  QPushButton *disp1But;
  QPushButton *disp2But;
  QPushButton *disp3But;
  QPushButton *disp4But;
  QPushButton *disp5But;
  QHBoxLayout *mainLayout;
  QVBoxLayout *leftLayout;
  ZChart *chart;
  ZSimpleSeries *dispGraph;
  ZSimpleSeries *phaseVelGraph;
  ZSimpleSeries *groupVelGraph;
  QLCDNumber *groupVelLCD;
  QLCDNumber *phaseVelLCD;
  QLabel *groupVelLabel;
  QLabel *phaseVelLabel;
  QGridLayout *middleBottLayout;
  QVBoxLayout *middleLayout;
  QVBoxLayout *gLayout1;
  QVBoxLayout *gLayout2;
  QVBoxLayout *rightLayout;
  QLineEdit *analEdit;
  QLabel *analLabel;
  QHBoxLayout *analLayout;
  QPushButton *apply;
  QLabel *kmaxLabel;
  QLabel *wmaxLabel;
  QLineEdit *kmaxEdit;
  QLineEdit *wmaxEdit;
};

#endif
