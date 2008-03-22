// Time-stamp: <less5.cpp - 09:54:23 17.04.2006>
/************************************************************************
 ***             Это пятый урок туториала по дисперсии                ***
 ************************************************************************/

/* code: Наконечный Павел <zorg1331@mail.ru> */

#include <QtGui>
#include <QLabel>
#include <QPushButton>

#include "less5.h"
#include <math.h>
#include <zchart.h>
#include <parser.h>
#include <translate.h>
#include <arthurwidgets.h>
#include <knz_fft_cpp.h>
#include "less3.h"
#include "glwidget5.h"

// Инициализируем статические переменные
double Less5::spektrAmpl[4096] = {0, 0};
double Less5::spektrPhase[4096] = {0, 0};
double Less5::depth = 10.0;
double Less5::time = 0;

// Рисуем дисперсионную характеристику
void Less5::drawWater()
{
  // Выясняем, какой из примеров задан
  int type;
  QString sType;
  int xMin = 0;
  int xMax = 3;
  double yMin = -0.3;
  double yMax = 0.3;
  if (water1But->isChecked()) {
    type = 1;
    sType = "Капля в мелкую лужу.";
    yMin = -0.05;
    yMax = 0.05;
    param1Label->setText(translate("Радиус, м: 0.003"));
    param2Label->setText(translate("Высота, м: 0.025"));
    param3Label->setText(translate("Глубина, м: 0.01"));
  }
  if (water2But->isChecked()) {
    type = 2;
    sType = "Капля в глубокую лужу.";
    yMin = -0.05;
    yMax = 0.05;
    param1Label->setText(translate("Радиус, м: 0.003"));
    param2Label->setText(translate("Высота, м: 0.025"));
    param3Label->setText(translate("Глубина, м: 1"));
  }
  if (water3But->isChecked()) {
    type = 3;
    sType = "Камень в мелкую лужу.";
    yMin = -0.12;
    yMax = 0.12;
    param1Label->setText(translate("Радиус, м: 0.035"));
    param2Label->setText(translate("Высота, м: 0.1"));
    param3Label->setText(translate("Глубина, м: 0.05"));
  }
  if (water4But->isChecked()) {
    type = 4;
    sType = "Камень в озеро.";
    yMin = -0.12;
    yMax = 0.12;
    param1Label->setText(translate("Радиус, м: 0.035"));
    param2Label->setText(translate("Высота, м: 0.1"));
    param3Label->setText(translate("Глубина, м: 3"));
  }

  // Инициализируем график
  waterGraph->clear();
  chart->setXRange(xMin, xMax);
  chart->setYRange(yMin, yMax);
  chart->setTitle("Распространение волн на воде от упавшего предмета. " + sType);

  double i;
  int j;

  switch (type) {
  case 1:
    for (i = xMin, j = 0; j < Less3::N; i += (double)(xMax-xMin)/Less3::N, j++) {
      if (i >= 0 && i <= 0.003) {
	waterGraph->add(i, 0.025);
	s[j] = 0.025;
      } else {
	waterGraph->add(i, 0);
	s[j] = 0;
      }
    }
    depth = 0.01;
    break;
  case 2:
    for (i = xMin, j = 0; j < Less3::N; i += (double)(xMax-xMin)/Less3::N, j++) {
      if (i >= 0 && i <= 0.003) {
	waterGraph->add(i, 0.025);
	s[j] = 0.025;
      } else {
	waterGraph->add(i, 0);
	s[j] = 0;
      }
    }
    depth = 1;
    break;
  case 3:
    for (i = xMin, j = 0; j < Less3::N; i += (double)(xMax-xMin)/Less3::N, j++) {
      if (i >= 0 && i <= 0.035) {
	waterGraph->add(i, 0.1);
	s[j] = 0.1;
      } else {
	waterGraph->add(i, 0);
	s[j] = 0;
      }
    }
    depth = 0.05;
    break;
  case 4:
    for (i = xMin, j = 0; j < Less3::N; i += (double)(xMax-xMin)/Less3::N, j++) {
      if (i >= 0 && i <= 0.035) {
	waterGraph->add(i, 0.1);
	s[j] = 0.1;
      } else {
	waterGraph->add(i, 0);
	s[j] = 0;
      }
    }
    depth = 3;
    break;
  };
  chart->update();
  doFFT();
}

