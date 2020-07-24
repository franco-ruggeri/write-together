//
// Created by Nino on 24/07/2020.
//

#include "fileInfo.h"

fileInfo::fileInfo(QString filename, QString file_content): filename(filename),
file_content(file_content){

}

const QString &fileInfo::getFileContent() const {
    return file_content;
}

const QString &fileInfo::getFilename() const {
    return filename;
}
