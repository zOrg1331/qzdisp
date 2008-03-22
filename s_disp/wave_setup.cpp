// Time-stamp: <wave_setup.cpp - 18:34:14 16.04.2006>
/*******************************************************************
 *** Реализация виджета по настройке начального возмущения среды ***
 *******************************************************************/

/* code: Наконечный Павел <zorg1331@gmail.com> */

#include <QtGui>
#include <QLabel>
#include <QPushButton>
#include <QLCDNumber>
#include <QTableWidget>

#include <math.h>
#include "wave_setup.h"
#include <zchart.h>
#include "drawdisparea.h"
#include <parser.h>
#include <translate.h>
#include <knz_fft_cpp.h>

// Инициализируем статические мемберы
double WaveSetup::T = 0;
int WaveSetup::power = 12, WaveSetup::N=4096;
double WaveSetup::spektrAmpl[4096] = {0, 0};
double WaveSetup::spektrPhase[4096] = {0, 0};
QString WaveSetup::waveString = "";

// {{{ Реагируем на нажатие кнопки "Применить"

// Реагируем на нажатие кнопки "Применить"
// Убиваем сразу кучу зайцев :)
void WaveSetup::applySett()
{
  // Период сигнала
  T = tmaxEdit->text().toFloat();
  // Счетчики
  int i = 0;
  double j = 0;
  // Если волна задается примерами, то кнопка Применить изменит только масштаб
  if (!chkManWave->isChecked() && !chkAnalWave->isChecked()
      && !chkSpektrWave->isChecked()) {
    chartWave->setXRange(0, T);
    chartWave->setYRange(uminEdit->text().toFloat(), umaxEdit->text().toFloat());
    chartWave->update();
  }
  // Если начальное возмущение задается рисованием мышкой
  if (chkManWave->isChecked()) {
    chartSpektr->setTitle("Спектр начального возмущения.");
    // Устанавливаем рабочий диапазон
    int xsMin = 0;
    int xsMax = drawwavearea->width();
    int ysMin = 0;
    int ysMax = drawwavearea->height();
    int xMax = tmaxEdit->text().toInt();
    int yMin = 0;//umaxEdit->text().toInt();
    int yMax = umaxEdit->text().toInt();
    int y = 0;
    // Координаты точки на графике
    double yG = 0;
    double xG = 0;
    waveGraph->clear();
    // Проходим по всей области рисования и переводим нарисованную
    // кривую в оцифрованную :)
    int x;
    for (i = 0, x = xsMin; x <= xsMax; x++, i++) {
      y = drawwavearea->getYValue(x);
      yG = yMin + (long double)(ysMax - y) / (long double)(ysMax - ysMin)
	* (long double)(yMax - yMin);
      xG = (long double)(x - xsMin) / (long double)(xsMax - xsMin)
	* (long double)(xMax - 0) + 0;
      if (y == 0) yG = 0;
      waveGraph->add(xG, yG);
      s[i] = yG;
    }
    while (i < N) { s[i] = 0; i++; }
    chartWave->setXRange(0, T);
    chartWave->setYRange(uminEdit->text().toFloat(), umaxEdit->text().toFloat());
    chartWave->update();
  }
  // Если начальное возмущение задается аналитическим выражением
  if (chkAnalWave->isChecked()) {
    chartSpektr->setTitle("Спектр начального возмущения. <b>U(t)="+analExprEdit->text()+"</b>");
    waveGraph->clear();
    // Если парсер разобрал заданное выражение, то ...
    try {
      myParser parser;
      parser.SetExpr(analExprEdit->text().replace("t", "0").toAscii().data());
      double tmp = parser.Eval();
    }
    catch(mu::ParserError &e) {
      QMessageBox::critical(0, translate("Ошибка расчета формулы!"),
			    "Возникла ошибка при разборе выражения, перепроверьте написанное.");
      return;
    }
    // проходим по всей заданной области и строим зависимость
    myParser parser;
    for (i = 0, j = 0; i < N; j += T/N, i++) {
      QString expr = QString("%1").arg(j, 0, 'f', 3);
      parser.SetExpr(analExprEdit->text().replace("t", expr).toAscii().data());
      waveGraph->add(j, parser.Eval());
      s[i] = parser.Eval();
    }
    chartWave->setXRange(0, T);
    chartWave->setYRange(uminEdit->text().toFloat(), umaxEdit->text().toFloat());
    chartWave->setTitle("Профиль начального возмущения. <b>U(t)="+analExprEdit->text()+"</b>");
    waveString = analExprEdit->text();
    chartWave->update();
  }
  // Если начальное возмущение задается в табличном режиме в виде спектра
  if (chkSpektrWave->isChecked()) {
    chartSpektr->setTitle("Спектр начального возмущения.");
    // Заполняем собственные массивы из таблицы с спектром волны
    // 30 - количество строк
    double ampl[30], freq[30], phase[30];
    for (i = 0; i < 30; i++) {
      ampl[i] = table->item(i, 0)->text().toFloat();
      freq[i] = table->item(i, 1)->text().toFloat();
      phase[i] = table->item(i, 2)->text().toFloat();
    }
    // Рисуем волну
    waveGraph->clear();
    for (j = 0, i = 0; i < N; j += T/N, i++) {
      double z = 0;
      for (int k = 0; k < 30; k++) {
	z += ampl[k]*cos(2*M_PI*j*freq[k] + phase[k]);
      }
      waveGraph->add(j, z);
      s[i] = z;
    }
    chartWave->setXRange(0, T);
    chartWave->setYRange(uminEdit->text().toFloat(), umaxEdit->text().toFloat());
    chartWave->update();
  }
  // Ниже напечатанные действия имеют смысл только при ручном,
  // аналитическом или табличном задании начального возмущения
  if (chkSpektrWave->isChecked() || chkAnalWave->isChecked()
      || chkManWave->isChecked()) {
    // Проводим преобразование Фурье
    doFFT();
  }
}

