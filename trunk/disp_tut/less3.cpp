// Time-stamp: <less3.cpp - 15:36:49 21.05.2006>
/************************************************************************
 ***              ��� ������ ���� ��������� �� ���������              ***
 ************************************************************************/

/* code: ���������� ����� <zorg1331@gmail.com> */

#include <QtGui>
#include <QLabel>
#include <QPushButton>
#include <QLCDNumber>
#include <QTableWidget>

#include <math.h>
#include "less3.h"
#include <zchart.h>
#include "drawdisparea.h"
#include <parser.h>
#include <translate.h>
#include <knz_fft_cpp.h>
#include <arthurwidgets.h>

// �������������� ����������� �������
double Less3::T = 0;
int Less3::power = 12, Less3::N=4096;
double Less3::spektrAmpl[4096] = {0, 0};
double Less3::spektrPhase[4096] = {0, 0};
QString Less3::waveString = "";

// {{{ ��������� �� ������� ������ "���������"

// {{{ ������ ������ ���������� ����������

// {{{ ������������ �������������� �����

void Less3::viewResSlot(bool checked)
{
  setUpdatesEnabled(false);
  if (checked) {
    chartSpektr->setVisible(true);
    drawwavearea->setVisible(false);
    clear->setHidden(true);
    viewWave->setVisible(true);
    viewWave->setChecked(true);
    table->setVisible(false);
    chartWave->setVisible(false);
  }
  if (!checked) {
    chartSpektr->setVisible(false);
    if (chkManWave->isChecked()) drawwavearea->setVisible(true);
    if (chkManWave->isChecked()) clear->setHidden(false);
    viewWave->setVisible(false);
    viewWave->setChecked(false);
    chartWave->setVisible(false);
    chartSpektr->setVisible(false);
    if (chkSpektrWave->isChecked()) table->setVisible(true);
  }
  setUpdatesEnabled(true);
}

void Less3::viewWaveSlot(bool checked)
{
  if (checked) {
    chartWave->setVisible(false);
    chartWave->update();
    chartSpektr->setHidden(false);
    chartSpektr->update();
    viewWave->setText(translate("���������� �����"));
  }
  if (!checked) {
    chartWave->setVisible(true);
    chartWave->update();
    chartSpektr->setHidden(true);
    chartSpektr->update();
    viewWave->setText(translate("���������� ������"));
  }
}

// {{{ �����������