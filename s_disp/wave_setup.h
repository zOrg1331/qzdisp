// Time-stamp: <wave_setup.h - 18:24:05 16.04.2006>
/*********************************************************************
 *** Заголовочный файл виджета для настройки начального возмущения ***
 *********************************************************************/

/* code: Наконечный Павел <zorg1331@mail.ru> */

#ifndef WAVESETUP_H
#define WAVESETUP_H

#include <QObject>
#include <QWidget>

#include <knz_fft_cpp.h>

class QLabel;
class QPushButton;
class QHBoxLayout;
class QVBoxLayout;
class QGridLayout;
class QRadioButton;
class QLineEdit;
class QTableWidget;
class QGroupBox;
class ZChart;
class ZSimpleSeries;
class DrawDispArea;

class WaveSetup : public QWidget
{
  Q_OBJECT

public:
  WaveSetup();
  // Задаем (экспортируем) период начального возмущения
  static double T;
  // Задаем количество отсчетов и соответствующую им степень двойки
  static int power, N;
  // Массив, задающий спектр начального возмущения
  // Для простоты, жестко задаем 4096 точки, каждая из которых соответствует
  // частоте, соответствующей :) периоду T
  static double spektrAmpl[4096]; // амплитуды гармоник
  static double spektrPhase[4096]; // фазы гармоник
  // Для пущей информативности...
  static QString waveString;
private slots:
  void drawWave();
  void applySett();
  void viewResSlot(bool);
  void viewWaveSlot(bool);
private:
  // Здесь будет содержатся спектр сигнала после преобразования Фурье
  KNZComplex data[4096], *a;
  // Здесь надо задать отсчеты, которые нужно подвергнуть преобразованию Фурье
  double s[4096];

  // Функция проводит подготовительные действия перед преобразованием Фурье,
  // выполняет само преобразование и строит спектр
  void doFFT();

  QGroupBox *gBox1;
  QGroupBox *gBox2;
  QPushButton *wave1But;
  QPushButton *wave2But;
  QPushButton *wave3But;
  QPushButton *wave4But;
  QPushButton *wave5But;
  QHBoxLayout *mainLayout;
  QVBoxLayout *leftLayout;
  QVBoxLayout *gLayout1;
  QVBoxLayout *gLayout2;
  ZChart *chartSpektr;
  ZChart *chartWave;
  ZSimpleSeries *waveSpektrGraph;
  ZSimpleSeries *waveGraph;
  QGridLayout *middleBottLayout;
  QVBoxLayout *middleLayout;
  QVBoxLayout *rightLayout;
  QRadioButton *chkAnalWave;
  QRadioButton *chkManWave;
  QRadioButton *chkSpektrWave;
  QLineEdit *analExprEdit;
  QLabel *analExprLabel;
  QHBoxLayout *analLayout;
  QPushButton *apply;
  DrawDispArea *drawwavearea;
  QPushButton *clear;
  QPushButton *viewRes;
  QPushButton *viewWave;
  QLabel *tmaxLabel;
  QLabel *umaxLabel;
  QLabel *uminLabel;
  QLineEdit *tmaxEdit;
  QLineEdit *umaxEdit;
  QLineEdit *uminEdit;
  QTableWidget *table;
};

#endif
