#pragma once

#include <QtCore/QSharedPointer>
#include <QtCore/QHash>
#include <QtCore/QUrl>
#include <QtGui/QCloseEvent>
#include <QtWidgets/QTreeWidgetItem>
#include <QtWidgets/QWidget>
#include <QtWidgets/QMainWindow>
#include <cte/client//user.h>
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
        QPointer<QLabel> word_char_count_;

        Document document_;
        QUrl sharing_link_;
        SharedEditor shared_editor_;

        int local_site_id_;
        QTextCursor local_cursor_;
        QSharedPointer<User> local_user_;
        QHash<int,QSharedPointer<User>> site_id_users_;         // fast lookup by site_id
        QHash<QString,QSharedPointer<User>> username_users_;    // fast lookup by username

        bool copy_paste_;

        void refresh_users();

    signals:
        void local_insert(const Symbol& symbol, const Format& format);
        void local_erase(const Symbol& symbol);
        void local_cursor_move(const Symbol& symbol);
        void local_format_change(const Symbol& symbol, const Format& format);
        void home_request();
        void closed();

    private slots:
        void refresh_cursors();
        void refresh_status_bar();
        void refresh_format_actions(const QTextCharFormat& format);
        void process_local_content_change(int position, int chars_removed, int chars_added);
        void process_local_cursor_move();
        void process_local_format_change();
        void simulate_paste();
        void export_pdf();
        void show_sharing_link();
        void on_users_itemClicked(QTreeWidgetItem *item, int column);
        void on_users_itemDoubleClicked(QTreeWidgetItem *item, int column);
        void textChange();
    protected:
        void closeEvent(QCloseEvent *event) override;

    public:
        Editor(const Document& document, const DocumentInfo& document_info, QWidget *parent=nullptr);
        void remote_insert(const Symbol& symbol, const Format& format);
        void remote_erase(int site_id, const Symbol& symbol);
        void remote_cursor_move(int site_id, const Symbol& symbol);
        void remote_format_change(const Symbol& symbol, const Format& format);
        void remote_open(int site_id, const Profile& profile);
        void remote_close(int site_id);
        int local_site_id() const;
        QUrl sharing_link() const;
        bool eventFilter(QObject *watched, QEvent *event) override;

    };
}
