#pragma once

#include <QtCore/QPointer>
#include <QtCore/QSet>
#include <QtCore/QHash>
#include <QtGui/QColor>
#include <cte/client_new/profile_dialog.h>
#include <cte/protocol/profile.h>
#include <cte/crdt/symbol.h>

namespace cte {
    class User {
        Profile profile_;
        QSet<int> site_ids_;
        QHash<int,int> cursors_;    // TODO: cambia value
        QColor color_;
        bool selected_;
        QPointer<ProfileDialog> profile_dialog_;

        void generate_color(double color_h);

    public:
        User(const Profile& profile, const QSet<int>& site_ids, const QColor& color);

        void add_cursor(int site_id, const Symbol& symbol);
        void remove_cursor(int site_id);
        void show_profile();

        Profile profile() const;
        bool online() const;
        QColor color() const;
        bool selected() const;
        void toggle_selected();
        bool contains(int site_id);
    };
}
