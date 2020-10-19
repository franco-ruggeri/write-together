/*
 * Author: Antonino Musmeci, Stefano Di Blasio
 */

#include <QtGlobal>
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
#include <QWindow>
#include <QDesktopWidget>
#include <QMainWindow>
#include <cte/client/texteditor.h>
#include <QDesktopWidget>
#include <cte/client/myClient.h>
#include <iostream>
#include <utility>
#include <cte/protocol/insert_message.h>
#include <cte/client/loginTextEditor.h>
#include <cte/protocol/erase_message.h>
#include <cte/protocol/cursor_message.h>
#include <cte/protocol/open_message.h>
#include <QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <cmath>
#include <QAbstractTextDocumentLayout>

const QString imgPath = ":/images";


texteditor::texteditor(QStackedWidget *parent, QSharedPointer<myClient> client, fileInfo file): QMainWindow(parent),
                        file(file){
    this->resize(QDesktopWidget().availableGeometry(this).size() * 0.7);
    this->setWindowTitle(file.document().full_name() + " " + APPLICATION);    editor = QSharedPointer<QTextEdit>::create(this);
    setCentralWidget(editor.data());
    this->client = client;
    change_from_server = false;
    content_change = false;
    editor->setFontPointSize(14);
    shared_editor = QSharedPointer<SharedEditor>::create(file.site_id(), file.getFileContent());
    editor->setText(shared_editor->to_string()); // set the text content
    number_of_words = editor->toPlainText().split(QRegExp("(\\s|\\n|\\r)+"), QString::SkipEmptyParts).count();
    number_of_characters = editor->toPlainText().count();
    this->statusBar()->showMessage("words: " + QString::number(number_of_words) + "   characters: " + QString::number(number_of_characters));
    setToolButtonStyle(Qt::ToolButtonFollowStyle);
    setupPeers();
    setupFileActions();
    setupEditActions();
    setupUserActions();
    connect(editor->document()->documentLayout(), &QAbstractTextDocumentLayout::documentSizeChanged , this, &texteditor::draw_cursors);
    connect(this->editor->verticalScrollBar(), &QScrollBar::valueChanged, this, &texteditor::draw_cursors);;
    connect(editor->document(), &QTextDocument::contentsChange, this, &texteditor::contentsChange);
    connect(editor.data(), &QTextEdit::textChanged, this, &texteditor::textChange);
    connect(editor.data(), &QTextEdit::cursorPositionChanged, this, &texteditor::cursorPositionChanged);

    connect(client.data(), &myClient::user_added, this, &texteditor::remote_open);
    connect(client.data(), &myClient::user_removed, this, &texteditor::remote_close);
    connect(client.data(), &myClient::char_inserted, this, &texteditor::remote_insert);
    connect(client.data(), &myClient::char_removed, this, &texteditor::remote_erase);
    connect(client.data(), &myClient::cursor, this, &texteditor::remote_cursor);

}


void texteditor::setupPeers(){
    h =  std::floor(rand());
//    auto ids = file.cursors().keys();

    // current user
    UserInfo user = client->user;
    user.format.setFontPointSize(14);
    username_to_user.insert(user.username(),user);
    if(!file.site_ids().count(file.site_id()))
        file.insert_new_userId(file.site_id(),user.username());
    // other users
    for(const auto & profile: file.users()){
        if(!username_to_user.count(profile.username())) {
            UserInfo user(profile, generate_color());
            username_to_user.insert(profile.username(), user);
        }
    }
}

