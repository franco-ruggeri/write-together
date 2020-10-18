#include <cte/widget/home.h>
#include <cte/widget/create_dialog.h>
#include <cte/widget/collaborate_dialog.h>
#include <ui_home.h>

inline void init_resource() { Q_INIT_RESOURCE(resource); }

namespace cte {
    Home::Home(QWidget *parent) : QWidget(parent) {
        init_resource();
        ui_ = QSharedPointer<Ui::Home>::create();
        ui_->setupUi(this);
    }

    void Home::set_profile(const Profile& profile) {
        // TODO
    }

    void Home::set_documents(const QList<Document>& documents) {
        ui_->documents->setRowCount(documents.size());
        int row = 0;
        for (const auto& document : documents) {
            ui_->documents->setItem(row, 0, new QTableWidgetItem(document.owner()));
            ui_->documents->setItem(row, 1, new QTableWidgetItem(document.name()));
            row++;
        }
    }

    void Home::on_new_document_clicked() {
        CreateDialog create_dialog(this);
        connect(&create_dialog, &CreateDialog::create_document, this, &Home::create_document);
        create_dialog.exec();
    }

    void Home::on_collaborate_clicked() {
        CollaborateDialog collaborate_dialog(this);
        connect(&collaborate_dialog, &CollaborateDialog::collaborate, this, &Home::collaborate);
        collaborate_dialog.exec();
    }

    void Home::on_documents_cellClicked(int row, int column) {
        QString document_owner = ui_->documents->item(row, 0)->text();
        QString document_name = ui_->documents->item(row, 1)->text();
        Document document(document_owner, document_name);
        emit open_document(document);
    }

    void Home::clear() {
        // TODO: clear profile
        ui_->documents->clearContents();
    }
}
