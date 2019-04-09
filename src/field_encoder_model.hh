#ifndef FIELD_ENCODER_MODEL_HH
#define FIELD_ENCODER_MODEL_HH

#include <Qt>
#include <QModelIndex>
#include <QStringListModel>

class FieldEncoderModel: public QAbstractItemModel {

public:
    FieldEncoderModel();

    Qt::ItemFlags flags(const QModelIndex& index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

private:
    QAbstractItemModel* innerModel;
};

#endif /* FIELD_ENCODER_MODEL_HH */