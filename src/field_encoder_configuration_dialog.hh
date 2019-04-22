#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QListView>
#include <QGroupBox>
#include <QDialog>
#include <QButtonGroup>
#include "optional.hpp"
#include "qt_dto.hh"

using nonstd::optional;
using nonstd::nullopt;

class FieldEncoderConfigurationDialog : public QDialog
{
    Q_OBJECT

public:
    FieldEncoderConfigurationDialog(
        optional<QModelIndex> editIndex, QWidget *parent = nullptr
    );
    void setContent();

signals:
    // This is manually forwarded all the way to the View.
    void fieldEncoderDialogConfirmed(
        qt_dto::FieldEncoderConfigurationOperation data
    );
    
public slots:
    void showEncoderOptionsDialog();
    void accept();
    void complain();

private:
    QGroupBox *createFirstGroup();
    QGroupBox *createSecondGroup();
    QGroupBox *createThirdGroup();
    QWidget* makeControls();

    optional<QModelIndex> editIndex;
    QListView* converterList;
    QListView* validatorList;
    QButtonGroup* targetFieldTypeGroup;
    QLineEdit* targetFieldName;
    QGroupBox* targetFieldGroupBox;
    QGroupBox* converterGroupBox;
    QGroupBox* validationRulesGroupBox;
};

#endif