void texteditor::setupPeersPanel() {
    number_online_users = 1;
    number_offline_users = 0;
    active_user_list_ = QSharedPointer<QListWidget>::create();
    active_user_list_->setObjectName("active_users");

    inactive_user_list_ = QSharedPointer<QListWidget>::create();
    inactive_user_list_->setObjectName("inactive_users");

    // current user
    auto my_user = username_to_user.find(client->user.username());
    my_user->color_ = generate_color();
    QIcon user_icon = QIcon(QPixmap::fromImage(my_user->icon()));
    auto *widget = new QListWidgetItem(user_icon, my_user->username() + " (you)");
    widget->setBackground(my_user->color());
    active_user_list_->addItem(widget);
    for (const auto& user : username_to_user) {
        if (user.username() != my_user->username()) {
            QIcon user_icon = QIcon(QPixmap::fromImage(user.icon()));
            auto *widget = new QListWidgetItem(user_icon, user.username());
            if (user.has_cursor(file.site_ids().key(user.username()))) { // an active user
                widget->setBackground(user.color());
                active_user_list_->addItem(widget);
                number_online_users ++;
            } else {
                widget->setBackground(QColor("gainsboro"));
                inactive_user_list_->addItem(widget);
                number_offline_users++;
            }
        }
    }
    QListWidgetItem* online_user_header = new QListWidgetItem("Online Users: " + QString::number(number_online_users));
    QListWidgetItem*  offline_user_header = new QListWidgetItem("Offline Users: " + QString::number(number_offline_users));


//    connect(editor, &texteditor::number_user_change, editor, &texteditor::update_number_user);

    active_user_list_->insertItem(0,online_user_header);
    inactive_user_list_->insertItem(0,offline_user_header );



    active_user_list_->setSelectionMode(QAbstractItemView::NoSelection);
    inactive_user_list_->setSelectionMode(QAbstractItemView::NoSelection);
    peers_wrapper_ = QSharedPointer<QSplitter>::create(Qt::Vertical);

    peers_wrapper_->insertWidget(0, active_user_list_.data());
    peers_wrapper_->insertWidget(1, inactive_user_list_.data());

    peers = QSharedPointer<QDockWidget>::create(); // widget to show the peers
    peers->setObjectName("Peers");
    peers->setWindowTitle("Peers");
    peers->setWidget(peers_wrapper_.data()); // add other user to the dock
    addDockWidget(Qt::RightDockWidgetArea,peers.data());
    connect(active_user_list_.data(), SIGNAL(itemClicked(QListWidgetItem *)),
            this, SLOT(on_list_user_itemClicked(QListWidgetItem*)));
    active_user_list_->item(0)->setFlags(active_user_list_->item(0)->flags() & !Qt::ItemIsUserCheckable);
    inactive_user_list_->item(0)->setFlags(inactive_user_list_->item(0)->flags() & !Qt::ItemIsUserCheckable);
    inactive_user_list_->item(0)->setForeground(Qt::black);
    active_user_list_->item(0)->setForeground(Qt::black);
}





void texteditor::on_list_user_itemClicked(QListWidgetItem *item) {

    if (active_user_list_->item(0) == item)
        return;
    QString username = item->text().split(' ')[0];

    auto user = username_to_user.find(username);
    qDebug() << username;
    qDebug() << user->color(); // ??
    qDebug() << user->username();


    int i = 0;
    disconnect(editor->document(), &QTextDocument::contentsChange, this, &texteditor::contentsChange);
    disconnect(editor.data(), &QTextEdit::textChanged, this, &texteditor::textChange);

    int start_pos = -1;
    auto s = shared_editor->text();

    for( i = 0; i <s.size(); i++){
        if(file.site_ids()[s[i].site_id()] == username && start_pos == -1){
            start_pos = i;
        }
        if(file.site_ids()[s[i].site_id()]!= username  && start_pos != -1 ){
            user->draw_background_char(editor.data(),start_pos,i);
            start_pos = -1;
        }
    }
    if(start_pos != -1)
        user->draw_background_char(editor.data(),start_pos,i );


    connect(editor->document(), &QTextDocument::contentsChange, this, &texteditor::contentsChange);
    connect(editor.data(), &QTextEdit::textChanged, this, &texteditor::textChange);

    user->selected = !user->selected;
   // editor->setCurrentCharFormat(user->format);

}

