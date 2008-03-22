// Time-stamp: <result.cpp - 18:31:27 16.04.2006>
/************************************************************************
 ***       ���������� ������� �� ��������� ���������� �����������     ***
 ***                   ����� � ����� � ����������                     ***
 ************************************************************************/

/* code: ���������� ����� <zorg1331@gmail.com> */

#include <QtGui>
#include <QLabel>
#include <QPushButton>

#include <math.h>
#include "result.h"
#include <zchart.h>
#include <translate.h>
#include "wave_setup.h"
#include "disp_setup.h"
#include "glwidget4.h"

// �������������� ����������� ����������
double Result::t = 0;

// {{{ �������������� �����

// �������������� �����
void Result::redrawWaveSlot()
{
  if (chkZeroHour->isChecked()) drawBegin(true);
  if (chkSpecMoment->isChecked()) drawSpec();
  if (chkStepByStep->isChecked()) drawStep();
  //  if (chkAnimation->isChecked()) drawAnim();
  if (draw3DBut->isChecked()) {
    glwidget->initializeGL();
    glwidget->updateGL();
  }
}

// }}}

// {{{ ������ ������� ����� � ������ ������ �������

// ������ ������� ����� � ������ ������ �������
void Result::drawBegin(bool checked)
{
  if (checked) {
    waveGraph->clear();
    double L = 10;
    double accuracy = 400;
    // ��������� �� ��� ����������
    for (double j = 0; j < L; j += L/accuracy) {
      // ���������� �� ����
      double z = 0;
      // �������� ��� �������
      for (int i = 0; i < WaveSetup::N/2; i++ ) {
	// ������ �������, � ������� �������� �����
	double t = 0;
	// z=A*cos(w*t-k*x+f), w=2*pi/T, k=k(w)
	double k = 2*M_PI*i/L;
	double w = DispSetup::disp[i];
	z += WaveSetup::spektrAmpl[i]*cos(w*t+k*j+WaveSetup::spektrPhase[i]);
      }
      waveGraph->add(j, z);
    }
    chart2D->setTitle("�������� ����� <b>U(t)="+WaveSetup::waveString+"</b>"+" � ����� � ���������� <b>w(k)="+DispSetup::dispString+"</b>");
    chart2D->update();
  }
}

// }}}

// {{{ ������ ������� ����� � ������������ ������ �������

void Result::drawSpec()
{
  waveGraph->clear();
  double L = 10;
  double accuracy = 400;
  // ��������� �� ��� ����������
  for (double j = 0; j < L; j += L/accuracy) {
    // ���������� �� ����
    double z = 0;
    // �������� ��� �������
    for (int i = 0; i < WaveSetup::N/2; i++ ) {
      // ������ �������, � ������� �������� �����
      double t = specTimeEdit->text().toFloat();
      // z=A*cos(w*t-k*x+f), w=2*pi/T, k=k(w)
      double k = 2*M_PI*i/L;
      double w = DispSetup::disp[i];
      z += WaveSetup::spektrAmpl[i]*cos(w*t+k*j+WaveSetup::spektrPhase[i]);
    }
    waveGraph->add(j, z);
  }
  chart2D->setTitle("�������� ����� <b>U(t)="+WaveSetup::waveString+"</b>"+" � ����� � ���������� <b>w(k)="+DispSetup::dispString+"</b>");
  chart2D->update();
}

// }}}

// {{{ ������ ������� ��������

void Result::drawStep()
{
  waveGraph->clear();
  double L = 10;
  double accuracy = 400;
  // ��������� �� ��� ����������
  for (double j = 0; j < L; j += L/accuracy) {
    // ���������� �� ����
    double z = 0;
    // �������� ��� �������
    for (int i = 0; i < WaveSetup::N/2; i++ ) {
      // z=A*cos(w*t-k*x+f), w=2*pi/T, k=k(w)
      double k = 2*M_PI*i/L;
      double w = DispSetup::disp[i];
      z += WaveSetup::spektrAmpl[i]*cos(w*t+k*j+WaveSetup::spektrPhase[i]);
    }
    waveGraph->add(j, z);
  }
  chart2D->setTitle("�������� ����� <b>U(t)="+WaveSetup::waveString+"</b>"+" � ����� � ���������� <b>w(k)="+DispSetup::dispString+"</b>");
  chart2D->update();
  // ������ �������, � ������� �������� �����
  Result::t -= intervalEdit->text().toFloat();
}

