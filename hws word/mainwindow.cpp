#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow( QWidget *parent ) :
    QMainWindow( parent ),
    ui( new Ui::MainWindow )
{
    window_init();
    slot_init();
    refresh_screen();
}

void MainWindow::window_init()
{
    ui->setupUi( this );
    this->setWindowTitle( "MiniWord Editor" );

    this->installEventFilter(this);
    ui->label_2->setAttribute(Qt::WA_ShowWithoutActivating,true);

    ui->label->setStyleSheet( "qproperty-alignment: 'AlignTop | AlignLeft'; " );
    QFont font = ui->label->font();
    int	pointsize = font.pointSize() * 18 / 7;
    font.setPixelSize( pointsize );
    ui->label->setFont( font );
    ui->label->setText( "" );
    ui->label_2->setGeometry( 12 * Doc.cursor.col - 11, 26 * Doc.cursor.row_cnt, 30, 30 );

    ui->scrollArea->setWidgetResizable( true );
    ui->scrollArea->setStyleSheet( "background:white" );
    QTextCodec *codec = QTextCodec::codecForName( "UTF-8" );
    QTextCodec::setCodecForLocale( codec );

    this->setMouseTracking( true );
    this->setAttribute( Qt::WA_KeyCompression );
    this->setAttribute( Qt::WA_InputMethodEnabled );
    ui->label_2->setAttribute(Qt::WA_ShowWithoutActivating,true);

    /* 一些初始化时应该默认关闭的操作 */
    ui->actionUndo_Z->setEnabled( false );
    ui->actionRedo_Y->setEnabled( false );

    this->setFocus();
}

void MainWindow::slot_init()
{
    // NEW FILE
    ui->actionNew_N->setShortcut(QKeySequence::New);
    connect(ui->actionNew_N, SIGNAL(triggered(bool)), this, SLOT(new_file()));

    // OPEN FILE
    ui->actionOpen_O->setShortcut( QKeySequence::Open );
    connect( ui->actionOpen_O, SIGNAL( triggered( bool ) ), this, SLOT( open_file() ) );

    // SAVE ACTIONS
    ui->actionSave_S->setShortcut( QKeySequence::Save );
    connect( ui->actionSave_S, SIGNAL( triggered( bool ) ), this, SLOT( save_all() ) );
    connect( ui->actionSave_as, SIGNAL( triggered( bool ) ), this, SLOT( save_as() ) );

    // BLOCK ACTIONS
    ui->actionPaste_P->setShortcut( QKeySequence::Copy );
    connect( ui->actionCopy_C, SIGNAL( triggered( bool ) ), this, SLOT( block_copy() ) );
    ui->actionPaste_P->setShortcut( QKeySequence::Paste );
    connect( ui->actionPaste_P, SIGNAL( triggered( bool ) ), this, SLOT( block_paste() ) );

    // SHOW ABOUT
    connect( ui->actionMiniword, SIGNAL( triggered( bool ) ), this, SLOT( show_about() ) );

    // FIND & REPLACE
    connect( ui->actionFind_F, SIGNAL( triggered() ), this, SLOT( find_text() ) );
    connect( &f, SIGNAL( FindSignal( QString ) ), this, SLOT( find( QString ) ) );
    connect( ui->actionreplace_R, SIGNAL( triggered() ), this, SLOT( replace_window() ) );
    connect( &r, SIGNAL( findSignal( QString ) ), this, SLOT( find( QString ) ) );
    connect( &r, SIGNAL( replaceSignal( QString, QString ) ), this, SLOT( replace( QString, QString ) ) );
    connect(&r, SIGNAL(replaceAllSignal(QString,QString)), this, SLOT(replace_all(QString,QString)));

}

