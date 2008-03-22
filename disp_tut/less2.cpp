// Time-stamp: <less2.cpp - 20:20:30 15.04.2006>
/************************************************************************
 ***             Это второй урок туториала по дисперсии               ***
 ************************************************************************/

/* code: Наконечный Павел <zorg1331@gmail.com> */

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

// Инициализируем статические переменные
float Less2::disp[4096] = {0, 0};
QString Less2::dispString = "";

// {{{ рисуем дисперсионную характеристику

// Рисуем дисперсионную характеристику
void Less2::drawDisp()
{
  // Раз уж выбрали стандартный пример, то аналитическое задание дисперсии отключаем
  gBox2->setChecked(false);
  // Выясняем, какой из примеров задан
  int type;
  QString sType;
  if (disp1But->isChecked()) {
    type = 1;
    sType = "k</b>. Дисперсии нет.";
    dispString = "k</b>";
  }
  if (disp2But->isChecked()) {
    type = 2;
    sType = "3*k-3</b>. Групповая скорость больше фазовой.";
    dispString = "3*k-3</b>";
  }
  if (disp3But->isChecked()) {
    type = 3;
    sType = "0.5*k+3</b>. Групповая скорость меньше фазовой.";
    dispString = "0.5*k+3</b>";
  }
  if (disp4But->isChecked()) {
    type = 4;
    sType = "-k+20</b>. Групповая скорость противонаправлена фазовой.";
    dispString = "-k+20</b>";
  }
  if (disp5But->isChecked()) {
    type = 5;
    sType = "20*tanh(0.008*k*k)</b>. Смешанная зависимость.";
    dispString = "20*tanh(0.008*k*k)</b>";
  }

  // Инициализируем график
  dispGraph->clear();
  groupVelGraph->clear();
  phaseVelGraph->clear();
  int xMin = 0;
  int xMax = kmaxEdit->text().toInt();
  chart->setXRange(xMin, xMax);
  chart->setYRange(xMin, xMax);
  chart->setTitle("Дисперсионная характеристика: <b>w(k)=" + sType);

  // В зависимости от выбранного примера, строим дисперсионную характеристику
  // Массив на экспорт заполняется следующим образом:
  // в дисперсионную характеристику, заданную, фактически, аналитически,
  // подставляется число 2*M_PI*i/L - волновое число i-й гармоники
  // после подстановки получаем круговую частоту, соответствующую этому волновому числу
  float L = 10;
  double accuracy = 400*Less1::perfomance;
  switch (type) {
  case 1:
    for (float i = xMin; i <= xMax; i += (xMax-xMin)/accuracy) {
      dispGraph->add(i, i);
    }
    // Заполняем массив на экспорт
    for (int i = 0; i < 4096; i++) {
      disp[i] = 2*M_PI*i/L;
    }
    break;
  case 2:
    for (float i = xMin; i <= xMax; i += (xMax-xMin)/accuracy) {
      if ((3*i-3) < 0) dispGraph->add(i, 0);
      else dispGraph->add(i, 3*i-3);
    }
    // Заполняем массив на экспорт
    for (int i = 0; i < 4096; i++) {
      disp[i] = 3*(2*M_PI*i/L)-3;
      if (disp[i] < 0) disp[i] = 0;
    }
    break;
  case 3:
    for (float i = xMin; i <= xMax; i += (xMax-xMin)/accuracy) {
      dispGraph->add(i, 0.5*i+3);
    }
    // Заполняем массив на экспорт
    for (int i = 0; i < 4096; i++) {
      disp[i] = 0.5*(2*M_PI*i/L)+3;
    }
    break;
  case 4:
    for (float i = xMin; i <= xMax; i += (xMax-xMin)/accuracy) {
      if ((-i+20) < 0) dispGraph->add(i, 0);
      else dispGraph->add(i, -i+20);
    }
    // Заполняем массив на экспорт
    for (int i = 0; i < 4096; i++) {
      disp[i] = -(2*M_PI*i/L)+20;
      if (disp[i] < 0) disp[i] = 0;
    }
    break;
  case 5:
    for (float i = xMin; i <= xMax; i += (xMax-xMin)/accuracy) {
      dispGraph->add(i, 20.0*tanh(0.008*i*i));
    }
    // Заполняем массив на экспорт
    for (int i = 0; i < 4096; i++) {
      disp[i] = 20.0*tanh(0.008*(2*M_PI*i/L)*(2*M_PI*i/L));
    }
    break;
  };
  chart->update();
}

