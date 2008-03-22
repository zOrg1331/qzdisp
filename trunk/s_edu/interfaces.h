/****************************************************************************
 ***              Здесь описывается интерфейс плагинов                    ***
 ****************************************************************************/

// Time-stamp: <interfaces.h - 16:25:11 09.05.2006>

#ifndef INTERFACES_H
#define INTERFACES_H

class QStringList;
class QWidget;

class TutorialInterface
{
public:
  virtual ~TutorialInterface() {}

  // название туториала
  virtual QString name() const = 0;
  // список уроков, которые есть в этом туториале
  virtual QStringList lessons() const = 0;
  // список указателей на виджеты, которые и демонстрируют соответствующие уроки
  virtual QList<QWidget *> widgetsList() const = 0;
  // список кратких описаний к каждому уроку
  virtual QStringList helps() const = 0;
};

Q_DECLARE_INTERFACE(TutorialInterface,
		    "org.z.SimpleEdu.TutorialInterface/1.0")

#endif