void MainWindow::refresh_screen()
{
    //显示窗口文件名
    if ( Doc.file_name.isEmpty() )
        this->setWindowTitle( "Untitled - MiniWord" );
    else {
        QFileInfo file_info = QFileInfo( Doc.file_name );
        this->setWindowTitle( file_info.fileName() + " - MiniWord" );
    }
    Row *temp = Doc.first_row;
    QString output_string = "";
    while ( temp )
    {
        output_string.append( temp->row_text );
        output_string.append( '\n' );
        temp = temp->next_row;
    }

    chinese_count();

    qDebug() << "窗口值" << ui->scrollArea->width()<<ui->scrollArea->height();
    ui->label->setText( output_string );
    ui->label->adjustSize();
    ui->label_2->setGeometry( 12 * (Doc.cursor.col - Doc.cursor.chi_cnt * 1.08) - 11, 26 * Doc.cursor.row_cnt, 30, 30 );
    ui->scrollAreaWidgetContents_2->setMinimumSize( ui->label->width(), ui->label->height() );

    this->setFocus();

    qDebug() << "\n当前行 - cur_len:" << Doc.cursor.row_ptr->cur_len << " max_len:" << Doc.cursor.row_ptr->max_len;
    qDebug() << "光标 - hang:" << Doc.cursor.row_ptr << "col:" << Doc.cursor.col << " row:" << Doc.cursor.row_cnt << " chi_cnt:" << Doc.cursor.chi_cnt;
    qDebug() << "修改 - Modified:" << Doc.isModified;
}

void MainWindow::chinese_count()
{
    qDebug() << "运行了chinese_count";
    Doc.cursor.chi_cnt = chinese_judge(Doc.cursor.col);
}

int MainWindow::chinese_judge(int &col)
{
    //自动加减得到Cursor.col考虑中文应该的位置
    //返回的是Cursor.col之前中文的个数
    qDebug() << "运行了chinese_judge";
    int chi_cnt = 0;
    for ( int i = 0; i < Doc.cursor.col; i++ )
        if ( Doc.cursor.row_ptr->row_text[i] < 0 )
            chi_cnt++;
    int n = chi_cnt % 3;
    while( n && Doc.cursor.col){
        n--;
        col--;
    }
    return chi_cnt/3;
}

void MainWindow::open_file()
{
    // 如果当前有没保存的先保存
    if ( Doc.isModified ){
        const QMessageBox::StandardButton ret
            = QMessageBox::warning( this, tr( "Application" ),
                        tr( "当前文档未保存，是否保存?" ),
                        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel );
        switch ( ret )
        {
        case QMessageBox::Save:
            save_all();
            break;
        case QMessageBox::Discard:
            Doc.clear_all();
            break;
        case QMessageBox::Cancel:
            return;
        default:
            return;
        }
    }
    // 打开新的文档
    QString tmp_file_name = QFileDialog::getOpenFileName( this );
    //选择的文件不为空
    if ( !tmp_file_name.isEmpty() ){
        // 检查文件后缀是否为 txt
        QFileInfo file_info	= QFileInfo( tmp_file_name );
        QString file_suffix	= file_info.suffix();
        if ( file_suffix == "txt" ){
            //载入文件
            string str = tmp_file_name.toStdString();
            this->Doc.read_file( (char *) str.c_str() );
            Doc.file_name = tmp_file_name;

            refresh_screen();
        }
    }
    else{
        qDebug() << "未选中文件";
    }
}

/* 把内容保存到文件中 */
void MainWindow::save_all()
{
    //如果没有默认路径，选择保存路径
    if ( Doc.file_name.isEmpty() )
        Doc.file_name = QFileDialog::getSaveFileName( this, tr( "选择保存位置" ), "", tr( "*.txt" ) );
    //保存
    string str = Doc.file_name.toStdString();
    if (!(this->Doc.save_file( (char *) str.c_str()))){
        QMessageBox::information( this,
                      tr( "保存结果" ),
                      tr( "文件保存失败!" ) );
        return;
    }
    refresh_screen();
}

void MainWindow::save_as()
{
    Doc.file_name = "";
    save_all();
    refresh_screen();
}

