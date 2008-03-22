// Time-stamp: <less1.h - 14:51:17 21.05.2006>
/*****************************************************************
 *** Заголовочный файл первого урока туториала по дисперсии    ***
 *****************************************************************/

/* code: Наконечный Павел <zorg1331@mail.ru> */

#ifndef LESS1_H
#define LESS1_H

#include <QObject>
#include <QWidget>

class QImage;
class QVBoxLayout;
class QHBoxLayout;
class QPushButton;
class QGroupBox;
class QLabel;

class Less1 : public QWidget
{
  Q_OBJECT

public:
  Less1(QWidget *);
  // величина, характеризующая производительность машины
  static float perfomance;
private slots:
  void setPerf();
private:
  void paintEvent(QPaintEvent *);
  QGroupBox *gBox;
  QGroupBox *gBox1;
  QPushButton *perf1But;
  QPushButton *perf2But;
  QPushButton *perf3But;
  QPushButton *perf4But;
  QLabel *helpLabel;
  QVBoxLayout *rightLayout;
  QVBoxLayout *leftLayout;
  QVBoxLayout *gLayout;
  QVBoxLayout *gLayout1;
  QHBoxLayout *mainLayout;
};

#endif
