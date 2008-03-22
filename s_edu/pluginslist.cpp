/********************************************************************************
 *** Файл реализации виджета, показывающего информацию о загруженных плагинах ***
 ********************************************************************************/

// Time-stamp: <pluginslist.cpp - 12:57:09 21.11.2005>

#include <QtGui>

#include "interfaces.h"
#include "pluginslist.h"

QString PluginsList::translate(QByteArray encodedString)
{
  QTextCodec *codec = QTextCodec::codecForName("koi8-r");
  QString string = codec->toUnicode(encodedString);
  return string;
}

PluginsList::PluginsList(const QString &path, const QStringList &fileNames,
			 QWidget *parent)
  : QWidget(parent)
{
  label = new QLabel;
  label->setWordWrap(true);

  QStringList headerLabels;
  headerLabels << tr("Components");

  treeWidget = new QTreeWidget;
  treeWidget->setAlternatingRowColors(false);
  treeWidget->setSelectionMode(QAbstractItemView::SingleSelection);
  treeWidget->setHeaderLabels(headerLabels);
  treeWidget->header()->hide();

  QGridLayout *mainLayout = new QGridLayout;
  mainLayout->setColumnStretch(0, 1);
  mainLayout->setColumnStretch(2, 1);
  mainLayout->addWidget(label, 0, 0, 1, 3);
  mainLayout->addWidget(treeWidget, 1, 0, 1, 3);
  setLayout(mainLayout);

  interfaceIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirOpenIcon),
			  QIcon::Normal, QIcon::On);
  interfaceIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirClosedIcon),
			  QIcon::Normal, QIcon::Off);
  featureIcon.addPixmap(style()->standardPixmap(QStyle::SP_FileIcon));

  populateTreeWidget(path, fileNames);

  connect(treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)),
	  this, SLOT(currItemChanged(QTreeWidgetItem *, int)));
}

void PluginsList::currItemChanged(QTreeWidgetItem *item, int column)
{
  if (column != -1) {
    emit lessonChanged(item->text(column));
  }
}

void PluginsList::populateTreeWidget(const QString &path,
				     const QStringList &fileNames)
{
  if (fileNames.isEmpty()) {
    label->setText(translate("Приложение не смогло найти\nплагины в директории\n")
		   + QDir::convertSeparators(path));
    treeWidget->hide();
  } else {
    label->setText(translate("Приложение смогло найти\nследующие плагины в\nдиректории\n")
		   + QDir::convertSeparators(path));

    QDir dir(path);

    foreach (QString fileName, fileNames) {
      QPluginLoader loader(dir.absoluteFilePath(fileName));
      QObject *plugin = loader.instance();

      if (plugin) {
	TutorialInterface *iTutorial = qobject_cast<TutorialInterface *>(plugin);
	if (iTutorial) {
	  QTreeWidgetItem *pluginItem = new QTreeWidgetItem(treeWidget);
	  // нижеследующие три строчки написаны только ради русифицированного
	  // корня дерева плагинов
	  QByteArray Name = "";
	  Name.append(iTutorial->name());
	  QString Top = translate(Name) + "(" + fileName + ")";
	  pluginItem->setText(0, Top);
	  treeWidget->setItemExpanded(pluginItem, true);

	  QFont boldFont = pluginItem->font(0);
	  boldFont.setBold(true);
	  pluginItem->setFont(0, boldFont);

	  addItems(pluginItem, translate("Список уроков"),
		   iTutorial->lessons());
	}
      }
    }
  }
}

void PluginsList::addItems(QTreeWidgetItem *pluginItem,
			   const QString interfaceName,
			   const QStringList &features)
{
  QTreeWidgetItem *interfaceItem = new QTreeWidgetItem(pluginItem);
  interfaceItem->setText(0, interfaceName);
  interfaceItem->setIcon(0, interfaceIcon);

  foreach (QString feature, features) {
    if (feature.endsWith("..."))
      feature.chop(3);
    QTreeWidgetItem *featureItem = new QTreeWidgetItem(interfaceItem);
    featureItem->setText(0, feature);
    featureItem->setIcon(0, featureIcon);
  }
  treeWidget->setItemExpanded(interfaceItem, true);
}
