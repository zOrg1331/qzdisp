// Time-stamp: <mainwindow.h - 18:41:48 16.04.2006>
/************************************************************************
 *** Заголовочный файл главного виджета простого построителя графиков ***
 ************************************************************************/

/* code: Наконечный Павел <zorg1331@mail.ru> */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QWidget>

class QTabWidget;
class QHBoxLayout;
class DispSetup;
class WaveSetup;
class Result;

class MainWindow : public QWidget
{
  Q_OBJECT
public:
  MainWindow();
private:
  QHBoxLayout *mainLayout;
  QTabWidget *tabWidget;
  DispSetup *dispSetup;
  WaveSetup *waveSetup;
  Result *result;
};

#endif
