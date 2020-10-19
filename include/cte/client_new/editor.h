#pragma once

#include <QtCore/QSharedPointer>
#include <QtCore/QHash>
#include <QtCore/QUrl>
#include <QtWidgets/QWidget>
#include <QtWidgets/QMainWindow>
#include <cte/crdt/shared_editor.h>
#include <cte/protocol/document.h>
#include <cte/protocol/document_info.h>

namespace Ui {
    class Editor;
}

namespace cte {
    class Editor : public QMainWindow {
        Q_OBJECT

        QSharedPointer<Ui::Editor> ui_;
        Document document_;
        QUrl sharing_link_;
        SharedEditor shared_editor_;
        int remote_changes_;

        QHash<int,QString> usernames_;
        QHash<QString,int> session_counts_;
        QHash<QString,Profile> online_users_, offline_users_;   // username -> profile

        void refresh_users();

    signals:
        void local_insert(const Symbol& symbol);
        void local_erase(const Symbol& symbol);
        void home_focus();
        void closed();

    private slots:
        void process_change(int position, int chars_removed, int chars_added);
        void remote_insert(int index, QChar value);
        void remote_erase(int index);
        void export_pdf();
        void show_sharing_link();

    protected:
        void closeEvent(QCloseEvent *event) override;

    public:
        Editor(const Document& document, const DocumentInfo& document_info, QWidget *parent=nullptr);
        void remote_insert(const Symbol& symbol);
        void remote_erase(const Symbol& symbol);
        void add_online_user(int site_id, const Profile& profile);
        void remove_online_user(int site_id);
        QUrl sharing_link() const;
    };
}
