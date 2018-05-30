#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow( QWidget *parent ) :
    QMainWindow( parent ),
    ui( new Ui::MainWindow )
{
    ui->setupUi( this );
    this->setFocus();
    this->setMouseTracking( true );
    ui->label->setStyleSheet( "qproperty-alignment: 'AlignTop | AlignLeft'; " );

//    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect();
//    effect->setOpacity( 0.6 );
//    for ( int i = 0; i < 1000; i++ )
//    {
//        test[i] = new QLabel( ui->scrollAreaWidgetContents_2 );
//        test[i]->setFixedSize( 24, 26 );
//        test[i]->setGeometry( -10, -40, 12, 26 );
//        test[i]->setGraphicsEffect( effect );
//    }
    //ui->scrollAreaWidgetContents_2->resize();

    QFont	font		= ui->label->font();
    int	pointsize	= font.pointSize() * 18 / 7;
    font.setPixelSize( pointsize );
    ui->label->setFont( font );
    ui->label->setText( qsentence );
    ui->label_2->setGeometry( 12 * sentence.cursor.col - 11, 26 * sentence.cursor.row, 30, 30 );
    ui->scrollArea->setWidgetResizable( true );
    QTextCodec *codec = QTextCodec::codecForName( "UTF-8" ); /* 情况2 */
    QTextCodec::setCodecForLocale( codec );
    this->setAttribute( Qt::WA_InputMethodEnabled );
    this->setAttribute( Qt::WA_KeyCompression );
    this->setFocusPolicy( Qt::WheelFocus );
    ui->scrollArea->setStyleSheet( "background:white" );


    /* ******* connect ******* */
    ui->actionOpen_O->setShortcut( QKeySequence::Open );
    connect( ui->actionOpen_O, SIGNAL( triggered( bool ) ), this, SLOT( Open_File() ) );

    connect( ui->actionMiniword, SIGNAL( triggered( bool ) ), this, SLOT( Show_About() ) );

    ui->actionSave_S->setShortcut( QKeySequence::Save );
    connect( ui->actionSave_S, SIGNAL( triggered( bool ) ), this, SLOT( Save_All() ) );

    ui->actionPaste_P->setShortcut( QKeySequence::Copy );
    connect( ui->actionCopy_C, SIGNAL( triggered( bool ) ), this, SLOT( Block_Copy() ) );

    ui->actionPaste_P->setShortcut( QKeySequence::Paste );
    connect( ui->actionPaste_P, SIGNAL( triggered( bool ) ), this, SLOT( Block_Paste() ) );

    connect( ui->actionSave_as, SIGNAL( triggered( bool ) ), this, SLOT( Save_As() ) ); /* MODIFIED */
    /* 一些初始化时应该默认关闭的操作 */
    ui->actionUndo_Z->setEnabled( false );
    ui->actionRedo_Y->setEnabled( false );
    /* 连接 */
    connect( ui->actionFind_F, SIGNAL( triggered() ), this, SLOT( Find_Text() ) );
    connect( &f, SIGNAL( FindSignal( QString ) ), this, SLOT( Find( QString ) ) );
    connect( ui->actionreplace_R, SIGNAL( triggered() ), this, SLOT( Replace_Window() ) );
    connect( &r, SIGNAL( findSignal( QString ) ), this, SLOT( Find( QString ) ) );
    connect( &r, SIGNAL( replaceSignal( QString, QString ) ), this, SLOT( Replace( QString, QString ) ) );
    connect(&r, SIGNAL(replaceAllSignal(QString,QString)), this, SLOT(ReplaceAll(QString,QString)));
}


