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

    signals:
        void local_insert(const Symbol& symbol);
        void local_erase(const Symbol& symbol);

    public slots:
        void remote_insert(const Symbol& symbol);
        void remote_erase(const Symbol &symbol);

    public:
        Editor(int site_id, const QList<Symbol>& text, QWidget *parent=nullptr);
    };
}