void MainWindow::new_file()
{
    //如果有未保存的修改，先提示是否保存
    if (this->Doc.isModified) {
        const QMessageBox::StandardButton ret
                = QMessageBox::warning(this, tr("Application"),
                                       tr("当前文档未保存，是否保存?"),
                                       QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        switch (ret) {
            case QMessageBox::Save:
                save_all();
                Doc.clear_all();
                break;
            case QMessageBox::Discard:
                Doc.clear_all();
                break;
            case QMessageBox::Cancel:
                break;
            default:
                break;
        }

    }
    refresh_screen();

}

/*
 * ===================重要问题=====================
 * tab，page up/down, home, end，小键盘enter 无效了
 * 光标-鼠标问题
 * 中文光标位置问题
 * 择问题
 * 输入的时候窗口位置（光标没有在可视范围内）
 * ===============================================
 */


/******* 块操作逻辑？？？？？？？？？？ *******
 * 1.粘贴（直接读取系统剪贴板，然后调用 edit() 函数）√
 * 2.其他 选择、剪切、复制、删除
 * *
 * *
 * *
 */
/* ？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？ */


/******************** Undo / Redo *********************
 *  用一个结构体数组保存操作序列
 *                    Undo/Redo 结构体数组
 *  ------------------------------------------------------------------
 * |  操作类型(剪切/删除/粘贴) |       内容 (字符串)       |   Cursor位置   |
 *  ------------------------------------------------------------------
 *   ······················  ························  ···············
 *  ------------------------------------------------------------------
 * |          删除          |         删除长度          |   Cursor位置   |
 *  ------------------------------------------------------------------
 * |          粘贴          |       内容 (字符串)       |   Cursor位置   |
 *  ------------------------------------------------------------------
 * |          剪切          |       内容 (字符串)       |   Cursor位置   |
 *  ------------------------------------------------------------------
 *
 * [输入] 看成是 [粘贴] 的特殊情况(字符串只有一个字符)
 *
 * Q?是不是还得加 前/后 ? 向前/向后 选择操作的情况
 *
 * 1. 当输入的时候(比如 a)
 * * Undo 序列保存： |删除|  1 | Cursor |
 * * Redo 序列保存： |粘贴|  a | Cursor |
 *
 * 2. 当删除的时候(比如 abc)
 * * Undo 序列保存： |粘贴| abc | Cursor |
 * * Redo 序列保存： |删除|  3  | Cursor |
 *
 * 3. 剪切的时候(比如 abc)
 * * Undo 序列保存： |粘贴| abc | Cursor |
 * * Redo 序列保存： |剪切|  3  | Cursor |
 * ***************************************************/


void MainWindow::find_text()
{
    found = false;
    notExist = false;
    qDebug() << "\n打开查找窗口\n" << endl;

    f.show();
}

void MainWindow::find(QString text){
    qDebug() << "\n查找功能可用" << endl;
    qDebug() << "text = " << text << endl;
    Row *cur_row = Doc.cursor.row_ptr; // 定位查找起始行
    int col = Doc.cursor.col; //查找起始位置
    int pos = -1; //查找位置，初始化为-1
    QString words = "";

    if(cur_row == NULL){
        //添加弹窗
        notExist = true; // 查找失败

        QMessageBox box(QMessageBox::Question,"查找", "无此内容！");
        box.setIcon(QMessageBox::Information);// 暂时没有加上icon
        box.setStandardButtons (QMessageBox::Ok);
        box.setButtonText (QMessageBox::Ok,QString("OK"));
        box.setWindowFlags( Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint );
        box.exec();

    }

    Q_ASSERT(cur_row != NULL);
    if(cur_row != NULL){

        words.append(cur_row->row_text);//将本行文本存入words


        qDebug() << "found is " << found << endl;

        if(found == false)//如果当前位置并非之前已经查找成功的位置
            pos = words.indexOf(text, col); //从col位置开始查找tex
        else if(col + text.length() > words.length() - 1) //剩余长度不足以容纳查找词
            pos = -1;
        else
            pos = words.indexOf(text, col + text.length()); //跳过当前查找成功词查找


        int row_to_plus = 0;
        while(cur_row != NULL && pos == -1){
            found = false;
            cur_row = cur_row->next_row;
            if(cur_row == NULL)
                break;
            col = 0;
            words = "";
            words.append(cur_row->row_text);
            qDebug() << "words are:\n" << words << endl;
            row_to_plus++;

            pos = words.indexOf(text, col);
//            if(cur_row->Next_Row == NULL){
//                qDebug() << "FUCK YOU!" << endl;
//            }
//            else
//                qDebug() << "not null" << endl;

        }

        if(cur_row == NULL){
            qDebug() << "not found" << endl;

            notExist = true; // 查找失败

            QMessageBox box(QMessageBox::Question,"查找", "无此内容！");
            box.setIcon(QMessageBox::Information);// 暂时没有加上icon
            box.setStandardButtons (QMessageBox::Ok);
            box.setButtonText (QMessageBox::Ok,QString("OK"));
            box.setWindowFlags( Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint );
            box.exec();


        }


        else if(pos != -1){
            found = true;
            Doc.cursor.row_ptr = cur_row;
            Q_ASSERT(cur_row != NULL);
            Doc.cursor.row_cnt += row_to_plus;
            Doc.cursor.col = pos; //移动光标到第一个查找成功的位
            col = Doc.cursor.col; // 更新col
            ui->label_2->setGeometry(12*Doc.cursor.col-11,26*Doc.cursor.row_cnt,30,30); //绘制光标

            start_col = Doc.cursor.col + 1;
            start_row = Doc.cursor.row_cnt;
            end_col = start_col + text.length();
            end_row = start_row;

            add_blockshadow();
            isSelectedblock = true;
        }
    }

}

// 打开替换窗口
void MainWindow:: replace_window(){
    isFirstReplace = true;
    r.exec();
}

// 替换
void MainWindow:: replace(QString target, QString value){
    // target: 查找文本
    // value: 应替换的文本

    // 如果是第一次替换，并且当前不是被被Find Next查找选中的状态
    if(isFirstReplace && !isSelectedblock){
        found = false;
        notExist = false;
        find(target);//执行查找操作
        isFirstReplace = false;

        qDebug() << "第一次替换"  << endl;
    }
    else{
        //先删除target
        qDebug() << "不是第一次替换" << endl;
        block_delete();
        isSelectedblock = false;
        found = false; // 替换后当前为未查找成功状态

        // 加上value
        std::string words = value.toStdString();
        const char* s = words.c_str();
        Doc.insert_text(s);

        refresh_screen();

        //查找下一个
        find(target);


    }

}

void MainWindow::replace_all(QString target, QString value){
   // qDebug() << "replace all " << endl;
    while(!notExist){
        replace(target, value);
    }

    notExist = false;
}

/* 显示 Help 函数 */
void MainWindow::show_help()
{
    QMessageBox::information( this, tr( "帮助" ), tr( "编辑文档。" ) );
}

/* 显示 About 函数 */
void MainWindow::show_about()
{
    QMessageBox::about( this, tr( "关于 MiniWord" ), tr( "欢迎您的使用!\n"
                               "此程序 MiniWord 由 Daoxu Sheng, Weiran Huang\n 以及 Zengrui Wang 共同开发完成。" ) );
}

/* 主窗口 关闭 函数 */
void MainWindow::closeEvent( QCloseEvent *event )
{
    if (this->Doc.isModified){
        const QMessageBox::StandardButton ret
            = QMessageBox::warning( this, tr( "Application" ),
                        tr( "当前文档未保存，是否保存?" ),
                        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel );
        switch ( ret )
        {
        case QMessageBox::Save:
            save_all();
            break;
        case QMessageBox::Discard:
            event->accept();
            break;
        case QMessageBox::Cancel:
            event->ignore();
            break;
        default:
            event->ignore();
            break;
        }
    }
}

bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    if ( object == this &&event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Tab) {
            // Special tab handling
            string n = "    ";
            Doc.insert_text( (char*)n.c_str() );
            refresh_screen();
            return true;
        } else
            return false;
    }
    return false;
}

