#pragma once

#include <QAbstractListModel>

class TreeModelItem;
class TreeModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit TreeModel(QObject *parent=0);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QHash<int, QByteArray> roleNames() const;
    enum Roles {
        NameRole = Qt::UserRole+1,
        LevelRole,
        IsOpenedRole,
        HasChildrenRole
    };
public slots:
    void openItem(int numIndex);
    void closeItem(int numIndex);
private:
    Q_DISABLE_COPY(TreeModel);
    QList<TreeModelItem *> items;
};

