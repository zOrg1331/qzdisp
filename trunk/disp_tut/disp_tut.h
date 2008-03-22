// Time-stamp: <disp_tut.h - 16:24:46 09.05.2006>
/*****************************************************************
 ***        Заголовочный файл туториала по дисперсии           ***
 *****************************************************************/

/* code: Наконечный Павел <zorg1331@gmail.com> */

#ifndef DISP_TUT_H
#define DISP_TUT_H

#include <QObject>
#include <QWidget>
#include <QStringList>

#include <s_edu/interfaces.h>

class DispTutPlugin : public QObject,
		      public TutorialInterface
{
  Q_OBJECT
  Q_INTERFACES(TutorialInterface)

public:
  // TutorialInterface
  QString name() const;
  QStringList lessons() const;
  QStringList helps() const;
  QList<QWidget *> widgetsList() const;
};

#endif
