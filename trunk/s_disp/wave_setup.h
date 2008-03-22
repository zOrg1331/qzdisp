// Time-stamp: <wave_setup.h - 18:24:05 16.04.2006>
/*********************************************************************
 *** ������������ ���� ������� ��� ��������� ���������� ���������� ***
 *********************************************************************/

/* code: ���������� ����� <zorg1331@mail.ru> */

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
  // ������ (������������) ������ ���������� ����������
  static double T;
  // ������ ���������� �������� � ��������������� �� ������� ������
  static int power, N;
  // ������, �������� ������ ���������� ����������
  // ��� ��������, ������ ������ 4096 �����, ������ �� ������� �������������
  // �������, ��������������� :) ������� T
  static double spektrAmpl[4096]; // ��������� ��������
  static double spektrPhase[4096]; // ���� ��������
  // ��� ����� ���������������...
  static QString waveString;
private slots:
  void drawWave();
  void applySett();
  void viewResSlot(bool);
  void viewWaveSlot(bool);
private:
  // ����� ����� ���������� ������ ������� ����� �������������� �����
  KNZComplex data[4096], *a;
  // ����� ���� ������ �������, ������� ����� ����������� �������������� �����
  double s[4096];

  // ������� �������� ���������������� �������� ����� ��������������� �����,
  // ��������� ���� �������������� � ������ ������
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
