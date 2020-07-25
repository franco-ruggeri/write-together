/*
 * Author: Antonino Musmeci
 */

#pragma once

#include <QString>

class fileInfo {
private:
    QString filename;
    QString file_content;
public:
    fileInfo(QString filename,QString file_content);

    const QString &getFileContent() const;

    const QString &getFilename() const;
};



