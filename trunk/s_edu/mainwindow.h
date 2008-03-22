/****************************************************************************
 *** Здесь описываются методы и переменные, необходимые для нормального   ***
 ***          функционирования главного окна приложения                   ***
 ****************************************************************************/

// Time-stamp: <mainwindow.h - 14:52:58 11.05.2006>

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDir>
#include <QMainWindow>
#include <QStringList>

class QAction;
class QActionGroup;
class QMenu;
class QSplitter;
class PluginsList;
class QTabWidget;
class QStringList;
class QVBoxLayout;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow();

  void saveResults(int, int);

public slots:
  void stopTest();

private slots:
  void changeLesson();
  void about();
  void changeLess(QString);
  void runTest();
  void validateUser();
  void setStudNum(QString);
  void setFaculty(QString);
  void setGroup(QString);
  void setSurname(QString);

private:
  // данные о пользователе, проходящем тест
  QString studNum;
  QString faculty;
  QString group;
  QString surname;
  void createActions();
  void createMenus();
  void loadPlugins();
  void addToMenu(QObject *plugin, const QString &name, const QStringList &texts,
		 const QStringList &helps, const QList<QWidget *> &widgetsList,
		 QString fileName,
		 QMenu *menu, const char *member, QActionGroup *actionGroup = 0);
  QVBoxLayout* createTestTabWidget();
  bool verifyTests(QString fileName, QString Name);

  QDir pluginsDir;
  QStringList pluginFileNames;

  QMenu *fileMenu;
  QMenu *lessonsMenu;
  QMenu *helpMenu;
  QActionGroup *lessonsActionGroup;
  QAction *openAct;
  QAction *exitAct;
  QAction *aboutAct;
  QAction *aboutQtAct;
  QSplitter *horizontalSplitter;
  PluginsList *pluginsList;
  QStringList tutorialsList;
  QTabWidget *tutorialsTabs;
};

#endif
