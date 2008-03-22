/****************************************************************************
 ***              ����� ����������� ��������� ��������                    ***
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

  // �������� ���������
  virtual QString name() const = 0;
  // ������ ������, ������� ���� � ���� ���������
  virtual QStringList lessons() const = 0;
  // ������ ���������� �� �������, ������� � ������������� ��������������� �����
  virtual QList<QWidget *> widgetsList() const = 0;
  // ������ ������� �������� � ������� �����
  virtual QStringList helps() const = 0;
};

Q_DECLARE_INTERFACE(TutorialInterface,
		    "org.z.SimpleEdu.TutorialInterface/1.0")

#endif
