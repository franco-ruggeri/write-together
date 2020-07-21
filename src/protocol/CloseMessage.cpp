/*
 * Author: Franco Ruggeri
 */

#include "CloseMessage.h"

namespace collaborative_text_editor {
    CloseMessage::CloseMessage(QString document, QString username) :
        Message(MessageType::close), document_(document), username_(username) {}

    QString CloseMessage::document() const {
        return document_;
    }

    QString CloseMessage::username() const {
        return username_;
    }

    QJsonObject CloseMessage::json() const {
        QJsonObject json_ = Message::json();
        json_.insert("document", document_);
        json_.insert("username", username_);
        return json_;
    }
}
