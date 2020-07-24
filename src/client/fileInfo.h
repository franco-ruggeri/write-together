//
// Created by Nino on 24/07/2020.
//

#ifndef COLLABORATIVE_TEXT_EDITOR_FILEINFO_H
#define COLLABORATIVE_TEXT_EDITOR_FILEINFO_H


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


#endif //COLLABORATIVE_TEXT_EDITOR_FILEINFO_H
