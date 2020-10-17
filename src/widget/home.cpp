#include <cte/widget/home.h>
#include <ui_home.h>
#include <QtWidgets/QTableWidgetItem>

namespace cte {
    Home::Home(QWidget *parent) : Widget(parent) {
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

    void Home::clear() {
        // TODO: clear profile
        ui_->documents->clearContents();
    }
}
