/*
 * Author: Antonino Musmeci
 */

#include "client/fileInfo.h"

fileInfo::fileInfo(QString filename, QString file_content): filename(filename),
file_content(file_content){

}

const QString &fileInfo::getFileContent() const {
    return file_content;
}

const QString &fileInfo::getFilename() const {
    return filename;
}
