#include <cte/client/home.h>
#include <ui_home.h>
#include <QtWidgets/QInputDialog>

namespace cte {
    Home::Home(QWidget *parent) : QWidget(parent) {
        ui_ = QSharedPointer<Ui::Home>::create();
        ui_->setupUi(this);
        clear();
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
        bool empty = true;

        // populate document list
        ui_->documents->clear();
        for (const auto& document : documents_) {
            QString owner = document.owner();
            if (yours && owner != username) continue;
            if (shared_with_you && owner == username) continue;
            empty = false;
            ui_->documents->addItem(document.full_name());
        }
        ui_->documents->sortItems();

        // set document title
        QString title;
        if (yours)
            title = empty ? tr("You don't own any document") : tr("Own documents");
        else if (shared_with_you)
            title = empty ? tr("No documents have been shared with you") : tr("Documents shared with you");
        else
            title = empty ? tr("No documents available") : tr("Accessible documents");
        ui_->documents_title->setText(title);
    }

    void Home::set_profile(const Profile& profile) {
        profile_ = profile;
        refresh_profile();
    }

    void Home::set_documents(const QList<Document>& documents) {
        documents_ = QSet<Document>::fromList(documents);
        refresh_documents();
    }

    void Home::add_document(const Document& document) {
        if (documents_.contains(document)) return;
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

    void Home::on_documents_itemClicked(QListWidgetItem *item) {
        Document document(item->text());
        emit document_request(document);
    }

    void Home::clear() {
        profile_ = Profile{};
        documents_.clear();
        ui_->icon->clear();
        ui_->username->clear();
        ui_->documents->clear();
        ui_->all_documents->setChecked(true);
        refresh_profile();
        refresh_documents();
    }
}
