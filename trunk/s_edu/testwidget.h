/****************************************************************************
 ***     Заголовочный файл виджета, позволяющего провети тестирование     ***
 ****************************************************************************/

// Time-stamp: <testwidget.h - 21:39:46 23.05.2006>

#ifndef TESTWIDGET_H
#define TESTWIDGET_H

#include <QObject>
#include <QWidget>
#include <QStringList>
#include "mainwindow.h"

class QStringList;
class QLabel;
class QVBoxLayout;
class QHBoxLayout;
class QPushButton;
class QGroupBox;

class TestWidget : public QWidget
{
  Q_OBJECT

public:
  TestWidget(QString fileName, MainWindow *mainwindow);

private slots:
  void nextQuestion();
  void prevQuestion();
  void makeAnswer();

  void firstToggledSlot(bool);
  void secondToggledSlot(bool);
  void thirdToggledSlot(bool);
  void fourthToggledSlot(bool);

private:
  void initializeTest(QString); // подготавливаем список вопросов,
				// который в последствии будет задаваться
  void askQuestion();	        // функция заполняет виджеты на вкладке
				// соответствующим вопросом
  void showResults();		// показываем результаты тестирования

  QStringList testList; // список вопросов, в порядке их задания
  QList<int> questStatus;       // соответствующий список, показывающий:
				// 0 - не отвечали на него
				// 1 - ответили правильно
				// 2 - ответили неправильно
  int totalQuestions;
  int currQuestion; // номер вопроса, который задается в текущий момент
  QVector <QLabel *> Images; // список иллюстраций к задаваемым вопросам
  MainWindow *MAINWINDOW;

  QLabel *questLabel;
  QVBoxLayout *vLayout;
  QHBoxLayout *hLayout;
  QHBoxLayout *questLayout;
  QPushButton *prevQuest;
  QPushButton *nextQuest;
  QPushButton *makeAns;
  QPushButton *quitButt;
  QPushButton *reView;
  QGroupBox *answsBox;
  QGroupBox *questBox;
  QGroupBox *firstAnsBox;
  QGroupBox *secondAnsBox;
  QGroupBox *thirdAnsBox;
  QGroupBox *fourthAnsBox;
  QVBoxLayout *questBoxLayout;
  QVBoxLayout *answsBoxLayout;
  QVBoxLayout *firstAnsLayout;
  QVBoxLayout *secondAnsLayout;
  QVBoxLayout *thirdAnsLayout;
  QVBoxLayout *fourthAnsLayout;
  QLabel *firstAnsLabel;
  QLabel *secondAnsLabel;
  QLabel *thirdAnsLabel;
  QLabel *fourthAnsLabel;
};

#endif
