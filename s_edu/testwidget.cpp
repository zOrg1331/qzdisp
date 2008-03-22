/**********************************************************
 ***     Виджета, позволяющего провети тестирование     ***
 **********************************************************/

// Time-stamp: <testwidget.cpp - 22:28:08 23.05.2006>

#include "testwidget.h"
#include <translate.h>
#include <xcrypt.h>

#include <QtGui>

TestWidget::TestWidget(QString fileName, MainWindow *mainwindow)
{
  questBox = new QGroupBox(translate("Вопрос:"));
  questBoxLayout = new QVBoxLayout();
  questLabel = new QLabel("");
  nextQuest = new QPushButton(translate("Следующий вопрос->"));
  prevQuest = new QPushButton(translate("<-Предыдущий вопрос"));
  makeAns = new QPushButton(translate("Ответить"));
  quitButt = new QPushButton(translate("Выход"));
  reView = new QPushButton(translate("Позаниматься с программой еще немного"));
  answsBox = new QGroupBox(translate("Варианты ответов:"));
  answsBoxLayout = new QVBoxLayout();
  firstAnsBox = new QGroupBox(" ");
  firstAnsLabel = new QLabel("");
  firstAnsLayout = new QVBoxLayout();
  secondAnsBox = new QGroupBox(" ");
  secondAnsLabel = new QLabel("");
  secondAnsLayout = new QVBoxLayout();
  thirdAnsBox = new QGroupBox(" ");
  thirdAnsLabel = new QLabel("");
  thirdAnsLayout = new QVBoxLayout();
  fourthAnsBox = new QGroupBox(" ");
  fourthAnsLabel = new QLabel("");
  fourthAnsLayout = new QVBoxLayout();
  vLayout = new QVBoxLayout;
  hLayout = new QHBoxLayout;
  questLayout = new QHBoxLayout;

  // настраиваем лейбл с вопросом
  questLabel->setWordWrap(true);

  // настраиваем гроупбоксы
  questBoxLayout->addWidget(questLabel);
  questBoxLayout->setMargin(1);
  questBox->setLayout(questBoxLayout);

  answsBoxLayout->addWidget(firstAnsBox);
  answsBoxLayout->addWidget(secondAnsBox);
  answsBoxLayout->addWidget(thirdAnsBox);
  answsBoxLayout->addWidget(fourthAnsBox);
  answsBoxLayout->setMargin(1);
  answsBox->setLayout(answsBoxLayout);

  firstAnsLayout->addWidget(firstAnsLabel);
  firstAnsLayout->setMargin(1);
  firstAnsBox->setLayout(firstAnsLayout);
  firstAnsBox->setCheckable(true);
  firstAnsBox->setChecked(true);
  firstAnsLabel->setWordWrap(true);

  secondAnsLayout->addWidget(secondAnsLabel);
  secondAnsLayout->setMargin(1);
  secondAnsBox->setLayout(secondAnsLayout);
  secondAnsBox->setCheckable(true);
  secondAnsBox->setChecked(false);
  secondAnsLabel->setWordWrap(true);

  thirdAnsLayout->addWidget(thirdAnsLabel);
  thirdAnsLayout->setMargin(1);
  thirdAnsBox->setLayout(thirdAnsLayout);
  thirdAnsBox->setCheckable(true);
  thirdAnsBox->setChecked(false);
  thirdAnsLabel->setWordWrap(true);

  fourthAnsLayout->addWidget(fourthAnsLabel);
  fourthAnsLayout->setMargin(1);
  fourthAnsBox->setLayout(fourthAnsLayout);
  fourthAnsBox->setCheckable(true);
  fourthAnsBox->setChecked(false);
  fourthAnsLabel->setWordWrap(true);

  connect(firstAnsBox, SIGNAL(toggled(bool)), this, SLOT(firstToggledSlot(bool)));
  connect(secondAnsBox, SIGNAL(toggled(bool)), this, SLOT(secondToggledSlot(bool)));
  connect(thirdAnsBox, SIGNAL(toggled(bool)), this, SLOT(thirdToggledSlot(bool)));
  connect(fourthAnsBox, SIGNAL(toggled(bool)), this, SLOT(fourthToggledSlot(bool)));

  MAINWINDOW = mainwindow;

  connect(nextQuest, SIGNAL(clicked()), this, SLOT(nextQuestion()));
  connect(prevQuest, SIGNAL(clicked()), this, SLOT(prevQuestion()));
  connect(makeAns, SIGNAL(clicked()), this, SLOT(makeAnswer()));
  connect(quitButt, SIGNAL(clicked()), qApp, SLOT(quit()));
  connect(reView, SIGNAL(clicked()), mainwindow, SLOT(stopTest()));

  quitButt->hide();
  reView->hide();

  hLayout->addStretch();
  hLayout->addWidget(prevQuest);
  hLayout->addWidget(makeAns);
  hLayout->addWidget(reView);
  hLayout->addWidget(quitButt);
  hLayout->addWidget(nextQuest);
  hLayout->addStretch();

  questLayout->addWidget(questBox);
  //  questLayout->addWidget(Images.at(0));

  vLayout->addLayout(questLayout);
  vLayout->addWidget(answsBox);
  vLayout->addLayout(hLayout);
  setLayout(vLayout);

  initializeTest(fileName);
  currQuestion = 1;
  askQuestion();
}

