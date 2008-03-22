/****************************************************************************
 ***      Файл реализации функциональности главного окна приложения       ***
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

  setWindowTitle(translate("Наглядная демонстрация сложных физических явлений"));

  if (!lessonsActionGroup->actions().isEmpty())
    lessonsActionGroup->actions().first()->trigger();

  // создаем и делаем основным разделяющий по горизонтали сплиттер
  horizontalSplitter = new QSplitter(Qt::Horizontal, this);
  setCentralWidget(horizontalSplitter);
  // создаем QTabWidget, который будет состоять из набора занятий
  tutorialsTabs = new QTabWidget(this);
  // добавляем его в левую часть horizontalSplitter
  horizontalSplitter->insertWidget(1, tutorialsTabs);
  // грузим плагины
  loadPlugins();
  // создаем (временно на главном окне) список плагинов
  pluginsList = new PluginsList(pluginsDir.path(), pluginFileNames, this);
  // переставляем его в левую часть horizontalSplitter
  horizontalSplitter->insertWidget(0, pluginsList);
  pluginsList->hide();
  // Задаем размеры
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
  // Перемещаемся в директорию, где должны лежать плагины
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

  // Пролистываем каждый файл в текущей директории
  foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
    // Загружаем текущий файл, как-будто это плагин
    QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
    QObject *plugin = loader.instance();
    // Если это реально, плагин...
    if (plugin) {
      // Создаем объект, который в нашем случае является "занятием"
      TutorialInterface *iTutorial = qobject_cast<TutorialInterface *>(plugin);
      if (iTutorial) {
	// Если в списке уроков данного занятия присутствует интересующий нас...
	if (iTutorial->lessons().indexOf(lesson) != -1) {
	  // Делаем активной вкладку с нужным занятием
	  tutorialsTabs->setCurrentIndex(tutorialsList.indexOf(iTutorial->name()));
	  // Делаем активной вкладку с интересующим нас уроком в вкладке с нужным занятием
	  // Это возможно, потому-что на вкладках с занятиями ничего, кроме
	  // вкладок с уроками нет, поэтому получаем указатель на виджет в
	  // текущей вкладке с занятиями, предполагая что этот виджет QTabWidget,
	  // делаем текущей необходимую вкладку
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
  // выясняем, какое Action вызвало этот SLOT
  QAction *action = qobject_cast<QAction *>(sender());
  // вызываем соответствующий Action плагин
  TutorialInterface *iTutorial = qobject_cast<TutorialInterface *>(action->parent());
  // по названию Action определяем, какой именно урок нас интересует
  QString lesson = action->text();
  changeLess(lesson);
}

void MainWindow::about()
{
  QMessageBox::about(this, translate("О программе"),
		     translate("Эта программа предназначена для просмотра демонстрационно-обучающих уроков,\n которые добавляются в виде подключаемых модулей(плагинов).\n                                                                                    code zOrg"));
}

void MainWindow::createActions()
{
  exitAct = new QAction(translate("Выход"), this);
  exitAct->setShortcut(tr("Ctrl+Q"));
  connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

  lessonsActionGroup = new QActionGroup(this);

  aboutAct = new QAction(translate("&О программе"), this);
  connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

  aboutQtAct = new QAction(translate("О &Qt"), this);
  connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void MainWindow::createMenus()
{
  fileMenu = menuBar()->addMenu(translate("&Файл"));
  fileMenu->addSeparator();
  fileMenu->addAction(exitAct);

  lessonsMenu = menuBar()->addMenu(translate("&Уроки"));

  menuBar()->addSeparator();

  helpMenu = menuBar()->addMenu(translate("&Справка"));
  helpMenu->addAction(aboutAct);
  helpMenu->addAction(aboutQtAct);
}

void MainWindow::runTest()
{
  // Перемещаемся в директорию, где должны лежать плагины
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

  // Пролистываем каждый файл в текущей директории
  int i = 0;
  foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
    // Загружаем текущий файл, как-будто это плагин
    QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
    QObject *plugin = loader.instance();
    // Если это реально, плагин...
    if (plugin) {
      // работаем только с туториалом, где только что нажали кнопку "Начать тест"
      if (i == tutorialsTabs->currentIndex()) {
	// скрываем все вкладки с уроками и пускаем тест
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
  // Перемещаемся в директорию, где должны лежать плагины
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

  // Пролистываем каждый файл в текущей директории
  int i = 0;
  foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
    // Загружаем текущий файл, как-будто это плагин
    QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
    QObject *plugin = loader.instance();
    // Если это реально, плагин...
    if (plugin) {
      // работаем только с туториалом, где только что нажали кнопку "Начать тест"
      if (i == tutorialsTabs->currentIndex()) {
	// открываем все вкладки с уроками и закрываем тест
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
  // Перемещаемся в директорию, где должны лежать плагины
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

  // Пролистываем каждый файл в текущей директории
  foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
    // Загружаем текущий файл, как-будто это плагин
    QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
    QObject *plugin = loader.instance();
    // Если это реально, плагин...
    if (plugin) {
      // Создаем объект, который в нашем случае является "занятием"
      TutorialInterface *iTutorial = qobject_cast<TutorialInterface *>(plugin);
      if (iTutorial) {
	// Вызываем процедуру, добавляющую нужные пункты в меню,
	// создающую виджеты с занятиями и уроками
	addToMenu(plugin, iTutorial->name(), iTutorial->lessons(),
		  iTutorial->helps(), iTutorial->widgetsList(),
		  fileName,
		  lessonsMenu, SLOT(changeLesson()), lessonsActionGroup);
      }
      pluginFileNames += fileName;
    }
  }
  // Если меню с уроками теперь не пустое, то разрешаем тыкать по нему
  lessonsMenu->setEnabled(!lessonsActionGroup->actions().isEmpty());
}

void MainWindow::addToMenu(QObject *plugin, const QString &name, const QStringList &texts,
			   const QStringList &helps, const QList<QWidget *> &widgetsList,
			   QString fileName,
			   QMenu *menu, const char *member, QActionGroup *actionGroup)
{
  QByteArray Name = "";
  Name.append(name);
  // Добавляем новый пункт в список занятий
  tutorialsList.append(name);
  // Добавляем новый раздел в меню
  QMenu *tmpMenu = new QMenu();
  tmpMenu = menu->addMenu(translate(Name));
  // создаем QTabWidget, который будет состоять из набора уроков
  QTabWidget *lessonsTabs = new QTabWidget();

  foreach (QString text, texts) {
    QAction *action = new QAction(text, plugin);
    connect(action, SIGNAL(triggered()), this, member);
    tmpMenu->addAction(action);

    if (actionGroup) {
      action->setCheckable(true);
      actionGroup->addAction(action);
    }
    // создаем необходимые виджеты
    // Создаем сплиттер, разделяющий область с виджетами и справкой
    QSplitter *verticalSplitter = new QSplitter(Qt::Vertical);
    verticalSplitter->addWidget(widgetsList.at(texts.indexOf(text, 0)));
    verticalSplitter->setChildrenCollapsible(false);
    QTextEdit *helpInfo = new QTextEdit(helps.at(texts.indexOf(text, 0)),
					verticalSplitter);
    helpInfo->setTabChangesFocus(true);
    helpInfo->setReadOnly(true);

    // Создаем QScrollArea для того, чтобы наша Label с хелпом скроллилась
    // QScrollArea *scrollArea = new QScrollArea(verticalSplitter);
    // Лейбл с хелпой.
    // TODO подумать, сделать ли не Лейбл, а TextEdit?
    // QLabel *helpInfo = new QLabel(helps.at(texts.indexOf(text, 0)));
    // helpInfo->setMargin(2);
    // helpInfo->setTextFormat(Qt::RichText);
    // helpInfo->setWordWrap(true);
    // helpInfo->setScaledContents(true);
    // scrollArea->setWidget(helpInfo);
    // scrollArea->setWidgetResizable(true);
    // Добавляем новый таб с уроком
    lessonsTabs->addTab(verticalSplitter, text);
    // Задаем размеры сплиттера
    QList<int> list;
    list << 300 << 150;
    verticalSplitter->setSizes(list);
  }
  // если у занятия имеются тесты, то добавляем закладку с кнопкой "Начать тест"
  fileName.chop(fileName.size()-fileName.lastIndexOf(".")-1);
  fileName.append("dat");
  fileName.prepend("plugins/");
  if (verifyTests(fileName, Name)) {
    QWidget *testTab = new QWidget; // сюда сложим все дочерние виджеты
    testTab->setLayout(createTestTabWidget());
    lessonsTabs->addTab(testTab, translate("Тест"));
    TestWidget *testWidget = new TestWidget(fileName, this);
    lessonsTabs->addTab(testWidget, translate("Прохождение теста"));
    lessonsTabs->setTabEnabled(lessonsTabs->count()-1, false);
  }
  // Добавляем созданные табы с уроками и, возможно, тестами в таб с занятиями
  tutorialsTabs->addTab(lessonsTabs, translate(Name));
}

QVBoxLayout* MainWindow::createTestTabWidget()
{
    // создаем виджет где тестируемый будет регистрироваться
    // для прохождения теста
    QPushButton *beginTest = new QPushButton(translate("Начать тест"));
    QLabel *studNumLabel = new QLabel(translate("Номер студенческого:"));
    QLineEdit *studNumEdit = new QLineEdit("");
    QLabel *facultyLabel = new QLabel(translate("Факультет:"));
    QLineEdit *facultyEdit = new QLineEdit("");
    QLabel *groupLabel = new QLabel(translate("Группа:"));
    QLineEdit *groupEdit = new QLineEdit("");
    QLabel *surnameLabel = new QLabel(translate("Фамилия:"));
    QLineEdit *surnameEdit = new QLineEdit("");
    QGroupBox *groupBox = new QGroupBox(translate("Введите регистрационные данные"));
    // настраиваем едиты
    studNumEdit->setMaxLength(8);
    QIntValidator *studNumValidator = new QIntValidator(20020000, 99999999, this);
    studNumEdit->setValidator(studNumValidator);
    facultyEdit->setMaxLength(16);
    QIntValidator *groupValidator = new QIntValidator(100, 600, this);
    groupEdit->setMaxLength(3);
    groupEdit->setValidator(groupValidator);
    surnameEdit->setMaxLength(16);
    // настраиваем групбокс
    groupBox->setFlat(true);

    connect(beginTest, SIGNAL(clicked()), this, SLOT(validateUser()));
    connect(studNumEdit, SIGNAL(textChanged(QString)), this, SLOT(setStudNum(QString)));
    connect(facultyEdit, SIGNAL(textChanged(QString)), this, SLOT(setFaculty(QString)));
    connect(groupEdit, SIGNAL(textChanged(QString)), this, SLOT(setGroup(QString)));
    connect(surnameEdit, SIGNAL(textChanged(QString)), this, SLOT(setSurname(QString)));

    // выравниваем
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
  // главное, чтоб юзер для начала ввел все данные:
  QIntValidator studNumValidator(20020000, 99999999, this);
  QIntValidator groupValidator(100, 600, this);
  int pos = 0;
  if (!(studNumValidator.validate(studNum, pos) &&
	groupValidator.validate(group, pos) &&
	faculty.length() > 0 &&
	surname > 0)) {
    switch (QMessageBox::warning(
				 this,
				 translate("Введены неполные данные"),
				 translate("Вы ввели не все данные.<br>Проверьте правильность информации и попробуйте еще раз."),
				 translate("&Хорошо, попробую"))) {
    case 0: return;
    }
  }

  // выясняем, имеет ли право этот пользователь проходить тест
  // считываем файл с результатами
  // файл зашифрованный в формате:
  // <длина строки><номер_студенческого>@<факультет>@<группа>@<фамилия>&<назв_теста>$<баллы>#<назв_теста>$<баллы>#<назв_теста>$<баллы>..
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

  // ищем тестируемого с тем же номером студенческого
  if (Results.filter(studNum).isEmpty()) {
    // студента с таким номером, пока не было
    if (Results.filter(group).filter(surname).isEmpty()) {
      // нет студентов с той же фамилией в той же группе
      switch (QMessageBox::question(
				    this,
				    translate("Запустить тест?"),
				    translate("Вы тестируетесь первый раз. Хотите продолжать?"),
				    translate("&Да"), translate("&Нет"))) {
      case 0: runTest();
      case 1: return;
      }
    } else {
      // имеются студенты с той же фамилией
      switch (QMessageBox::question(
				    this,
				    translate("Запустить тест?"),
				    translate("У вас есть однофамилец в группе?<br>Если нет, то укажите правильный номер студенческого."),
				    translate("&Да, есть, запустить тест"), translate("&Нет, я опечатался(ась)"))) {
      case 0: runTest();
      case 1: return;
      }
    }
  } else {
    // студент с таким номером студенческого уже был
    // надо выяснить, проходил ли он именно этот тест
    if (Results.filter(studNum).at(0).contains(tutorialsTabs->
					       tabText(tutorialsTabs->
						       currentIndex()))) {
      // ага, этот тест он уже проходил
      switch (QMessageBox::warning(
				   this,
				   translate("Этот тест вы уже проходили"),
				   translate("Вы уже выполняли этот тест.<br>Повторное его прохождение вами невозможно.<br>Сконцентрируйтесь на теории."),
				   translate("&Хорошо, больше не буду"))) {
      case 0: return;
      }
    } else {
      // именно этот тест он не проходил, пусть проходит!
      switch (QMessageBox::question(
				    this,
				    translate("Запустить тест?"),
				    translate("Вы уже тестировались, но именно этот тест проходите впервые.<br>Хотите продолжать?"),
				    translate("&Да"), translate("&Нет"))) {
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
    // делаем проверку заголовка файла
    quint32 magic;
    in >> magic;
    if ((magic != 0xA1A2A3A4) && (file.size() > 0)) {
      file.close();
      return false;
    }
    // проверяем совпадение названия теста и туториала
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
  // <длина строки><номер_студенческого>@<факультет>@<группа>@<фамилия>&<назв_теста>$<баллы>#<назв_теста>$<баллы>#<назв_теста>$<баллы>..
  // ищем тестируемого с тем же номером студенческого
  if (Results.filter(studNum).isEmpty()) {
    // студента с таким номером, пока не было
    if (Results.filter(group).filter(surname).isEmpty()) {
      // нет студентов с той же фамилией в той же группе
      QString result = studNum+"@"+faculty+"@"+group+"@"+
	surname+"&"+tutorialsTabs->tabText(tutorialsTabs->currentIndex())+"$"+
	QString("%1").arg(rightAnswers)+"."+QString("%1").arg(totalQuestions);
      Results.append(result);
    } else {
      // имеются студенты с той же фамилией
      QString result = studNum+"@"+faculty+"@"+group+"@"+
	surname+"&"+tutorialsTabs->tabText(tutorialsTabs->currentIndex())+"$"+
	QString("%1").arg(rightAnswers)+"."+QString("%1").arg(totalQuestions);
      Results.append(result);
    }
  } else {
    // студент с таким номером студенческого уже был
    // надо выяснить, проходил ли он именно этот тест
    if (Results.filter(studNum).at(0).contains(tutorialsTabs->
					       tabText(tutorialsTabs->
						       currentIndex()))) {
      // ага, этот тест он уже проходил
    } else {
      // именно этот тест он не проходил, пусть проходит!
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
