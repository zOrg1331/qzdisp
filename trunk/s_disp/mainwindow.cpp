// Time-stamp: <mainwindow.cpp - 18:43:10 16.04.2006>
/***********************************************************************************
 *** ���������� ������� �������� ���� ���������� �� �������� ���������� �������� ***
 ***********************************************************************************/

/* code: ���������� ����� <zorg1331@gmail.com> */

#include <QtGui>

#include <translate.h>
#include "mainwindow.h"
#include "disp_setup.h"
#include "wave_setup.h"
#include "result.h"

// {{{ �����������

// ������ ������ �������� ����
MainWindow::MainWindow()
{
  // ������� ����������� �������
  mainLayout = new QHBoxLayout();
  tabWidget = new QTabWidget(this);
  dispSetup = new DispSetup();
  waveSetup = new WaveSetup();
  result = new Result();

  tabWidget->addTab(waveSetup, translate("��������� ���������� ����������"));
  tabWidget->addTab(dispSetup, translate("��������� ��������� �����"));
  tabWidget->addTab(result, translate("��������� ����������� ����� ����� ����� �����"));

  mainLayout->addWidget(tabWidget);
  mainLayout->setMargin(0);

  setLayout(mainLayout);
}

// }}}
