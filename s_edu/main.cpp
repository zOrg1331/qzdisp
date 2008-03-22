/*******************************************************************************
 *** Главный программный модуль приложения для демонстрации различных уроков ***
 *******************************************************************************/

// Time-stamp: <main.cpp - 00:48:26 12.04.2006>

#include <QApplication>
#include <QPlastiqueStyle>
#include <QSplashScreen>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  QApplication::setStyle(new QPlastiqueStyle());
  QFont defaultFont("Sans Serif", 10, QFont::Normal);
  defaultFont.setPointSize(10);
  QApplication::setFont(defaultFont);

  QPixmap pixmap(":splash.jpg");
  QSplashScreen *splash = new QSplashScreen(pixmap);
  splash->show();

  MainWindow window;
  window.show();
  splash->finish(&window);
  delete splash;
  return app.exec();
}
