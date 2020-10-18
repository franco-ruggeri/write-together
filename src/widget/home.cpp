#include <cte/widget/home.h>
#include <cte/widget/create_dialog.h>
#include <ui_home.h>
#include <QtWidgets/QTableWidgetItem>

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

    void Home::clear() {
        // TODO: clear profile
        ui_->documents->clearContents();
    }
}
