#pragma once

#include <QtCore/QSharedPointer>
#include <QtCore/QHash>
#include <QtCore/QUrl>
#include <QtWidgets/QTreeWidgetItem>
#include <QtWidgets/QWidget>
#include <QtWidgets/QMainWindow>
#include <cte/client_new//user.h>
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

        QHash<int,QSharedPointer<User>> site_id_users_;         // fast lookup by site_id
        QHash<QString,QSharedPointer<User>> username_users_;    // fast lookup by username
        double color_h_;

        QColor generate_color();
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
        void on_users_itemClicked(QTreeWidgetItem *item, int column);
        void on_users_itemDoubleClicked(QTreeWidgetItem *item, int column);

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
