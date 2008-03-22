/****************************************************************************
 ***      ���� ���������� ���������������� �������� ���� ����������       ***
 ****************************************************************************/

// Time-stamp: <mainwindow.cpp - 16:11:56 21.05.2006>

#include <QtGui>
#include <QSplitter>
#include <QTextEdit>

#include "interfaces.h"
#include "mainwindow.h"
#include "pluginslist.h"
#include "testwidget.h"
#include <translate.h>
#include "xcrypt.h"

MainWindow::MainWindow()
{
  createActions();
  createMenus();

  setWindowTitle(translate("��������� ������������ ������� ���������� �������"));

  if (!lessonsActionGroup->actions().isEmpty())
    lessonsActionGroup->actions().first()->trigger();

  // ������� � ������ �������� ����������� �� ����������� ��������
  horizontalSplitter = new QSplitter(Qt::Horizontal, this);
  setCentralWidget(horizontalSplitter);
  // ������� QTabWidget, ������� ����� �������� �� ������ �������
  tutorialsTabs = new QTabWidget(this);
  // ��������� ��� � ����� ����� horizontalSplitter
  horizontalSplitter->insertWidget(1, tutorialsTabs);
  // ������ �������
  loadPlugins();
  // ������� (�������� �� ������� ����) ������ ��������
  pluginsList = new PluginsList(pluginsDir.path(), pluginFileNames, this);
  // ������������ ��� � ����� ����� horizontalSplitter
  horizontalSplitter->insertWidget(0, pluginsList);
  pluginsList->hide();
  // ������ �������
  QList<int> list;
  list << 100 << 700;
  horizontalSplitter->setSizes(list);
  horizontalSplitter->setChildrenCollapsible(false);

  setGeometry(0, 20, 1024, 700);

  connect(pluginsList, SIGNAL(lessonChanged(QString)),
	  this, SLOT(changeLess(QString)));
}

void MainWindow::changeLess(QString lesson)
{
  // ������������ � ����������, ��� ������ ������ �������
  pluginsDir = QDir(qApp->applicationDirPath());
#if defined(Q_OS_WIN)
  if (pluginsDir.dirName() == "debug" || pluginsDir.dirName() == "release")
    pluginsDir.cdUp();
#elif defined(Q_OS_MAC)
  if (pluginsDir.dirName() == "MacOS") {
    pluginsDir.cdUp();
    pluginsDir.cdUp();
    pluginsDir.cdUp();
  }
#endif
  pluginsDir.cd("plugins");

  // ������������ ������ ���� � ������� ����������
  foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
    // ��������� ������� ����, ���-����� ��� ������
    QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
    QObject *plugin = loader.instance();
    // ���� ��� �������, ������...
    if (plugin) {
      // ������� ������, ������� � ����� ������ �������� "��������"
      TutorialInterface *iTutorial = qobject_cast<TutorialInterface *>(plugin);
      if (iTutorial) {
	// ���� � ������ ������ ������� ������� ������������ ������������ ���...
	if (iTutorial->lessons().indexOf(lesson) != -1) {
	  // ������ �������� ������� � ������ ��������
	  tutorialsTabs->setCurrentIndex(tutorialsList.indexOf(iTutorial->name()));
	  // ������ �������� ������� � ������������ ��� ������ � ������� � ������ ��������
	  // ��� ��������, ������-��� �� �������� � ��������� ������, �����
	  // ������� � ������� ���, ������� �������� ��������� �� ������ �
	  // ������� ������� � ���������, ����������� ��� ���� ������ QTabWidget,
	  // ������ ������� ����������� �������
	  QTabWidget *tmpTabWidget;
	  tmpTabWidget = (QTabWidget *)tutorialsTabs->currentWidget();
	  tmpTabWidget->setCurrentIndex(iTutorial->lessons().indexOf(lesson));
	  return;
	}
      }
      pluginFileNames += fileName;
    }
  }
}

