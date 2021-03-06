#include <QWidget>
#include <QMenu>
#include <QVBoxLayout>
#include <QLabel>
#include <QStringListModel>
#include <QDebug>
#include "slot_adapter.hh"
#include "field_encoder_list_view.hh"
#include "field_encoder_configuration_dialog.hh"
#include "field_encoder_model.hh"
#include "field_encoder_list_context_menu.hh"

FieldEncoderListView::FieldEncoderListView(
    FieldEncoderModel* theModel, QAbstractItemModel* converterListModel, QWidget* parent
): QWidget(parent) {
    this->theModel = theModel;
    this->converterListModel = converterListModel;
    QVBoxLayout* vbox = new QVBoxLayout(this);

    QLabel* help1 = new QLabel("Available field encoders are listed below.", this);
    vbox->addWidget(help1);

    listView = new QListView(this);
    listView->setModel(theModel);
    vbox->addWidget(listView);
}

void FieldEncoderListView::contextMenuEvent(QContextMenuEvent* event) {
    // The reason that we need to use mapFromGlobal here is because we aren't
    // the direct child, we're not a subclass of QListView.  Contrast the
    // approach in OptionsEditorView.
    QPoint widgetPos = listView->mapFromGlobal(event->globalPos());
    QModelIndex result = listView->indexAt(widgetPos);

    if (result.isValid()) {
        // They clicked on a 'real' item
    } else {
        // They probably clicked outside of the list view
    }

    FieldEncoderListContextMenu* thisMenu = new FieldEncoderListContextMenu(
        result, this
    );

    connect(
        thisMenu, &FieldEncoderListContextMenu::newRequested, 
        this, &FieldEncoderListView::triggerNew
    );

    connect(
        thisMenu, &FieldEncoderListContextMenu::deleteRequested, 
        this, &FieldEncoderListView::deleteItem
    );
    
    connect(
        thisMenu, &FieldEncoderListContextMenu::editRequested, 
        this, &FieldEncoderListView::triggerEdit
    );
 
    thisMenu->exec(event->globalPos());
}


void FieldEncoderListView::triggerEdit(QModelIndex index) {
    FieldEncoder initializingEncoder = theModel->getFieldEncoderByRow(index.row());

    FieldEncoderConfigurationDialog* dialog = new FieldEncoderConfigurationDialog(
        optional<QModelIndex>(index),
        optional<FieldEncoder>(initializingEncoder),
        converterListModel,
        this
    );

    // Forward this signal
    connect(
        dialog, &FieldEncoderConfigurationDialog::fieldEncoderDialogConfirmed,
        this, &FieldEncoderListView::fieldEncoderDialogConfirmed
    );

    int result = dialog->exec();
    Q_UNUSED(result);
}

void FieldEncoderListView::triggerNew() {
    FieldEncoderConfigurationDialog* dialog = new FieldEncoderConfigurationDialog(
        nullopt, nullopt, converterListModel, this
    );

    // Forward this signal
    connect(
        dialog, &FieldEncoderConfigurationDialog::fieldEncoderDialogConfirmed,
        this, &FieldEncoderListView::forwardDialogConfirmedSignal
    );

    int result = dialog->exec();
    Q_UNUSED(result);
}

void FieldEncoderListView::deleteItem(QModelIndex index) {
    theModel->removeRow(index.row());
}

void FieldEncoderListView::forwardDialogConfirmedSignal(
    qt_dto::FieldEncoderConfigurationOperation dto
) {
    // This is a bit weird, it would be more sensible for the model to be
    // directly connected to the event, and for the model to be forwarding it.
    // But it seems to work fine.
    theModel->layoutAboutToBeChanged();
    emit fieldEncoderDialogConfirmed(dto);
    theModel->layoutChanged();
}