void TestWidget::askQuestion()
{
  QString question = testList.at(currQuestion);
  // секции в строке вопроса разделяются символом "@"
  questLabel->setText(question.remove(0, 1).section('@', 0, 0));
  QStringList answers;
  answers << question.section('@', 1, 1)
	  << question.section('@', 2, 2)
	  << question.section('@', 3, 3)
	  << question.section('@', 4, 4);
  // перемешиваем список ответов
  QTime midnight(0, 0, 0);
  srand(midnight.msecsTo(QTime::currentTime()));
  // типа, ждем, чтобы прошло время, а то случайные величины будут повторяться
  for (int dummy = 0; dummy < 200000; dummy++) {}
  for (int i = 0; i < 10; i++) {
    answers.swap(3, rand()%2);
  }
  firstAnsLabel->setText(answers.at(0));
  secondAnsLabel->setText(answers.at(1));
  thirdAnsLabel->setText(answers.at(2));
  fourthAnsLabel->setText(answers.at(3));

  // скрываем все картинки
  for (int i = 0; i < Images.size(); i++) {
    Images.at(i)->hide();
  }
  // суем ту, что нужно в лайоут
  questLayout->insertWidget(3, Images.at(currQuestion-1));
  // показываем ту, что нужно
  Images.at(currQuestion-1)->show();
}

void TestWidget::initializeTest(QString fileName)
{
  // заполняем QStringList tests и устанавливаем totalQuestions
  QStringList Tests;
  QVector <QLabel *> allImages;
  QFile file(fileName);
  file.open(QIODevice::ReadOnly);
  QDataStream in(&file);
  // проверка уже была сделана
  quint32 magic;
  in >> magic;
  qint32 size = 0;
  in >> size;
  QByteArray testName;
  in >> testName;
  in >> totalQuestions;
  while (!in.atEnd()) {
    int size = 0;
    QByteArray tmp;
    QImage image;
    in >> size;
    in >> tmp;
    in >> image;
    QString decrypted = decrypt(tmp);
    // контролируем длину, т.к. после шифрования длина кратна 16ти символам
    decrypted.truncate(size);
    // !! обязательно вызывать translate() !! //
    Tests.append(translate(decrypted));

    QLabel *imageLabel = new QLabel(this);
    imageLabel->setBackgroundRole(QPalette::Base);
    // imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);
    imageLabel->hide();
    allImages.append(imageLabel);
    allImages[allImages.size()-1]->setPixmap(QPixmap::fromImage(image));
  }
  file.close();
  // сие есть список списков строк
  // каждый элемент этого списка соответствует списку вопросов, принадлежыщих
  //  группе с номером равным индексу элемента
  QList<QStringList> groups;
  QStringList empty;
  empty << "";
  for (int i = 1; i < 100; i++) {
    groups.append(empty);
  }
  int totalGroups = 0;
  for (int i = 0; i < Tests.size(); i++) {
    int group = Tests.at(i).at(0).digitValue();
    if (group > totalGroups) totalGroups = group;
    groups[group].append(Tests.at(i));
  }
  // for (int i = 1; i <= totalGroups; i++) {
  //   for (int j = 0; j < groups.at(i).size(); j++) {
  //     qDebug() << groups.at(i).at(j);
  //   }
  // }
  // составляем список вопросов, которые будем задавать
  // выбираются в случайном порядке из группы (сами группы идут по порядку)
  // количество вопросов, которое нужно задать делится поровну между группами
  int questsPerGroup = totalQuestions/totalGroups;
  testList.append("");		// дабы можно было нумеровать вопросы с единицы
  for (int i = 1; i <= totalGroups; i++) {
    QList<int> choosed; // номера уже выбранных вопросов из текущей группы
    choosed << 0;
    for (int j = 1; j <= questsPerGroup; j++) {
      QTime midnight(0, 0, 0);
      srand(midnight.msecsTo(QTime::currentTime()));
      // типа, ждем, чтобы прошло время, а то случайные величины будут повторяться
      for (int dummy = 0; dummy < 200000; dummy++) {}
      int k = 0;
      // номер вопроса выбираем случайным образом, избегая повторений
      // нужно учитывать, что первый элемент в списке вопросов в группе - пустой
      while (choosed.indexOf(k) != -1) {
	k = 1+rand()%(groups.at(i).size()-1);
      }
      choosed.append(k);
      testList.append(groups.at(i).at(k));

      Images.append(allImages.at(Tests.indexOf(groups.at(i).at(k))));
    }
  }
  // for (int i = 0; i < testList.size(); i++) {
  //   qDebug() << testList.at(i);
  // }
  // заполняем список статуса нулями, т.к. вопросы еще не задавали
  for (int i = 0; i <= totalQuestions; i++) {
    questStatus.append(0);
  }
}