void MainWindow::refresh()
{
    if ( sentence.file_name.isEmpty() )
    {
        this->setWindowTitle( "Untitled - MiniWord" );
    } else {
        QFileInfo file_info = QFileInfo( sentence.file_name );
        this->setWindowTitle( file_info.fileName() + " - MiniWord" );
    }
    cnt_chinese();
    Row *temp = sentence.first_row;
    qsentence = "";
    while ( temp )
    {
        qsentence.append( temp->row_text );
        qsentence.append( '\n' );
        temp = temp->Next_Row;
    }
    //
    //当输入字超出当前画面大小时，使光标跟随
    //
//    while(12 * (sentence.cursor.col - sentence.cursor.chi_cnt * 1.08) - 11 > ui->scrollArea->horizontalScrollBar()->value() + ui->scrollArea->width()-ui->scrollArea->verticalScrollBar()->width())
//        ui->scrollArea->horizontalScrollBar()->setValue(ui->scrollArea->horizontalScrollBar()->value()+12);
//    while(26 * sentence.cursor.row > ui->scrollArea->verticalScrollBar()->value() + ui->scrollArea->height() - ui->scrollArea->horizontalScrollBar()->height())
//        ui->scrollArea->verticalScrollBar()->setValue(ui->scrollArea->verticalScrollBar()->value()+26);
    qDebug() << "窗口值" << ui->scrollArea->width()<<ui->scrollArea->height();
    ui->label->setText( qsentence );
    ui->label->adjustSize();
    ui->label_2->setGeometry( 12 * (sentence.cursor.col - sentence.cursor.chi_cnt * 1.08) - 11, 26 * sentence.cursor.row,
                  30, 30 );
    ui->scrollAreaWidgetContents_2->setMinimumSize( ui->label->width(), ui->label->height() );
    this->setFocus();

    qDebug() << "\n当前行 - cur_len:" << sentence.cursor.hang->cur_len << " max_len:" << sentence.cursor.hang->max_len;
    qDebug()	<< "光标 - hang:" << sentence.cursor.hang << "col:" << sentence.cursor.col << " row:" << sentence.cursor.row
            << " chi_cnt:" << sentence.cursor.chi_cnt;
    qDebug() << "修改 - Modified:" << sentence.isModified;
}


void MainWindow::cnt_chinese()
{
    qDebug() << "运行了cnt_Chinese";
    int cnt = 0;
    for ( int i = 0; i < sentence.cursor.col; i++ )
        if ( sentence.cursor.hang->row_text[i] < 0 )
            cnt++;
    cnt			/= 3;
    sentence.cursor.chi_cnt = cnt;
}


void MainWindow::Open_File()
{
    /* 如果当前有没保存的先保存 */
    if ( sentence.isModified )
    {
        const QMessageBox::StandardButton ret
            = QMessageBox::warning( this, tr( "Application" ),
                        tr( "当前文档未保存，是否保存?" ),
                        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel );
        switch ( ret )
        {
        case QMessageBox::Save:
            Save_All();
            break;
        case QMessageBox::Discard:
            sentence.clear_all();
            break;
        case QMessageBox::Cancel:
            return;
        default:
            return;
        }
    }
    /* 打开新的文档 */
    QString tmp_file_name = QFileDialog::getOpenFileName( this );
    /*选择的文件不为空 */
    if ( !tmp_file_name.isEmpty() )
    {
        /* 检查文件后缀是否为 txt 。 */
        QFileInfo	file_info	= QFileInfo( tmp_file_name );
        QString		file_suffix	= file_info.suffix();
        if ( file_suffix == "txt" )
        {
            qDebug() << tmp_file_name << endl << file_suffix;

            /* 载入文件 */
            string str = tmp_file_name.toStdString();
            this->sentence.read_file( (char *) str.c_str() );
            sentence.file_name = tmp_file_name;

            /* 刷新 */
            refresh();
        }
    } else {
        /* 未选中文件 */
        qDebug() << "未选中文件";
    }
}


