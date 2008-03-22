// Time-stamp: <disp_tut.cpp - 10:20:46 21.05.2006>
/************************************************************************
 ***  ���� �������� ������ ���� ��������� ������������� � ���������   ***
 ************************************************************************/

/* code: ���������� ����� <zorg1331@gmail.com> */

#include <QtGui>

#include "less1.h"
#include "less2.h"
#include "less3.h"
#include "less4.h"
#include "less5.h"
#include "disp_tut.h"
#include <translate.h>

// �������� �������� �������
QString DispTutPlugin::name() const
{
  QString Name = "������� ���������";
  return Name;
}

// ������ ������ ������
QStringList DispTutPlugin::lessons() const
{
  QStringList Lessons;
  Lessons << translate("1. �������� ->")
	  << translate("2. ������� ��������� ->")
	  << translate("3. ������� ���������� ���������� ->")
	  << translate("4. �������� ����� � ����� ->")
	  << translate("5. ������ � ������� �� ���� ->");
  return Lessons;
}

// ����� �������� ���������� ��������� � ������� �����
QStringList DispTutPlugin::helps() const
{
#include "help.txt"
}

// ������ ������ ��������, ������ ������ ������������� ������ �����
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