void MainWindow::changeLesson()
{
  // ��������, ����� Action ������� ���� SLOT
  QAction *action = qobject_cast<QAction *>(sender());
  // �������� ��������������� Action ������
  TutorialInterface *iTutorial = qobject_cast<TutorialInterface *>(action->parent());
  // �� �������� Action ����������, ����� ������ ���� ��� ����������
  QString lesson = action->text();
  changeLess(lesson);
}

void MainWindow::about()
{
  QMessageBox::about(this, translate("� ���������"),
		     translate("��� ��������� ������������� ��� ��������� ���������������-��������� ������,\n ������� ����������� � ���� ������������ �������(��������).\n                                                                                    code zOrg"));
}

void MainWindow::createActions()
{
  exitAct = new QAction(translate("�����"), this);
  exitAct->setShortcut(tr("Ctrl+Q"));
  connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

  lessonsActionGroup = new QActionGroup(this);

  aboutAct = new QAction(translate("&� ���������"), this);
  connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

  aboutQtAct = new QAction(translate("� &Qt"), this);
  connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void MainWindow::createMenus()
{
  fileMenu = menuBar()->addMenu(translate("&����"));
  fileMenu->addSeparator();
  fileMenu->addAction(exitAct);

  lessonsMenu = menuBar()->addMenu(translate("&�����"));

  menuBar()->addSeparator();

  helpMenu = menuBar()->addMenu(translate("&�������"));
  helpMenu->addAction(aboutAct);
  helpMenu->addAction(aboutQtAct);
}

void MainWindow::runTest()
{
  // ������������ � ����������, ��� ������ ������ �������
  pluginsDir = QDir(qApp->applicationDirPath());
#if defined(Q_OS_WIN)
  if (pluginsDir.dirName() == "debug" || pluginsDir.dirName() == "release")
    pluginsDir.cdUp();
#elif defined(Q_OS_MAC)
  if (pluginsDir.dirName() == "MacOS") {
    pluginsDir.cdUp();
    pluginsDir.cdUp();
    pluginsDir.cdUp();
  }
#endif
  pluginsDir.cd("plugins");

  // ������������ ������ ���� � ������� ����������
  int i = 0;
  foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
    // ��������� ������� ����, ���-����� ��� ������
    QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
    QObject *plugin = loader.instance();
    // ���� ��� �������, ������...
    if (plugin) {
      // �������� ������ � ����������, ��� ������ ��� ������ ������ "������ ����"
      if (i == tutorialsTabs->currentIndex()) {
	// �������� ��� ������� � ������� � ������� ����
	QTabWidget *tmpTabWidget;
	tmpTabWidget = (QTabWidget *)tutorialsTabs->currentWidget();
	for (int j = 0; j < tmpTabWidget->count()-1; j++)
	  tmpTabWidget->setTabEnabled(j, false);
	tmpTabWidget->setTabEnabled(tmpTabWidget->count()-1, true);
      }
      pluginFileNames += fileName;
      i++;
    }
  }
}

void MainWindow::stopTest()
{
  // ������������ � ����������, ��� ������ ������ �������
  pluginsDir = QDir(qApp->applicationDirPath());
#if defined(Q_OS_WIN)
  if (pluginsDir.dirName() == "debug" || pluginsDir.dirName() == "release")
    pluginsDir.cdUp();
#elif defined(Q_OS_MAC)
  if (pluginsDir.dirName() == "MacOS") {
    pluginsDir.cdUp();
    pluginsDir.cdUp();
    pluginsDir.cdUp();
  }
#endif
  pluginsDir.cd("plugins");

  // ������������ ������ ���� � ������� ����������
  int i = 0;
  foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
    // ��������� ������� ����, ���-����� ��� ������
    QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
    QObject *plugin = loader.instance();
    // ���� ��� �������, ������...
    if (plugin) {
      // �������� ������ � ����������, ��� ������ ��� ������ ������ "������ ����"
      if (i == tutorialsTabs->currentIndex()) {
	// ��������� ��� ������� � ������� � ��������� ����
	QTabWidget *tmpTabWidget;
	tmpTabWidget = (QTabWidget *)tutorialsTabs->currentWidget();
	for (int j = 0; j < tmpTabWidget->count()-1; j++)
	  tmpTabWidget->setTabEnabled(j, true);
	tmpTabWidget->setTabEnabled(tmpTabWidget->count()-1, false);
      }
      pluginFileNames += fileName;
      i++;
    }
  }
}

