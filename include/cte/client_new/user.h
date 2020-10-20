#pragma once

#include <QtCore/QPointer>
#include <QtCore/QHash>
#include <QtGui/QColor>
#include <cte/client_new/profile_dialog.h>
#include <cte/protocol/profile.h>
#include <cte/crdt/symbol.h>

namespace cte {
    class User {
        Profile profile_;
        QList<int> site_ids_;
        QHash<int,int> cursors_;    // TODO: cambia value
        QColor color_;
        bool selected_;
        QPointer<ProfileDialog> profile_dialog_;

        void generate_color(double color_h);

    public:
        User(const Profile& profile, const QList<int>& site_ids, double color_h);

        void add_cursor(int site_id, const Symbol& symbol);
        void remove_cursor(int site_id);
        void show_profile();

        Profile profile() const;
        bool online() const;
        bool selected() const;
        void set_selected(bool selected);
    };
}
