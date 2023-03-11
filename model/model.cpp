#include "model.h"

class TreeModelItem
{
public:
    TreeModelItem(QString name_ = QString())
                    : name(name_),
                    level(0),
                    isOpened(false)
    {}
    void adjustChildrenLevels()
    {
        foreach(TreeModelItem *item, children)
        {
            item->level = level+1;
            item->adjustChildrenLevels();
        }
    }
    QString name;
    int level;
    bool isOpened;
    QList<TreeModelItem *> children;
    inline bool hasChildren() {return !children.empty();}
};

TreeModel::TreeModel(QObject *parent) :
    QAbstractListModel(parent)
{
    TreeModelItem *toAdd;
    items << new TreeModelItem("First");
    toAdd = new TreeModelItem("Child 1.1");
    toAdd->children << new TreeModelItem("Child 1.1.1");
    items[0]->children << toAdd;
    items[0]->adjustChildrenLevels();
    items << new TreeModelItem("Second");
    items << new TreeModelItem("Third");
    items << new TreeModelItem("Fourth");
    toAdd = new TreeModelItem("Fifth");
}

QHash<int, QByteArray> TreeModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
    roles[NameRole] = "name";
    roles[LevelRole] = "level";
    roles[IsOpenedRole] = "isOpened";
    roles[HasChildrenRole] = "hasChildren";

    return roles;
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (index.row() > (items.size()-1) )
        return QVariant();
    TreeModelItem *item = items.at(index.row());
    switch (role)
    {
    case Qt::DisplayRole:
    case NameRole:
        return QVariant::fromValue(item->name);
    case LevelRole:
        return QVariant::fromValue(item->level);
    case IsOpenedRole:
        return QVariant::fromValue(item->isOpened);
    case HasChildrenRole:
        return QVariant::fromValue(item->hasChildren());
    default:
        return QVariant();
    }
}

void TreeModel::closeItem(int numIndex)
{
    if (numIndex > (items.size()-1))
        return;
    if (!items[numIndex]->isOpened)
        return;
    QModelIndex modelIndex = index(numIndex);
    items[numIndex]->isOpened = false;
    emit dataChanged(modelIndex, modelIndex);
    int i = numIndex+1;
    for (; i < items.size() && (items[i]->level > items[numIndex]->level); ++i) {}
    --i;
    beginRemoveRows(QModelIndex(), numIndex+1, i);
    while (i > numIndex)
    {
        items[i]->isOpened = false;
        items.removeAt(i--);
    }
    endRemoveRows();
}

void TreeModel::openItem(int numIndex)
{
    if (numIndex > (items.size()-1))
        return;
    if (items[numIndex]->isOpened)
        return;
    QModelIndex modelIndex = index(numIndex);
    items[numIndex]->isOpened = true;
    emit dataChanged(modelIndex, modelIndex);
    int i = numIndex+1;
    beginInsertRows(QModelIndex(), i, i+items[numIndex]->children.size()-1);
    foreach(TreeModelItem *item, items[numIndex]->children)
        items.insert(i++, item);
    endInsertRows();
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return items.size();
}