// }}}

// {{{ Рисуем спектр начального возмущения

// Рисуем спектр начального возмущения
void WaveSetup::drawWave()
{
  gBox2->setChecked(false);
  // Показываем только необходимые графики и виджеты
  viewRes->setVisible(false);
  chartSpektr->setVisible(false);
  chartWave->setVisible(true);
  drawwavearea->setVisible(false);
  clear->setVisible(false);
  viewWave->setVisible(true);
  if (viewWave->isChecked()) viewWave->setChecked(false);
  int type;
  QString sType;
  // Выясняем, какой из примеров задан
  if (wave1But->isChecked()) {
    type = 1;
    sType = "Синусоида. <b>U(t)=sin(pi*t)</b>";
    waveString = "sin(pi*t)";
  }
  if (wave2But->isChecked()) {
    type = 2;
    sType = "Биения. <b>U(t)=cos(pi*t/2)*cos(2*2*pi*t)</b>";
    waveString = "cos(pi*t/2)*cos(2*2*pi*t)";
  }
  if (wave3But->isChecked()) {
    type = 3;
    waveString = sType = "Импульс.";
  }
  if (wave4But->isChecked()) {
    type = 4;
    waveString = sType = "Широкий импульс.";
  }
  if (wave5But->isChecked()) {
    type = 5;
    waveString = sType = "Солитон.";
  }

  // Инициализируем график
  waveSpektrGraph->clear();
  waveGraph->clear();
  chartSpektr->setXRange(0, tmaxEdit->text().toFloat());
  chartSpektr->setYRange(0, umaxEdit->text().toFloat());
  chartSpektr->setTitle("Спектр начального возмущения. " + sType);
  chartWave->setTitle("Профиль начального возмущения при t=0. " + sType);

  // Период сигнала
  T = tmaxEdit->text().toFloat();
  // Счетчики
  int i = 0;
  double j = 0;

  // В зависимости от выбранного примера, строим спектр начального возмущения
  switch (type) {
  case 1:
    // Заготавливаем дискретизированный сигнал (синусоиду) и рисуем её
    for (i = 0, j = 0; i < N; i++, j += T/N) {
      s[i] = sin(M_PI*j);
      waveGraph->add(j, sin(M_PI*j));
    }
    break;
  case 2:
    // Задаем исходный сигнал (биения)
    for (j = 0, i = 0; i < N; j += T/N, i++) {
      waveGraph->add(j, cos(M_PI*j/2.0)*cos(2.0*2.0*M_PI*j));
      s[i] = cos(M_PI*j/2.0)*cos(2.0*2.0*M_PI*j);
    }
    break;
  case 3:
    // Задаем исходный сигнал (импульс)
    for (j = 0, i = 0; i < N; j += T/N, i++) {
      if (j > ((T/2.0)-0.001*T) && j < ((T/2.0)+0.001*T)) {
	waveGraph->add(j, 3);
	s[i] = 3;
      } else {
	waveGraph->add(j, 0);
	s[i] = 0;
      }
    }
    break;
  case 4:
    // Задаем исходный сигнал (широкий импульс)
    for (j = 0, i = 0; i < N; j += T/N, i++) {
      if (j > ((T/2.0)-0.3*T) && j < ((T/2.0)+0.3*T)) {
	waveGraph->add(j, 3);
	s[i] = 3;
      } else {
	waveGraph->add(j, 0);
	s[i] = 0;
      }
    }
    break;
  case 5:
    waveSpektrGraph->add(5, 9);
    waveSpektrGraph->add(5, 0);
    break;
  };
  // проводим преобразование Фурье
  doFFT();
}