void MainWindow::enter_pressed()
{
    if(isSelectedblock)
        block_delete();
    int	i, j = 0;
    int	col	= Doc.cursor.col;
    Row	*hang	= Doc.cursor.row_ptr;
    Doc.new_row( Doc.cursor.row_ptr );
    Doc.cursor.row_ptr = Doc.cursor.row_ptr->next_row;
    Doc.cursor.cur_height++;
    for ( i = col; i < hang->cur_len; i++ )
    {
        Doc.cursor.row_ptr->row_text[j++]	= hang->row_text[i];
        hang->row_text[i] = '\0';
    }
    hang->cur_len = col;
    Doc.cursor.row_ptr->cur_len	= j;
    Doc.cursor.col = 0;
    Doc.cursor.row_cnt++;
    Doc.isModified = true;
}

void MainWindow::delete_pressed()
{
    if(isSelectedblock)
        block_delete();
    else if ( Doc.cursor.col < Doc.cursor.row_ptr->cur_len )  //当指针不在行尾的时候(之前有字符可以删除)
    {
        // 判断中文字符
        int mov = 1;
        if ( Doc.cursor.row_ptr->row_text[Doc.cursor.col] < 0 )
            mov = 3;

        for ( int i = Doc.cursor.col; i <= Doc.cursor.row_ptr->cur_len - mov; i++ )
        {
            Doc.cursor.row_ptr->row_text[i]	= Doc.cursor.row_ptr->row_text[i + mov];
            Doc.cursor.row_ptr->row_text[i + mov] = '\0';
        }
        Doc.cursor.row_ptr->cur_len	-= mov;
        Doc.isModified = true;
    }
    else if ( Doc.cursor.col == Doc.cursor.row_ptr->cur_len && Doc.cursor.row_ptr->next_row )
    {// 当在行尾，并且不是最后一行一行(最后一行行尾delete就不用操作了)
        Row *temp = Doc.cursor.row_ptr->next_row;
        if ( temp->next_row )
        {
            Doc.cursor.row_ptr->next_row = temp->next_row;
        } else {
            Doc.cursor.row_ptr->next_row = NULL;
        }
        // 需要把之后行的加到后面
        int	i	= 0;
        int	col	= Doc.cursor.col;
        while ( i < temp->cur_len )
        {
            Doc.cursor.row_ptr->row_text[col++] = temp->row_text[i++];
            Doc.cursor.row_ptr->cur_len++;
        }
        delete temp;
        Doc.cursor.cur_height--;
        Doc.isModified = true;
    }

}