void MainWindow::loadPlugins()
{
  // ������������ � ����������, ��� ������ ������ �������
  pluginsDir = QDir(qApp->applicationDirPath());
#if defined(Q_OS_WIN)
  if (pluginsDir.dirName() == "debug" || pluginsDir.dirName() == "release")
    pluginsDir.cdUp();
#elif defined(Q_OS_MAC)
  if (pluginsDir.dirName() == "MacOS") {
    pluginsDir.cdUp();
    pluginsDir.cdUp();
    pluginsDir.cdUp();
  }
#endif
  pluginsDir.cd("plugins");

  // ������������ ������ ���� � ������� ����������
  foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
    // ��������� ������� ����, ���-����� ��� ������
    QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
    QObject *plugin = loader.instance();
    // ���� ��� �������, ������...
    if (plugin) {
      // ������� ������, ������� � ����� ������ �������� "��������"
      TutorialInterface *iTutorial = qobject_cast<TutorialInterface *>(plugin);
      if (iTutorial) {
	// �������� ���������, ����������� ������ ������ � ����,
	// ��������� ������� � ��������� � �������
	addToMenu(plugin, iTutorial->name(), iTutorial->lessons(),
		  iTutorial->helps(), iTutorial->widgetsList(),
		  fileName,
		  lessonsMenu, SLOT(changeLesson()), lessonsActionGroup);
      }
      pluginFileNames += fileName;
    }
  }
  // ���� ���� � ������� ������ �� ������, �� ��������� ������ �� ����
  lessonsMenu->setEnabled(!lessonsActionGroup->actions().isEmpty());
}

void MainWindow::addToMenu(QObject *plugin, const QString &name, const QStringList &texts,
			   const QStringList &helps, const QList<QWidget *> &widgetsList,
			   QString fileName,
			   QMenu *menu, const char *member, QActionGroup *actionGroup)
{
  QByteArray Name = "";
  Name.append(name);
  // ��������� ����� ����� � ������ �������
  tutorialsList.append(name);
  // ��������� ����� ������ � ����
  QMenu *tmpMenu = new QMenu();
  tmpMenu = menu->addMenu(translate(Name));
  // ������� QTabWidget, ������� ����� �������� �� ������ ������
  QTabWidget *lessonsTabs = new QTabWidget();

  foreach (QString text, texts) {
    QAction *action = new QAction(text, plugin);
    connect(action, SIGNAL(triggered()), this, member);
    tmpMenu->addAction(action);

    if (actionGroup) {
      action->setCheckable(true);
      actionGroup->addAction(action);
    }
    // ������� ����������� �������
    // ������� ��������, ����������� ������� � ��������� � ��������
    QSplitter *verticalSplitter = new QSplitter(Qt::Vertical);
    verticalSplitter->addWidget(widgetsList.at(texts.indexOf(text, 0)));
    verticalSplitter->setChildrenCollapsible(false);
    QTextEdit *helpInfo = new QTextEdit(helps.at(texts.indexOf(text, 0)),
					verticalSplitter);
    helpInfo->setTabChangesFocus(true);
    helpInfo->setReadOnly(true);

    // ������� QScrollArea ��� ����, ����� ���� Label � ������ �����������
    // QScrollArea *scrollArea = new QScrollArea(verticalSplitter);
    // ����� � ������.
    // TODO ��������, ������� �� �� �����, � TextEdit?
    // QLabel *helpInfo = new QLabel(helps.at(texts.indexOf(text, 0)));
    // helpInfo->setMargin(2);
    // helpInfo->setTextFormat(Qt::RichText);
    // helpInfo->setWordWrap(true);
    // helpInfo->setScaledContents(true);
    // scrollArea->setWidget(helpInfo);
    // scrollArea->setWidgetResizable(true);
    // ��������� ����� ��� � ������
    lessonsTabs->addTab(verticalSplitter, text);
    // ������ ������� ���������
    QList<int> list;
    list << 300 << 150;
    verticalSplitter->setSizes(list);
  }
  // ���� � ������� ������� �����, �� ��������� �������� � ������� "������ ����"
  fileName.chop(fileName.size()-fileName.lastIndexOf(".")-1);
  fileName.append("dat");
  fileName.prepend("plugins/");
  if (verifyTests(fileName, Name)) {
    QWidget *testTab = new QWidget; // ���� ������ ��� �������� �������
    testTab->setLayout(createTestTabWidget());
    lessonsTabs->addTab(testTab, translate("����"));
    TestWidget *testWidget = new TestWidget(fileName, this);
    lessonsTabs->addTab(testWidget, translate("����������� �����"));
    lessonsTabs->setTabEnabled(lessonsTabs->count()-1, false);
  }
  // ��������� ��������� ���� � ������� �, ��������, ������� � ��� � ���������
  tutorialsTabs->addTab(lessonsTabs, translate(Name));
}

