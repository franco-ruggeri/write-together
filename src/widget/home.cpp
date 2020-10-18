#include <cte/widget/home.h>
#include <ui_home.h>
#include <QtWidgets/QInputDialog>

inline void init_resource() { Q_INIT_RESOURCE(resource); }

namespace cte {
    Home::Home(QWidget *parent) : QWidget(parent) {
        init_resource();
        ui_ = QSharedPointer<Ui::Home>::create();
        ui_->setupUi(this);
        clear();
    }

    void Home::refresh() {
        // update profile
        ui_->icon->setPixmap(QPixmap::fromImage(profile_.icon()));
        ui_->username->setText(profile_.username());

        // filter documents
        QList<Document> filtered_documents;
        std::copy_if(documents_.begin(), documents_.end(), std::back_inserter(filtered_documents),
                     [this](const Document &document) {
                         QString username = profile_.username();
                         QString owner = document.owner();
                         return filter_ == Filter::all_documents ||
                                (filter_ == Filter::your_documents && owner == username) ||
                                (filter_ == Filter::shared_with_you && owner != username);
                     });

        // populate document list
        ui_->documents->setRowCount(0);
        int row = 0;
        for (const auto& document : filtered_documents) {
            ui_->documents->insertRow(row);
            ui_->documents->setItem(row, 0, new QTableWidgetItem(document.owner()));
            ui_->documents->setItem(row, 1, new QTableWidgetItem(document.name()));
            row++;
        }

        // sort document list (by owner, name)
        // TODO: in another thread (?)
        ui_->documents->sortItems(1);
        ui_->documents->sortItems(0);
    }

    void Home::set_profile(const Profile& profile) {
        profile_ = profile;
        refresh();
    }

    void Home::set_documents(const QList<Document>& documents) {
        documents_ = QSet<Document>::fromList(documents);
        refresh();
    }

    void Home::add_document(const Document& document) {
        documents_.insert(document);
        refresh();
    }

    void Home::profile_updated() {
        profile_ = profile_dialog_->profile();
        profile_dialog_->accept();
        profile_dialog_->deleteLater();
        refresh();
    }

    void Home::on_new_document_clicked() {
        QString document_name = QInputDialog::getText(this, tr("Create document"), tr("Document name:"));
        if (!document_name.isNull()) emit create_document(document_name);
    }

    void Home::on_collaborate_clicked() {
        QString sharing_link = QInputDialog::getText(this, tr("Collaborate"), tr("Sharing link:"));
        if (!sharing_link.isNull()) emit collaborate(sharing_link);
    }

    void Home::update_filter(Filter filter) {
        if (filter_ == filter) return;
        filter_ = filter;
        refresh();
    }

    void Home::on_all_documents_clicked() {
        update_filter(Filter::all_documents);
    }

    void Home::on_your_documents_clicked() {
        update_filter(Filter::your_documents);
    }

    void Home::on_shared_with_you_clicked() {
        update_filter(Filter::shared_with_you);
    }

    void Home::on_profile_clicked() {
        profile_dialog_ = new ProfileDialog(profile_, this);
        connect(profile_dialog_, qOverload<const Profile&>(&ProfileDialog::update_profile),
                this, qOverload<const Profile&>(&Home::update_profile));
        connect(profile_dialog_, qOverload<const Profile&, const QString&>(&ProfileDialog::update_profile),
                this, qOverload<const Profile&, const QString&>(&Home::update_profile));
        profile_dialog_->setModal(true);
        profile_dialog_->show();
    }

    void Home::on_documents_cellClicked(int row, int column) {
        QString document_owner = ui_->documents->item(row, 0)->text();
        QString document_name = ui_->documents->item(row, 1)->text();
        Document document(document_owner, document_name);
        emit open_document(document);
    }

    void Home::clear() {
        profile_ = Profile{};
        documents_.clear();
        filter_ = Filter::all_documents;
        ui_->icon->clear();
        ui_->username->clear();
        ui_->documents->clearContents();
    }
}
