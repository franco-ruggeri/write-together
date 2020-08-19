/*
 * Author: Antonino Musmeci
 */

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
#include <cte/protocol/OpenMessage.h>
#include <QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <cmath>

const QString imgPath = ":/images";


texteditor::texteditor(QStackedWidget *parent, QSharedPointer<myClient> client, fileInfo file): QMainWindow(parent),
file(file), user_row_(0){
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

//    connect(this->client->socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(editor->document(), &QTextDocument::contentsChange, this, &texteditor::contentsChange);
    connect(editor.get(), &QTextEdit::textChanged, this, &texteditor::textChange);
    connect(editor.get(), &QTextEdit::cursorPositionChanged, this, &texteditor::cursorPositionChanged);

    connect(client.get(), &myClient::user_added, this, &texteditor::remote_open);
    connect(client.get(), &myClient::user_removed, this, &texteditor::remote_close);
    connect(client.get(), &myClient::char_inserted, this, &texteditor::remote_insert);
    connect(client.get(), &myClient::char_removed, this, &texteditor::remote_erase);
    connect(client.get(), &myClient::cursor, this, &texteditor::remote_cursor);
}


void texteditor::setupPeers(){

    h =  std::floor(rand());
    auto ids = file.cursors().keys();

    list_user = QSharedPointer<QListWidget>::create();
    list_user->setObjectName("list_user");

    for(const auto & profile: file.users()){
        UserInfo user(profile,generate_color());
        username_to_user.insert(profile.username(),user);
        QIcon user_icon = QIcon(QPixmap::fromImage(user.icon()));
        auto* widget = new QListWidgetItem(user_icon, user.username());
        widget->setBackground(user.color());
        username_to_row.insert(profile.username(), user_row_);
        list_user->insertItem(user_row_, widget);
        user_row_++;
    }

    list_user->setSelectionMode(QAbstractItemView::NoSelection);
    connected_client = QSharedPointer<QDockWidget>::create(); // widget to show the peers
    connected_client->setObjectName("Peers");
    connected_client->setWindowTitle("Peers");
    connected_client->setWidget(list_user.get()); // add other user to the dock
    addDockWidget(Qt::RightDockWidgetArea,connected_client.get());
    connect(list_user.get(), SIGNAL(itemClicked(QListWidgetItem*)),
            this, SLOT(on_list_user_itemClicked(QListWidgetItem*)));

}


void texteditor::on_list_user_itemClicked(QListWidgetItem *item) {
    QString username = item->text();
    auto user = username_to_user.find(username);

    int i = 0;
    disconnect(editor->document(), &QTextDocument::contentsChange, this, &texteditor::contentsChange);
    for(auto const &s : shared_editor->text()){

        if(file.site_ids()[s.site_id()] == username){
           user->draw_background_char(editor.get(),i,i+1);
        }
        i++;
    }
    connect(editor->document(), &QTextDocument::contentsChange, this, &texteditor::contentsChange);

    user->selected = !user->selected;
    editor->currentCharFormat().setBackground(Qt::transparent);
}

void texteditor::init_cursors(){
    auto ids = file.cursors().keys();
    for( const auto& id : ids) {
        QString username = file.site_ids().find(id).value();

        if(id != file.site_id()) {
            Symbol symbol = file.cursors().find(id).value();
            int cursor_pos = shared_editor->find(file.cursors().find(id).value());
            username_to_user.find(username)->add_cursor(editor.get(),cursor_pos,id);
        }

    }
}


void texteditor::setupFileActions(){
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
   qDebug() << "INIT_USER_ACTION";
   QToolBar *tb = addToolBar(tr("User Actions"));
   QMenu *menu = menuBar()->addMenu(tr("&User"));
   const QIcon showPeersIcon = QIcon::fromTheme("show-peers", QIcon(imgPath + "/user_icon.png"));
   show_peers = menu->addAction(showPeersIcon, tr("&Show"), connected_client.get(),&QDockWidget::show);
   tb->addAction(show_peers);
}

void texteditor::setupEditActions() {
   qDebug() << "INIT_SETUP_ACTION";
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
   this->deleteLater();
   emit show_user_page();
}

void texteditor::file_share(){
   emit share_file(file.sharing_link().toString());
}


void texteditor::contentsChange(int position, int charsRemoved, int charsAdded) {
   if(change_from_server) return;
   for(int i = 0 ; i < charsAdded; i++) {
       Symbol s = shared_editor->local_insert(position, editor->toPlainText()[position + i]);
       client->sendInsert(file.document(),s);
   }
   for(int i = 0 ; i < charsRemoved; i++) {
       Symbol s = shared_editor->local_erase(position);
       client->sendErase(file.document(), s);
   }
}

void  texteditor::remote_insert(const Symbol& symbol){
    change_from_server = true;
    qDebug() << "remote_insert" << " char: " << symbol.value();
    shared_editor->remote_insert(symbol);
    this->editor->setText(shared_editor->to_string());
}


void  texteditor::remote_erase(const Symbol& symbol){

    change_from_server = true;
    shared_editor->remote_erase(symbol);
    this->editor->setText(shared_editor->to_string());
}

void texteditor::remote_cursor(const Symbol& symbol, const QString & username){
    
    int position = shared_editor->find(symbol);
    username_to_user.find(username)->change_cursor_position(editor.get(),position,symbol.site_id());
}

void texteditor::remote_open(const Profile &profile, int site_id){

    if(!username_to_user.count(profile.username())){
        UserInfo user(profile,generate_color());
        username_to_user.insert(user.username(),user);
        QIcon user_icon = QIcon(QPixmap::fromImage(profile.icon()));
        QListWidgetItem* widget = new QListWidgetItem(user_icon,profile.username());
        widget->setBackground(generate_color());
        username_to_row.insert(user.username(), user_row_);
        list_user->insertItem(user_row_, widget);
        user_row_++;
    }
    username_to_user.find(profile.username()).value().add_cursor(editor.get(),0,site_id);
}

void texteditor::remote_close(const QString &username) {
    int row = username_to_row.value(username);
    username_to_user.remove(username);
    auto widget = list_user->takeItem(row);
    delete widget;
}

void texteditor::closeEvent(QCloseEvent *event){
    file_close();
    event->accept();
}

void texteditor::textChange() {
    editor->setFontPointSize(14);
    if(change_from_server) {
        current_position = editor->textCursor().position();
        change_from_server  = false;
        return;
    }
}

void texteditor::cursorPositionChanged() {
    QTextCursor cursor = editor->textCursor();
    if(cursor.position() != current_position) {
        current_position = cursor.position();
        qDebug() << "cursor position:" << cursor.position();
        this->client->send_cursor(this->file.document(),
                                  this->shared_editor->insert_cursor(cursor.position(), QChar()));
    }
}

QColor texteditor::generate_color(){
    QColor color;
    double golden_ratio_conjugate = 0.618033988749895;
    h += golden_ratio_conjugate;
    h = h - (int)h;
    color.setHsvF(h, 0.5, 0.95);
    return color;
}