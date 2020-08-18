//
// Created by Claudio on 18/08/20.
//

#include <QTStylesheet.h>
#include <QString>

const QString list_stylesheet =
        "QListView { "
        "outline: none;"
        "font-size: 14pt;"
        "border: 0px;"
        "padding-left: 10px;"
        "padding-right: 20px;"
        "background-color: transparent; }"
        "QListView::item { "
        "border-bottom: 1px solid #F2F2F2;"
        "color: #102E4A;"
        "margin-left: 5px;"
        "margin-right: 5px;"
        "padding: 15px;"
        "background-color: white; }"
        "QListView::item:hover { "
        "color: #102E4A;"
        "border: 2px solid white;"
        "background-color: transparent; }"
        "QListView::item:hover:active { "
        "color: #102E4A;"
        "border: 2px solid white;"
        "background-color: transparent; }";

const QString btnStylesheet =
        "QPushButton{"
        "border-radius: 2px;"
        "margin-top: 5px;"
        "outline: none;"
        "background-color:  #00ABE7;"
        "}"
        "QPushButton:hover{"
        "background-color: #1C4073;"
        "}"
        "QPushButton:hover:pressed{"
        "background-color: #1C4073;"
        "border-radius: 15px;"
        "}"
        "QPushButton:focus{"
        "outline: none;"
        "}";

const QString btnRedirectStylesheet =
        "QPushButton{"
        "background: transparent;"
        "color:  #00ABE7;"
        "}"
        "QPushButton:hover{"
        "color: #1C4073;"
        "}"
        "QPushButton:hover:pressed{"
        "color: #1C4073;"
        "}"
        "QPushButton:focus{"
        "outline: none;"
        "}";


const QString btnLogOutStylesheet =
        "QPushButton{"
        "margin-top: 5px;\n"
        "color: white;\n"
        "border: 1px solid transparent;\n"
        "background-color:  transparent;"
        "}"
        "QPushButton:hover{"
        "color: white;\n"
        "border: 1px solid #C0180C;\n"
        "background-color:  #C0180C;"
        "}"
        "QPushButton:focus{"
        "outline: none;"
        "}";

const QString btnCancelStylesheet =
        "QPushButton{"
        "margin-top: 5px;\n"
        "color: white;\n"
        "border: 1px solid red;\n"
        "background-color:  red;"
        "}"
        "QPushButton:hover{"
        "color: white;\n"
        "border: 1px solid #C0180C;\n"
        "background-color:  #C0180C;"
        "}"
        "QPushButton:focus{"
        "outline: none;"
        "}";

const QString btnEditProfileStylesheet =
        "QPushButton {"
        "margin-top: 5px;\n"
        "color: white;\n"
        "border: 1px solid transparent;\n"
        "background-color:  transparent;"
        "}"
        "QPushButton:hover {"
        "color: #1C4073;\n"
        "border: 1px solid white;\n"
        "background-color:  white;"
        "}"
        "QPushButton:focus{"
        "outline: none;"
        "}";

const QString btnNewFileStylesheet =
        "QPushButton {"
        "margin-left: 5px;"
        "margin-bottom: 5px;"
        "font: RobotoRegular;"
        "font-size: 14pt;"
        "text-align: left;"
        "border-radius: 2px;"
        "margin-top: 5px;\n"
        "color: #1C4073;\n"
        "background-color:  transparent;"
        "}"
        "QPushButton:hover {"
        "color: white;\n"
        "border: 1px solid #139E2C;\n"
        "background-color:  #139E2C;"
        "}"
        "QPushButton:focus{"
        "outline: none;"
        "}";
const QString btnFilterDocumentsStylesheet =
        "QPushButton {"
        "margin-left: 5px;"
        "margin-bottom: 5px;"
        "font: RobotoRegular;"
        "font-size: 14pt;"
        "text-align: left;"
        "border-radius: 2px;"
        "margin-top: 5px;\n"
        "color: #1C4073;\n"
        "background-color:  transparent;"
        "}"
        "QPushButton:hover {"
        "color: white;\n"
        "border: 1px solid #1C4073;\n"
        "background-color:  #1C4073;"
        "}"
        "QPushButton:focus{"
        "outline: none;"
        "}";