QVBoxLayout* MainWindow::createTestTabWidget()
{
    // ������� ������ ��� ����������� ����� ����������������
    // ��� ����������� �����
    QPushButton *beginTest = new QPushButton(translate("������ ����"));
    QLabel *studNumLabel = new QLabel(translate("����� �������������:"));
    QLineEdit *studNumEdit = new QLineEdit("");
    QLabel *facultyLabel = new QLabel(translate("���������:"));
    QLineEdit *facultyEdit = new QLineEdit("");
    QLabel *groupLabel = new QLabel(translate("������:"));
    QLineEdit *groupEdit = new QLineEdit("");
    QLabel *surnameLabel = new QLabel(translate("�������:"));
    QLineEdit *surnameEdit = new QLineEdit("");
    QGroupBox *groupBox = new QGroupBox(translate("������� ��������������� ������"));
    // ����������� �����
    studNumEdit->setMaxLength(8);
    QIntValidator *studNumValidator = new QIntValidator(20020000, 99999999, this);
    studNumEdit->setValidator(studNumValidator);
    facultyEdit->setMaxLength(16);
    QIntValidator *groupValidator = new QIntValidator(100, 600, this);
    groupEdit->setMaxLength(3);
    groupEdit->setValidator(groupValidator);
    surnameEdit->setMaxLength(16);
    // ����������� ��������
    groupBox->setFlat(true);

    connect(beginTest, SIGNAL(clicked()), this, SLOT(validateUser()));
    connect(studNumEdit, SIGNAL(textChanged(QString)), this, SLOT(setStudNum(QString)));
    connect(facultyEdit, SIGNAL(textChanged(QString)), this, SLOT(setFaculty(QString)));
    connect(groupEdit, SIGNAL(textChanged(QString)), this, SLOT(setGroup(QString)));
    connect(surnameEdit, SIGNAL(textChanged(QString)), this, SLOT(setSurname(QString)));

    // �����������
    QGridLayout *mainGLayout = new QGridLayout;
    mainGLayout->addWidget(studNumLabel, 0, 0);
    mainGLayout->addWidget(studNumEdit, 0, 1);
    mainGLayout->addWidget(facultyLabel, 1, 0);
    mainGLayout->addWidget(facultyEdit, 1, 1);
    mainGLayout->addWidget(groupLabel, 2, 0);
    mainGLayout->addWidget(groupEdit, 2, 1);
    mainGLayout->addWidget(surnameLabel, 3, 0);
    mainGLayout->addWidget(surnameEdit, 3, 1);
    mainGLayout->addWidget(beginTest, 4, 0, 1, 2);

    QHBoxLayout *mainHLayout = new QHBoxLayout;
    mainHLayout->addStretch();
    mainHLayout->addWidget(groupBox);
    mainHLayout->addStretch();

    QVBoxLayout *mainVLayout = new QVBoxLayout;
    mainVLayout->addStretch();
    mainVLayout->addLayout(mainHLayout);
    mainVLayout->addStretch();

    groupBox->setLayout(mainGLayout);
    return mainVLayout;
}

