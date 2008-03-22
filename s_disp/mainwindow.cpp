// Time-stamp: <mainwindow.cpp - 18:43:10 16.04.2006>
/***********************************************************************************
 *** Реализация виджета главного окна приложения по простому построению графиков ***
 ***********************************************************************************/

/* code: Наконечный Павел <zorg1331@gmail.com> */

#include <QtGui>

#include <translate.h>
#include "mainwindow.h"
#include "disp_setup.h"
#include "wave_setup.h"
#include "result.h"

// {{{ конструктор

// Строим виджет главного окна
MainWindow::MainWindow()
{
  // Создаем необходимые виджеты
  mainLayout = new QHBoxLayout();
  tabWidget = new QTabWidget(this);
  dispSetup = new DispSetup();
  waveSetup = new WaveSetup();
  result = new Result();

  tabWidget->addTab(waveSetup, translate("Настройка начального возмущения"));
  tabWidget->addTab(dispSetup, translate("Настройка дисперсии среды"));
  tabWidget->addTab(result, translate("Результат прохождения волны через такую среду"));

  mainLayout->addWidget(tabWidget);
  mainLayout->setMargin(0);

  setLayout(mainLayout);
}

// }}}
