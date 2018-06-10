#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow( QWidget *parent ) :
    QMainWindow( parent ),
    ui( new Ui::MainWindow )
{
    window_init();
    slot_init();
    cursor_blink();
    refresh_screen();
}

void MainWindow::cursor_blink()
{
    QTimer *mtimer = new QTimer(this);
    mtimer->setInterval(500);
    connect(mtimer, SIGNAL(timeout()), this, SLOT(blink()));
    mtimer->start(500);
}

void MainWindow::blink()
{
    QTime time = QTime::currentTime();
    if(!isSelectedblock && time.msecsSinceStartOfDay()%1000 > 500)
        ui->label_cursor->setGeometry( word_width * (Doc->cursor.col - Doc->cursor.chi_cnt * 1.08) - 15, word_height * Doc->cursor.row_cnt, 30, 30 );
    else
        ui->label_cursor->setGeometry( -50,-50,30,30);
}

void MainWindow::window_init()//窗口初始化
{
    this->Doc = new Document;
    ui->setupUi( this );
    this->setWindowTitle( "MiniWord Editor" );//标题
    this->installEventFilter(this);

    /*文字及光标部分初始化设置*/
    ui->label_text->setStyleSheet( "qproperty-alignment: 'AlignTop | AlignLeft'; " );
    QFont font = ui->label_text->font();
    int	pointsize = font.pointSize() * 18 / 7;
    font.setPixelSize( pointsize );
    ui->label_text->setFont( font );
    ui->label_text->setText( "" );
    ui->label_cursor->setAttribute(Qt::WA_ShowWithoutActivating,true);//透明
    ui->label_cursor->setGeometry( word_width * Doc->cursor.col - 11, word_height * Doc->cursor.row_cnt, 30, 30 );
    QTextCodec *codec = QTextCodec::codecForName( "UTF-8" );
    QTextCodec::setCodecForLocale( codec );

    /*滚动窗口初始化*/
    ui->scrollArea->setWidgetResizable( true );
    ui->scrollArea->setStyleSheet( "background:white" );

    /*鼠标，键盘，中文等初始化*/
    this->setMouseTracking( true );
    this->setAttribute( Qt::WA_KeyCompression );
    this->setAttribute( Qt::WA_InputMethodEnabled );

    /* 一些初始化时应该默认关闭的操作 */
//    ui->actionUndo_Z->setEnabled( false );
//    ui->actionRedo_Y->setEnabled( false );

    /* 状态栏 */
    QString sta_str;
    sta_str.append("Row: ");
    sta_str.append(QString::number(Doc->cursor.row_cnt, 10));
    sta_str.append(", Col: ");
    sta_str.append(QString::number(Doc->cursor.col, 10));
    status = new QLabel(this);
    status->setText(sta_str);
    QFont sta_font;
    sta_font.setPointSize(8);
    status->setFont(sta_font);
    QPalette pe;
    pe.setColor(QPalette::Window, Qt::lightGray);
    status->setPalette(pe);
    statusBar()->addPermanentWidget(status);

    buffer_cache();

    /*设置焦点*/
    this->setFocus();
}

void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *menu = new QMenu(this);
    menu->addAction(ui->actionSelect_All_A);
    menu->addAction(ui->actionCopy_C);
    menu->addAction(ui->actionCut_X);
    menu->addAction(ui->actionPaste_P);
    menu->addAction(ui->actionUndo_Z);
    menu->move(cursor().pos());
    menu->show();
}

void MainWindow::slot_init()
{
    // NEW FILE
    ui->actionNew_N->setShortcut(QKeySequence::New);
    connect( ui->actionNew_N, SIGNAL(triggered(bool)), this, SLOT(new_file()));

    // OPEN FILE
    ui->actionOpen_O->setShortcut( QKeySequence::Open );
    connect( ui->actionOpen_O, SIGNAL( triggered( bool ) ), this, SLOT( open_file() ) );

    // SAVE ACTIONS
    ui->actionSave_S->setShortcut( QKeySequence::Save );
    connect( ui->actionSave_S, SIGNAL( triggered( bool ) ), this, SLOT( save_all() ) );
    connect( ui->actionSave_as, SIGNAL( triggered( bool ) ), this, SLOT( save_as() ) );

    // BLOCK ACTIONS
    ui->actionCut_X->setShortcut(QKeySequence::Cut);
    connect( ui->actionCut_X, SIGNAL(triggered(bool)), this, SLOT( block_cut() ));
    ui->actionCopy_C->setShortcut( QKeySequence::Copy );
    connect( ui->actionCopy_C, SIGNAL( triggered( bool ) ), this, SLOT( block_copy() ) );
    ui->actionPaste_P->setShortcut( QKeySequence::Paste );
    connect( ui->actionPaste_P, SIGNAL( triggered( bool ) ), this, SLOT( block_paste() ));

    //SELECT ALL
    ui->actionSelect_All_A->setShortcut(QKeySequence::SelectAll);
    connect( ui->actionSelect_All_A, SIGNAL(triggered(bool)),this,SLOT(block_selectall()));

    //UNDO / REDO
    ui->actionUndo_Z->setShortcut(QKeySequence::Undo);
    ui->actionRedo_Y->setShortcut(QKeySequence::Redo);
    connect( ui->actionUndo_Z, SIGNAL(triggered(bool)), this, SLOT(undo()));
    connect( ui->actionRedo_Y, SIGNAL(triggered(bool)), this, SLOT(redo()));

    //PRINT
    connect( ui->actionPrint_P, SIGNAL(triggered(bool)), this, SLOT(print_pdf()));

    // SHOW ABOUT
    connect( ui->actionMiniword, SIGNAL( triggered( bool ) ), this, SLOT( show_about() ) );

    // FIND & REPLACE
    connect( ui->actionFind_F, SIGNAL( triggered() ), this, SLOT( find_window() ) );
    connect( &f, SIGNAL( FindSignal( QString ) ), this, SLOT( find( QString ) ) );
    connect( ui->actionreplace_R, SIGNAL( triggered() ), this, SLOT( replace_window() ) );
    connect( &r, SIGNAL( findSignal( QString ) ), this, SLOT( find( QString ) ) );
    connect( &r, SIGNAL( replaceSignal( QString, QString ) ), this, SLOT( replace( QString, QString ) ) );
    connect(&r, SIGNAL(replaceAllSignal(QString,QString)), this, SLOT(replace_all(QString,QString)));
}

