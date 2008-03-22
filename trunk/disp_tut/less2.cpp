// Time-stamp: <less2.cpp - 20:20:30 15.04.2006>
/************************************************************************
 ***             ��� ������ ���� ��������� �� ���������               ***
 ************************************************************************/

/* code: ���������� ����� <zorg1331@gmail.com> */

#include <QtGui>
#include <QLabel>
#include <QPushButton>
#include <QLCDNumber>

#include <math.h>
#include "less1.h"
#include "less2.h"
#include <zchart.h>
#include "parser.h"
#include <translate.h>
#include "less3.h"
#include <arthurwidgets.h>

// �������������� ����������� ����������
float Less2::disp[4096] = {0, 0};
QString Less2::dispString = "";

// {{{ ������ ������������� ��������������

// ������ ������������� ��������������
void Less2::drawDisp()
{
  // ��� �� ������� ����������� ������, �� ������������� ������� ��������� ���������
  gBox2->setChecked(false);
  // ��������, ����� �� �������� �����
  int type;
  QString sType;
  if (disp1But->isChecked()) {
    type = 1;
    sType = "k</b>. ��������� ���.";
    dispString = "k</b>";
  }
  if (disp2But->isChecked()) {
    type = 2;
    sType = "3*k-3</b>. ��������� �������� ������ �������.";
    dispString = "3*k-3</b>";
  }
  if (disp3But->isChecked()) {
    type = 3;
    sType = "0.5*k+3</b>. ��������� �������� ������ �������.";
    dispString = "0.5*k+3</b>";
  }
  if (disp4But->isChecked()) {
    type = 4;
    sType = "-k+20</b>. ��������� �������� ����������������� �������.";
    dispString = "-k+20</b>";
  }
  if (disp5But->isChecked()) {
    type = 5;
    sType = "20*tanh(0.008*k*k)</b>. ��������� �����������.";
    dispString = "20*tanh(0.008*k*k)</b>";
  }

  // �������������� ������
  dispGraph->clear();
  groupVelGraph->clear();
  phaseVelGraph->clear();
  int xMin = 0;
  int xMax = kmaxEdit->text().toInt();
  chart->setXRange(xMin, xMax);
  chart->setYRange(xMin, xMax);
  chart->setTitle("������������� ��������������: <b>w(k)=" + sType);

  // � ����������� �� ���������� �������, ������ ������������� ��������������
  // ������ �� ������� ����������� ��������� �������:
  // � ������������� ��������������, ��������, ����������, ������������,
  // ������������� ����� 2*M_PI*i/L - �������� ����� i-� ���������
  // ����� ����������� �������� �������� �������, ��������������� ����� ��������� �����
  float L = 10;
  double accuracy = 400*Less1::perfomance;
  switch (type) {
  case 1:
    for (float i = xMin; i <= xMax; i += (xMax-xMin)/accuracy) {
      dispGraph->add(i, i);
    }
    // ��������� ������ �� �������
    for (int i = 0; i < 4096; i++) {
      disp[i] = 2*M_PI*i/L;
    }
    break;
  case 2:
    for (float i = xMin; i <= xMax; i += (xMax-xMin)/accuracy) {
      if ((3*i-3) < 0) dispGraph->add(i, 0);
      else dispGraph->add(i, 3*i-3);
    }
    // ��������� ������ �� �������
    for (int i = 0; i < 4096; i++) {
      disp[i] = 3*(2*M_PI*i/L)-3;
      if (disp[i] < 0) disp[i] = 0;
    }
    break;
  case 3:
    for (float i = xMin; i <= xMax; i += (xMax-xMin)/accuracy) {
      dispGraph->add(i, 0.5*i+3);
    }
    // ��������� ������ �� �������
    for (int i = 0; i < 4096; i++) {
      disp[i] = 0.5*(2*M_PI*i/L)+3;
    }
    break;
  case 4:
    for (float i = xMin; i <= xMax; i += (xMax-xMin)/accuracy) {
      if ((-i+20) < 0) dispGraph->add(i, 0);
      else dispGraph->add(i, -i+20);
    }
    // ��������� ������ �� �������
    for (int i = 0; i < 4096; i++) {
      disp[i] = -(2*M_PI*i/L)+20;
      if (disp[i] < 0) disp[i] = 0;
    }
    break;
  case 5:
    for (float i = xMin; i <= xMax; i += (xMax-xMin)/accuracy) {
      dispGraph->add(i, 20.0*tanh(0.008*i*i));
    }
    // ��������� ������ �� �������
    for (int i = 0; i < 4096; i++) {
      disp[i] = 20.0*tanh(0.008*(2*M_PI*i/L)*(2*M_PI*i/L));
    }
    break;
  };
  chart->update();
}

// }}}

// {{{ ������ ������� � ��������� ��������

// ������ ����������� � ������� ��������� � ����� � (��������� ��������)
// � ������, ����������� � � (0, 0)
void Less2::drawVelocities(double x)
{
  // �������� ���������� ����������� �������� �����
  double accuracy = 400.0*Less1::perfomance;
  double delta = 4*kmaxEdit->text().toInt()/accuracy;
  double y0 = dispGraph->getYValue(x);
  double y1 = dispGraph->getYValue(x-delta);
  double y2 = dispGraph->getYValue(x+delta);
  // �������������� ������
  groupVelGraph->clear();
  phaseVelGraph->clear();
  int xMin = 0;
  int xMax = kmaxEdit->text().toInt();
  // ������ ������
  // y=(y1-y2)*(x-x1)/(x1-x2)+y1
  // x1, x2, y1, y2 - �����, ����� ������� �������� ������
  // x, y - �����, ����� ������� ����� �������� ������
  groupVelGraph->add(xMin, (y1-y2)*(xMin-x+delta)/(-2.0*delta)+y1);
  groupVelGraph->add(xMax, (y1-y2)*(xMax-x+delta)/(-2.0*delta)+y1);
  phaseVelGraph->add(0, 0);
  phaseVelGraph->add(x, y0);
  phaseVelGraph->add(xMax, (0-y2)*(xMax-0)/(0-x-delta)+0);
  chart->update();
  // ���������� �������������� �������� ������� � ��������� ���������
  groupVelLCD->display((y0-((y1-y2)*(xMin-x+delta)/(-2.0*delta)+y1))/x);
  phaseVelLCD->display(y0/x);
}

// }}}

// {{{ ��������� �� ������� ������ "���������"

// ��������� �� ������� ������ "���������"
// ������� ����� ���� ������ :)
void Less2::applySett()
{
  double accuracy = 400.0*Less1::perfomance;
  // ������ �������� ����
  chart->setXRange(0, kmaxEdit->text().toInt());
  chart->setYRange(0, wmaxEdit->text().toInt());

  // ���� ��������� �������� ������������� ����������
  if (gBox2->isChecked()) {
    dispGraph->clear();
    // ���� ������ �������� �������� ���������, �� ...
    try {
      myParser parser;
      parser.SetExpr(analEdit->text().replace("k", "0").toAscii().data());
      double tmp = parser.Eval();
    }
    catch(mu::ParserError &e) {
      QMessageBox::critical(0, translate("������ ������� �������!"),
			    "�������� ������ ��� ������� ���������, ������������� ����������.");
      return;
    }
    // �������� �� ���� �������� ������� � ������ �����������
    myParser parser;
    for (double i = 0; i <= kmaxEdit->text().toInt(); i += kmaxEdit->text().toInt()/accuracy) {
      QString expr = QString("%1").arg(i, 0, 'f', 3);
      parser.SetExpr(analEdit->text().replace("k", expr).toAscii().data());
      float w = parser.Eval();
      if (w < 0) w = 0;
      dispGraph->add(i, w);
    }
    // ��������� ������ �� �������
    float L = 10;
    for (int i = 0; i < 4096; i++) {
      QString expr = QString("%1").arg(2*M_PI*i/L, 0, 'f', 3);
      parser.SetExpr(analEdit->text().replace("k", expr).toAscii().data());
      disp[i] = parser.Eval();
      if (disp[i] < 0) disp[i] = 0;
    }
  }
  chart->setTitle("������������� �������������� <b>w(k)="+analEdit->text()+"</b>.");
  dispString = analEdit->text();
  chart->update();
}

// }}}

// {{{ �����������

// ������ ������ ������� �����
Less2::Less2(QWidget *)
{
  // ������� ����������� �������
  gBox1 = new QGroupBox(translate("�������"));
  gBox2 = new QGroupBox(translate("������. ���. ����."));
  disp1But = new QPushButton(translate("��� ���������"));
  disp2But = new QPushButton(translate("�����. ����. > ���."));
  disp3But = new QPushButton(translate("�����. ����. < ���."));
  disp4But = new QPushButton(translate("�����. ����. <-> ���."));
  disp5But = new QPushButton(translate("��������� ���������"));
  mainLayout = new QHBoxLayout();
  leftLayout = new QVBoxLayout();
  gLayout1 = new QVBoxLayout();
  gLayout2 = new QVBoxLayout();
  middleLayout = new QVBoxLayout();
  middleBottLayout = new QGridLayout();
  rightLayout = new QVBoxLayout();
  analLayout = new QHBoxLayout();
  chart = new ZChart();
  groupVelLabel = new QLabel(translate("��������� ��������, �/�:"));
  phaseVelLabel = new QLabel(translate("������� ��������, �/�:"));
  groupVelLCD = new QLCDNumber(4, this);
  phaseVelLCD = new QLCDNumber(4, this);
  analLabel = new QLabel(translate("w(k)="));
  analEdit = new QLineEdit("2*k-3");
  apply = new QPushButton(translate("���������"));
  kmaxLabel = new QLabel(translate("Kmax, 1/�"), this);
  wmaxLabel = new QLabel(translate("Wmax, ���/�"), this);
  kmaxEdit = new QLineEdit("20", this);
  wmaxEdit = new QLineEdit("20", this);

  // ����� tooltips � ��������
  disp1But->setToolTip(translate("� ���� ����� <b>���������</b> �����������,<br> �.�. <i>������� ��������</i> ����� ����� <i>���������</i>"));
  disp2But->setToolTip(translate("���� �� �������� <b>���������</b>, ��� ������� <br>������ ��������� ������������ �� ���������"));
  disp3But->setToolTip(translate("���� �� �������� <b>���������</b>, ��� ������� <br>������ ������� �� ������������ �� ��������"));
  disp4But->setToolTip(translate("� ������ ������ <i>���������</i> � <i>������� ��������</i><br> �����������������, �.�. <b>���������</b> ����������"));
  disp5But->setToolTip(translate("� �� ���� <i>������������� ��������������</i><br> ����������� ����� ��������� ����� <b>���������</b>"));
  groupVelLCD->setToolTip(translate("����� ������������ <i>��������� ��������</i> � ����� ��<br> <i>������������� ��������������</i>, ��� �� �������� ������"));
  phaseVelLCD->setToolTip(translate("����� ������������ <i>������� ��������</i> � ����� ��<br> <i>������������� ��������������</i>, ��� �� �������� ������"));
  kmaxEdit->setToolTip(translate("����� �������� ������������ �������� <i>��������� �����</i> <font color=green>k=2*pi/l</font><br> ����������� ��� ��� ��������� ��������� ��������� �������"));
  wmaxEdit->setToolTip(translate("����� �������� ������������ �������� <i>�������� �������</i> <font color=green>w=2*pi/T</font><br> ����������� ��� ��� ��������� ��������� ��������� �������"));
  apply->setToolTip(translate("����������� ��� ������ ��� ����������� �������,<br> ���� �� �������� �����-���� ���������"));
  gBox2->setToolTip(translate("�������� �����, ���� ���������� ������ <br><i>������������� ��������������</i> � ������������� ����"));
  analEdit->setToolTip(translate("�������� ����� ���������, �����������<br> ������ ��� <i>������������� ��������������</i><br>�� �������� ������ ������ \"���������\"!<br><i>��������� ����� �������� �� ��������� ��������:</i><br>������������ ����� � ������� ����� ����� �������� <b>�����</b>,<br> � �������� ���������� ����� ������������ <b>k</b>.<br> <i>����������� ��������� ��������:</i> <br><b>+</b>, <b>-</b>, <b>*</b>, <b>/</b>, <b>%</b>, <b>^</b>.<br> <i>����� ������������ ��������� �������:</i><br> <b>sin()</b>, <b>cos()</b>, <b>tg()</b>, <b>ctg()</b>, <b>arcsin()</b>, <b>arccos()</b>, <b>arctg()</b>, <b>arcctg()</b>,<br> <b>sh()</b>, <b>ch()</b>, <b>th()</b>, <b>cth()</b>, <b>exp()</b>, <b>lg()</b>, <b>ln()</b>, <b>sqrt()</b>."));

  // ��������� ������������� ����� ������ � �������
  QStyle *arthur = new ArthurStyle();
  apply->setStyle(arthur);
  disp1But->setStyle(arthur);
  disp2But->setStyle(arthur);
  disp3But->setStyle(arthur);
  disp4But->setStyle(arthur);
  disp5But->setStyle(arthur);

  QFont boldFont("Sans Serif", 10, QFont::Bold);
  boldFont.setPointSize(10);
  apply->setFont(boldFont);

  // ��������� ���������
  connect(apply, SIGNAL(clicked()),
	  this, SLOT(applySett()));

  // �������������, ��� ����� ����������� ��� ������� �� ���������
  gBox2->setCheckable(true);
  gBox2->setChecked(false);
  apply->setEnabled(false);
  connect(gBox2, SIGNAL(toggled(bool)),
	  analEdit, SLOT(setEnabled(bool)));
  connect(gBox2, SIGNAL(toggled(bool)),
	  apply, SLOT(setEnabled(bool)));

  // ����������� �������, ������������ ������� � ��������� ��������
  groupVelLCD->setSmallDecimalPoint(true);
  groupVelLCD->setSegmentStyle(QLCDNumber::Flat);
  phaseVelLCD->setSmallDecimalPoint(true);
  phaseVelLCD->setSegmentStyle(QLCDNumber::Flat);

  // ����������� �������
  chart->setBorderType(0);
  chart->setGraphPanelBorderType(0);
  chart->setMargin(2);
  chart->setSpacing(2);
  chart->setFirstText("");
  chart->setSecondText("�������� �� ������� ��� ��������� ������� � ��������� ���������");
  chart->setTitle("������������� ��������������.");
  chart->setXRange(0, 10);
  chart->setYRange(0, 10);
  chart->setAxesName("k, 1/�", "w, ���/�");
  chart->showLegend(true);
  chart->setAutoscale(true);

  // ������� ����� ������
  dispGraph = new ZSimpleSeries("����. ���-��");
  dispGraph->setColor(Qt::blue);
  chart->addSeries(dispGraph);
  groupVelGraph = new ZSimpleSeries("�����. ��-��");
  groupVelGraph->setColor(Qt::red);
  chart->addSeries(groupVelGraph);
  phaseVelGraph = new ZSimpleSeries("���. ��-��");
  phaseVelGraph->setColor(Qt::magenta);
  chart->addSeries(phaseVelGraph);

  // ��� ����� �� ������� �������� ����� ������� � ��������� ���������
  connect(chart, SIGNAL(clickCoordX(double)),
	  this, SLOT(drawVelocities(double)));

  // ����������� �����
  QValidator *validator = new QIntValidator(0, 500, this);
  analEdit->setMaxLength(256);
  analEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  analEdit->setEnabled(false);
  kmaxEdit->setMaxLength(7);
  kmaxEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  kmaxEdit->setValidator(validator);
  wmaxEdit->setMaxLength(7);
  wmaxEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  wmaxEdit->setValidator(validator);

  // ����������� ������, �������� ��������� ���������
  disp1But->setCheckable(true);
  disp2But->setCheckable(true);
  disp3But->setCheckable(true);
  disp4But->setCheckable(true);
  disp5But->setCheckable(true);
  disp1But->setAutoExclusive(true);
  disp2But->setAutoExclusive(true);
  disp3But->setAutoExclusive(true);
  disp4But->setAutoExclusive(true);
  disp5But->setAutoExclusive(true);

  connect(disp1But, SIGNAL(clicked()), this, SLOT(drawDisp()));
  connect(disp2But, SIGNAL(clicked()), this, SLOT(drawDisp()));
  connect(disp3But, SIGNAL(clicked()), this, SLOT(drawDisp()));
  connect(disp4But, SIGNAL(clicked()), this, SLOT(drawDisp()));
  connect(disp5But, SIGNAL(clicked()), this, SLOT(drawDisp()));

  // ����������� ����� �����
  gLayout1->addWidget(disp1But);
  gLayout1->addWidget(disp2But);
  gLayout1->addWidget(disp3But);
  gLayout1->addWidget(disp4But);
  gLayout1->addWidget(disp5But);
  gLayout1->setMargin(1);
  gBox1->setLayout(gLayout1);

  leftLayout->addStretch();
  leftLayout->addWidget(gBox1);
  leftLayout->addStretch();

  // ����������� ����������� �������
  middleBottLayout->addWidget(phaseVelLabel, 0, 0);
  middleBottLayout->addWidget(phaseVelLCD, 0, 1);
  middleBottLayout->addWidget(groupVelLabel, 1, 0);
  middleBottLayout->addWidget(groupVelLCD, 1, 1);
  middleBottLayout->addWidget(kmaxLabel, 0, 4);
  middleBottLayout->addWidget(kmaxEdit, 0, 5);
  middleBottLayout->addWidget(wmaxLabel, 1, 4);
  middleBottLayout->addWidget(wmaxEdit, 1, 5);
  middleBottLayout->setMargin(1);
  middleLayout->addWidget(chart);
  middleLayout->addLayout(middleBottLayout);
  middleLayout->setMargin(1);

  // ����� ��������� �������� � ������������� ����
  analLayout->addWidget(analLabel);
  analLayout->addWidget(analEdit);
  analLayout->addStretch();

  // ����������� ������ �������
  gLayout2->addLayout(analLayout);
  gLayout2->addWidget(apply);
  gLayout2->setMargin(1);
  gBox2->setLayout(gLayout2);

  rightLayout->addStretch();
  rightLayout->addWidget(gBox2);
  rightLayout->addStretch();

  // � ������ ��� ������!
  mainLayout->addLayout(leftLayout);
  mainLayout->addLayout(middleLayout);
  mainLayout->addLayout(rightLayout);
  mainLayout->setStretchFactor(middleLayout, 1);
  mainLayout->setMargin(1);

  setLayout(mainLayout);

  // ������� �� ������ � �������� ��� ���������
  disp1But->toggle();
  drawDisp();
}

// }}}