/* 把内容保存到文件中 */
void MainWindow::Save_All()
{
    if ( sentence.file_name.isEmpty() )
    {
        sentence.file_name = QFileDialog::getSaveFileName( this, tr( "选择保存位置" ), "", tr( "*.txt" ) );   /*选择路径 */
    }

    string str = sentence.file_name.toStdString();
    if ( !(this->sentence.save_file( (char *) str.c_str() ) ) )                                             /* 保存文档 */
    {
        QMessageBox::information( this,
                      tr( "Failed to save the document" ),
                      tr( "Failed to save the document!" ) );
        return;
    }
    refresh();
}


void MainWindow::Save_As() /* MODIFIED */
{
    sentence.file_name = "";
    Save_All();
    refresh();
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




/* 查找文本 函数 */
void MainWindow::Find_Text()
{
    found = false;
    notExist = false;
    qDebug() << "\n打开查找窗口\n" << endl;
//    f.exec();
    f.show();

}

void MainWindow::Find(QString text){
    qDebug() << "\n查找功能可用" << endl;
    qDebug() << "text = " << text << endl;
    Row *cur_row = sentence.cursor.hang; // 定位查找起始行
    int col = sentence.cursor.col; //查找起始位置
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
            cur_row = cur_row->Next_Row;
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
            sentence.cursor.hang = cur_row;
            Q_ASSERT(cur_row != NULL);
            sentence.cursor.row += row_to_plus;
            sentence.cursor.col = pos; //移动光标到第一个查找成功的位
            col = sentence.cursor.col; // 更新col
            ui->label_2->setGeometry(12*sentence.cursor.col-11,26*sentence.cursor.row,30,30); //绘制光标

            start_col = sentence.cursor.col + 1;
            start_row = sentence.cursor.row;
            end_col = start_col + text.length();
            end_row = start_row;

            add_blockshadow();
            isSelectedblock = true;
        }
    }

}

// 打开替换窗口
void MainWindow:: Replace_Window(){
    isFirstReplace = true;
    r.exec();
}

// 替换
void MainWindow:: Replace(QString target, QString value){
    // target: 查找文本
    // value: 应替换的文本

    // 如果是第一次替换，并且当前不是被被Find Next查找选中的状态
    if(isFirstReplace && !isSelectedblock){
        found = false;
        notExist = false;
        Find(target);//执行查找操作
        isFirstReplace = false;

        qDebug() << "第一次替换"  << endl;
    }
    else{
        //先删除target
        qDebug() << "不是第一次替换" << endl;
        Block_Delete();
        isSelectedblock = false;
        found = false; // 替换后当前为未查找成功状态

        // 加上value
        std::string words = value.toStdString();
        const char* s = words.c_str();
        sentence.edit(s);

        refresh();

        //查找下一个
        Find(target);


    }

}

void MainWindow::ReplaceAll(QString target, QString value){
   // qDebug() << "replace all " << endl;
    while(!notExist){
        Replace(target, value);
    }

    notExist = false;
}


/* 显示 Help 函数 */
void MainWindow::Show_Help()
{
    QMessageBox::information( this, tr( "帮助" ), tr( "编辑文档。" ) );
}


/* 显示 About 函数 */
void MainWindow::Show_About()
{
    QMessageBox::about( this, tr( "关于 MiniWord" ), tr( "欢迎您的使用!\n"
                               "此程序 MiniWord 由 Daoxu Sheng, Weiran Huang\n 以及 Zengrui Wang 共同开发完成。" ) );
}


