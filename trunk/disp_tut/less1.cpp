// Time-stamp: <less1.cpp - 15:17:22 21.05.2006>
/************************************************************************
 ***            Это первый урок туториала по дисперсии                ***
 ************************************************************************/

/* code: Наконечный Павел <zorg1331@gmail.com> */

#include <QtGui>

#include "less1.h"
#include <translate.h>
#include <arthurwidgets.h>

float Less1::perfomance = .5;

// {{{ конструктор

// Виджет первого урока, просто картинка
Less1::Less1(QWidget *)
{
  gBox = new QGroupBox(translate("&Произв-ть машины"), this);
  gBox1 = new QGroupBox(translate("&Краткая справка"), this);
  perf1But = new QPushButton(translate("Слабая"));
  perf2But = new QPushButton(translate("Средняя"));
  perf3But = new QPushButton(translate("Хорошая"));
  perf4But = new QPushButton(translate("Очень хорошая"));
  rightLayout = new QVBoxLayout();
  leftLayout = new QVBoxLayout();
  mainLayout = new QHBoxLayout();
  gLayout = new QVBoxLayout();
  gLayout1 = new QVBoxLayout();

  // Применяем нестандартный стиль только к кнопкам
  QStyle *arthur = new ArthurStyle();
  perf1But->setStyle(arthur);
  perf2But->setStyle(arthur);
  perf3But->setStyle(arthur);
  perf4But->setStyle(arthur);

  // Настраиваем кнопки
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
   Это занятие организовано следующим образом:<br>\
   Вам предлагается несколько уроков, которые<br>\
   помогут ознакомиться с явлением дисперсии.<br>\
   Каждый урок заключён в отдельной вкладке.<br>\
   В первый раз рекомендуется ознакомиться<br>\
   с кратким описанием к уроку, которое <br>\
   располагается в нижней части каждой вкладки.<br>\
   Затем, исходя из рекомендаций в описании,<br>\
   попробуйте задать какие-либо параметры.<br>\
   Уроки построены таким образом, что каждый<br>\
   следующий использует данные предыдущего.<br>\
   И в конце все данные объединяются в<br>\
   демонстрации того, что получилось.<br>\
   Кроме того, последним уроком можно<br>\
   будет пронаблюдать красивое практическое<br>\
   применение знаний о дисперсии.<br>\
   Так же у вас будет возможность пройти тест.<br>\
   Пройти его можно будет только один раз.<br>\
   Поэтому, не заканчивайте работу с программой<br>\
   раньше времени. ;)<br>\
  ";

  helpLabel = new QLabel(translate(help));
  QFont smallFont("Sans Serif", 8, QFont::Normal);
  smallFont.setPointSize(8);
  helpLabel->setFont(smallFont);

  // Выравниваем правую часть
  gLayout->addWidget(perf1But);
  gLayout->addWidget(perf2But);
  gLayout->addWidget(perf3But);
  gLayout->addWidget(perf4But);
  gLayout->setMargin(1);
  gBox->setLayout(gLayout);

  rightLayout->addStretch();
  rightLayout->addWidget(gBox);
  rightLayout->addStretch();

  // выравниваем левую часть
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

// устанавливаем параметр производительности машины
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

// при событии отрисовки рисуем картинку
void Less1::paintEvent(QPaintEvent * /*event*/)
{
  QPainter painter(this);
  QImage image = QImage(":/images/main_image.jpg");
  painter.drawImage(QRectF((width()-image.width())/2.0,
			   (height()-image.height())/2.0,
			   image.width(), image.height()), image);
}
