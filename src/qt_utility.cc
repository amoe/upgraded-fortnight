#include "qt_utility.hh"

namespace qt_utility {
    void popEditors(QAbstractItemModel* model, QAbstractItemView* view) {
        for (int i = 0; i < model->rowCount(); i++) {
            for (int j = 0; j < model->columnCount(); j++) {
                QModelIndex index = model->index(i, j, QModelIndex());
                view->openPersistentEditor(index);
            }
        }
    }

    void handleHttpError(QString logPrefix, QNetworkReply* reply, QByteArray result) {
        QNetworkReply::NetworkError error = reply->error();
        qDebug() << error;
        qDebug() << result;
        if (error != QNetworkReply::NoError) {
            QString errorMessage;
            QDebug debugStream(&errorMessage);
            debugStream << logPrefix << error << "; content =" << QString::fromUtf8(result);
            throw std::runtime_error(errorMessage.toStdString());
        }
    }
}
