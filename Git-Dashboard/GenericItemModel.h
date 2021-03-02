#pragma once

#include <iostream>
#include <vector>
#include <memory>

#include <QAbstractTableModel>

#include <showlib/JSONSerializable.h>

using std::cout;
using std::endl;

using namespace ShowLib;

/**
 * This defines a generic item model as a template. It assumes you're using
 * a std::vector<std::shared_ptr<T>> to hold the data, and you can use
 * lambdas to hold accessors for the various rows.
 **/
template <class T>
class GenericItemModel: public QAbstractTableModel {
public:
    //======================================================================
    // Some data types.
    //======================================================================
    typedef std::shared_ptr<T> Pointer;
    typedef ShowLib::JSONSerializableVector<T> Vector;
    typedef std::function<QVariant(const Pointer &)> Accessor;

    template <class U>
    class Column {
    public:
        QString header;
        Accessor accessor;

        Column() = default;
        Column(const QString &h, Accessor a): header(h), accessor(a) {}
    };

    typedef std::shared_ptr<Column<T>> ColumnPointer;

    //======================================================================
    // Setup.
    //======================================================================
    GenericItemModel(QObject *parent = nullptr) : QAbstractTableModel(parent) {}
    void addColumn(const QString & header, Accessor accessor) {
        ColumnPointer ptr = std::make_shared<Column<T>>(header, accessor);
        columns.push_back(ptr);
    }

    /** This copies the pointers. */
    void setVector(const Vector &vec) {
        vector = vec;
        layoutChanged();
    }

    /** Be careful with this. If you manipulate the vector, you'll need to call layoutChanged() when done. */
    Vector &getVector() { return vector; }

    //======================================================================
    // Return data for the QTableView.
    //======================================================================

    bool hasChildren(const QModelIndex &index) const override {
        return  !index.isValid();
    }

    int rowCount(const QModelIndex &parent) const override {
        int retVal = (parent.column() <= 0) ? vector.size() :  0;
        return retVal;
    }

    int columnCount(const QModelIndex &) const override {
        return columns.size();
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override {
        if (role == Qt::DisplayRole
            && orientation == Qt::Horizontal
            && section >= 0
            && static_cast<size_t>(section) < columns.size())
        {
            return columns[section]->header;
        }

        return QVariant();
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override {
        if (role == Qt::DisplayRole
            && index.row() >= 0
            && index.column() >= 0
            && static_cast<size_t>(index.row()) < vector.size()
            && static_cast<size_t>(index.column()) < columns.size())
        {
            const Pointer &value = vector[index.row()];
            return columns[index.column()]->accessor(value);
        }

        return QVariant();
    }

private:
    Vector	vector;
    std::vector<std::shared_ptr<Column<T>>> columns;

};