void MainWindow::setStudNum(QString newStudNum)
{
  QIntValidator studNumValidator(20020000, 99999999, this);
  int pos = 0;
  if (studNumValidator.validate(newStudNum, pos)) studNum = newStudNum;
}

void MainWindow::setFaculty(QString newFaculty) { faculty = newFaculty; }

void MainWindow::setGroup(QString newGroup) {
  QIntValidator groupValidator(100, 600, this);
  int pos = 0;
  if (groupValidator.validate(newGroup, pos)) group = newGroup;
}

void MainWindow::setSurname(QString newSurname) { surname = newSurname; }

void MainWindow::validateUser()
{
  // �������, ���� ���� ��� ������ ���� ��� ������:
  QIntValidator studNumValidator(20020000, 99999999, this);
  QIntValidator groupValidator(100, 600, this);
  int pos = 0;
  if (!(studNumValidator.validate(studNum, pos) &&
	groupValidator.validate(group, pos) &&
	faculty.length() > 0 &&
	surname > 0)) {
    switch (QMessageBox::warning(
				 this,
				 translate("������� �������� ������"),
				 translate("�� ����� �� ��� ������.<br>��������� ������������ ���������� � ���������� ��� ���."),
				 translate("&������, ��������"))) {
    case 0: return;
    }
  }

  // ��������, ����� �� ����� ���� ������������ ��������� ����
  // ��������� ���� � ������������
  // ���� ������������� � �������:
  // <����� ������><�����_�������������>@<���������>@<������>@<�������>&<����_�����>$<�����>#<����_�����>$<�����>#<����_�����>$<�����>..
  QStringList Results;
  QFile file("./results.dat");
  file.open(QIODevice::ReadOnly);
  QDataStream in(&file);
  while (!in.atEnd()) {
    int size = 0;
    QByteArray tmp;
    in >> size;
    in >> tmp;
    QString decrypted = decrypt(tmp);
    decrypted.truncate(size);
    Results.append(translate(decrypted));
  }
  file.close();

  // ���� ������������ � ��� �� ������� �������������
  if (Results.filter(studNum).isEmpty()) {
    // �������� � ����� �������, ���� �� ����
    if (Results.filter(group).filter(surname).isEmpty()) {
      // ��� ��������� � ��� �� �������� � ��� �� ������
      switch (QMessageBox::question(
				    this,
				    translate("��������� ����?"),
				    translate("�� ������������ ������ ���. ������ ����������?"),
				    translate("&��"), translate("&���"))) {
      case 0: runTest();
      case 1: return;
      }
    } else {
      // ������� �������� � ��� �� ��������
      switch (QMessageBox::question(
				    this,
				    translate("��������� ����?"),
				    translate("� ��� ���� ����������� � ������?<br>���� ���, �� ������� ���������� ����� �������������."),
				    translate("&��, ����, ��������� ����"), translate("&���, � ����������(���)"))) {
      case 0: runTest();
      case 1: return;
      }
    }
  } else {
    // ������� � ����� ������� ������������� ��� ���
    // ���� ��������, �������� �� �� ������ ���� ����
    if (Results.filter(studNum).at(0).contains(tutorialsTabs->
					       tabText(tutorialsTabs->
						       currentIndex()))) {
      // ���, ���� ���� �� ��� ��������
      switch (QMessageBox::warning(
				   this,
				   translate("���� ���� �� ��� ���������"),
				   translate("�� ��� ��������� ���� ����.<br>��������� ��� ����������� ���� ����������.<br>����������������� �� ������."),
				   translate("&������, ������ �� ����"))) {
      case 0: return;
      }
    } else {
      // ������ ���� ���� �� �� ��������, ����� ��������!
      switch (QMessageBox::question(
				    this,
				    translate("��������� ����?"),
				    translate("�� ��� �������������, �� ������ ���� ���� ��������� �������.<br>������ ����������?"),
				    translate("&��"), translate("&���"))) {
      case 0: runTest();
      case 1: return;
      }
    }
  }
}