// Реагируем на нажатие кнопки "Применить"
void Less5::applySett()
{
  int xMin = 0, xMax = 3, j = 0;
  double i = 0, yMin = -2*heightEdit->text().toFloat(), yMax = 2*heightEdit->text().toFloat();
  // Инициализируем график
  waterGraph->clear();
  chart->setXRange(xMin, xMax);
  chart->setYRange(yMin, yMax);
  chart->setTitle("Параметры заданы вручную.");
  for (i = xMin, j = 0; j < Less3::N; i += (double)(xMax-xMin)/Less3::N, j++) {
    if (i >= 0 && i <= radiusEdit->text().toFloat()) {
      waterGraph->add(i, heightEdit->text().toFloat());
      s[j] = heightEdit->text().toFloat();
    } else {
      waterGraph->add(i, 0);
      s[j] = 0;
    }
  }
  depth = depthEdit->text().toFloat();
  chart->update();
  doFFT();
}

void Less5::doFFT()
{
  // Количество отсчетов
  int number = Less3::N;
  // Амплитуда текущей гармоники
  double Ak = 0;
  // Для определения максимальной амплитуды гармоники
  double maxAk = 0;
  // обнуляем массив с будущим результатом
  for (int i = 0; i < number; i++) data[i].re = data[i].im = 0;
  a = data;
  // Проводим преобразование Фурье заданного сигнала
  fft(Less3::power, s, &a);
  // Избавляемся от зеркального эффекта
  for (int i = 0; i < number/2; i++) {
    data[number-i].re = data[number-i].re-data[i].re;
    data[number-i].im = data[number-i].im+data[i].im;
  }
  // Рисуем спектр
  for (int i = 0; i < number/2; i++) {
    Ak = sqrt(data[i].re*data[i].re+data[i].im*data[i].im);
    // избавляемся от слишком малых величин
    data[i].re *= 1000000;
    if (data[i].re < 0.01 && data[i].re > -0.01) data[i].re = 0;
    data[i].re /= 1000000;
    // Запoлняем массив на экспорт
    spektrAmpl[i] = Ak;
    double Fk;
    if (data[i].re != 0) {
      if (data[i].re > 0) Fk = atan(data[i].im/data[i].re);
      else Fk = atan(data[i].im/data[i].re)+M_PI;
    } else {
      if (data[i].im > 0) Fk = -M_PI/2.0;
      if (data[i].im < 0) Fk = M_PI/2.0;
      if (data[i].im = 0) Fk = 0;
    }
    Fk += 2.0*M_PI*i/number;
    spektrPhase[i] = Fk;
    if (Ak > maxAk) maxAk = Ak;
  }
}

void Less5::animation()
{
  time = .0;
  int i = 0;
  double L = 3.0;
  // составим таблицу дисперсии заранее
  double disp_w[Less3::N];
  // если параметры задаются аналитически
  if (group2->isChecked()) {
    for (int i = 0; i < Less3::N/2; i++) {
      double k = 2.0*M_PI*i/L;
      double w = sqrt((k*9.8142+surfEdit->text().toFloat()*k*k*k/1000.0)*tanh(depth*k));
      disp_w[i] = w;
    }
  } else {
    for (int i = 0; i < Less3::N/2; i++) {
      double k = 2.0*M_PI*i/L;
      double w = sqrt((k*9.8142 + 0.0725*k*k*k/1000.0)*tanh(depth*k));
      disp_w[i] = w;
    }
  }
  while (i < 25) {
    if (!draw3DBut->isChecked()) {
      waterGraph->clear();
      // Пробегаем по оси расстояний
      for (double j = 0; j < L; j += 0.01) {
	// Отклонение от нуля
	double z = 0;
	// Проходим все частоты
	for (int i = 0; i < Less3::N/2; i++ ) {
	  // z=A*cos(w*t-k*x+f), w=2*pi/T, k=k(w)
	  // Для воды дисперсия: w=sqrt((k*9.8 + alpha*k*k*k/1000)*tanh(H*k))
	  // где alpha - вязкость
	  //     H - глубина
	  double k = 2.0*M_PI*i/L;
	  z += spektrAmpl[i]*cos(disp_w[i]*time+k*j+spektrPhase[i]);
	}
	waterGraph->add(j, z);
      }
      chart->repaint();
      time -= 0.1;
      emit timeChanged(time);
    } else {
      glwidget->initializeGL();
      glwidget->updateGL();
      emit timeChanged(time);
      time += 0.1;
    }
    i++;
  }
}

void Less5::draw3DSlot()
{
  if (draw3DBut->isChecked()) {
    draw3DBut->setText(translate("2D вид"));
    glwidget->setVisible(true);
    chart->setVisible(false);
  } else {
    draw3DBut->setText(translate("3D вид"));
    glwidget->setVisible(false);
    chart->setVisible(true);
  }
}

void Less5::timeChangedSlot(double _time)
{
  QString sTime = QString("%1").arg(sqrt(_time*_time), 0, 'f', 2);
  currTimeLCD->display(sTime);
  currTimeLCD->repaint();
}

