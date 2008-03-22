// Time-stamp: <result.cpp - 18:31:27 16.04.2006>
/************************************************************************
 ***       Реализация виджета по просмотру результата прохождения     ***
 ***                   волны в среде с дисперсией                     ***
 ************************************************************************/

/* code: Наконечный Павел <zorg1331@gmail.com> */

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

// Инициализируем статические переменные
double Result::t = 0;

// {{{ перерисовываем волну

// Перерисовываем волну
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

// {{{ рисуем профиль волны в первый момент времени

// Рисуем профиль волны в первый момент времени
void Result::drawBegin(bool checked)
{
  if (checked) {
    waveGraph->clear();
    double L = 10;
    double accuracy = 400;
    // Пробегаем по оси расстояний
    for (double j = 0; j < L; j += L/accuracy) {
      // Отклонение от нуля
      double z = 0;
      // Проходим все частоты
      for (int i = 0; i < WaveSetup::N/2; i++ ) {
	// Момент времени, в который строится волна
	double t = 0;
	// z=A*cos(w*t-k*x+f), w=2*pi/T, k=k(w)
	double k = 2*M_PI*i/L;
	double w = DispSetup::disp[i];
	z += WaveSetup::spektrAmpl[i]*cos(w*t+k*j+WaveSetup::spektrPhase[i]);
      }
      waveGraph->add(j, z);
    }
    chart2D->setTitle("Эволюция волны <b>U(t)="+WaveSetup::waveString+"</b>"+" в среде с дисперсией <b>w(k)="+DispSetup::dispString+"</b>");
    chart2D->update();
  }
}

// }}}

// {{{ рисуем профиль волны в определенный момент времени

void Result::drawSpec()
{
  waveGraph->clear();
  double L = 10;
  double accuracy = 400;
  // Пробегаем по оси расстояний
  for (double j = 0; j < L; j += L/accuracy) {
    // Отклонение от нуля
    double z = 0;
    // Проходим все частоты
    for (int i = 0; i < WaveSetup::N/2; i++ ) {
      // Момент времени, в который строится волна
      double t = specTimeEdit->text().toFloat();
      // z=A*cos(w*t-k*x+f), w=2*pi/T, k=k(w)
      double k = 2*M_PI*i/L;
      double w = DispSetup::disp[i];
      z += WaveSetup::spektrAmpl[i]*cos(w*t+k*j+WaveSetup::spektrPhase[i]);
    }
    waveGraph->add(j, z);
  }
  chart2D->setTitle("Эволюция волны <b>U(t)="+WaveSetup::waveString+"</b>"+" в среде с дисперсией <b>w(k)="+DispSetup::dispString+"</b>");
  chart2D->update();
}

// }}}

// {{{ рисуем профиль пошагово

void Result::drawStep()
{
  waveGraph->clear();
  double L = 10;
  double accuracy = 400;
  // Пробегаем по оси расстояний
  for (double j = 0; j < L; j += L/accuracy) {
    // Отклонение от нуля
    double z = 0;
    // Проходим все частоты
    for (int i = 0; i < WaveSetup::N/2; i++ ) {
      // z=A*cos(w*t-k*x+f), w=2*pi/T, k=k(w)
      double k = 2*M_PI*i/L;
      double w = DispSetup::disp[i];
      z += WaveSetup::spektrAmpl[i]*cos(w*t+k*j+WaveSetup::spektrPhase[i]);
    }
    waveGraph->add(j, z);
  }
  chart2D->setTitle("Эволюция волны <b>U(t)="+WaveSetup::waveString+"</b>"+" в среде с дисперсией <b>w(k)="+DispSetup::dispString+"</b>");
  chart2D->update();
  // Момент времени, в который строится волна
  Result::t -= intervalEdit->text().toFloat();
}

// }}}

void Result::resetSlot()
{
  Result::t = 0;
  drawStep();
}

// {{{ рисуем анимацию

