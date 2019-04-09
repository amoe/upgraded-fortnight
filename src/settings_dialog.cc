#include <QTabWidget>
#include <QStringList>
#include <QStringListModel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QDebug>
#include <iostream>

#include "field_mapping_table_model.hh"
#include "settings_dialog.hh"
#include "field_encoder_list_view.hh"
#include "mapping_table_widget.hh"

SettingsDialog::SettingsDialog(vector<string> headerFields, QWidget* parent): QDialog(parent) {
    setWindowTitle("Settings");

    QTabWidget* container = new QTabWidget(this);
        
    QStringList stringList = {"fry", "bender", "leela"};
    QStringListModel* fieldEncoderModel = new QStringListModel(stringList, this);


    // construct model with fields
    FieldMappingTableModel* fieldMappingModel = new FieldMappingTableModel(headerFields, this);

    MappingTableWidget* mappingTable = new MappingTableWidget(
        fieldEncoderModel, fieldMappingModel, this
    );
    FieldEncoderListView* fieldEncoderList = new FieldEncoderListView(fieldEncoderModel, this);

    container->addTab(mappingTable, "Field mapping");
    container->addTab(fieldEncoderList, "Encoder configuration");

    QVBoxLayout* dialogLayout = new QVBoxLayout;
    dialogLayout->addWidget(container);

    // Standard-ass buttons
    QDialogButtonBox* buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel
    );
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    dialogLayout->addWidget(buttonBox);

    setLayout(dialogLayout);

    // Empirically determined.  Yuck
    //resize(320, 480);
}

QSize SettingsDialog::sizeHint() const {
    return QSize(320, 480);
}