// }}}

// {{{ осуществляем преобразование Фурье

void WaveSetup::doFFT()
{
  // Амплитуда текущей гармоники
  double Ak = 0;
  // Для определения максимальной амплитуды гармоники
  double maxAk = 0;
  // обнуляем массив с будущим результатом
  for (int i = 0; i < N; i++) data[i].re = data[i].im = 0;
  a = data;
  // Проводим преобразование Фурье заданного сигнала
  fft(power, s, &a);
  // Избавляемся от зеркального эффекта
  for (int i = 0; i < N/2; i++) {
    data[N-i].re = data[N-i].re-data[i].re;
    data[N-i].im = data[N-i].im+data[i].im;
  }
  // Рисуем спектр
  waveSpektrGraph->clear();
  for (int i = 0; i < N/2; i++) {
    Ak = sqrt(data[i].re*data[i].re+data[i].im*data[i].im);
    // избавляемся от слишком малых величин
    data[i].re *= 1000000;
    if (data[i].re < 0.01 && data[i].re > -0.01) data[i].re = 0;
    data[i].re /= 1000000;
    // Запoлняем массив на экспорт
    WaveSetup::spektrAmpl[i] = Ak;
    double Fk;
    if (data[i].re != 0) {
      if (data[i].re > 0) Fk = atan(data[i].im/data[i].re);
      else Fk = atan(data[i].im/data[i].re)+M_PI;
    } else {
      if (data[i].im > 0) Fk = -M_PI/2.0;
      if (data[i].im < 0) Fk = M_PI/2.0;
      if (data[i].im = 0) Fk = 0;
    }
    Fk += 2.0*M_PI*i/N;
    WaveSetup::spektrPhase[i] = Fk;
    if (Ak > maxAk) maxAk = Ak;
    if (Ak != 0) {
      waveSpektrGraph->add(i/T, 0);
      waveSpektrGraph->add(i/T, Ak);
      waveSpektrGraph->add(i/T, 0);
    }
  }
  chartSpektr->setXRange(0, T/2);
  chartSpektr->setYRange(0, maxAk);
  chartSpektr->update();
  chartWave->update();
  // Пример того, как надо восстанавливать сигнал
  /*
    waveSpektrGraph->add(0, 0);
    for (j = 0; j < T; j += T/N) {
    double z = 0;
    for (i = 0; i < N/2; i++) {
    double Ak = sqrt(data[i].re*data[i].re + data[i].im*data[i].im);
    double Fk;
    if (data[i].re != 0) {
    if (data[i].re > 0) {
    Fk = atan(data[i].im/data[i].re);
    } else Fk = atan(data[i].im/data[i].re)+M_PI;
    } else {
    if (data[i].im > 0) Fk = M_PI/2.0;
    if (data[i].im < 0) Fk = -M_PI/2.0;
    if (data[i].im = 0) Fk = 0;
    }
    Fk += 2.0*M_PI*i/N;
    z += Ak*cos(2.0*M_PI*i*j/T+Fk);
    }
    waveSpektrGraph->add(j, z);
    }
  */

}