// }}}

// {{{ рисуем фазовую и групповую скорости

// Рисуем касательную к графику дисперсии в точке х (групповая скорость)
// и прямую, соединяющую х и (0, 0)
void Less2::drawVelocities(double x)
{
  // Выясняем координаты окрестности заданной точки
  double accuracy = 400.0*Less1::perfomance;
  double delta = 4*kmaxEdit->text().toInt()/accuracy;
  double y0 = dispGraph->getYValue(x);
  double y1 = dispGraph->getYValue(x-delta);
  double y2 = dispGraph->getYValue(x+delta);
  // Инициализируем график
  groupVelGraph->clear();
  phaseVelGraph->clear();
  int xMin = 0;
  int xMax = kmaxEdit->text().toInt();
  // Строим прямые
  // y=(y1-y2)*(x-x1)/(x1-x2)+y1
  // x1, x2, y1, y2 - точки, через которые проходит прямая
  // x, y - точка, через которую хотим провести прямую
  groupVelGraph->add(xMin, (y1-y2)*(xMin-x+delta)/(-2.0*delta)+y1);
  groupVelGraph->add(xMax, (y1-y2)*(xMax-x+delta)/(-2.0*delta)+y1);
  phaseVelGraph->add(0, 0);
  phaseVelGraph->add(x, y0);
  phaseVelGraph->add(xMax, (0-y2)*(xMax-0)/(0-x-delta)+0);
  chart->update();
  // Показываем соответствущие значения фазовой и групповой скоростей
  groupVelLCD->display((y0-((y1-y2)*(xMin-x+delta)/(-2.0*delta)+y1))/x);
  phaseVelLCD->display(y0/x);
}

// }}}

// {{{ Реагируем на нажатие кнопки "Применить"

// Реагируем на нажатие кнопки "Применить"
// Убиваем сразу кучу зайцев :)
void Less2::applySett()
{
  double accuracy = 400.0*Less1::perfomance;
  // Задаем диапазон осей
  chart->setXRange(0, kmaxEdit->text().toInt());
  chart->setYRange(0, wmaxEdit->text().toInt());

  // Если дисперсия задается аналитическим выражением
  if (gBox2->isChecked()) {
    dispGraph->clear();
    // Если парсер разобрал заданное выражение, то ...
    try {
      myParser parser;
      parser.SetExpr(analEdit->text().replace("k", "0").toAscii().data());
      double tmp = parser.Eval();
    }
    catch(mu::ParserError &e) {
      QMessageBox::critical(0, translate("Ошибка расчета формулы!"),
			    "Возникла ошибка при разборе выражения, перепроверьте написанное.");
      return;
    }
    // проходим по всей заданной области и строим зависимость
    myParser parser;
    for (double i = 0; i <= kmaxEdit->text().toInt(); i += kmaxEdit->text().toInt()/accuracy) {
      QString expr = QString("%1").arg(i, 0, 'f', 3);
      parser.SetExpr(analEdit->text().replace("k", expr).toAscii().data());
      float w = parser.Eval();
      if (w < 0) w = 0;
      dispGraph->add(i, w);
    }
    // Заполняем массив на экспорт
    float L = 10;
    for (int i = 0; i < 4096; i++) {
      QString expr = QString("%1").arg(2*M_PI*i/L, 0, 'f', 3);
      parser.SetExpr(analEdit->text().replace("k", expr).toAscii().data());
      disp[i] = parser.Eval();
      if (disp[i] < 0) disp[i] = 0;
    }
  }
  chart->setTitle("Дисперсионная характеристика <b>w(k)="+analEdit->text()+"</b>.");
  dispString = analEdit->text();
  chart->update();
}

