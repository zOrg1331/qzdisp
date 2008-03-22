/**********************************************************************************
 ***                    Главный программный модуль приложения для               ***
 ***          моделирования распространения волн в среде с дисперсией           ***
 **********************************************************************************/

// Time-stamp: <main.cpp - 17:32:28 16.04.2006>

#include <QApplication>
#include <QPlastiqueStyle>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  QApplication::setStyle(new QPlastiqueStyle());
  QFont defaultFont("Sans Serif", 10, QFont::Normal);
  defaultFont.setPointSize(10);
  QApplication::setFont(defaultFont);

  MainWindow window;
  window.resize(800, 600);
  window.show();
  return app.exec();
}
