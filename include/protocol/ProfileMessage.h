/*
 * Author: Franco Ruggeri
 */

#pragma once

#include <QString>
#include <QImage>
#include <QJsonObject>
#include "Message.h"

namespace collaborative_text_editor {
    class ProfileMessage : public Message {
        QString username_, password_;
        QImage icon_;

        QJsonObject json() const override;
    public:
        ProfileMessage(const QString& username, const QString& password, const QImage& icon);
        QString username() const;
        QString password() const;
        QImage icon() const;
    };
}