// }}}

void Result::resetSlot()
{
  Result::t = 0;
  drawStep();
}

// {{{ ������ ��������

void Result::drawAnim()
{
  chart2D->setTitle("�������� ����� <b>U(t)="+WaveSetup::waveString+"</b>"+" � ����� � ���������� <b>w(k)="+DispSetup::dispString+"</b>");
  double time = 0;
  int i = 0;
  double accuracy = 400;
  bool pointI_old = false, pointII_old = false;	// �� ���������� ���� ����� �� ��������
  double point1X_old = 0.0, point3X_old = 0.0; // ���������� �����, ��������� �� ���������� ����
  double dz = 0.05; // ������������ ���� ����� ����, ��� ������ ����
  // ������� ����, ������� ������������� ��� ���������� � false
  // ��� ��������, �.�. � ������ ������� ���������� qApp->processEvents();
  stillAnim = true;
  while (stillAnim) {
    waveGraph->clear();
    markWave->clear();
    double L = 10;
    bool point1 = false; // ������ ����� ���� ��� ���
    double point1X = 0.0;
    bool point2 = false; // ������ ��� �����
    double point2X = 0.0;
    bool point3 = false; // ������� ����� ���� ��� ���
    double point3X = 0.0;
    bool point4 = false; // ��������� ��� �����
    double point4X = 0.0;
    double z_old = 0.0;		// �������� ���������� ��� ���������� �����
    // ��������� �� ��� ����������
    for (double j = 0; j < L; j += L/accuracy) {
      // ���������� �� ����
      double z = 0.0;
      // �������� ��� �������
      for (int i = 0; i < WaveSetup::N/2; i++ ) {
	// z=A*cos(w*t-k*x+f), w=2*pi/T, k=k(w)
	double k = 2*M_PI*i/L;
	double w = DispSetup::disp[i];
	z += WaveSetup::spektrAmpl[i]*cos(w*time+k*j+WaveSetup::spektrPhase[i]);
      }
      // {{{ ����� �������, ������������ ��� �������� ����� ������      // ����� ������ ����      if ((j > 0.3*L) || (pointI_old)) { // �������� ������ � ������ �����	if ((z_old < dz) && (z > dz)) { // ������ ����� ����������� ����	  if (!pointI_old) {	// ���� ��� ������ ���	    if (point1 && !point2) { // ������ ����� ��� ����	      if (fabs(j-point1X) > 0.01*L) { // ���� ���������� ����� ���������� ������� ���������� ������		point2 = true;		point2X = j;	      } else { // ���� ����� ����������� ������� ������, �� ������ ���������� ������		point1X = j;	      }	    } else if (!point1) { // ���� ������ ����� ��� ���	      point1 = true;	      point1X = j;	    }	  } else {		// ���� ����� ��� ��������	    if (point1 && !point2) { // ������ ����� ��� ����	      if (fabs(j-point1X) > 0.01*L) { // ���� ���������� ����� ���������� ������� ���������� ������		point2 = true;		point2X = j;	      }	    } else if (!point1) { // ���� ������ ����� ��� ���	      if (fabs(j-point1X_old) <= 2.0*L/accuracy) { // ���� ��������� ����� ������ � ����������		point1 = true;		point1X = j;	      }	    }	  }	}      }      // ����� ������ ����      if ((j > 0.5*L) || (pointII_old)) { // �������� ������ � ������ �����	if ((z_old < -dz) && (z > -dz)) { // ������ ����� ����	  if (!pointII_old) {	// ���� ��� ������ ���	    if (point3 && !point4) { // ������ ����� ��� ����	      if (fabs(j-point3X) > 0.002*L) { // ���� ���������� ����� ���������� ������� ���������� ������		point4 = true;		point4X = j;	      } else { // ���� ����� ����������� ������� ������, �� ������ ���������� ������		point3X = j;	      }	    } else if (!point3) { // ���� ������ ����� ��� ���	      point3 = true;	      point3X = j;	    }	  } else {		// ���� ����� ��� ��������	    if (point3 && !point4) { // ������ ����� ��� ����	      if (fabs(j-point3X) > 0.002*L) { // ���� ���������� ����� ���������� ������� ���������� ������		point4 = true;		point4X = j;	      }	    } else if (!point3) { // ���� ������ ����� ��� ���	      if (fabs(j-point3X_old) <= 2.0*L/accuracy) { // ���� ��������� ����� ������ � ����������		point3 = true;		point3X = j;	      }	    }	  }	}      }      // }}}
      waveGraph->add(j, z);
      z_old = z;
    }
    pointI_old = true;
    pointII_old = true;
    point1X_old = point1X;
    point3X_old = point3X;
    if (point1X == 0.0) pointI_old = false;
    if (point3X == 0.0) pointII_old = false;
    markWave->add(point1X, 0.1);
    markWave->add(point1X, -0.1);
    markWave->add(point2X, -0.1);
    markWave->add(point2X, 0.1);
    markWave->add(point1X, 0.1);
    markWave->add(point2X, 0.1);
    markWave->add(point2X, 0.0);
    markWave->add(point3X, 0.0);
    markWave->add(point3X, 0.1);
    markWave->add(point3X, -0.1);
    markWave->add(point4X, -0.1);
    markWave->add(point4X, 0.1);
    markWave->add(point3X, 0.1);
    chart2D->repaint();
    time -= animSpeedEdit->text().toFloat();
    i++;
    qApp->processEvents();
  }
  //  animThread->start();
}