void MainWindow::backspace_pressed()
{
    if ( isSelectedblock )
        block_delete();
    else if ( Doc.cursor.col > 0 )  //当指针不在行首的时候(之前有字符可以删除)
    {
        //判断中文字符
        int mov = 1;
        if ( Doc.cursor.row_ptr->row_text[Doc.cursor.col - 1] < 0 )
            mov = 3;
        for ( int i = Doc.cursor.col - mov; i <= Doc.cursor.row_ptr->cur_len - mov; i++ )
        {
            Doc.cursor.row_ptr->row_text[i]	= Doc.cursor.row_ptr->row_text[i + mov];
            Doc.cursor.row_ptr->row_text[i + mov] = '\0';
        }
        Doc.cursor.col -= mov;
        Doc.cursor.row_ptr->cur_len	-= mov;
        Doc.isModified = true;
    }
    else if ( Doc.cursor.col == 0 && Doc.cursor.row_ptr != Doc.first_row ) /* 当在行首，并且不是第一行(第一行行首删除就不用操作了) */
    {
        Row *temp = Doc.cursor.row_ptr;
        Doc.cursor.row_ptr = temp->prev_row;
        if ( temp->next_row )
        {
            temp->next_row->prev_row = Doc.cursor.row_ptr;
        }
        Doc.cursor.row_ptr->next_row = temp->next_row;
        Doc.cursor.col = Doc.cursor.row_ptr->cur_len;
        // 需要把之后行的加到后面
        int	i = 0;
        int	col	= Doc.cursor.col;
        while ( i < temp->cur_len )
        {
            Doc.cursor.row_ptr->row_text[col++] = temp->row_text[i++];
            Doc.cursor.row_ptr->cur_len++;
        }
        Doc.cursor.row_cnt--;
        Doc.cursor.cur_height--;
        delete temp;
        Doc.isModified = true;
    }
}

