// Time-stamp: <disp_tut.cpp - 10:20:46 21.05.2006>
/************************************************************************
 ***  Этот туториал должен дать наглядное представление о дисперсии   ***
 ************************************************************************/

/* code: Наконечный Павел <zorg1331@gmail.com> */

#include <QtGui>

#include "less1.h"
#include "less2.h"
#include "less3.h"
#include "less4.h"
#include "less5.h"
#include "disp_tut.h"
#include <translate.h>

// Сообщаем название плагина
QString DispTutPlugin::name() const
{
  QString Name = "Понятие дисперсии";
  return Name;
}

// Выдаем список уроков
QStringList DispTutPlugin::lessons() const
{
  QStringList Lessons;
  Lessons << translate("1. Введение ->")
	  << translate("2. Задание дисперсии ->")
	  << translate("3. Задание начального возмущения ->")
	  << translate("4. Эволюция волны в среде ->")
	  << translate("5. Пример с волнами на воде ->");
  return Lessons;
}

// Здесь задаются справочные сообщения к каждому уроку
QStringList DispTutPlugin::helps() const
{
#include "help.txt"
}

// Выдаем список виджетов, каждый виджет соответствует одному уроку
QList<QWidget *> DispTutPlugin::widgetsList() const
{
  QList<QWidget *> list;
  QWidget *parent;
  list.append(new Less1(parent));
  list.append(new Less2(parent));
  list.append(new Less3(parent));
  list.append(new Less4(parent));
  list.append(new Less5(parent));
  return list;
}

Q_EXPORT_PLUGIN(DispTutPlugin)