// Строим виджет пятого урока
Less5::Less5(QWidget *)
{
  // Создаем необходимые виджеты
  group1 = new QGroupBox(translate("Примеры"));
  group2 = new QGroupBox(translate("Собственные пар-ры"));
  group3 = new QGroupBox(translate("Пар-ры возм."));
  water1But = new QPushButton(translate("Капля в лужу"));
  water2But = new QPushButton(translate("Капля в море"));
  water3But = new QPushButton(translate("Камень в лужу"));
  water4But = new QPushButton(translate("Камень в море"));
  mainLayout = new QHBoxLayout();
  leftLayout = new QVBoxLayout();
  group1Layout = new QVBoxLayout();
  group3Layout = new QVBoxLayout();
  middleLayout = new QVBoxLayout();
  rightLayout = new QVBoxLayout();
  analLayout = new QGridLayout();
  chart = new ZChart();
  surfLabel = new QLabel(translate("Поверхностное<br>натяж., 10^-3"));
  surfEdit = new QLineEdit("0.0725");
  depthLabel = new QLabel(translate("Глубина"));
  depthEdit = new QLineEdit("1");
  radiusLabel = new QLabel(translate("Радиус"));
  radiusEdit = new QLineEdit("0.01");
  heightLabel = new QLabel(translate("Высота"));
  heightEdit = new QLineEdit("0.1");
  param1Label = new QLabel(translate("Радиус, м: 0"));
  param2Label = new QLabel(translate("Высота, м: 0"));
  param3Label = new QLabel(translate("Глубина, м: 0"));
  apply = new QPushButton(translate("Применить"));
  drawAnim = new QPushButton(translate("Пуск анимации"));
  draw3DBut = new QPushButton(translate("3D вид"));
  glwidget = new GLWidget5();
  currTimeLCD = new QLCDNumber(4);
  currTimeLabel = new QLabel(translate("Текущее время"));
  currTimeLayout = new QHBoxLayout();

  // Лепим tooltips к виджетам
  water1But->setToolTip(translate("Это типичная ситуация во время дождя,<br>когда маленькие капли падают в маленькие лужи"));
  water2But->setToolTip(translate("Тоже самое, что и выше, но капли падают в озеро"));
  water3But->setToolTip(translate("Если во время дождя бросить камень в неглубокую лужу, то получится это"));
  water4But->setToolTip(translate("Сидите вы на берегу озера и бросаете камешки..."));
  apply->setToolTip(translate("Используйте эту кнопку для перерисовки графика,<br> если вы изменили какие-либо параметры"));
  group2->setToolTip(translate("Щелкните здесь, если хотите полностью контролировать процесс"));
  depthEdit->setToolTip(translate("Здесь задается глубина водоема"));
  radiusEdit->setToolTip(translate("Здесь задается радиус упавшего объекта"));
  heightEdit->setToolTip(translate("А здесь высота поднявшегося столба воды"));
  drawAnim->setToolTip(translate("Задав параметры, запускайте анимацию!"));

  // Применяем нестандартный стиль только к кнопкам
  QStyle *arthur = new ArthurStyle();
  apply->setStyle(arthur);
  water1But->setStyle(arthur);
  water2But->setStyle(arthur);
  water3But->setStyle(arthur);
  water4But->setStyle(arthur);
  drawAnim->setStyle(arthur);
  draw3DBut->setStyle(arthur);

  QFont boldFont("Sans Serif", 10, QFont::Bold);
  boldFont.setPointSize(10);
  apply->setFont(boldFont);
  drawAnim->setFont(boldFont);
  draw3DBut->setFont(boldFont);

  draw3DBut->setCheckable(true);
  glwidget->setVisible(false);
  connect(draw3DBut, SIGNAL(clicked()),
	  this, SLOT(draw3DSlot()));

  // настраиваем LCD
  currTimeLCD->setSmallDecimalPoint(true);
  currTimeLCD->setSegmentStyle(QLCDNumber::Flat);

  connect(this, SIGNAL(timeChanged(double)),
	  this, SLOT(timeChangedSlot(double)));

  // Применяем изменения
  connect(apply, SIGNAL(clicked()),
	  this, SLOT(applySett()));

  // Рисуем анимацию
  connect(drawAnim, SIGNAL(clicked()),
	  this, SLOT(animation()));

  // Устанавливаем, что будет происходить при нажатии на радиобаттон
  group2->setCheckable(true);
  group2->setChecked(false);

  connect(group2, SIGNAL(toggled(bool)),
	  surfEdit, SLOT(setEnabled(bool)));
  connect(group2, SIGNAL(toggled(bool)),
	  depthEdit, SLOT(setEnabled(bool)));
  connect(group2, SIGNAL(toggled(bool)),
	  radiusEdit, SLOT(setEnabled(bool)));
  connect(group2, SIGNAL(toggled(bool)),
	  heightEdit, SLOT(setEnabled(bool)));

  // настраиваем плоттер
  chart->setBorderType(0);
  chart->setGraphPanelBorderType(0);
  chart->setMargin(2);
  chart->setSpacing(2);
  chart->setFirstText("");
  chart->setSecondText("");
  chart->setTitle("Распространение волн на воде от упавшего предмета.");
  chart->setXRange(0, 3);
  chart->setYRange(-0.3, 0.3);

  // Создаем серии данных
  waterGraph = new ZSimpleSeries("Water waves");
  waterGraph->setColor(Qt::blue);
  chart->addSeries(waterGraph);

  // Настраиваем Едиты
  QValidator *validator = new QDoubleValidator(0, 500, 4, this);
  surfEdit->setMaxLength(256);
  surfEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  surfEdit->setValidator(validator);
  surfEdit->setEnabled(false);
  depthEdit->setValidator(validator);
  depthEdit->setMaxLength(256);
  depthEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  depthEdit->setEnabled(false);
  radiusEdit->setValidator(validator);
  radiusEdit->setMaxLength(256);
  radiusEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  radiusEdit->setEnabled(false);
  heightEdit->setValidator(validator);
  heightEdit->setMaxLength(256);
  heightEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  heightEdit->setEnabled(false);

  // Настраиваем кнопки, задающие примерные волны на воде
  water1But->setCheckable(true);
  water2But->setCheckable(true);
  water3But->setCheckable(true);
  water4But->setCheckable(true);
  water1But->setAutoExclusive(true);
  water2But->setAutoExclusive(true);
  water3But->setAutoExclusive(true);
  water4But->setAutoExclusive(true);

  connect(water1But, SIGNAL(clicked()), this, SLOT(drawWater()));
  connect(water2But, SIGNAL(clicked()), this, SLOT(drawWater()));
  connect(water3But, SIGNAL(clicked()), this, SLOT(drawWater()));
  connect(water4But, SIGNAL(clicked()), this, SLOT(drawWater()));

  currTimeLayout->addWidget(currTimeLabel);
  currTimeLayout->addWidget(currTimeLCD);

  // Выравниваем левую часть
  group1Layout->addWidget(water1But);
  group1Layout->addWidget(water2But);
  group1Layout->addWidget(water3But);
  group1Layout->addWidget(water4But);
  group1Layout->setMargin(1);
  group1->setLayout(group1Layout);

  group3Layout->addWidget(param1Label);
  group3Layout->addWidget(param2Label);
  group3Layout->addWidget(param3Label);
  group3Layout->setMargin(1);
  group3->setLayout(group3Layout);

  leftLayout->addStretch();
  leftLayout->addWidget(group1);
  leftLayout->addStretch();
  leftLayout->addWidget(group3);
  leftLayout->addStretch();
  leftLayout->setMargin(0);

  // Выравниваем центральные виджеты
  middleLayout->addWidget(chart);
  middleLayout->addWidget(glwidget);
  middleLayout->setMargin(0);

  // Здесь шлепок на воде задается в аналитическом виде
  analLayout->addWidget(surfLabel, 0, 0);
  analLayout->addWidget(surfEdit, 0, 1);
  analLayout->addWidget(depthLabel, 1, 0);
  analLayout->addWidget(depthEdit, 1, 1);
  analLayout->addWidget(radiusLabel, 2, 0);
  analLayout->addWidget(radiusEdit, 2, 1);
  analLayout->addWidget(heightLabel, 3, 0);
  analLayout->addWidget(heightEdit, 3, 1);
  analLayout->addWidget(apply, 4, 0, 1, 2);
  analLayout->setMargin(1);
  group2->setLayout(analLayout);

  // Выравниваем правую область
  rightLayout->addWidget(draw3DBut);
  rightLayout->addLayout(currTimeLayout);
  rightLayout->addStretch();
  rightLayout->addWidget(group2);
  rightLayout->addStretch();
  rightLayout->addWidget(drawAnim);
  rightLayout->addStretch();
  rightLayout->setMargin(0);

  // А теперь все вместе!
  mainLayout->addLayout(leftLayout);
  mainLayout->addLayout(middleLayout);
  mainLayout->addLayout(rightLayout);
  mainLayout->setStretchFactor(middleLayout, 1);
  mainLayout->setMargin(0);

  setLayout(mainLayout);

  // Кликаем по кнопке с примером без дисперсии
  water1But->toggle();
  drawWater();
}