// }}}

// {{{ конструктор

// Строим виджет второго урока
Less2::Less2(QWidget *)
{
  // Создаем необходимые виджеты
  gBox1 = new QGroupBox(translate("Примеры"));
  gBox2 = new QGroupBox(translate("Аналит. зад. дисп."));
  disp1But = new QPushButton(translate("Без дисперсии"));
  disp2But = new QPushButton(translate("Групп. скор. > фаз."));
  disp3But = new QPushButton(translate("Групп. скор. < фаз."));
  disp4But = new QPushButton(translate("Групп. скор. <-> фаз."));
  disp5But = new QPushButton(translate("Смешанная дисперсия"));
  mainLayout = new QHBoxLayout();
  leftLayout = new QVBoxLayout();
  gLayout1 = new QVBoxLayout();
  gLayout2 = new QVBoxLayout();
  middleLayout = new QVBoxLayout();
  middleBottLayout = new QGridLayout();
  rightLayout = new QVBoxLayout();
  analLayout = new QHBoxLayout();
  chart = new ZChart();
  groupVelLabel = new QLabel(translate("Групповая скорость, м/с:"));
  phaseVelLabel = new QLabel(translate("Фазовая скорость, м/с:"));
  groupVelLCD = new QLCDNumber(4, this);
  phaseVelLCD = new QLCDNumber(4, this);
  analLabel = new QLabel(translate("w(k)="));
  analEdit = new QLineEdit("2*k-3");
  apply = new QPushButton(translate("Применить"));
  kmaxLabel = new QLabel(translate("Kmax, 1/м"), this);
  wmaxLabel = new QLabel(translate("Wmax, рад/с"), this);
  kmaxEdit = new QLineEdit("20", this);
  wmaxEdit = new QLineEdit("20", this);

  // Лепим tooltips к виджетам
  disp1But->setToolTip(translate("В этой среде <b>дисперсия</b> отсутствует,<br> т.е. <i>фазовая скорость</i> волны равна <i>групповой</i>"));
  disp2But->setToolTip(translate("Этим вы зададите <b>дисперсию</b>, при которой <br>группы опережают составляющие их гармоники"));
  disp3But->setToolTip(translate("Этим вы зададите <b>дисперсию</b>, при которой <br>группы отстают от составляющих их гармоник"));
  disp4But->setToolTip(translate("В данном случае <i>групповая</i> и <i>фазовая скорости</i><br> противонаправлены, т.е. <b>дисперсия</b> аномальная"));
  disp5But->setToolTip(translate("А на этой <i>дисперсионной характеристике</i><br> встречается сразу несколько видов <b>дисперсии</b>"));
  groupVelLCD->setToolTip(translate("Здесь показывается <i>групповая скорость</i> в точке на<br> <i>дисперсионной характеристике</i>, где вы кликнули мышкой"));
  phaseVelLCD->setToolTip(translate("Здесь показывается <i>фазовая скорость</i> в точке на<br> <i>дисперсионной характеристике</i>, где вы кликнули мышкой"));
  kmaxEdit->setToolTip(translate("Здесь задается максимальное значение <i>волнового числа</i> <font color=green>k=2*pi/l</font><br> используйте это для изменения диапазона отрисовки графика"));
  wmaxEdit->setToolTip(translate("Здесь задается максимальное значение <i>круговой частоты</i> <font color=green>w=2*pi/T</font><br> используйте это для изменения диапазона отрисовки графика"));
  apply->setToolTip(translate("Используйте эту кнопку для перерисовки графика,<br> если вы изменили какие-либо параметры"));
  gBox2->setToolTip(translate("Щелкните здесь, если необходимо задать <br><i>дисперсионную характеристику</i> в аналитическом виде"));
  analEdit->setToolTip(translate("Напишите здесь уравнение, описывающее<br> нужную вам <i>дисперсионную характеристику</i><br>Не забудьте нажать кнопку \"Применить\"!<br><i>Уравнение можно задавать по следующим правилам:</i><br>разделителем целой и дробной части чисел является <b>точка</b>,<br> в качестве переменной нужно использовать <b>k</b>.<br> <i>Разрешаются следующие операции:</i> <br><b>+</b>, <b>-</b>, <b>*</b>, <b>/</b>, <b>%</b>, <b>^</b>.<br> <i>Можно использовать следующие функции:</i><br> <b>sin()</b>, <b>cos()</b>, <b>tg()</b>, <b>ctg()</b>, <b>arcsin()</b>, <b>arccos()</b>, <b>arctg()</b>, <b>arcctg()</b>,<br> <b>sh()</b>, <b>ch()</b>, <b>th()</b>, <b>cth()</b>, <b>exp()</b>, <b>lg()</b>, <b>ln()</b>, <b>sqrt()</b>."));

  // Применяем нестандартный стиль только к кнопкам
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

  // Применяем изменения
  connect(apply, SIGNAL(clicked()),
	  this, SLOT(applySett()));

  // Устанавливаем, что будет происходить при нажатии на группбокс
  gBox2->setCheckable(true);
  gBox2->setChecked(false);
  apply->setEnabled(false);
  connect(gBox2, SIGNAL(toggled(bool)),
	  analEdit, SLOT(setEnabled(bool)));
  connect(gBox2, SIGNAL(toggled(bool)),
	  apply, SLOT(setEnabled(bool)));

  // Настраиваем дисплеи, отображающие фазовую и групповую скорости
  groupVelLCD->setSmallDecimalPoint(true);
  groupVelLCD->setSegmentStyle(QLCDNumber::Flat);
  phaseVelLCD->setSmallDecimalPoint(true);
  phaseVelLCD->setSegmentStyle(QLCDNumber::Flat);

  // настраиваем плоттер
  chart->setBorderType(0);
  chart->setGraphPanelBorderType(0);
  chart->setMargin(2);
  chart->setSpacing(2);
  chart->setFirstText("");
  chart->setSecondText("кликните на графике для получения фазовой и групповой скоростей");
  chart->setTitle("Дисперсионная характеристика.");
  chart->setXRange(0, 10);
  chart->setYRange(0, 10);
  chart->setAxesName("k, 1/м", "w, рад/с");
  chart->showLegend(true);
  chart->setAutoscale(true);

  // Создаем серии данных
  dispGraph = new ZSimpleSeries("Дисп. хар-ка");
  dispGraph->setColor(Qt::blue);
  chart->addSeries(dispGraph);
  groupVelGraph = new ZSimpleSeries("Групп. ск-ть");
  groupVelGraph->setColor(Qt::red);
  chart->addSeries(groupVelGraph);
  phaseVelGraph = new ZSimpleSeries("Фаз. ск-ть");
  phaseVelGraph->setColor(Qt::magenta);
  chart->addSeries(phaseVelGraph);

  // При клике по графику строятся линии фазовой и групповой скоростей
  connect(chart, SIGNAL(clickCoordX(double)),
	  this, SLOT(drawVelocities(double)));

  // Настраиваем Едиты
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

  // Настраиваем кнопки, задающие примерную дисперсию
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

  // Выравниваем левую часть
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

  // Выравниваем центральные виджеты
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

  // Здесь дисперсия задается в аналитическом виде
  analLayout->addWidget(analLabel);
  analLayout->addWidget(analEdit);
  analLayout->addStretch();

  // Выравниваем правую область
  gLayout2->addLayout(analLayout);
  gLayout2->addWidget(apply);
  gLayout2->setMargin(1);
  gBox2->setLayout(gLayout2);

  rightLayout->addStretch();
  rightLayout->addWidget(gBox2);
  rightLayout->addStretch();

  // А теперь все вместе!
  mainLayout->addLayout(leftLayout);
  mainLayout->addLayout(middleLayout);
  mainLayout->addLayout(rightLayout);
  mainLayout->setStretchFactor(middleLayout, 1);
  mainLayout->setMargin(1);

  setLayout(mainLayout);

  // Кликаем по кнопке с примером без дисперсии
  disp1But->toggle();
  drawDisp();
}

// }}}
