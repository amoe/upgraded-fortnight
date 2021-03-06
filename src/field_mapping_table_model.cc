#include <QDebug>
#include "field_mapping_table_model.hh"

using std::string;
using std::vector;

FieldMappingTableModel::FieldMappingTableModel(
    vector<string> availableFields,
    const MappingScheme& mappingScheme,
    QObject* parent
): QAbstractTableModel(parent),
   availableFields(availableFields),
   mappingScheme(mappingScheme)
{
    qDebug() << "initializing FieldMappingTableModel";
}

int FieldMappingTableModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return availableFields.size();
}

int FieldMappingTableModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return 1;
}

QVariant FieldMappingTableModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
        int row = index.row();
        FieldEncoder encoder = mappingScheme.at(row);
        QString description = QString::fromStdString(encoder.describe());
        return QVariant(description);
    } else {
        return QVariant();
    }
}

QVariant FieldMappingTableModel::headerData(
    int section, Qt::Orientation orientation, int role
) const {
    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    vector<string> headers = {
        "Field encoder"
    };

    if (orientation == Qt::Horizontal) {
        return QString::fromStdString(headers.at(section));
    } else {
        return QString::fromStdString(availableFields.at(section));
    }
}

bool FieldMappingTableModel::setData(
    const QModelIndex& index, const QVariant& value, int role
) {
    Q_UNUSED(role);
    qDebug() << "called to set data in model, received value" << value;
    qt_dto::MappingEncoderSetOperation dto(index, value);
    emit mappingEncoderSet(dto);
    return true;
}

Qt::ItemFlags FieldMappingTableModel::flags(const QModelIndex& index) const {
    Q_UNUSED(index);
    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}