void texteditor::init_cursors(){
    auto ids = file.cursors().keys();
    for( const auto& id : ids) {
        QString username = file.site_ids().find(id).value();

        if(id != file.site_id()) {
            Symbol symbol = file.cursors().find(id).value();
            int cursor_pos = shared_editor->find(file.cursors().find(id).value());
            username_to_user.find(username)->add_cursor(editor.data(),cursor_pos,id);
        }
    }
    setupPeersPanel();
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
                   tr("&Close"), this, [this](){
      file_close(true);
   });

   tb->addAction(a);
   menu->addSeparator();
}

void texteditor::setupUserActions(){
   QToolBar *tb = addToolBar(tr("User Actions"));
   QMenu *menu = menuBar()->addMenu(tr("&User"));
   const QIcon showPeersIcon = QIcon::fromTheme("show-peers", QIcon(imgPath + "/user_group.png"));
   show_peers = menu->addAction(showPeersIcon, tr("&Show"), peers.data(),&QDockWidget::show);
   tb->addAction(show_peers);
   const QIcon peerProfile = QIcon::fromTheme("peer-profile", QIcon(imgPath + "/user_icon.png"));
   peer_profile = menu->addAction(peerProfile, tr("&Profile"), this, &texteditor::show_user_details);
   tb->addAction(peer_profile);
}