/* 主窗口关闭 函数 */
void MainWindow::closeEvent( QCloseEvent *event )
{
    if ( this->sentence.isModified )
    {
        const QMessageBox::StandardButton ret
            = QMessageBox::warning( this, tr( "Application" ),
                        tr( "当前文档未保存，是否保存?" ),
                        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel );
        switch ( ret )
        {
        case QMessageBox::Save:
            Save_All();
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


void MainWindow::keyPressEvent( QKeyEvent *ev )                                                                 /*按键事件 */
{
    this->setFocus();
    char	n1[10];
    string	n;
    n = ev->key();
    if ( ev->key() == Qt::Key_CapsLock )                                                                    /* 大小写切换 */
    {
        if ( caps == true )
            caps = false;
        else
            caps = true;
    } else if ( ev->key() == Qt::Key_Shift )
        ;                                                                                               /* 单独按shift */
    else if ( (ev->key() <= Qt::Key_Z && ev->key() >= Qt::Key_A) && ev->modifiers() == Qt::ShiftModifier )  /*按住shift的大小写切换 */
    {
        if ( caps == true )
        {
            transform( n.begin(), n.end(), n.begin(), ::tolower );
            strcpy( n1, n.c_str() );
            sentence.edit( n1 );
        } else {
            transform( n.begin(), n.end(), n.begin(), ::toupper );
            strcpy( n1, n.c_str() );
            sentence.edit( n1 );
        }
    } else if ( ev->modifiers() == Qt::ShiftModifier && ev->key() == Qt::CTRL )
    {
/*        qDebug()<<"Discard"; */
    } else if ( ev->key() == Qt::SHIFT && ev->modifiers() == Qt::ControlModifier )
    {
/*        qDebug()<<"Discard"; */
    } else if ( ev->modifiers() == Qt::ShiftModifier && ev->key() == Qt::ALT )
    {
/*        qDebug()<<"Discard"; */
    } else if ( ev->key() == Qt::SHIFT && ev->modifiers() == Qt::AltModifier )
    {
/*        qDebug()<<"Discard"; */
    } else if ( ev->modifiers() == Qt::ControlModifier )
    {
/*        qDebug()<<"Discard"; */
    } else if ( ev->key() == Qt::CTRL )
    {
/*        qDebug()<<"Discard"; */
    } else if ( ev->key() == Qt::ALT )
    {
/*        qDebug()<<"Discard"; */
    } else if ( ev->modifiers() == Qt::AltModifier )
    {
/*        qDebug()<<"Discard"; */
    } else if ( n == "\u0004" ) /* 输入为回车 */
    {
        if(isSelectedblock)
            Block_Delete();
        int	i, j = 0;
        int	col	= sentence.cursor.col;
        Row	*hang	= sentence.cursor.hang;
        sentence.add_row( sentence.cursor.hang );
        sentence.cursor.hang = sentence.cursor.hang->Next_Row;
        sentence.cursor.cur_height++;
        for ( i = col; i < hang->cur_len; i++ )
        {
            sentence.cursor.hang->row_text[j++]	= hang->row_text[i];
            hang->row_text[i]			= '\0';
        }
        hang->cur_len			= col;
        sentence.cursor.hang->cur_len	= j;
        sentence.cursor.col		= 0;
        sentence.cursor.row++;
        sentence.isModified = true;
    } else if ( n == "\u0001" ) /* 输入为TAB */
    {
        /* DEBUG 无法检测到TAB */
        if(isSelectedblock)
            Block_Delete();
        qDebug() << "TABBBBBBBBBBBBBBBB";
        n = "    ";
        strcpy( n1, n.c_str() );
        sentence.edit( n1 );
        /* sentence.add_row(sentence.cur_row); */
    } else if ( n == "\u0003" )                     /* 输入为backspace */
    {
        if ( isSelectedblock )
        {
            Block_Delete();
        } else if ( sentence.cursor.col > 0 )   /* 当指针不在行首的时候(之前有字符可以删除) */
        {
            /* 判断中文字符 */
            int mov = 1;
            if ( sentence.cursor.hang->row_text[sentence.cursor.col - 1] < 0 )
                mov = 3;
            for ( int i = sentence.cursor.col - mov; i <= sentence.cursor.hang->cur_len - mov; i++ )
            {
                sentence.cursor.hang->row_text[i]	= sentence.cursor.hang->row_text[i + mov];
                sentence.cursor.hang->row_text[i + mov] = '\0';
            }
            sentence.cursor.col		-= mov;
            sentence.cursor.hang->cur_len	-= mov;
            sentence.isModified		= true;
        } else if ( sentence.cursor.col == 0 && sentence.cursor.hang != sentence.first_row ) /* 当在行首，并且不是第一行(第一行行首删除就不用操作了) */
        {
            Row *temp = sentence.cursor.hang;
            sentence.cursor.hang = temp->Prev_Row;
            if ( temp->Next_Row )
            {
                temp->Next_Row->Prev_Row = sentence.cursor.hang;
            }
            sentence.cursor.hang->Next_Row	= temp->Next_Row;
            sentence.cursor.col		= sentence.cursor.hang->cur_len;
            /* 需要把之后行的加到后面 */
            int	i	= 0;
            int	col	= sentence.cursor.col;
            while ( i < temp->cur_len )
            {
                sentence.cursor.hang->row_text[col++] = temp->row_text[i++];
                sentence.cursor.hang->cur_len++;
            }
            sentence.cursor.row--;
            sentence.cursor.cur_height--;
            delete temp;
            sentence.isModified = true;
        }
    } else if ( n == "\u0007" )                                             /* 输入为delete */
    {
        if(isSelectedblock)
            Block_Delete();
        else if ( sentence.cursor.col < sentence.cursor.hang->cur_len )      /* 当指针不在行尾的时候(之前有字符可以删除) */
        {
            /* 判断中文字符 */
            int mov = 1;
            if ( sentence.cursor.hang->row_text[sentence.cursor.col] < 0 )
                mov = 3;

            for ( int i = sentence.cursor.col; i <= sentence.cursor.hang->cur_len - mov; i++ )
            {
                sentence.cursor.hang->row_text[i]	= sentence.cursor.hang->row_text[i + mov];
                sentence.cursor.hang->row_text[i + mov] = '\0';
            }
            sentence.cursor.hang->cur_len	-= mov;
            sentence.isModified		= true;
        } else if ( sentence.cursor.col == sentence.cursor.hang->cur_len &&
                sentence.cursor.hang->Next_Row ) /* 当在行尾，并且不是最后一行一行(最后一行行尾删除就不用操作了) */
        {
            Row *temp = sentence.cursor.hang->Next_Row;
            if ( temp->Next_Row )
            {
                sentence.cursor.hang->Next_Row = temp->Next_Row;
            } else {
                sentence.cursor.hang->Next_Row = NULL;
            }
            /* 需要把之后行的加到后面 */
            int	i	= 0;
            int	col	= sentence.cursor.col;
            while ( i < temp->cur_len )
            {
                sentence.cursor.hang->row_text[col++] = temp->row_text[i++];
                sentence.cursor.hang->cur_len++;
            }
            delete temp;
            sentence.cursor.cur_height--;
            sentence.isModified = true;
        }
    } else if ( n == "\u0012" )     /* 左 */
    {
        if(isSelectedblock)
            Block_Delete();
        sentence.cursor_left();
    } else if ( n == "\u0013" )     /*上 */
    {
        if(isSelectedblock)
            Block_Delete();
        sentence.cursor_up();
    } else if ( n == "\u0014" )     /* 右 */
    {
        if(isSelectedblock)
            Block_Delete();
        sentence.cursor_right();
    } else if ( n == "\u0015" )     /*下 */
    {
        if(isSelectedblock)
            Block_Delete();
        sentence.cursor_down();
    } else{ /* 英文输入 */
        if(isSelectedblock)
            Block_Delete();
        if ( caps == false )
            transform( n.begin(), n.end(), n.begin(), ::tolower );
        strcpy( n1, n.c_str() );
        sentence.edit( n1 );
        /*
         * qDebug() << n1 << endl;
         * MODIFIED
         * sentence.cursor.col += strlen(n1);
         */
    }

    refresh();
}


void MainWindow::inputMethodEvent( QInputMethodEvent *a )
{
    refresh();
    qDebug() << "当前输入为" << a->commitString().toUtf8().data();
    qDebug() << sentence.cursor.chi_cnt;
    qDebug() << sentence.cursor.col;
    sentence.edit( a->commitString().toUtf8().data() );
    Row *temp = sentence.first_row;
    qsentence = "";
    while ( temp )
    {
        qsentence.append( temp->row_text );
        qsentence.append( '\n' ); /* MODIFIED */
        temp = temp->Next_Row;
    }
    ui->label->setText( qsentence );
    ui->label->adjustSize();
    ui->label_2->setGeometry( 12 * (sentence.cursor.col - sentence.cursor.chi_cnt * 1.08) - 11, 26 * sentence.cursor.row,
                  30, 30 );
    ui->scrollAreaWidgetContents_2->setMinimumSize( ui->label->width(), ui->label->height() );
    this->setFocus();
    refresh();
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
            start_col	= sentence.cursor.col + 1;
            start_row	= sentence.cursor.row;
        }
        end_x	= event->pos().x();
        end_y	= event->pos().y();

        pos_to_coordinate( end_x, end_y, false );
        end_col = sentence.cursor.col + 1;
        end_row = sentence.cursor.row;

        add_blockshadow();
        refresh();
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
        refresh();
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
    for ( int j = 0; j < sentence.cursor.cur_height; j++ )
    {
        if ( y + vbar_height > 26 * j + toolbar_height && y + vbar_height < 26 * (j + 1) + toolbar_height )
        {
            if ( sentence.cursor.row < j )
                for ( int temp = sentence.cursor.row; temp < j; temp++ )
                    sentence.cursor.hang = sentence.cursor.hang->Next_Row;
            else if ( sentence.cursor.row >= j )
                for ( int temp = 0; temp < sentence.cursor.row - j; temp++ )
                    sentence.cursor.hang = sentence.cursor.hang->Prev_Row;
            sentence.cursor.row = j;

            for ( int i = 1; i <= sentence.cursor.hang->cur_len; i++ )
            {
                if ( x + hbar_height > (12 * i - 11) && x + hbar_height <= (12 * (i + 1) - 11) )
                {
                    sentence.cursor.col = i - 1;
                    if ( flag )
                        ui->label_2->setGeometry( 12 * (sentence.cursor.col - sentence.cursor.chi_cnt * 1.08) - 11,
                                      26 * sentence.cursor.row, 30, 30 );
                    break;
                } else if ( x + hbar_height >= 12 * (sentence.cursor.hang->cur_len + 1) - 11 )
                {
                    sentence.cursor.col = sentence.cursor.hang->cur_len;
                    if ( flag )
                        ui->label_2->setGeometry( 12 * (sentence.cursor.col - sentence.cursor.chi_cnt * 1.08) - 11,
                                      26 * sentence.cursor.row, 30, 30 );
                    break;
                }
            }
            break;
        }
    }
    if ( y + vbar_height >= 26 * sentence.cursor.cur_height + toolbar_height )
    {
        while ( sentence.cursor.hang->Next_Row )
            sentence.cursor.hang = sentence.cursor.hang->Next_Row;
        sentence.cursor.row = sentence.cursor.cur_height;
        for ( int i = 1; i <= sentence.cursor.hang->cur_len; i++ )
        {
            if ( x + hbar_height > (12 * i - 11) && x + hbar_height <= (12 * (i + 1) - 11) )
            {
                sentence.cursor.col = i - 1;
                if ( flag )
                    ui->label_2->setGeometry( 12 * (sentence.cursor.col - sentence.cursor.chi_cnt * 1.08) - 11,
                                  26 * sentence.cursor.row, 30, 30 );
                break;
            } else if ( x + hbar_height >= 12 * (sentence.cursor.hang->cur_len + 1) - 11 )
            {
                sentence.cursor.col = sentence.cursor.hang->cur_len;
                if ( flag )
                    ui->label_2->setGeometry( 12 * (sentence.cursor.col - sentence.cursor.chi_cnt * 1.08) - 11,
                                  26 * sentence.cursor.row, 30, 30 );
                break;
            }
        }
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}


