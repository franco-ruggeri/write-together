#include <cte/client_new/home.h>
#include <ui_home.h>
#include <QtWidgets/QInputDialog>

namespace cte {
    Home::Home(QWidget *parent) : QWidget(parent) {
        ui_ = QSharedPointer<Ui::Home>::create();
        ui_->setupUi(this);
        ui_->documents->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        connect(ui_->logout, &QPushButton::clicked, this, &Home::logout_request);
        connect(ui_->all_documents, &QRadioButton::clicked, this, &Home::refresh_documents);
        connect(ui_->your_documents, &QPushButton::clicked, this, &Home::refresh_documents);
        connect(ui_->shared_with_you, &QPushButton::clicked, this, &Home::refresh_documents);
    }

    void Home::refresh_profile() {
        ui_->icon->setPixmap(QPixmap::fromImage(profile_.icon()));
        ui_->username->setText(profile_.username());
    }

    void Home::refresh_documents() {
        QString username = profile_.username();
        bool yours = ui_->your_documents->isChecked();
        bool shared_with_you = ui_->shared_with_you->isChecked();

        // populate document list
        ui_->documents->setSortingEnabled(false);
        ui_->documents->setRowCount(0);
        int row = 0;
        for (const auto& document : documents_) {
            QString owner = document.owner();
            if (yours && owner != username) continue;
            if (shared_with_you && owner == username) continue;

            auto *owner_item = new QTableWidgetItem(owner);
            auto *name_item = new QTableWidgetItem(document.name());
            owner_item->setTextAlignment(Qt::AlignCenter);
            name_item->setTextAlignment(Qt::AlignCenter);

            ui_->documents->insertRow(row);
            ui_->documents->setItem(row, 0, owner_item);
            ui_->documents->setItem(row, 1, name_item);

            row++;
        }
        ui_->documents->setSortingEnabled(true);
    }

    void Home::set_profile(const Profile& profile) {
        profile_ = profile;
        refresh_profile();
    }

    void Home::set_documents(const QList<Document>& documents) {
        documents_ = QSet<Document>::fromList(documents);
        ui_->all_documents->click();
        ui_->documents->sortItems(0);   // sort document list by owner
    }

    void Home::add_document(const Document& document) {
        documents_.insert(document);
        refresh_documents();
    }

    void Home::profile_updated() {
        profile_ = profile_dialog_->profile();
        profile_dialog_->accept();
        profile_dialog_->deleteLater();
        refresh_profile();
    }

    void Home::on_new_document_clicked() {
        QString document_name = QInputDialog::getText(this, tr("Create document"), tr("Document name:"));
        if (!document_name.isNull()) emit new_document_request(document_name);
    }

    void Home::on_collaborate_clicked() {
        QString sharing_link = QInputDialog::getText(this, tr("Collaborate"), tr("Sharing link:"));
        if (!sharing_link.isNull()) emit document_request(sharing_link);
    }

    void Home::on_profile_clicked() {
        profile_dialog_ = new ProfileDialog(profile_, true, this);
        connect(profile_dialog_, qOverload<const Profile&>(&ProfileDialog::profile_update_request),
                this, qOverload<const Profile&>(&Home::profile_update_request));
        connect(profile_dialog_, qOverload<const Profile&, const QString&>(&ProfileDialog::profile_update_request),
                this, qOverload<const Profile&, const QString&>(&Home::profile_update_request));
        profile_dialog_->setModal(true);
        profile_dialog_->show();
    }

    void Home::on_documents_cellClicked(int row, int column) {
        QString document_owner = ui_->documents->item(row, 0)->text();
        QString document_name = ui_->documents->item(row, 1)->text();
        Document document(document_owner, document_name);
        emit document_request(document);
    }

    void Home::clear() {
        profile_ = Profile{};
        documents_.clear();
        ui_->icon->clear();
        ui_->username->clear();
        ui_->documents->clearContents();
    }
}
