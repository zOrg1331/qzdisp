// Time-stamp: <zchart.h - 01:37:39 04.07.2006>
/******************************************************************
 *** Виджет представляет собой построитель графиков с широкими  ***
 ***         возможностями и высокой скоростью работы           ***
 ******************************************************************/

/* code: Князев Алексей <knzsoft@mail.ru> */
/* some changes & improvements: Наконечный Павел <zorg1331@mail.ru> */

#ifndef ZCHART_H
#define ZCHART_H

#include <QWidget>
#include <QPen>

#include "zframes.h"

// констаны типа бордюра (bt - border type)
#define btNoneNone 0
#define btRaisedNone 1
#define btRaisedLowered 2

// структура, передаваемая из класса оболочки графиков в классы
// наборов данных, для того, чтобы классы наборов могли отобразить
// свои данные в классе оболочке
struct ZGraphArea {
  long double xMin,xMax;
  long double yMin,yMax;
  int xsMin,xsMax;
  int ysMin,ysMax;
  QPainter *painter;
};

// класс набора данных для графика ZChart
class ZChartSeries
{
public:
  QString name;
  QColor color;
  ZChartSeries(const QString &aName);
  ZChartSeries(const QString &aName, const QColor &aColor);
  virtual ~ZChartSeries() {}
  void setGraphArea(const ZGraphArea &g);
  void setColor(const QColor &c);
  virtual void draw(void);
  // для нужд автомасштабирования
  virtual double getMaxX(void);
  virtual double getMaxY(void);
  virtual double getMinX(void);
  virtual double getMinY(void);

private:
  ZGraphArea ga;

  // копии переменных из структуры "ga" для быстрого обращения
  long double xMin,xMax;
  long double yMin,yMax;
  int xsMin,xsMax;
  int ysMin,ysMax;
  int xPenPos, yPenPos;

  // вычисленные коэффициенты масштаба
  long double xScreen;
  long double yScreen;
  bool isLastPointVisible;

protected:
  int xToScreen(long double x);
  int yToScreen(long double y);
  void moveTo(int x, int y);
  void lineTo(int x, int y);
  QPainter *painter;
};

struct LongDoublePoint {
  long double x, y;
};

class ZSimpleSeries: public ZChartSeries
{
public:
  ZSimpleSeries(const QString &aName);
  ZSimpleSeries(const QString &aName, const QColor &aColor);
  virtual ~ZSimpleSeries() {}
  virtual void draw(void);
  void add(long double x, long double y);
  // Возвращает игрековое значение серии, соответствующее данному иксовому
  double getYValue(double x);
  void clear(void);
  // для нужд автомасштабирования
  virtual double getMaxX();
  virtual double getMaxY();
  virtual double getMinX();
  virtual double getMinY();

private:
  QVector <LongDoublePoint> point;
};

class ZLimitSeries: public ZChartSeries
{
public:
  ZLimitSeries(const QString &aName);
  ZLimitSeries(const QString &aName, const QColor &aColor,
	       double baseLine, bool isFilled);
  virtual ~ZLimitSeries() {}
  virtual void draw(void);
  void add(long double x, long double y);
  // Возвращает игрековое значение серии, соответствующее данному иксовому
  double getYValue(double x);
  void clear(void);
  // для нужд автомасштабирования
  virtual double getMaxX();
  virtual double getMaxY();
  virtual double getMinX();
  virtual double getMinY();

private:
  QVector <LongDoublePoint> point;
  double baseLine;
  bool isFilled;
  double x_old, y_old;
};

// класс оболочки графика
class ZChart: public QWidget
{
  Q_OBJECT

public:
  ZChart(QWidget *parent=0);