bool MainWindow::verifyTests(QString fileName, QString Name)
{
  QFile file;
  QDataStream in;
  if (QFile::exists(fileName)) {
    file.setFileName(fileName);
    file.open(QIODevice::ReadOnly);
    in.setDevice(&file);
    // ������ �������� ��������� �����
    quint32 magic;
    in >> magic;
    if ((magic != 0xA1A2A3A4) && (file.size() > 0)) {
      file.close();
      return false;
    }
    // ��������� ���������� �������� ����� � ���������
    qint32 size = 0;
    in >> size;
    QByteArray testName;
    in >> testName;
    QString decrypted = decrypt(testName);
    decrypted.truncate(size);
    if (decrypted != Name) {
      file.close();
      return false;
    }
    file.close();
    return true;
  }
  file.close();
  return false;
}

void MainWindow::saveResults(int rightAnswers, int totalQuestions)
{
  QStringList Results;
  QFile file("./results.dat");
  file.open(QIODevice::ReadOnly);
  QDataStream in(&file);
  while (!in.atEnd()) {
    int size = 0;
    QByteArray tmp;
    in >> size;
    in >> tmp;
    QString decrypted = decrypt(tmp);
    decrypted.truncate(size);
    Results.append(translate(decrypted));
  }
  file.close();
  // <����� ������><�����_�������������>@<���������>@<������>@<�������>&<����_�����>$<�����>#<����_�����>$<�����>#<����_�����>$<�����>..
  // ���� ������������ � ��� �� ������� �������������
  if (Results.filter(studNum).isEmpty()) {
    // �������� � ����� �������, ���� �� ����
    if (Results.filter(group).filter(surname).isEmpty()) {
      // ��� ��������� � ��� �� �������� � ��� �� ������
      QString result = studNum+"@"+faculty+"@"+group+"@"+
	surname+"&"+tutorialsTabs->tabText(tutorialsTabs->currentIndex())+"$"+
	QString("%1").arg(rightAnswers)+"."+QString("%1").arg(totalQuestions);
      Results.append(result);
    } else {
      // ������� �������� � ��� �� ��������
      QString result = studNum+"@"+faculty+"@"+group+"@"+
	surname+"&"+tutorialsTabs->tabText(tutorialsTabs->currentIndex())+"$"+
	QString("%1").arg(rightAnswers)+"."+QString("%1").arg(totalQuestions);
      Results.append(result);
    }
  } else {
    // ������� � ����� ������� ������������� ��� ���
    // ���� ��������, �������� �� �� ������ ���� ����
    if (Results.filter(studNum).at(0).contains(tutorialsTabs->
					       tabText(tutorialsTabs->
						       currentIndex()))) {
      // ���, ���� ���� �� ��� ��������
    } else {
      // ������ ���� ���� �� �� ��������, ����� ��������!
      QString result = Results.filter(studNum).at(0)+
	"#"+tutorialsTabs->tabText(tutorialsTabs->currentIndex())+"$"+
	QString("%1").arg(rightAnswers)+"."+QString("%1").arg(totalQuestions);
      Results.filter(studNum)[0] = result;
    }
  }
  file.open(QIODevice::WriteOnly);
  file.resize(0);
  QDataStream out(&file);
  for (int i = 0; i < Results.size(); i++) {
    out << Results.at(i).size();
    out << encrypt(Results.at(i));
  }
  file.close();

}
