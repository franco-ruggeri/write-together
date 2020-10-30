#pragma once

#include <QtCore/QSharedPointer>
#include <QtCore/QSet>
#include <QtCore/QHash>
#include <QtGui/QColor>
#include <QtWidgets/QTextEdit>
#include <cte/client/profile_dialog.h>
#include <cte/client/remote_cursor.h>
#include <cte/protocol/profile.h>
#include <cte/crdt/symbol.h>

namespace cte {
    class User {
        Profile profile_;
        QSet<int> site_ids_;    // all, some may not be mapped to remote cursors
        QHash<int,QSharedPointer<RemoteCursor>> remote_cursors_;    // pointers for assignability
        QColor color_;
        bool selected_, online_, local_;
        QPointer<ProfileDialog> profile_dialog_;

        static const QList<QColor> colors;

        void generate_color();

    public:
        User(const Profile& profile, const QList<int>& site_ids);

        void add_remote_cursor(QTextEdit *editor, int site_id);
        void add_remote_cursor(QTextEdit *editor, int site_id, int position);
        void move_remote_cursor(int site_id, int position);
        void remove_remote_cursor(int site_id);
        void refresh_remote_cursors();
        void show_profile();

        bool local() const;
        void set_local(bool local);
        Profile profile() const;
        bool online() const;
        QColor color() const;
        bool selected() const;
        void toggle_selected();
        bool contains(int site_id);
    };
}
