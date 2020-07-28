/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QVector>
#include <QtCore/QString>
#include <QtCore/QJsonObject>
#include <protocol/Message.h>

namespace collaborative_text_editor {
    class DocumentsMessage : public Message {
        QVector<QString> documents_;

        DocumentsMessage(const QJsonObject& json_object);
        QJsonObject json() const override;
        friend Message;

    public:
        DocumentsMessage(const QVector<QString>& documents);
        bool operator==(const Message& other) const override;
        QVector<QString> documents() const;
    };
}
