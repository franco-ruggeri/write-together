#pragma once

#include <QtCore/QSharedPointer>
#include <QtWidgets/QWidget>
#include <QtWidgets/QMainWindow>
#include <cte/crdt/shared_editor.h>

namespace Ui {
    class Editor;
}

namespace cte {
    class Editor : public QMainWindow {
        Q_OBJECT

        QSharedPointer<Ui::Editor> ui_;
        SharedEditor shared_editor_;
        int remote_changes_;

    signals:
        void local_insert(const Symbol& symbol);
        void local_erase(const Symbol& symbol);

    private slots:
        void process_change(int position, int chars_removed, int chars_added);
        void remote_insert(int index, QChar value);
        void remote_erase(int index);

    public:
        Editor(int site_id, const QList<Symbol>& text, QWidget *parent=nullptr);
        void remote_insert(const Symbol& symbol);
        void remote_erase(const Symbol& symbol);
    };
}