void MainWindow::refresh_screen()//刷新
{
    /*撤销重做按钮的变化*/
    if(Undo.empty()){
        ui->actionUndo_Z->setEnabled(false);
    }else{
        ui->actionUndo_Z->setEnabled(true);
    }
    if(Redo.empty()){
        ui->actionRedo_Y->setEnabled(false);
    }else{
        ui->actionRedo_Y->setEnabled(true);
    }
    /*显示窗口文件名*/
    if ( Doc->file_name.isEmpty() )
        this->setWindowTitle( "Untitled - MiniWord" );
    else {
        QFileInfo file_info = QFileInfo( Doc->file_name );
        this->setWindowTitle( file_info.fileName() + " - MiniWord" );
    }
    /*显示内容*/
    Row *temp = Doc->first_row;
    QString output_string = "";
    while ( temp )
    {
        output_string.append( temp->row_text );
        output_string.append( '\n' );
        temp = temp->next_row;
    }
    /*计算当前光标之前的英文和中文字符个数*/
    chinese_count();
    english_count();

    /*显示文字和光标*/
    ui->label_text->setText( output_string );
    ui->label_text->adjustSize();
    if(!isSelectedblock)
        ui->label_cursor->setGeometry( word_width * (Doc->cursor.col - Doc->cursor.chi_cnt * 1.08) - 15, word_height * Doc->cursor.row_cnt, 30, 30 );
    else
        ui->label_cursor->setGeometry( -50,-50,30,30);

    ui->scrollAreaWidgetContents_2->setMinimumSize( ui->label_text->width()+word_width, ui->label_text->height() );
    scrollbarfollow();

    //状态栏
    QString sta_str;
    sta_str.append("Row: ");
    sta_str.append(QString::number(Doc->cursor.row_cnt, 10));
    sta_str.append(" Col: ");
    sta_str.append(QString::number(Doc->cursor.col - Doc->cursor.chi_cnt * 2, 10));
    sta_str.append(", ");
    sta_str.append(QString::number(output_string.size()-this->Doc->cursor.cur_height-1));
    sta_str.append(" Words");
    status->setText(sta_str);

    //自动保存
    auto_save();

    this->setFocus();

    qDebug() << "\n当前行 - cur_len:" << Doc->cursor.row_ptr->cur_len << " max_len:" << Doc->cursor.row_ptr->max_len;
    qDebug() << "光标 - hang:" << Doc->cursor.row_ptr << "col:" << Doc->cursor.col <<
                " row:" << Doc->cursor.row_cnt << " chi_cnt:" << Doc->cursor.chi_cnt <<
                "cur_height" << Doc->cursor.cur_height;
    qDebug() << "修改 - Modified:" << Doc->isModified;
    qDebug() << "块选 - isSelectedblock:" << this->isSelectedblock << "start_col:" << start_col << "end_col:" << end_col
             << "start_row:" <<start_row<<"end_row:"<<end_row
             << "start_x:" << start_x << "start_y:" << start_y;
    qDebug() << "滚动条 - ver:" << ui->scrollArea->verticalScrollBar()->value() << "hor:" << ui->scrollArea->horizontalScrollBar()->value();
}

void MainWindow::scrollbarfollow()//当输入字超出当前画面大小时，使光标跟随
{
    if(Doc->cursor.col > (ui->scrollArea->horizontalScrollBar()->value()+ui->scrollArea->width() - ui->scrollArea->verticalScrollBar()->width())/word_width - 1)//右超
        ui->scrollArea->horizontalScrollBar()->setValue(word_width*Doc->cursor.col);
    else if(Doc->cursor.col < ui->scrollArea->horizontalScrollBar()->value()/word_width)//左超
        ui->scrollArea->horizontalScrollBar()->setValue(word_width*Doc->cursor.col);
    else if(Doc->cursor.row_cnt > (ui->scrollArea->verticalScrollBar()->value()+ui->scrollArea->height() - ui->scrollArea->verticalScrollBar()->width() - ui->statusBar->height())/word_height+1)//下超
        ui->scrollArea->verticalScrollBar()->setValue(word_height*Doc->cursor.row_cnt);
    else if(Doc->cursor.row_cnt < ui->scrollArea->verticalScrollBar()->value()/word_height)//上超
        ui->scrollArea->verticalScrollBar()->setValue(word_height*Doc->cursor.row_cnt);
}

void MainWindow::auto_save()
{
    QTime current_time =QTime::currentTime();
    int minute = current_time.minute();//当前的分钟
    if((minute%10)==0){//每隔十分钟
        if(Doc->file_name.isEmpty()){
            string str = "./auto_backup.txt";
            Doc->save_file((char*)str.c_str());
        }else{
            //有当前路径
            string str = Doc->file_name.toStdString();
            Doc->save_file((char*)str.c_str());
            Doc->isModified = true;
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
