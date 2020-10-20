#pragma once

#include <QtCore/QPointer>
#include <QtCore/QSet>
#include <QtCore/QHash>
#include <QtGui/QColor>
#include <cte/client_new/profile_dialog.h>
#include <cte/client_new/remote_cursor.h>
#include <cte/protocol/profile.h>
#include <cte/crdt/symbol.h>

namespace cte {
    class User {
        Profile profile_;
        QSet<int> site_ids_;
        QHash<int,QSharedPointer<RemoteCursor>> remote_cursors_;
        QColor color_;
        bool selected_, online_, local_;
        QPointer<ProfileDialog> profile_dialog_;

    public:
        User(const Profile& profile, const QSet<int>& site_ids, const QColor& color);

        void add_remote_cursor(QTextEdit *editor, int site_id, const Symbol& symbol);
        void remove_remote_cursor(int site_id);
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