// }}}

void Result::stopAnimSlot()
{
  stillAnim = false;
  //  animThread->quit();
}

// {{{ ������ 3������� �������

void Result::draw3DSlot()
{
  if (draw3DBut->isChecked()) {
    indLCD->setVisible(true);
    indLabel->setVisible(true);
    indLabel->repaint();
    draw3DBut->setText(translate("2D ���"));
    glwidget->setVisible(true);
    chart2D->setVisible(false);
    chkZeroHour->setEnabled(false);
    chkSpecMoment->setEnabled(false);
    chkStepByStep->setEnabled(false);
    chkAnimation->setEnabled(false);
  } else {
    draw3DBut->setText(translate("3D ���"));
    glwidget->setVisible(false);
    chart2D->setVisible(true);
    chkZeroHour->setEnabled(true);
    chkSpecMoment->setEnabled(true);
    chkStepByStep->setEnabled(true);
    chkAnimation->setEnabled(true);
    indLCD->setVisible(true);
    indLabel->setVisible(true);
  }
}

// }}}

void Result::setPercents(double _perc)
{
  QString perc = QString("%1").arg(_perc, 0, 'f', 2);
  indLCD->display(perc);
  indLCD->repaint();
}

// {{{ �����������

Result::Result()
{
  // ������� ����������� �������
  groupBox = new QGroupBox(translate("������ ���������"));
  chkZeroHour = new QRadioButton(translate("��������� ������"));
  chkSpecMoment = new QRadioButton(translate("���������� ������"));
  chkStepByStep = new QRadioButton(translate("��������"));
  chkAnimation = new QRadioButton(translate("��������"));
  specTimeLabel = new QLabel(translate("������ �������"));
  intervalLabel = new QLabel(translate("��������"));
  animSpeedLabel = new QLabel(translate("�������� ��������"));
  specTimeEdit = new QLineEdit(translate("0"));
  intervalEdit = new QLineEdit(translate("0.1"));
  animSpeedEdit = new QLineEdit(translate("0.1"));
  redraw = new QPushButton(translate("������������"));
  nextStep = new QPushButton(translate("��������� ���"));
  reset = new QPushButton(translate("�����"));
  startAnim = new QPushButton(translate("����!"));
  stopAnim = new QPushButton(translate("����������"));
  redrawWave = new QPushButton(translate("��������"));
  draw3DBut = new QPushButton(translate("3D ���"));
  leftLayout = new QVBoxLayout();
  rightLayout = new QVBoxLayout();
  groupLayout = new QVBoxLayout();
  hor1Layout = new QHBoxLayout();
  hor2Layout = new QHBoxLayout();
  hor3Layout = new QHBoxLayout();
  hor4Layout = new QHBoxLayout();
  hor5Layout = new QHBoxLayout();
  hor6Layout = new QHBoxLayout();
  mainLayout = new QHBoxLayout();
  indLCD = new QLCDNumber(4);
  indLabel = new QLabel(translate("����������:"));
  chart2D = new ZChart();
  glwidget = new GLWidget4();

  // ����� tooltips � ��������
  chkZeroHour->setToolTip(translate("�������� ���� ��� �����������<br> ����� � ������ ������ �������"));
  chkSpecMoment->setToolTip(translate("����� �������� ����� ����������,<br> ��� ����� ��������� ����� �<br> ������� �������� ������ �������"));
  chkStepByStep->setToolTip(translate("���� ����� ��� �� ����� �������<br> �������� ���������� ����������,<br> �� ���� ������� ��� ���"));
  chkAnimation->setToolTip(translate("�������� ���, ���� ������ ������������<br> �� ���������� ���������������� ������"));
  redrawWave->setToolTip(translate("�� ��������� �������� ��� ������, ���� ��<br> �������� ��������� �� ���������� �������"));
  specTimeEdit->setToolTip(translate("����� ����� ������ ���������� ������ �������,<br> � ������� ���������� ������� �����"));
  intervalEdit->setToolTip(translate("����� �������� ��������,<br> ����� ������� ����� �������� �����"));
  animSpeedEdit->setToolTip(translate("����� �������� �������� ��������, � ����������<br> �������� �������, ����� ������� ����� ��������� �����"));
  redraw->setToolTip(translate("�������������� �����, ����� ����� ��������,<br> ���� �� �������� ������ �������"));
  draw3DBut->setToolTip(translate("��� ������� ����������� �������������� ������������<br> ��������� ���������� ������� ��������������� ����"));

  // ������ ��������� 3D ���� ������ ��������
  draw3DBut->setCheckable(true);

  QFont boldFont("Sans Serif", 10, QFont::Bold);
  boldFont.setPointSize(10);
  redrawWave->setFont(boldFont);

  // ����������� LCD
  indLCD->setSmallDecimalPoint(true);
  indLCD->setSegmentStyle(QLCDNumber::Flat);

  connect(glwidget, SIGNAL(complete(double)),
	  this, SLOT(setPercents(double)));

  // ������ ��������� ���������
  chkZeroHour->setChecked(true);
  specTimeEdit->setEnabled(false);
  intervalEdit->setEnabled(false);
  animSpeedEdit->setEnabled(false);
  redraw->setEnabled(false);
  nextStep->setEnabled(false);
  reset->setEnabled(false);
  startAnim->setEnabled(false);
  stopAnim->setEnabled(false);
  glwidget->setVisible(false);
  indLCD->setVisible(false);
  indLabel->setVisible(false);

  connect(chkZeroHour, SIGNAL(toggled(bool)),
	  this, SLOT(drawBegin(bool)));
  connect(chkSpecMoment, SIGNAL(toggled(bool)),
	  redraw, SLOT(setEnabled(bool)));
  connect(chkSpecMoment, SIGNAL(toggled(bool)),
	  specTimeEdit, SLOT(setEnabled(bool)));
  connect(redraw, SIGNAL(clicked()),
	  this, SLOT(drawSpec()));
  connect(chkStepByStep, SIGNAL(toggled(bool)),
	  nextStep, SLOT(setEnabled(bool)));
  connect(chkStepByStep, SIGNAL(toggled(bool)),
	  intervalEdit, SLOT(setEnabled(bool)));
  connect(chkStepByStep, SIGNAL(toggled(bool)),
	  reset, SLOT(setEnabled(bool)));
  connect(nextStep, SIGNAL(clicked()),
	  this, SLOT(drawStep()));
  connect(reset, SIGNAL(clicked()),
	  this, SLOT(resetSlot()));
  connect(chkAnimation, SIGNAL(toggled(bool)),
	  startAnim, SLOT(setEnabled(bool)));
  connect(chkAnimation, SIGNAL(toggled(bool)),
	  stopAnim, SLOT(setEnabled(bool)));
  connect(chkAnimation, SIGNAL(toggled(bool)),
	  animSpeedEdit, SLOT(setEnabled(bool)));
  connect(startAnim, SIGNAL(clicked()),
	  this, SLOT(drawAnim()));
  connect(stopAnim, SIGNAL(clicked()),
	  this, SLOT(stopAnimSlot()));
  connect(redrawWave, SIGNAL(clicked()),
	  this, SLOT(redrawWaveSlot()));
  connect(draw3DBut, SIGNAL(clicked()),
	  this, SLOT(draw3DSlot()));

  // ����������� �������
  chart2D->setBorderType(0);
  chart2D->setGraphPanelBorderType(0);
  chart2D->setMargin(2);
  chart2D->setSpacing(2);
  chart2D->setFirstText("");
  chart2D->setSecondText("");
  chart2D->setTitle("�������� ����� � �����");
  chart2D->setXRange(0, WaveSetup::T);
  chart2D->setYRange(-4, 4);
  chart2D->setAxesName("x, �", "U, �");
  chart2D->showLegend(false);

  // ������� ����� ������
  waveGraph = new ZSimpleSeries("Wave");
  waveGraph->setColor(Qt::blue);
  chart2D->addSeries(waveGraph);
  markWave = new ZSimpleSeries("Mark of wave");
  markWave->setColor(Qt::red);
  chart2D->addSeries(markWave);

  hor1Layout->addWidget(specTimeLabel);
  hor1Layout->addWidget(specTimeEdit);
  hor2Layout->addWidget(intervalLabel);
  hor2Layout->addWidget(intervalEdit);
  hor3Layout->addWidget(animSpeedLabel);
  hor3Layout->addWidget(animSpeedEdit);
  hor4Layout->addWidget(indLabel);
  hor4Layout->addWidget(indLCD);
  hor5Layout->addWidget(nextStep);
  hor5Layout->addWidget(reset);
  hor6Layout->addWidget(startAnim);
  hor6Layout->addWidget(stopAnim);

  groupLayout->addWidget(chkZeroHour);
  groupLayout->addWidget(chkSpecMoment);
  groupLayout->addLayout(hor1Layout);
  groupLayout->addWidget(redraw);
  groupLayout->addWidget(chkStepByStep);
  groupLayout->addLayout(hor2Layout);
  groupLayout->addLayout(hor5Layout);
  groupLayout->addWidget(chkAnimation);
  groupLayout->addLayout(hor3Layout);
  groupLayout->addLayout(hor6Layout);
  groupLayout->setMargin(1);
  groupBox->setLayout(groupLayout);

  leftLayout->addWidget(groupBox);
  leftLayout->addStretch();
  leftLayout->addWidget(draw3DBut);
  leftLayout->addLayout(hor4Layout);
  leftLayout->addStretch();
  leftLayout->addWidget(redrawWave);
  leftLayout->setMargin(0);

  rightLayout->addWidget(chart2D);
  rightLayout->addWidget(glwidget);

  mainLayout->addLayout(leftLayout);
  mainLayout->addLayout(rightLayout);
  mainLayout->setStretchFactor(rightLayout, 1);
  mainLayout->setMargin(0);

  setLayout(mainLayout);

  // ������ ��, ��� ���� � ������
  drawBegin(true);
}

// }}}
