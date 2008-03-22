/****************************************************************************
 ***   Здесь описываются функции и переменные, использующиеся в виджете,  ***
 ***            показывающем информацию о загруженных плагинах            ***
 ****************************************************************************/

// Time-stamp: <pluginslist.h - 22:49:23 23.09.2005>

#ifndef PLUGINSLIST_H
#define PLUGINSLIST_H

#include <QWidget>
#include <QIcon>

class QLabel;
class QStringList;
class QTreeWidget;
class QTreeWidgetItem;

class PluginsList : public QWidget
{
    Q_OBJECT

public:
    PluginsList(const QString &path, const QStringList &fileNames,
		QWidget *parent = 0);

private slots:
    void currItemChanged(QTreeWidgetItem *item, int column);

signals:
    void lessonChanged(QString lesson);

private:
    void populateTreeWidget(const QString &path, const QStringList &fileNames);
    void addItems(QTreeWidgetItem *pluginItem, const QString interfaceName,
		  const QStringList &features);
    QString translate(QByteArray encodedString);

    QLabel *label;
    QTreeWidget *treeWidget;
    QIcon interfaceIcon;
    QIcon featureIcon;
};

#endif
