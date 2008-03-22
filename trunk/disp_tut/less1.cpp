// Time-stamp: <less1.cpp - 15:17:22 21.05.2006>
/************************************************************************
 ***            ��� ������ ���� ��������� �� ���������                ***
 ************************************************************************/

/* code: ���������� ����� <zorg1331@gmail.com> */

#include <QtGui>

#include "less1.h"
#include <translate.h>
#include <arthurwidgets.h>

float Less1::perfomance = .5;

// {{{ �����������

// ������ ������� �����, ������ ��������
Less1::Less1(QWidget *)
{
  gBox = new QGroupBox(translate("&������-�� ������"), this);
  gBox1 = new QGroupBox(translate("&������� �������"), this);
  perf1But = new QPushButton(translate("������"));
  perf2But = new QPushButton(translate("�������"));
  perf3But = new QPushButton(translate("�������"));
  perf4But = new QPushButton(translate("����� �������"));
  rightLayout = new QVBoxLayout();
  leftLayout = new QVBoxLayout();
  mainLayout = new QHBoxLayout();
  gLayout = new QVBoxLayout();
  gLayout1 = new QVBoxLayout();

  // ��������� ������������� ����� ������ � �������
  QStyle *arthur = new ArthurStyle();
  perf1But->setStyle(arthur);
  perf2But->setStyle(arthur);
  perf3But->setStyle(arthur);
  perf4But->setStyle(arthur);

  // ����������� ������
  perf1But->setCheckable(true);
  perf2But->setCheckable(true);
  perf3But->setCheckable(true);
  perf4But->setCheckable(true);
  perf1But->setAutoExclusive(true);
  perf2But->setAutoExclusive(true);
  perf3But->setAutoExclusive(true);
  perf4But->setAutoExclusive(true);

  perf2But->setChecked(true);

  connect(perf1But, SIGNAL(clicked()), this, SLOT(setPerf()));
  connect(perf2But, SIGNAL(clicked()), this, SLOT(setPerf()));
  connect(perf3But, SIGNAL(clicked()), this, SLOT(setPerf()));
  connect(perf4But, SIGNAL(clicked()), this, SLOT(setPerf()));

  QString help = "\
   ��� ������� ������������ ��������� �������:<br>\
   ��� ������������ ��������� ������, �������<br>\
   ������� ������������ � �������� ���������.<br>\
   ������ ���� �����ޣ� � ��������� �������.<br>\
   � ������ ��� ������������� ������������<br>\
   � ������� ��������� � �����, ������� <br>\
   ������������� � ������ ����� ������ �������.<br>\
   �����, ������ �� ������������ � ��������,<br>\
   ���������� ������ �����-���� ���������.<br>\
   ����� ��������� ����� �������, ��� ������<br>\
   ��������� ���������� ������ �����������.<br>\
   � � ����� ��� ������ ������������ �<br>\
   ������������ ����, ��� ����������.<br>\
   ����� ����, ��������� ������ �����<br>\
   ����� ������������ �������� ������������<br>\
   ���������� ������ � ���������.<br>\
   ��� �� � ��� ����� ����������� ������ ����.<br>\
   ������ ��� ����� ����� ������ ���� ���.<br>\
   �������, �� ������������ ������ � ����������<br>\
   ������ �������. ;)<br>\
  ";

  helpLabel = new QLabel(translate(help));
  QFont smallFont("Sans Serif", 8, QFont::Normal);
  smallFont.setPointSize(8);
  helpLabel->setFont(smallFont);

  // ����������� ������ �����
  gLayout->addWidget(perf1But);
  gLayout->addWidget(perf2But);
  gLayout->addWidget(perf3But);
  gLayout->addWidget(perf4But);
  gLayout->setMargin(1);
  gBox->setLayout(gLayout);

  rightLayout->addStretch();
  rightLayout->addWidget(gBox);
  rightLayout->addStretch();

  // ����������� ����� �����
  gLayout1->addWidget(helpLabel);
  gLayout1->setMargin(0);
  gBox1->setLayout(gLayout1);

  leftLayout->addStretch();
  leftLayout->addWidget(gBox1);
  leftLayout->addStretch();

  mainLayout->addLayout(leftLayout);
  mainLayout->addStretch();
  mainLayout->addLayout(rightLayout);
  mainLayout->setMargin(0);

  setLayout(mainLayout);
}

// }}}

// ������������� �������� ������������������ ������
void Less1::setPerf()
{
  if (perf1But->isChecked()) {
    perfomance = 0.25;
  } else if (perf2But->isChecked()) {
    perfomance = 0.5;
  } else if (perf3But->isChecked()) {
    perfomance = 0.75;
  } else if (perf4But->isChecked()) {
    perfomance = 1.0;
  }
}

// ��� ������� ��������� ������ ��������
void Less1::paintEvent(QPaintEvent * /*event*/)
{
  QPainter painter(this);
  QImage image = QImage(":/images/main_image.jpg");
  painter.drawImage(QRectF((width()-image.width())/2.0,
			   (height()-image.height())/2.0,
			   image.width(), image.height()), image);
}