void MainWindow::keyPressEvent( QKeyEvent *ev )
{
    this->setFocus();
    string n;
    n = ev->key();
    if ( ev->key() == Qt::Key_CapsLock ){
        if ( caps == true )
            caps = false;
        else
            caps = true;
    }
    else if ( ev->key() == Qt::Key_Left ){
        Doc.cursor_left();
    }
    else if ( ev->key() == Qt::Key_Up ){
        Doc.cursor_up();
    }
    else if ( ev->key() == Qt::Key_Right ){
        Doc.cursor_right();
    }
    else if ( ev->key() == Qt::Key_Down ){
        Doc.cursor_down();
    }
    else if( ev->key() == Qt::Key_Home ){
        Doc.cursor_home();
    }
    else if( ev->key() == Qt::Key_End ){
        Doc.cursor_end();
    }
    else if( ev->key() == Qt::Key_Shift || ev->key() == Qt::Key_Control ||
             ev->key() == Qt::Key_Alt || ev->key() == Qt::Key_Insert ){
        ;
    }
    else if ( (ev->key() <= Qt::Key_Z && ev->key() >= Qt::Key_A) && ev->modifiers() == Qt::ShiftModifier ){
        if(isSelectedblock)
            block_delete();
        if ( caps == true )
        {
            transform( n.begin(), n.end(), n.begin(), ::tolower );
            Doc.insert_text( (char*)n.c_str() );
        } else {
            transform( n.begin(), n.end(), n.begin(), ::toupper );
            Doc.insert_text( (char*)n.c_str() );
        }
    }
    else if ( ev->key() == Qt::Key_Enter || ev->key() == Qt::Key_Return ){
        enter_pressed();
    }
    else if ( ev->key() == Qt::Key_Backspace ) {
        backspace_pressed();
    }
    else if ( ev->key() == Qt::Key_Delete ) {
        delete_pressed();
    }
    else { // 英文输入
        if(isSelectedblock)
            block_delete();
        if ( caps == false )
            transform( n.begin(), n.end(), n.begin(), ::tolower );
        Doc.insert_text((char*)n.c_str());
    }

    refresh_screen();

}

void MainWindow::inputMethodEvent( QInputMethodEvent *a )
{
    qDebug() << "当前输入为" << a->commitString().toUtf8().data();
    qDebug() << Doc.cursor.chi_cnt;
    qDebug() << Doc.cursor.col;
    Doc.insert_text( a->commitString().toUtf8().data() );
    this->setFocus();
    refresh_screen();
}

void MainWindow::mouseMoveEvent( QMouseEvent *event )
{
    if ( event->buttons() & Qt::LeftButton )
    {
        if ( start_x == -1 && start_y == -1 )
        {
            start_x = event->pos().x();
            start_y = event->pos().y();
            pos_to_coordinate( start_x, start_y, false );
            start_col	= Doc.cursor.col + 1;
            start_row	= Doc.cursor.row_cnt;
        }
        end_x	= event->pos().x();
        end_y	= event->pos().y();

        pos_to_coordinate( end_x, end_y, false );
        end_col = Doc.cursor.col + 1;
        end_row = Doc.cursor.row_cnt;

        add_blockshadow();
        refresh_screen();
    }
}

void MainWindow::mouseReleaseEvent( QMouseEvent *event )
{
    if ( event->button() == Qt::LeftButton )
    {

        qDebug() << "开始位置" << start_col << start_row;
        qDebug() << "结束位置" << end_col << end_row;

        start_x		= -1;
        start_y		= -1;
        refresh_screen();
    }
}