void Result::drawAnim()
{
  chart2D->setTitle("Эволюция волны <b>U(t)="+WaveSetup::waveString+"</b>"+" в среде с дисперсией <b>w(k)="+DispSetup::dispString+"</b>");
  double time = 0;
  int i = 0;
  double accuracy = 400;
  bool pointI_old = false, pointII_old = false;	// на предидущем шаге точки не находили
  double point1X_old = 0.0, point3X_old = 0.0; // координата точки, найденной на предидущем шаге
  double dz = 0.05; // относительно этой цифры ищем, где метить горб
  // имеется слот, который устанавливает эту переменную в false
  // это работает, т.к. в нужные моменты вызывается qApp->processEvents();
  stillAnim = true;
  while (stillAnim) {
    waveGraph->clear();
    markWave->clear();
    double L = 10;
    bool point1 = false; // первой точки пока еще нет
    double point1X = 0.0;
    bool point2 = false; // второй тем более
    double point2X = 0.0;
    bool point3 = false; // третьей точки пока еще нет
    double point3X = 0.0;
    bool point4 = false; // четвертой тем более
    double point4X = 0.0;
    double z_old = 0.0;		// значение отклонения для предидущей точки
    // Пробегаем по оси расстояний
    for (double j = 0; j < L; j += L/accuracy) {
      // Отклонение от нуля
      double z = 0.0;
      // Проходим все частоты
      for (int i = 0; i < WaveSetup::N/2; i++ ) {
	// z=A*cos(w*t-k*x+f), w=2*pi/T, k=k(w)
	double k = 2*M_PI*i/L;
	double w = DispSetup::disp[i];
	z += WaveSetup::spektrAmpl[i]*cos(w*time+k*j+WaveSetup::spektrPhase[i]);
      }
      // {{{ набор условий, определяющих где рисовать метки горбов      // Метим первый горб      if ((j > 0.3*L) || (pointI_old)) { // начинаем только с первой трети	if ((z_old < dz) && (z > dz)) { // прошли через виртуальный ноль	  if (!pointI_old) {	// если это первый раз	    if (point1 && !point2) { // первая точка уже есть	      if (fabs(j-point1X) > 0.01*L) { // если расстояние между найденными точками достаточно велико		point2 = true;		point2X = j;	      } else { // если точки оказываются слишком близко, то вторая становится первой		point1X = j;	      }	    } else if (!point1) { // если первой точки еще нет	      point1 = true;	      point1X = j;	    }	  } else {		// если точки уже находили	    if (point1 && !point2) { // первая точка уже есть	      if (fabs(j-point1X) > 0.01*L) { // если расстояние между найденными точками достаточно велико		point2 = true;		point2X = j;	      }	    } else if (!point1) { // если первой точки еще нет	      if (fabs(j-point1X_old) <= 2.0*L/accuracy) { // если найденная точка близко к предидущей		point1 = true;		point1X = j;	      }	    }	  }	}      }      // Метим второй горб      if ((j > 0.5*L) || (pointII_old)) { // начинаем только с первой трети	if ((z_old < -dz) && (z > -dz)) { // прошли через ноль	  if (!pointII_old) {	// если это первый раз	    if (point3 && !point4) { // первая точка уже есть	      if (fabs(j-point3X) > 0.002*L) { // если расстояние между найденными точками достаточно велико		point4 = true;		point4X = j;	      } else { // если точки оказываются слишком близко, то вторая становится первой		point3X = j;	      }	    } else if (!point3) { // если первой точки еще нет	      point3 = true;	      point3X = j;	    }	  } else {		// если точки уже находили	    if (point3 && !point4) { // первая точка уже есть	      if (fabs(j-point3X) > 0.002*L) { // если расстояние между найденными точками достаточно велико		point4 = true;		point4X = j;	      }	    } else if (!point3) { // если первой точки еще нет	      if (fabs(j-point3X_old) <= 2.0*L/accuracy) { // если найденная точка близко к предидущей		point3 = true;		point3X = j;	      }	    }	  }	}      }      // }}}
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

// {{{ рисуем 3хмерную водичку

void Result::draw3DSlot()
{
  if (draw3DBut->isChecked()) {
    indLCD->setVisible(true);
    indLabel->setVisible(true);
    indLabel->repaint();
    draw3DBut->setText(translate("2D вид"));
    glwidget->setVisible(true);
    chart2D->setVisible(false);
    chkZeroHour->setEnabled(false);
    chkSpecMoment->setEnabled(false);
    chkStepByStep->setEnabled(false);
    chkAnimation->setEnabled(false);
  } else {
    draw3DBut->setText(translate("3D вид"));
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

// {{{ конструктор

Result::Result()
{
  // Создаем необходимые виджеты
  groupBox = new QGroupBox(translate("Способ отрисовки"));
  chkZeroHour = new QRadioButton(translate("Начальный момент"));
  chkSpecMoment = new QRadioButton(translate("Конкретный момент"));
  chkStepByStep = new QRadioButton(translate("Пошагово"));
  chkAnimation = new QRadioButton(translate("Анимация"));
  specTimeLabel = new QLabel(translate("момент времени"));
  intervalLabel = new QLabel(translate("интервал"));
  animSpeedLabel = new QLabel(translate("скорость анимации"));
  specTimeEdit = new QLineEdit(translate("0"));
  intervalEdit = new QLineEdit(translate("0.1"));
  animSpeedEdit = new QLineEdit(translate("0.1"));
  redraw = new QPushButton(translate("Перерисовать"));
  nextStep = new QPushButton(translate("Следующий шаг"));
  reset = new QPushButton(translate("Сброс"));
  startAnim = new QPushButton(translate("Пуск!"));
  stopAnim = new QPushButton(translate("Остановить"));
  redrawWave = new QPushButton(translate("Обновить"));
  draw3DBut = new QPushButton(translate("3D вид"));
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
  indLabel = new QLabel(translate("Готовность:"));
  chart2D = new ZChart();
  glwidget = new GLWidget4();

  // Лепим tooltips к виджетам
  chkZeroHour->setToolTip(translate("Кликните сюда для изображения<br> волны в первый момент времени"));
  chkSpecMoment->setToolTip(translate("Таким способом можно посмотреть,<br> как будет выглядеть волна в<br> наперед заданный момент времени"));
  chkStepByStep->setToolTip(translate("Если нужно шаг за шагом изучить<br> эволюцию начального возмущения,<br> то этот чекбокс для вас"));
  chkAnimation->setToolTip(translate("Выберете это, если хотите пронаблюдать<br> за постепенно видоизменяющейся волной"));
  redrawWave->setToolTip(translate("Не забывайте нажимать эту кнопку, если вы<br> изменили параметры на предыдущих стадиях"));
  specTimeEdit->setToolTip(translate("Здесь можно задать конкретный момент времени,<br> в который необходимо изучить волну"));
  intervalEdit->setToolTip(translate("Здесь задается интервал,<br> через который будет строится волна"));
  animSpeedEdit->setToolTip(translate("Здесь задается скорость анимации, а фактически<br> интервал времени, через который будет строиться волна"));
  redraw->setToolTip(translate("Перерисовывает волну, имеет смысл нажимать,<br> если вы изменили момент времени"));
  draw3DBut->setToolTip(translate("Для большей наглядности воспользуйтесь возможностью<br> просмотра трехмерной картины распространения волн"));

  // Кнопка просмотра 3D вида толжна чекаться
  draw3DBut->setCheckable(true);

  QFont boldFont("Sans Serif", 10, QFont::Bold);
  boldFont.setPointSize(10);
  redrawWave->setFont(boldFont);

  // настраиваем LCD
  indLCD->setSmallDecimalPoint(true);
  indLCD->setSegmentStyle(QLCDNumber::Flat);

  connect(glwidget, SIGNAL(complete(double)),
	  this, SLOT(setPercents(double)));

  // Задаем начальные состояния
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

  // настраиваем плоттер
  chart2D->setBorderType(0);
  chart2D->setGraphPanelBorderType(0);
  chart2D->setMargin(2);
  chart2D->setSpacing(2);
  chart2D->setFirstText("");
  chart2D->setSecondText("");
  chart2D->setTitle("Эволюция волны в среде");
  chart2D->setXRange(0, WaveSetup::T);
  chart2D->setYRange(-4, 4);
  chart2D->setAxesName("x, м", "U, м");
  chart2D->showLegend(false);

  // Создаем серии данных
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

  // Рисуем то, что было в начале
  drawBegin(true);
}

// }}}
