/*
 * Author: Antonino Musmeci
 */

#include <qdebug>
#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QColorDialog>
#include <QComboBox>
#include <QFontComboBox>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QFontDatabase>
#include <QMenu>
#include <QMenuBar>
#include <QTextCodec>
#include <QTextEdit>
#include <QStatusBar>
#include <QToolBar>
#include <QTextCursor>
#include <QTextDocumentWriter>
#include <QTextList>
#include <QtDebug>
#include <QMessageBox>
#include <QMimeData>
#include <QPrinter>
#include <QStackedWidget>
#include <QThread>
#include <QScrollBar>
#include <QDesktopWidget>
#include <QMainWindow>
#include <cte/client/texteditor.h>
#include <QDesktopWidget>
#include <cte/client/myClient.h>
#include <iostream>
#include <utility>
#include <cte/protocol/InsertMessage.h>
#include <cte/client/loginTextEditor.h>
#include <cte/protocol/EraseMessage.h>
#include <cte/protocol/CursorMessage.h>

const QString imgPath = ":/images";


int counter_id(){
    return 0;
}


texteditor::texteditor(QStackedWidget *parent, QSharedPointer<myClient> client, fileInfo file): QMainWindow(parent),
file(file){
    this->resize(QDesktopWidget().availableGeometry(this).size() * 0.7);
    this->setWindowTitle(APPLICATION);
    editor = QSharedPointer<QTextEdit>::create(this);
    setCentralWidget(editor.get());
    this->client = client;
    change_from_server = false;
    editor->setFontPointSize(14);

    shared_editor = QSharedPointer<SharedEditor>::create(file.site_id(), file.getFileContent());

    editor->setText(shared_editor->to_string()); // set the text content


    setToolButtonStyle(Qt::ToolButtonFollowStyle);
    setupPeers();
    setupFileActions();
    setupEditActions();
    setupUserActions();

    connect(this->client->socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(editor->document(), &QTextDocument::contentsChange, this, &texteditor::contentsChange);
    connect(editor.get(), &QTextEdit::textChanged, this, &texteditor::textChange);
    connect(editor.get(), &QTextEdit::cursorPositionChanged, this, &texteditor::cursorPositionChanged);


}


void texteditor::setupPeers(){

    auto ids = file.cursors().keys();
    list_user = QSharedPointer<QListWidget>::create();
    QIcon userIcon = QIcon(QPixmap::fromImage(client->user.icon()));

    for(const auto& user: file.users()){
        list_user->addItem(new QListWidgetItem(userIcon, user.username()));
    }

    for(const auto& id : ids) {
        UserInfo user(file.users().find(file.connected_user().find(id).value()).value());
        site_id_to_user.insert(id,user);
    }

    qDebug() << file.connected_user().size();
    connected_client = QSharedPointer<QDockWidget>::create(); // widget to show the peers
    connected_client->setObjectName("Peers");
    connected_client->setWindowTitle("Peers");
    connected_client->setWidget(list_user.get()); // add other user to the dock
    addDockWidget(Qt::RightDockWidgetArea,connected_client.get());
}

void texteditor::init_cursors(){
    qDebug() << "inizializzazione cursori";
    for( const auto& symbol : file.cursors()) {
        int cursor_pos = shared_editor->find(symbol);
        site_id_to_user[symbol.site_id()].init_cursor(editor.get(), cursor_pos);
    }
}


void texteditor::setupFileActions()
{
    QToolBar *tb = addToolBar(tr("File Actions"));
    QMenu *menu = menuBar()->addMenu(tr("&File"));
    QAction *a;
    a = menu->addAction(QIcon::fromTheme("document-share", QIcon(imgPath + "/filesave.png")),
                                 tr("&Share Document"),this, &texteditor::file_share);

    tb->addAction(a);
    a = menu->addAction(QIcon::fromTheme("exportpdf", QIcon(imgPath + "/exportpdf.png")),
                    tr("&Export PDF"), this, &texteditor::file_to_pdf);
    a->setPriority(QAction::LowPriority);
    a->setShortcut(Qt::CTRL + Qt::Key_D);

    tb->addAction(a);
    menu->addSeparator();
    a = menu->addAction(QIcon::fromTheme("document-close", QIcon(imgPath + "/fileopen.png")),
                    tr("&Close"), this, &texteditor::file_close);

    tb->addAction(a);
    menu->addSeparator();
}

void texteditor::setupUserActions(){
    QToolBar *tb = addToolBar(tr("User Actions"));
    QMenu *menu = menuBar()->addMenu(tr("&User"));
    const QIcon showPeersIcon = QIcon::fromTheme("show-peers", QIcon(imgPath + "/user_icon.png"));
    show_peers = menu->addAction(showPeersIcon, tr("&Show"), connected_client.get(),&QDockWidget::show);
    tb->addAction(show_peers);
}

void texteditor::setupEditActions() {
    QToolBar *tb = addToolBar(tr("Edit Actions"));
    QMenu *menu = menuBar()->addMenu(tr("&Edit"));
    const QIcon undoIcon = QIcon::fromTheme("edit-undo", QIcon(imgPath + "/editundo.png"));
    actionUndo = menu->addAction(undoIcon, tr("&Undo"), editor.get(), &QTextEdit::undo);
    actionUndo->setShortcut(QKeySequence::Undo);
    tb->addAction(actionUndo);

    const QIcon redoIcon = QIcon::fromTheme("edit-redo", QIcon(imgPath + "/editredo.png"));
    actionRedo = menu->addAction(redoIcon, tr("&Redo"), editor.get(), &QTextEdit::redo);
    actionRedo->setPriority(QAction::LowPriority);
    actionRedo->setShortcut(QKeySequence::Redo);
    tb->addAction(actionRedo);
    menu->addSeparator();

#ifndef QT_NO_CLIPBOARD
    const QIcon cutIcon = QIcon::fromTheme("edit-cut", QIcon(imgPath + "/editcut.png"));
    actionCut = menu->addAction(cutIcon, tr("Cu&t"), editor.get(), &QTextEdit::cut);
    actionCut->setPriority(QAction::LowPriority);
    actionCut->setShortcut(QKeySequence::Cut);
    tb->addAction(actionCut);

    const QIcon copyIcon = QIcon::fromTheme("edit-copy", QIcon(imgPath + "/editcopy.png"));
    actionCopy = menu->addAction(copyIcon, tr("&Copy"), editor.get(), &QTextEdit::copy);
    actionCopy->setPriority(QAction::LowPriority);
    actionCopy->setShortcut(QKeySequence::Copy);
    tb->addAction(actionCopy);

    const QIcon pasteIcon = QIcon::fromTheme("edit-paste", QIcon(imgPath + "/editpaste.png"));
    actionPaste = menu->addAction(pasteIcon, tr("&Paste"), editor.get(), &QTextEdit::paste);
    actionPaste->setPriority(QAction::LowPriority);
    actionPaste->setShortcut(QKeySequence::Paste);
    tb->addAction(actionPaste);
    if (const QMimeData *md = QApplication::clipboard()->mimeData())
        actionPaste->setEnabled(md->hasText());
#endif
}


void texteditor::file_to_pdf() {
    QFileDialog fileDialog(this, tr("Export PDF"));
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.setMimeTypeFilters(QStringList("application/pdf"));
    fileDialog.setDefaultSuffix("pdf");
    if (fileDialog.exec() != QDialog::Accepted)
        return;
    QString fileName = fileDialog.selectedFiles().first();
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    editor->document()->print(&printer);
    statusBar()->showMessage(tr("Exported \"%1\"")
                                     .arg(QDir::toNativeSeparators(fileName)));

}

void texteditor::file_close() {
    this->client->file_close(this->file);
    disconnect(this->client->socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    this->deleteLater();
    emit show_user_page();
}

void texteditor::file_share(){
    emit share_file(file.sharing_link().toString());
}


void texteditor::contentsChange(int position, int charsRemoved, int charsAdded) {
    if(change_from_server) return;
    for(int i = 0 ; i < charsAdded; i++) {
        client->sendInsert(file.document(),shared_editor->local_insert(position, editor->toPlainText()[position + i]));
    }
    for(int i = 0 ; i < charsRemoved; i++) {
        client->sendErase(file.document(),shared_editor->local_erase(position));
    }
}


void texteditor::readyRead(){
    while(client->socket->canReadLine()) {
        QByteArray request = this->client->socket->readLine();
        QSharedPointer<Message> m = Message::deserialize(request);

        if (m->type() == MessageType::insert) {
            qDebug() << "remote Insert";
            change_from_server = true;
            shared_editor->remote_insert(m.staticCast<InsertMessage>()->symbol());
            this->editor->setText(shared_editor->to_string());
        }
        if (m->type() == MessageType::erase) {
            change_from_server = true;
            shared_editor->remote_erase(m.staticCast<EraseMessage>()->symbol());
            this->editor->setText(shared_editor->to_string());
        }
        if (m->type() == MessageType::cursor) {
            qDebug() <<"remote cursor";
            change_from_server = true;
            auto cursor_message = m.staticCast<CursorMessage>();
            int position = shared_editor->find(cursor_message->symbol());
            site_id_to_user.find(cursor_message->symbol().site_id())->change_cursor_position(editor.get(),position);
        }
    }
}

void texteditor::closeEvent(QCloseEvent *event){
    file_close();
    event->accept();
}


void texteditor::textChange() {
    editor->setFontPointSize(14);
    if(change_from_server) {
        change_from_server  = false;
        return;
    }
}


void texteditor::cursorPositionChanged() {
    if(change_from_server) return;
    QTextCursor cursor = editor->textCursor();
    qDebug() << "cursor position:" << cursor.position();
    this->client->send_cursor(this->file.document(), this->shared_editor->insert_cursor(cursor.position(),QChar()));
}