void TestWidget::nextQuestion()
{
  // если есть неотвеченный вопрос в конце списка
  if (questStatus.indexOf(0, currQuestion+1) != -1) {
    currQuestion = questStatus.indexOf(0, currQuestion+1);
    askQuestion();
    // если неотвеченный вопрос остался в начале
  } else if (questStatus.indexOf(0, 1) != -1) {
    currQuestion = questStatus.indexOf(0, 1);
    askQuestion();
    // если неотвеченных не осталось, завершаем тест
  } else {
    showResults();
  }
}

void TestWidget::prevQuestion()
{
  int i = currQuestion - 1;
  // перемещаемся в начало списка
  while ((questStatus.at(i) > 0) && (i >= 1)) { i--; }
  if (i > 1) {
    currQuestion = i;
    askQuestion();
    // если неотвеченным является самый первый вопрос
  } else if ((i == 1) && (questStatus.at(i) == 0)) {
    currQuestion = 1;
    askQuestion();
    // если же нет, то передвигаемся в конец списка
  } else {
    i = questStatus.size() - 1;
    while ((questStatus.at(i) > 0) && (i > currQuestion)) { i--; }
    if ((i == currQuestion) && (questStatus.at(currQuestion) > 0)) {
      showResults();
    } else {
      currQuestion = i;
      askQuestion();
    }
  }
}

void TestWidget::makeAnswer()
{
  QString userAnswer;
  // ищем, какой радиобаттон выбран
  if (firstAnsBox->isChecked()) { userAnswer = firstAnsLabel->text(); }
  if (secondAnsBox->isChecked()) { userAnswer = secondAnsLabel->text(); }
  if (thirdAnsBox->isChecked()) { userAnswer = thirdAnsLabel->text(); }
  if (fourthAnsBox->isChecked()) { userAnswer = fourthAnsLabel->text(); }
  // проверяем верность ответа
  if (userAnswer == testList.at(currQuestion).section('@', 1, 1)) {
    // типа, правильно ответили :)
    questStatus[currQuestion] = 1;
  } else {
    questStatus[currQuestion] = 2;
  }
  nextQuestion();
}

void TestWidget::showResults()
{
  // считаем результаты
  int rightAnswers = 0;
  for (int i = 1; i <= totalQuestions; i++) {
    if (questStatus.at(i) == 1) { rightAnswers++; }
  }
  double perc = (double)rightAnswers/(double)totalQuestions;
  MAINWINDOW->saveResults(rightAnswers, totalQuestions);
  QString str = QString("Результаты теста: <b>%1</b> из <b>%2</b>, что составляет <b>%3</b> процентов.<br>")
    .arg(rightAnswers)
    .arg(totalQuestions)
    .arg(perc*100);
  if (perc <= 0.3) {
    str.append("Это не хорошо, позанимайтесь дополнительно.</b>");
    str.prepend("<b>");
  }
  if ((perc > 0.3) && (perc <= 0.6)) {
    str.append("Это не очень хорошо, позанимайтесь дополнительно.</font>");
    str.prepend("<font color=brown>");
  }
  if ((perc > 0.6) && (perc <= 0.8)) {
    str.append("Это хорошо, можете считать, что вы знакомы с основами изучаемого явления.</font>");
    str.prepend("<font color=blue>");
    quitButt->show();
  }
  if ((perc > 0.8) && (perc <= 1)) {
    str.append("Это очень хорошо, можете считать, что вы знакомы с изучаемым явлением, или вам повезло <font color=blue>^_^</font>.</font>");
    str.prepend("<font color=green>");
    quitButt->show();
  }
  questLabel->setText(translate(str));
  firstAnsBox->hide();
  secondAnsBox->hide();
  thirdAnsBox->hide();
  fourthAnsBox->hide();
  answsBox->hide();
  questBox->setTitle("");
  nextQuest->hide();
  prevQuest->hide();
  makeAns->hide();
  // скрываем все картинки
  for (int i = 0; i < Images.size(); i++) {
    Images.at(i)->hide();
  }
  reView->show();
}

void TestWidget::firstToggledSlot(bool checked)
{
  if (checked) {
    secondAnsBox->setChecked(false);
    thirdAnsBox->setChecked(false);
    fourthAnsBox->setChecked(false);
  }
}

void TestWidget::secondToggledSlot(bool checked)
{
  if (checked) {
    firstAnsBox->setChecked(false);
    thirdAnsBox->setChecked(false);
    fourthAnsBox->setChecked(false);
  }
}

void TestWidget::thirdToggledSlot(bool checked)
{
  if (checked) {
    secondAnsBox->setChecked(false);
    firstAnsBox->setChecked(false);
    fourthAnsBox->setChecked(false);
  }
}

void TestWidget::fourthToggledSlot(bool checked)
{
  if (checked) {
    secondAnsBox->setChecked(false);
    thirdAnsBox->setChecked(false);
    firstAnsBox->setChecked(false);
  }
}