// }}}

void WaveSetup::viewResSlot(bool checked)
{
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
}

void WaveSetup::viewWaveSlot(bool checked)
{
  if (checked) {
    chartWave->setVisible(false);
    chartWave->update();
    chartSpektr->setHidden(false);
    chartSpektr->update();
    viewWave->setText(translate("Посмотреть волну"));
  }
  if (!checked) {
    chartWave->setVisible(true);
    chartWave->update();
    chartSpektr->setHidden(true);
    chartSpektr->update();
    viewWave->setText(translate("Посмотреть спектр"));
  }
}

// {{{ конструктор

WaveSetup::WaveSetup()
{
  // Создаем необходимые виджеты
  gBox1 = new QGroupBox(translate("Примеры"));
  gBox2 = new QGroupBox(translate("Способ зад. возмущ."));
  wave1But = new QPushButton(translate("Синусоида"));
  wave2But = new QPushButton(translate("Биения"));
  wave3But = new QPushButton(translate("Импульс"));
  wave4But = new QPushButton(translate("Широкий импульс"));
  wave5But = new QPushButton(translate("Солитон"));
  mainLayout = new QHBoxLayout();
  leftLayout = new QVBoxLayout();
  gLayout1 = new QVBoxLayout();
  gLayout2 = new QVBoxLayout();
  middleLayout = new QVBoxLayout();
  middleBottLayout = new QGridLayout();
  rightLayout = new QVBoxLayout();
  analLayout = new QHBoxLayout();
  chartSpektr = new ZChart();
  chartWave = new ZChart();
  chkAnalWave = new QRadioButton(translate("Задать волну аналитич."));
  chkSpektrWave = new QRadioButton(translate("Задать волну спектром"));
  chkManWave = new QRadioButton(translate("Задать волну вручную"));
  analExprLabel = new QLabel(translate("U(t)="));
  analExprEdit = new QLineEdit("sin(6.28*t)");
  apply = new QPushButton(translate("Применить"), this);
  drawwavearea = new DrawDispArea();
  clear = new QPushButton(translate("Очистить"));
  viewRes = new QPushButton(translate("Посмотреть результат"));
  tmaxLabel = new QLabel(translate("T"), this);
  umaxLabel = new QLabel(translate("Umax"), this);
  tmaxEdit = new QLineEdit("10", this);
  uminLabel = new QLabel("Umin", this);
  uminEdit = new QLineEdit("-4", this);
  umaxEdit = new QLineEdit("4", this);
  viewWave = new QPushButton(translate("Посмотреть спектр"));
  table = new QTableWidget(30, 3, this);

  // Лепим tooltips к виджетам
  wave1But->setToolTip(translate("Для начала лучше попробовать что-нибудь простое"));
  wave2But->setToolTip(translate("Попробуйте \"биения\", т.к. на этом<br> типе колебаний очень хорошо наблюдать<br> особенности явления <b>дисперсии</b>"));
  wave3But->setToolTip(translate("Один из самых распространенных<br> видов колебаний в реальной жизни"));
  wave4But->setToolTip(translate("Будет ли разница по сравнению<br> с предыдущим случаем?"));
  wave5But->setToolTip(translate("TODO: надо что-то придумать"));
  tmaxEdit->setToolTip(translate("Здесь задается <i>период</i> <font color=green>T</font> колебаний"));
  uminEdit->setToolTip(translate("Здесь задается минимальное значение <i>амплитуды</i> колебаний<br> Это только меняет масштаб построения, а не вид волны!<br>Физический смысл имеет только в режиме ручного рисования волны"));
  umaxEdit->setToolTip(translate("Здесь задается максимальное значение <i>амплитуды</i> колебаний<br> Это только меняет масштаб построения, а не вид волны!<br>Физический смысл имеет только в режиме ручного рисования волны"));
  apply->setToolTip(translate("Используйте эту кнопку для перерисовки графика,<br> если вы изменили какие-либо параметры"));
  chkAnalWave->setToolTip(translate("Щелкните здесь, если необходимо задать <br>начальное возмущение в аналитическом виде"));
  analExprEdit->setToolTip(translate("Напишите здесь формулу, описывающую<br> нужный вам вид колебаний<br>Не забудьте нажать кнопку \"Применить\"!<br>TODO: описать синтаксис"));
  chkSpektrWave->setToolTip(translate("Щелкните здесь, если необходимо задать <br>начальное возмущение в виде таблицы<br> значений, задающих спектр волны"));
  chkManWave->setToolTip(translate("Щелкните здесь, если необходимо задать <br>начальное возмущение простым рисованием мышкой<br>Учтите диапазон амплитуд, когда будете рисовать!"));
  viewRes->setToolTip(translate("Посмотрите оцифрованный вариант того, что вы нарисовали"));
  viewWave->setToolTip(translate("В зависимости от состояния<br> переключателя можно наблюдать<br> или спектр волны, или саму волну"));
  clear->setToolTip(translate("Полностью очищает все, что было нарисовано"));

  QFont boldFont("Sans Serif", 10, QFont::Bold);
  boldFont.setPointSize(10);
  apply->setFont(boldFont);

  // Спектр начального возмущения сначала скрыт
  chartSpektr->setHidden(true);

  // Таблица, задающая спектр нач. возмущения тоже скрыта
  table->setHidden(true);
  // Настраиваем таблицу
  QStringList headers;
  headers << translate("Амплитуда, U")
	  << translate("Частота, F")
	  << translate("Фаза, f");
  table->setHorizontalHeaderLabels(headers);
  for (int i = 0; i <= 30; i++) {
    for (int j = 0; j <= 3; j++) {
      QTableWidgetItem *newItem = new QTableWidgetItem("0");
      newItem->setTextAlignment(Qt::AlignRight);
      table->setItem(i, j, newItem);
    }
  }
  // устанавливаем умолчальные значения для таблицы
  QTableWidgetItem *newItem = new QTableWidgetItem("0.5");
  newItem->setTextAlignment(Qt::AlignRight);
  table->setItem(0, 0, newItem);
  newItem = new QTableWidgetItem("0.3");
  newItem->setTextAlignment(Qt::AlignRight);
  table->setItem(1, 0, newItem);
  newItem = new QTableWidgetItem("0.3");
  newItem->setTextAlignment(Qt::AlignRight);
  table->setItem(2, 0, newItem);
  newItem = new QTableWidgetItem("0.1");
  newItem->setTextAlignment(Qt::AlignRight);
  table->setItem(3, 0, newItem);
  newItem = new QTableWidgetItem("0.1");
  newItem->setTextAlignment(Qt::AlignRight);
  table->setItem(4, 0, newItem);
  newItem = new QTableWidgetItem("6");
  newItem->setTextAlignment(Qt::AlignRight);
  table->setItem(0, 1, newItem);
  newItem = new QTableWidgetItem("7");
  newItem->setTextAlignment(Qt::AlignRight);
  table->setItem(1, 1, newItem);
  newItem = new QTableWidgetItem("5");
  newItem->setTextAlignment(Qt::AlignRight);
  table->setItem(2, 1, newItem);
  newItem = new QTableWidgetItem("8");
  newItem->setTextAlignment(Qt::AlignRight);
  table->setItem(3, 1, newItem);
  newItem = new QTableWidgetItem("4");
  newItem->setTextAlignment(Qt::AlignRight);
  table->setItem(4, 1, newItem);

  // Настраиваем кнопки для режима ручного задания начального возмущения
  clear->setHidden(true);
  viewRes->setHidden(true);
  viewRes->setCheckable(true);
  viewWave->setCheckable(true);

  connect(viewRes, SIGNAL(toggled(bool)),
	  this, SLOT(viewResSlot(bool)));
  connect(clear, SIGNAL(clicked()),
	  drawwavearea, SLOT(clearImage()));
  connect(viewWave, SIGNAL(toggled(bool)),
	  this, SLOT(viewWaveSlot(bool)));

  // Применяем изменения
  connect(apply, SIGNAL(clicked()),
	  this, SLOT(applySett()));

  // Изначально область рисования, естественно, скрыта
  drawwavearea->setHidden(true);

  // если анчеким группбокс с дополнительными способами задания
  // возмущения, то и все радиобаттоны анчекятся
  connect(gBox2, SIGNAL(toggled(bool)), chkSpektrWave, SLOT(setChecked(bool)));
  connect(gBox2, SIGNAL(toggled(bool)), chkManWave, SLOT(setChecked(bool)));
  connect(gBox2, SIGNAL(toggled(bool)), chkAnalWave, SLOT(setChecked(bool)));

  // Устанавливаем, что будет происходить при нажатии на радиобаттоны
  connect(chkAnalWave, SIGNAL(toggled(bool)),
	  analExprEdit, SLOT(setEnabled(bool)));
  connect(chkAnalWave, SIGNAL(toggled(bool)),
	  chartSpektr, SLOT(setHidden(bool)));
  connect(chkAnalWave, SIGNAL(toggled(bool)),
	  chartWave, SLOT(setVisible(bool)));

  connect(chkManWave, SIGNAL(toggled(bool)),
	  drawwavearea, SLOT(setVisible(bool)));
  connect(chkManWave, SIGNAL(toggled(bool)),
	  clear, SLOT(setVisible(bool)));
  connect(chkManWave, SIGNAL(toggled(bool)),
	  viewRes, SLOT(setVisible(bool)));
  connect(chkManWave, SIGNAL(toggled(bool)),
	  viewWave, SLOT(setHidden(bool)));
  connect(chkManWave, SIGNAL(toggled(bool)),
	  chartWave, SLOT(setHidden(bool)));
  connect(chkManWave, SIGNAL(toggled(bool)),
	  viewWave, SLOT(setChecked(bool)));
  connect(chkManWave, SIGNAL(toggled(bool)),
	  chartSpektr, SLOT(setHidden(bool)));

  connect(chkSpektrWave, SIGNAL(toggled(bool)),
	  chartSpektr, SLOT(setHidden(bool)));
  connect(chkSpektrWave, SIGNAL(toggled(bool)),
	  chartWave, SLOT(setHidden(bool)));
  connect(chkSpektrWave, SIGNAL(toggled(bool)),
	  table, SLOT(setVisible(bool)));
  connect(chkSpektrWave, SIGNAL(toggled(bool)),
	  viewRes, SLOT(setVisible(bool)));
  connect(chkSpektrWave, SIGNAL(toggled(bool)),
	  viewWave, SLOT(setHidden(bool)));

  // настраиваем плоттеры
  chartSpektr->setBorderType(0);
  chartSpektr->setGraphPanelBorderType(0);
  chartSpektr->setMargin(2);
  chartSpektr->setSpacing(2);
  chartSpektr->setFirstText("");
  chartSpektr->setSecondText("Выделяйте мышкой интересующий вас участок спектра для его увеличения");
  chartSpektr->setTitle("Спектр начального возмущения");
  chartSpektr->setXRange(0, 10);
  chartSpektr->setYRange(0, 10);
  chartSpektr->setAxesName("f, Гц", "U, м");
  chartSpektr->showLegend(false);
  chartSpektr->setAutoscale(true);
  chartWave->setBorderType(0);
  chartWave->setGraphPanelBorderType(0);
  chartWave->setMargin(2);
  chartWave->setSpacing(2);
  chartWave->setFirstText("");
  chartWave->setSecondText("");
  chartWave->setTitle("Профиль начального возмущения при t = 0");
  chartWave->setXRange(0, 10);
  chartWave->setYRange(-4, 4);
  chartWave->setAxesName("t, с", "U, м");
  chartWave->showLegend(false);
  chartWave->setAutoscale(true);

  // Создаем серии данных
  waveSpektrGraph = new ZSimpleSeries("Spektr of wave");
  waveSpektrGraph->setColor(Qt::red);
  chartSpektr->addSeries(waveSpektrGraph);
  waveGraph = new ZSimpleSeries("Wave");
  waveGraph->setColor(Qt::blue);
  chartWave->addSeries(waveGraph);

  // Настраиваем Едиты
  QValidator *validator = new QIntValidator(0, 1000, this);
  analExprEdit->setMaxLength(40);
  analExprEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  analExprEdit->setEnabled(false);
  tmaxEdit->setMaxLength(4);
  tmaxEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  tmaxEdit->setValidator(validator);
  validator = new QIntValidator(0, 1000, this);
  umaxEdit->setMaxLength(4);
  umaxEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  umaxEdit->setValidator(validator);
  validator = new QIntValidator(-1000, 0, this);
  uminEdit->setMaxLength(4);
  uminEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  uminEdit->setValidator(validator);

  // Настраиваем кнопки, задающие примерное начальное возмущение
  wave1But->setCheckable(true);
  wave2But->setCheckable(true);
  wave3But->setCheckable(true);
  wave4But->setCheckable(true);
  wave5But->setCheckable(true);
  wave1But->setAutoExclusive(true);
  wave2But->setAutoExclusive(true);
  wave3But->setAutoExclusive(true);
  wave4But->setAutoExclusive(true);
  wave5But->setAutoExclusive(true);

  connect(wave1But, SIGNAL(clicked()), this, SLOT(drawWave()));
  connect(wave2But, SIGNAL(clicked()), this, SLOT(drawWave()));
  connect(wave3But, SIGNAL(clicked()), this, SLOT(drawWave()));
  connect(wave4But, SIGNAL(clicked()), this, SLOT(drawWave()));
  connect(wave5But, SIGNAL(clicked()), this, SLOT(drawWave()));

  // Выравниваем левую часть
  gLayout1->addWidget(wave1But);
  gLayout1->addWidget(wave2But);
  gLayout1->addWidget(wave3But);
  gLayout1->addWidget(wave4But);
  gLayout1->addWidget(wave5But);
  gLayout1->setMargin(1);
  gBox1->setLayout(gLayout1);

  leftLayout->addStretch();
  leftLayout->addWidget(gBox1);
  leftLayout->addStretch();

  // Выравниваем центральные виджеты
  middleBottLayout->addWidget(tmaxLabel, 0, 3);
  middleBottLayout->addWidget(tmaxEdit, 0, 4);
  middleBottLayout->addWidget(uminLabel, 0, 5);
  middleBottLayout->addWidget(uminEdit, 0, 6);
  middleBottLayout->addWidget(umaxLabel, 0, 7);
  middleBottLayout->addWidget(umaxEdit, 0, 8);
  middleBottLayout->addWidget(clear, 0, 0, 1, 2);
  middleBottLayout->addWidget(viewWave, 0, 0, 1, 2);
  middleBottLayout->addWidget(viewRes, 1, 0, 1, 2);
  middleBottLayout->setMargin(1);
  middleLayout->addWidget(chartSpektr);
  middleLayout->addWidget(chartWave);
  middleLayout->addWidget(drawwavearea);
  middleLayout->addWidget(table);
  middleLayout->addLayout(middleBottLayout);
  middleLayout->setMargin(1);

  // Здесь дисперсия задается в аналитическом виде
  analLayout->addWidget(analExprLabel);
  analLayout->addWidget(analExprEdit);
  analLayout->addStretch();

  // Выравниваем правую область
  gLayout2->addWidget(chkAnalWave);
  gLayout2->addLayout(analLayout);
  gLayout2->addWidget(chkSpektrWave);
  gLayout2->addWidget(chkManWave);
  gLayout2->setMargin(1);
  gBox2->setCheckable(true);
  gBox2->setChecked(false);
  gBox2->setLayout(gLayout2);

  rightLayout->addStretch();
  rightLayout->addWidget(gBox2);
  rightLayout->addStretch();
  rightLayout->addWidget(apply);
  rightLayout->addStretch();

  // А теперь все вместе!
  mainLayout->addLayout(leftLayout);
  mainLayout->addLayout(middleLayout);
  mainLayout->addLayout(rightLayout);
  mainLayout->setStretchFactor(middleLayout, 1);
  mainLayout->setMargin(1);

  setLayout(mainLayout);

  // Кликаем на кнопку с примером в виде синусоиды
  wave1But->toggle();
  drawWave();
}

// }}}