  void draw(void);
  void setAxesName(const QString &xName, const QString &yName);
  void setBorderType(int aBorderType);
  void setGraphPanelBorderType(int aGraphPanelBorderType);
  void setMargin(int aMargin);
  void setSpacing(int aSpacing);
  void setFirstText(const QString &s);
  void setSecondText(const QString &s);
  void setFirstStatusPanelWidth(int w);
  void setTitle(const QString &s);
  void setSettings(QFont _mainFont, QFont _boldFont, QPen _axesPen, QPen _gridPen,
		   QPen _legendFramePen, QPen _rubberPen, QPen _zeroPen);
  void showLegend(bool show);
  void addSeries(ZChartSeries *s);
  void addSeriesAt(ZChartSeries *s, int index);
  void removeSeries();
  void removeSeriesAt(int index);

  // установка диапазонов шкал
  void setXRange(long double aXMin, long double aXMax);
  void setYRange(long double aYMin, long double aYMax);

  // включить автоматическое автомасштабирование
  void setAutoscale(bool _autoscale);
  void autoscale();

  // окраснить виджет :)
  bool isLightGraph;

signals:
  // Куда кликнули мышкой
  void clickCoord(double x, double y);
  void clickCoordX(double x);
  void clickCoordY(double y);

public slots:
  void zoomIn();
  void zoomOut();

//private slots:
  // автомасштабирование
//  void autoscale();

private:
  // экземляр класса для рисования рамок
  ZFrames *frames;
  QList <ZChartSeries *> series;

  // настройки отображения плоттера
  QFont mainFont;		// основной шрифт
  QFont boldFont;		// жирный шрифт
  QPen axesPen;			// стиль рисования осей
  QPen gridPen;			// стиль рисования сетки
  QPen legendFramePen;		// стиль рисования рамки легенды
  QPen rubberPen;		// стиль рисования рамки выделения
  QPen zeroPen;			// линия нулевой толщины
  QPen zoomButtonsPen;		// линия границы кнопок перемещения по истории масштабов

  int borderType;  // тип общего бордюра
  int margin;      // размер внешних полей
  int spacing;     // размер внутренних полей
  int fontSize;    // высота используемого шрифта

  // переменные для панели заголовка
  int titleLeft,titleTop,titleWidth,titleHeight; // геометрия панели
  int xTitleText,yTitleText;                     // координаты вывода текста
  QString titleText;   // текст заголовка
  QString xAxName, yAxName;  // наименование осей
  int titleBorderType; // тип бордюра панели заголовка

  bool showLegendFlag; // показывать ли легенду с описанием построенных графиков
  bool isTitleTruncated; // обрезан ли тайтл

  // переменные для строки статуса
  int sbLeft,sbTop,sbWidth,sbHeight;             // геометрия панели
  int sbBaffles[1];                              // массив перегородок в строке статуса
  int xFirstText,xSecondText,yStatusBarText;     // координаты вывода текста
  QString sbFirstText;                           // текст первой ячейки строки статуса
  QString sbSecondText;                          // текст второй ячейки строки статуса

  // переменные для обозначения панели графика
  int graphPanelBorderType;
  int gpLeft,gpTop,gpWidth,gpHeight;
  // переменные для обозначения шкал
  int xScaleLeft,xScaleTop,xScaleWidth,xScaleHeight;
  int yScaleLeft,yScaleTop,yScaleWidth,yScaleHeight;
  // переменные для обозначения области графика
  int axisLeft,axisTop,axisWidth,axisHeight;

  // переменные связанные с диапазонами шкал
  long double xMin,xMax;         // масштабируемый диапазон шкалы X
  long double xMinOrig,xMaxOrig; // оригинальный диапазон шкалы X
  long double yMin,yMax;         // масштабируемый диапазон шкалы Y
  long double yMinOrig,yMaxOrig; // оригинальный диапазон шкалы Y
  long double xMinTick;          // минимальная реперная точка на шкале X
  long double yMinTick;          // минимальная реперная точка на шкале Y
  long double xStepTick;         // шаг шкалы X
  long double yStepTick;         // шаг шкалы Y
  int xsMin,xsMax;               // минимальная и максимальная экранная x-координата в пикселях
  int ysMin,ysMax;               // минимальная и максимальная экранная y-координата в пикселях
  // точность шкалирования осей - показатель степени 10, используемый в шкале
  int xPrecision;
  int yPrecision;
  bool autoscaleFlag;