void MainWindow::mousePressEvent( QMouseEvent *event )
{
    for ( int i = 0; i < test.size(); i++ )
    {
        test[i]->hide();
    }
/*
 *    start_col = -1;
 *    start_row = -1;
 */
    /* 如果是鼠标左键按下   改变指针形状，并且存储当前指针位置与窗口位置的差值.Hello Please Tina */
    if ( event->button() == Qt::LeftButton )
    {
        isSelectedblock = false;
        pos_to_coordinate( event->pos().x(), event->pos().y(), true );
    }
    /* 如果是鼠标右键按下 */
    else if ( event->button() == Qt::RightButton )
    {
    }
    this->setFocus();
}

void MainWindow::pos_to_coordinate( int x, int y, bool flag )
{
    int	toolbar_height	= ui->mainToolBar->height() + ui->menuBar->height();
    int	hbar_height	= ui->scrollArea->horizontalScrollBar()->value();
    int	vbar_height	= ui->scrollArea->verticalScrollBar()->value();
    for ( int j = 0; j < Doc.cursor.cur_height; j++ )
    {
        if ( y + vbar_height > 26 * j + toolbar_height && y + vbar_height < 26 * (j + 1) + toolbar_height )
        {
            if ( Doc.cursor.row_cnt < j )
                for ( int temp = Doc.cursor.row_cnt; temp < j; temp++ )
                    Doc.cursor.row_ptr = Doc.cursor.row_ptr->next_row;
            else if ( Doc.cursor.row_cnt >= j )
                for ( int temp = 0; temp < Doc.cursor.row_cnt - j; temp++ )
                    Doc.cursor.row_ptr = Doc.cursor.row_ptr->prev_row;
            Doc.cursor.row_cnt = j;

            for ( int i = 1; i <= Doc.cursor.row_ptr->cur_len; i++ )
            {
                if ( x + hbar_height > (12 * i - 11) && x + hbar_height <= (12 * (i + 1) - 11) )
                {
                    Doc.cursor.col = i - 1;
                    if ( flag )
                        ui->label_2->setGeometry( 12 * (Doc.cursor.col - Doc.cursor.chi_cnt * 1.08) - 11,
                                      26 * Doc.cursor.row_cnt, 30, 30 );
                    break;
                } else if ( x + hbar_height >= 12 * (Doc.cursor.row_ptr->cur_len + 1) - 11 )
                {
                    Doc.cursor.col = Doc.cursor.row_ptr->cur_len;
                    if ( flag )
                        ui->label_2->setGeometry( 12 * (Doc.cursor.col - Doc.cursor.chi_cnt * 1.08) - 11,
                                      26 * Doc.cursor.row_cnt, 30, 30 );
                    break;
                }
            }
            break;
        }
    }
    if ( y + vbar_height >= 26 * Doc.cursor.cur_height + toolbar_height )
    {
        while ( Doc.cursor.row_ptr->next_row )
            Doc.cursor.row_ptr = Doc.cursor.row_ptr->next_row;
        Doc.cursor.row_cnt = Doc.cursor.cur_height;
        for ( int i = 1; i <= Doc.cursor.row_ptr->cur_len; i++ )
        {
            if ( x + hbar_height > (12 * i - 11) && x + hbar_height <= (12 * (i + 1) - 11) )
            {
                Doc.cursor.col = i - 1;
                if ( flag )
                    ui->label_2->setGeometry( 12 * (Doc.cursor.col - Doc.cursor.chi_cnt * 1.08) - 11,
                                  26 * Doc.cursor.row_cnt, 30, 30 );
                break;
            } else if ( x + hbar_height >= 12 * (Doc.cursor.row_ptr->cur_len + 1) - 11 )
            {
                Doc.cursor.col = Doc.cursor.row_ptr->cur_len;
                if ( flag )
                    ui->label_2->setGeometry( 12 * (Doc.cursor.col - Doc.cursor.chi_cnt * 1.08) - 11,
                                  26 * Doc.cursor.row_cnt, 30, 30 );
                break;
            }
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