void texteditor::setupEditActions() {



   QToolBar *tb = addToolBar(tr("Edit Actions"));
   QMenu *menu = menuBar()->addMenu(tr("&Edit"));
   const QIcon undoIcon = QIcon::fromTheme("edit-undo", QIcon(imgPath + "/editundo.png"));
   actionUndo = menu->addAction(undoIcon, tr("&Undo"), editor.data(), &QTextEdit::undo);
   actionUndo->setShortcut(QKeySequence::Undo);
   tb->addAction(actionUndo);

   const QIcon redoIcon = QIcon::fromTheme("edit-redo", QIcon(imgPath + "/editredo.png"));
   actionRedo = menu->addAction(redoIcon, tr("&Redo"), editor.data(), &QTextEdit::redo);
   actionRedo->setPriority(QAction::LowPriority);
   actionRedo->setShortcut(QKeySequence::Redo);
   tb->addAction(actionRedo);
   menu->addSeparator();

#ifndef QT_NO_CLIPBOARD
   const QIcon cutIcon = QIcon::fromTheme("edit-cut", QIcon(imgPath + "/editcut.png"));
   actionCut = menu->addAction(cutIcon, tr("Cu&t"), editor.data(), &QTextEdit::cut);
   actionCut->setPriority(QAction::LowPriority);
   actionCut->setShortcut(QKeySequence::Cut);
   tb->addAction(actionCut);

   const QIcon copyIcon = QIcon::fromTheme("edit-copy", QIcon(imgPath + "/editcopy.png"));
   actionCopy = menu->addAction(copyIcon, tr("&Copy"), editor.data(), &QTextEdit::copy);
   actionCopy->setPriority(QAction::LowPriority);
   actionCopy->setShortcut(QKeySequence::Copy);
   tb->addAction(actionCopy);

   const QIcon pasteIcon = QIcon::fromTheme("edit-paste", QIcon(imgPath + "/editpaste.png"));
   actionPaste = menu->addAction(pasteIcon, tr("&Paste"), editor.data(), &QTextEdit::paste);
   actionPaste->setPriority(QAction::LowPriority);
   actionPaste->setShortcut(QKeySequence::Paste);
   tb->addAction(actionPaste);
   if (const QMimeData *md = QApplication::clipboard()->mimeData())
       actionPaste->setEnabled(md->hasText());

   QObject::connect(QApplication::clipboard(), &QClipboard::dataChanged, this, &texteditor::clipboard_changed);
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

void texteditor::file_close(bool spontaneous) {
    if (spontaneous) this->client->file_close(this->file);
#if (QT_VERSION < QT_VERSION_CHECK(5, 14, 0))
    emit show_user_page(client->user.filename_to_owner_map.values().toSet());
#else
    QList<Document> docs = client->user.filename_to_owner_map.values();
    emit show_user_page(QSet<Document>(docs.rbegin(), docs.rend()));
#endif
    this->close();
}

void texteditor::file_share(){
   emit share_file(file.sharing_link().toString());
}

void texteditor::show_user_details() {
    emit show_profile_update();
}

void texteditor::contentsChange(int position, int charsRemoved, int charsAdded) {
   content_change = true;
   if(change_from_server) return;   // TODO: perche'? contentsChange() non viene emesso quando il testo viene cambiato da codice

    // TODO: perche'? cosa fa di preciso e in che caso succede? un esempio?
    if(position ==  editor->textCursor().block().position() && charsAdded > 1){

        charsAdded  = charsAdded - charsRemoved;
        charsRemoved = 0;
    }

    // TODO: perche' il check empty()? non puo' essere vuoto se non ci sono errori, e' sincronizzato con la GUI
    for(int i = 0 ; i < charsRemoved && !shared_editor->text().empty(); i++) {
        Symbol s = shared_editor->local_erase(position);
        client->sendErase(file.document(), s);

    }

    for(int i = 0 ; i < charsAdded; i++) {
        QChar c = editor->toPlainText()[position + i];
        if(!c.isNull()) {   // TODO: perche' dovrebbe essere nullo?
            Symbol s = shared_editor->local_insert(position + i, c);
            client->sendInsert(file.document(), s);
        }
    }
}

//void  texteditor::remote_insert(const Symbol& symbol){
//    QString username = file.site_ids().find(symbol.site_id()).value();
//    change_from_server = true;
//    int pos = shared_editor->find(symbol);
//    shared_editor->remote_insert(symbol);
//    editor->toPlainText().insert(pos,symbol.value());
//    QTextCursor cursor = editor->textCursor();
//    cursor.setPosition(pos);
//    cursor.insertText(symbol.value(),username_to_user.find(username)->format);
//
//}

//
void  texteditor::remote_insert(const Symbol& symbol){
    QString username = file.site_ids().find(symbol.site_id()).value();
    change_from_server = true;
    int pos = shared_editor->find(symbol);
    qDebug() << pos;
    qDebug() << symbol.value();
    QTextCursor cursor = editor->textCursor();
    if(pos == cursor.position()) {
        shared_editor->remote_insert(symbol);
        editor->toPlainText().insert(pos,symbol.value());
//        cursor.setPosition(pos);
        int old_pos = cursor.position();
        cursor.insertText(symbol.value(),username_to_user.find(username)->format);
        cursor.setPosition(old_pos);
        editor->setTextCursor(cursor);
    }

    else {
        shared_editor->remote_insert(symbol);
        editor->toPlainText().insert(pos, symbol.value());
        cursor.setPosition(pos);
        cursor.insertText(symbol.value(), username_to_user.find(username)->format);
    }
    username_to_user.find(username)->change_cursor_position(editor.data(),pos + 1,symbol.site_id());

}



void  texteditor::remote_erase(const Symbol& symbol){
    change_from_server = true;
    int pos = shared_editor->find(symbol);
    shared_editor->remote_erase(symbol);
    QTextCursor cursor = editor->textCursor();
    cursor.setPosition(pos);
    cursor.deleteChar();
//    QString username = file.site_ids().find(symbol.site_id()).value();
//    username_to_user.find(username)->change_cursor_position(editor.get(),pos,symbol.site_id());
//    remote_cursor(symbol, username);

}

void texteditor::remote_cursor(const Symbol& symbol, const QString & username){
    int position = shared_editor->find(symbol);
    username_to_user.find(username)->change_cursor_position(editor.data(),position,symbol.site_id());
}

void texteditor::remote_open(const Profile &profile, int site_id){
    if(!username_to_user.count(profile.username())){ // user just joined the file
        UserInfo user(profile,generate_color());
        username_to_user.insert(user.username(),user);
        QIcon user_icon = QIcon(QPixmap::fromImage(profile.icon()));
        QListWidgetItem* widget = new QListWidgetItem(user_icon,profile.username());
        widget->setBackground(user.color());
        active_user_list_->addItem(widget);
    } else {
        QListWidget *inactive_list = static_cast<QListWidget*>((static_cast<QSplitter*>(peers->widget()))->widget(1));
        QListWidget *active_list = static_cast<QListWidget*>((static_cast<QSplitter*>(peers->widget()))->widget(0));
        QList<QListWidgetItem *> profile_matches = inactive_list->findItems(profile.username(), Qt::MatchContains);
        UserInfo user = username_to_user.value(profile.username());
        QListWidgetItem *widget = profile_matches.first();
        auto* widget_to_move = inactive_list->takeItem(inactive_list->row(widget));
        widget_to_move->setBackground(user.color());
        active_list->addItem(widget_to_move);

        number_offline_users --;
        number_online_users ++;
        inactive_user_list_->item(0)->setText("Offline Users: " + QString::number(number_offline_users));
        active_user_list_->item(0)->setText("Online Users: " + QString::number(number_online_users));
    }
    username_to_user.find(profile.username()).value().add_cursor(editor.data(),0,site_id);
    file.insert_new_userId(site_id,profile.username());

}

void texteditor::remote_close(const QString &username, int site_id) {
    QListWidget *active_list = static_cast<QListWidget*>((static_cast<QSplitter*>(peers->widget()))->widget(0));
    QListWidget *inactive_list = static_cast<QListWidget*>((static_cast<QSplitter*>(peers->widget()))->widget(1));
    QList<QListWidgetItem *> profile_matches = active_list->findItems(username, Qt::MatchContains);
    QListWidgetItem *widget = profile_matches.first();
    auto* widget_to_move = active_list->takeItem(active_list->row(widget));
    widget_to_move->setBackground(QColor("gainsboro"));
    inactive_list->addItem(widget_to_move);
    username_to_user.find(username)->remove_cursors(site_id);

    number_offline_users ++;
    number_online_users --;
    inactive_user_list_->item(0)->setText("Offline Users: " + QString::number(number_offline_users));
    active_user_list_->item(0)->setText("Online Users: " + QString::number(number_online_users));
}

#ifndef QT_NO_CLIPBOARD
void texteditor::clipboard_changed() {
    const QMimeData *md = QApplication::clipboard()->mimeData();
    if (md) {
        actionPaste->setEnabled(md->hasText());
    }
}
#endif

void texteditor::closeEvent(QCloseEvent *event){
    file_close(event->spontaneous());
    event->accept();
}




void texteditor::textChange() {
    draw_cursors();
    number_of_words = editor->toPlainText().split(QRegExp("(\\s|\\n|\\r)+"), QString::SkipEmptyParts).count();
    number_of_characters = editor->toPlainText().count();
    this->statusBar()->showMessage("words: " + QString::number(number_of_words) + "   characters: " + QString::number(number_of_characters));
    editor->setFontPointSize(14);
    if(change_from_server) {
       current_position = editor->textCursor().position();
       change_from_server  = false;
//        return;
    }
}

void texteditor::cursorPositionChanged() {
    if(!content_change) {
        QTextCursor cursor = editor->textCursor();
        if (cursor.hasSelection())
            return;
        editor->setCurrentCharFormat(username_to_user.find(client->user.username())->format);
        if (cursor.position() != current_position) {
            qDebug() << "cursor pos change";
            current_position = cursor.position();
            this->client->send_cursor(this->file.document(),
                                      this->shared_editor->insert_cursor(cursor.position(), QChar()));
        }
    }
    content_change = false;
}

QColor texteditor::generate_color(){
    QColor color;
    double golden_ratio_conjugate = 0.618033988749895;
    h += golden_ratio_conjugate;
    h = h - (int)h;
    color.setHsvF(h, 0.5, 0.95);
    return color;
}

void texteditor::draw_cursors(){
    for(auto &user: username_to_user){
        user.update_cursor(editor.data());
    }
}