  // переменные связанные с выделением области мышью
  bool leftMouseDownActive;  // нажата левая клавиша мыши
  bool rightMouseDownActive; // нажата правая клавиша мыши
  bool baffleMovedActive;    // двигаем перегородку в строке статуса
  long double xDown,yDown;   // реальная координата нажатия мыши
  long double xUp,yUp;       // реальная координата отпускания мыши
  int xsDown,ysDown;         // экранная координата нажатия мыши
  int xsUp,ysUp;             // экранная координата отпускания мыши

  // расчет минимальных реперных точек и шага шкал
  void adjustXScale(void);
  void adjustYScale(void);
  // перерассчет экранных и реальных координат
  int xToScreen(long double x);
  int yToScreen(long double y);
  long double xToReal(int x);
  long double yToReal(int y);

  void fixSize(void);        // считаем размеры элементов внутри заданного внешнего размера
  int  fixXScaleHeight(void);// считаем высоту горизонтальной шкалы
  int  fixYScaleWidth(void); // считаем ширину вертикальной шкалы
  void drawTitle(void);      // рисуем панель заголовка
  void drawStatusBar(void);  // рисуем строку статуса
  void drawFirstText(void);  // рисуем текст первой панели строки статуса
  void drawSecondText(void); // рисуем текст второй панели строки статуса
  void drawGraphPanel(void); // рисуем панель графиков
  void drawXScale(QPainter *p, int xOffset, int yOffset);     // рисуем горизонтальную шкалу
  void drawYScale(QPainter *p, int xOffset, int yOffset);     // рисуем вертикальную шкалу
  void drawAxes(QPainter *p, int xOffset, int yOffset);       // рисуем оси
  void drawGrid(QPainter *p, int xOffset, int yOffset);       // рисуем сетку
  void drawSeries(QPainter *p, int xOffset, int yOffset);     // рисуем данные
  void updateRubberBandRegion(void);                          // обновляем область рамки
  void drawRubberBand(void);                                  // рисуем рамку
  void drawLegend(QPainter *p);     // рисуем легенду с описанием построенных серий
  void drawZoomButtons(void); // рисуем кнопки, с помощью которых идет перемещение по очереди изменения масштаба
  void drawAutoscaleButton(void); // рисуем кнопку автомасштабирования

  // проверим, принадлежат ли координаты x,y области перегородки в строке статуса
  bool isStatusBarFirstBaffle(int x, int y);
  // возвращает возможное значение позиции перегородки в строке статуса, или 0
  int  isPositionStatusBarFirstBafflePossible(int x);
  // принадлежат ли координаты кнопке назад
  bool isZoomOutButton(int x, int y);
  // принадлежат ли координаты кнопке вперед
  bool isZoomInButton(int x, int y);
  // принадлежат ли координаты кнопке автомасштабирования
  bool isAutoscaleButton(int x, int y);
  // надо ли рисовать рамку
  bool rubberBandIsShown;
  // прямоугольник рамки
  QRect rubberBandRect;

  // Переменная задает текущие настройки масштаба
  int curZoom;

  // Вектор из указателей на настройки графика
  // это нужно, чтобы создать очередь из изменений масштаба
  QVector <ZGraphArea> zoomStack;

  // Изменяем текущие параметры масштаба в стеке
  void chCurrStack();

  // увеличиваем/уменьшаем масштаб к центру
  void scaleIn();
  void scaleOut();

protected:
  virtual bool event(QEvent *event);
  virtual void paintEvent(QPaintEvent *ev);
  virtual void resizeEvent(QResizeEvent *ev);
  virtual void mouseMoveEvent(QMouseEvent *ev);
  virtual void mousePressEvent(QMouseEvent *ev);
  virtual void mouseReleaseEvent(QMouseEvent *ev);
  virtual void wheelEvent(QWheelEvent *event);
  virtual void keyPressEvent(QKeyEvent *event);
};

#endif
