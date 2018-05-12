#include "mainwindow.h"
#include "document.h"
#include <windows.h>
#include <qDebug>
//==========================================================
// = 基本完成项：
//     -- 图标的加入
//     -- 菜单栏的设置(具体功能待完成)
//     -- 窗体程序框架
//     --
//     --
// = Todo：
//     -- 内部数据的实现√
//     -- 如何在主界面显示Document对象的文本√
//     -- 光标如何显示
//     -- 粘贴，插入，删除操作
//     -- 文字块编辑
//==========================================================

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->resize(QSize(1024,768));

    setWindowIcon(QIcon(":/pics/icons/logo.ico")); // add the logo of the application
    cenWid = new QWidget;
    this->setCentralWidget(cenWid);

    cenWid->setFocusPolicy(Qt::StrongFocus);
    cenWid->grabKeyboard();
    QVBoxLayout *pVlayout = new QVBoxLayout;

    s = new QScrollArea;
    s->setGeometry(0, 0, this->width(), this->height());
    s->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    s->setStyleSheet("background-color:white;font-size:50px");
    s->setStyleSheet("QLabel{background-image: url(:/res/on.png);"
                   "background-position:center left;"
                   "background-repeat: no-repeat}");
    QTime time = QTime::currentTime();
    pLabel=new QLabel;
    if(time.msec()%1000 < 700)
    {
        QPixmap pixmap(":/image/cursor.png");
        pLabel->setPixmap(pixmap);
        pLabel->setScaledContents(true);
    }
    pLabel->setStyleSheet("qproperty-alignment: 'AlignTop | AlignLeft'; ");
    pLabel->setText(qsentence);

    s->setWidget(pLabel);
    CreateMenu();
    initToolBar(); // todo
    ColorSelect();
//    update();
    pVlayout->addWidget(s);
    pVlayout->setMargin(0);
    cenWid->setLayout(pVlayout);


//    QTimer *timer=new QTimer(this);
//    connect(timer,SIGNAL(timeout()),this,SLOT(print_cursor()));
//    timer->start(1000);

//    QTimer *timer=new QTimer(this);
//    connect(timer,SIGNAL(timeout()),this,SLOT(blink()));
//    timer->start(10);
}

void MainWindow::CreateMenu()
{
    // File 菜单栏

    /* New */
    File_New = new QAction(tr("&New"), this);
    File_New->setShortcut(QKeySequence::New);
    File_New->setToolTip("Create a new file");
    File_New->setStatusTip("Create file");
    File_New->setIcon(QIcon(":/pics/icons/filenew.png"));
    File_New->showStatusText(this->statusBar());


    /* Open */
    File_Open=new QAction(tr("O&pen"), this);
    File_Open->setShortcut(QKeySequence::Open);
    File_Open->setToolTip("Open an existing file");
    File_Open->setStatusTip("Open file");
    File_Open->setIcon(QIcon(":pics/icons/fileopen.png"));

    /* Save */
    File_Save=new QAction(tr("&Save"), this);
    File_Save->setToolTip("Save current file");
    File_Save->setStatusTip("Save file");
    File_Save->setIcon(QIcon(":/pics/icons/filesave.png"));
    File_Save->setShortcut(QKeySequence::Save);


    /* Save as */
    File_Save_as = new QAction(tr("Save &As"), this);
    File_Save_as->setIcon(QIcon(":/pics/icons/filesaveas.png"));

    /* Print */
    File_Print = new QAction(tr("Print"),this);
    File_Print->setToolTip("Print the page");
    File_Print->setStatusTip("Print File");
    File_Print->setShortcut(QKeySequence("Ctrl+P"));
    File_Print->setIcon(QIcon(":/pics/icons/print.png"));

    /* Exit */
    File_Exit = new QAction(tr("E&xit"),this);
    File_Exit->setToolTip("Exit the Application");
    File_Exit->setStatusTip("Exit Miniword");
    File_Exit->setIcon(QIcon(":/pics/icons/fileexit.png"));

   /* add all these into the menu */
    File = menuBar()->addMenu((tr("&File")));
    File->addAction(File_New);
    File->addAction(File_Open);
    File->addAction(File_Save);
    File->addAction(File_Save_as);
    File->addSeparator();
    File->addAction(File_Print);
    File->addAction(File_Exit);


    // Edit 菜单栏


    /* find */
    Edit_Search = new QAction(tr("&Find"), this);
    Edit_Search->setToolTip("Find text and/or replace");
    Edit_Search->setStatusTip("Find text");
    Edit_Search->setIcon(QIcon(":/pics/icons/editfind.png"));
    Edit_Search->setShortcut(QKeySequence::Find);

    /* copy */
    Edit_Copy = new QAction(tr("&Copy"), this);
    Edit_Copy->setToolTip("Copy");
    Edit_Copy->setStatusTip("Copy Text");
    Edit_Copy->setShortcut(QKeySequence::Copy);
    Edit_Copy->setIcon(QIcon(":/pics/icons/editcopy.png"));

    /* paste */
    Edit_Paste = new QAction(tr("P&aste"), this);
    Edit_Paste->setToolTip("Paste");
    Edit_Paste->setStatusTip("Paste Text");
    Edit_Paste->setShortcut(QKeySequence::Paste);
    Edit_Paste->setIcon(QIcon(":/pics/icons/editpaste.png"));

    /* cut */
    Edit_Cut = new QAction(tr("C&ut"), this);
    Edit_Cut->setToolTip("Cut");
    Edit_Cut->setStatusTip("Cut Text");
    Edit_Cut->setShortcut(QKeySequence::Cut);
    Edit_Cut->setIcon(QIcon(":/pics/icons/editcut.png"));

    /* undo */
    Edit_Undo = new QAction(tr("Undo"),this);
    Edit_Undo->setIcon(QIcon(":/pics/icons/editundo.png"));
    Edit_Undo->setToolTip("Undo");
    Edit_Undo->setStatusTip("Undo Operation");
    Edit_Undo->setShortcut(QKeySequence::Undo);
    Edit_Undo->setEnabled(false); // initially no text in the editor

    /* Redo */
    Edit_Redo =new QAction(tr("Redo"),this);
    Edit_Redo->setIcon(QIcon(":/pics/icons/editredo.png"));
    Edit_Redo->setToolTip("Redo");
    Edit_Redo->setStatusTip("Redo Operation");
    Edit_Redo->setShortcut(QKeySequence::Redo);
    Edit_Redo->setEnabled(false); // initiallly no text in the editor

    /* Select All */
    Edit_Select_all = new QAction(tr("Select All"),this);
    Edit_Select_all->setShortcut(QKeySequence::SelectAll);
    Edit_Select_all->setIcon(QIcon(":/pics/icons/editselect.png"));

    Edit = menuBar()->addMenu(tr("&Edit"));
    Edit->addAction(Edit_Cut);
    Edit->addAction(Edit_Copy);
    Edit->addAction(Edit_Paste);
    Edit->addAction(Edit_Undo);
    Edit->addAction(Edit_Redo);
    Edit->addSeparator();
    Edit->addAction(Edit_Search);
    Edit->addAction(Edit_Select_all);




    // Help 菜单栏
    Help = menuBar()->addMenu(tr("Help"));
    /* help */
    Help_Help = new QAction(tr("Help docs"),this);
    Help_Help->setIcon(QIcon(":/pics/icons/helpabout.png"));

    Help->addAction(Help_Help);


    // About 菜单栏
    About = menuBar()->addMenu(tr("About"));

    /* about */
    About_About=new QAction(tr("Miniword"),this);
    About_About->setIcon(QIcon(":/pics/icons/logo.ico"));

    About->addAction(About_About);

    //动作 connect
    connect(File_Open,SIGNAL(triggered(bool)),this,SLOT(Open_File()));
    /*
    connect(this->,SIGNAL(triggered(bool)),this,SLOT());*/
    connect(this->Edit_Search,SIGNAL(triggered(bool)),this,SLOT(Find_Text()));
    connect(this->Help_Help,SIGNAL(triggered(bool)),this,SLOT(Show_Help()));
    connect(About_About,SIGNAL(triggered(bool)),this,SLOT(Show_About()));
}

void MainWindow::initToolBar()
{

    toolBar=addToolBar(tr("Main"));
    toolBar->addAction(File_New);
    toolBar->addAction(File_Open);
    toolBar->addAction(File_Save);
    toolBar->addSeparator();
    toolBar->addAction(Edit_Cut);
    toolBar->addAction(Edit_Copy);
    toolBar->addAction(Edit_Paste);
    toolBar->addAction(Edit_Search);
    toolBar->addAction(Edit_Undo);
    toolBar->addAction(Edit_Redo);
    toolBar->addSeparator();
    toolBar->addAction(About_About);
    toolBar->addAction(File_Print);
    toolBar->setVisible(true);
    //connect(toolEditAction,SIGNAL(toggled(bool)),toolBar,SLOT(setVisible(bool)));
}

// 打开文件选项
void MainWindow::Open_File()
{
    QString file_name = QFileDialog::getOpenFileName(this);
    //选择的文件不为空
    if(!file_name.isEmpty()){
        //检查文件后缀是否为 txt 。
        QFileInfo file_info = QFileInfo(file_name);
        QString file_suffix = file_info.suffix();
        if(file_suffix=="txt"){
            qDebug()<<file_name<<endl<<file_suffix;

            //载入文件
            string str = file_name.toStdString();
            //datta.read_file(char*)(str.c_str());
            this->sentence.read_file((char*)str.c_str());
            Row *temp = sentence.first_row;
            qsentence = "";
            while(temp)
            {
                qsentence.append(temp->row_text);
                temp = temp->Next_Row;
            }
        }
    }
    else{
        //未选中文件
        qDebug()<<"未选中文件";
    }
}

// 查找文本 函数
void MainWindow::Find_Text()
{
    //Todo
}

// 显示 Help 函数
void MainWindow::Show_Help()
{
    QMessageBox::information(this,tr("帮助"),tr("编辑文档。"));
}

// 显示 About 函数
void MainWindow::Show_About()
{
    QMessageBox::about(this,tr("关于 MiniWord"),tr("欢迎您的使用!\n"
                                                 "此程序 MiniWord 由 Daoxu Sheng, Weiran Huang\n 以及 Zengrui Wang 共同开发完成。"));
}

// 主窗口关闭 函数
void MainWindow::closeEvent(QCloseEvent)
{

    //Todo

    const QMessageBox::StandardButton ret
            = QMessageBox::warning(this, tr("Application"),
                                   tr("还差文件保存判断!"),
                                   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    switch (ret) {
    case QMessageBox::Save:
        qDebug()<<"Save";   break;
    case QMessageBox::Discard:
        qDebug()<<"Discard";    break;
    case QMessageBox::Cancel:
        qDebug()<<"Cancel"; break;
    default:
        break;
    }
}

void MainWindow::ColorSelect()//颜色控制
{
    pal.setColor(QPalette::Background,Qt::white);
    setStyleSheet(""
                  "QMenuBar{"
                  "background-color:#f0f0f0}"
                  "");
    setAutoFillBackground(true);
    setPalette(pal);
}

void MainWindow::keyReleaseEvent(QKeyEvent *ev)//按键事件
{
    char n1[10];
    string n;
    n = ev->key();
    if(ev->key() == Qt::Key_CapsLock)//大小写切换
    {
        if(caps == true)
            caps = false;
        else
            caps = true;
    }
    else if(ev->key() == Qt::Key_Shift);//单独按shift
    else if((ev->key() <= Qt::Key_Z && ev->key() >= Qt::Key_A) && ev->modifiers() == Qt::ShiftModifier)//按住shift的大小写切换
    {
        if(caps == true)
        {
            transform(n.begin(),n.end(),n.begin(),::tolower);
            strcpy(n1,n.c_str());
            sentence.edit(n1);
        }
        else
        {
            transform(n.begin(),n.end(),n.begin(),::toupper);
            strcpy(n1,n.c_str());
            sentence.edit(n1);
        }
    }

    else if(n == "\u0004")//输入为回车
    {
        n = "\n";
        strcpy(n1,n.c_str());
        sentence.edit(n1);
        sentence.add_row(sentence.cursor.hang);
        sentence.cursor.col = 0;
    }
    else if(n == "\u0001")//输入为TAB
    {
        n = "    ";
        strcpy(n1,n.c_str());
        sentence.edit(n1);
        //sentence.add_row(sentence.cur_row);
    }
    else if(n == "\u0003")//输入为退格且当前字符串大小大于0，删掉最后一个字符
    {
        if(sentence.cursor.col > 0)
        {
            sentence.cursor.hang->row_text[--sentence.cursor.hang->cur_len] = '\0';
            sentence.cursor.col--;
        }
        else if(sentence.cursor.col == 0 && sentence.cursor.hang != sentence.first_row)
        {
            Row *temp = sentence.cursor.hang;
            sentence.cursor.hang = temp->Prev_Row;
            if(temp->Next_Row){
                temp->Next_Row->Prev_Row = sentence.cursor.hang;
            }
            sentence.cursor.hang->Next_Row = temp->Next_Row;
            delete temp;
            //现在cursor.hang指向的就是前一行,然后还需要删掉上一行的'\n'符号。
            sentence.cursor.hang->row_text[--sentence.cursor.hang->cur_len] = '\0';
            sentence.cursor.hang = sentence.cursor.hang;
            sentence.cursor.col = sentence.cursor.hang->cur_len;
        }
    }
    else if(n=="\u0012"){//左
        sentence.cursor_left();
    }
    else if(n=="\u0013"){//上
        sentence.cursor_up();
    }
    else if(n=="\u0014"){//右
        sentence.cursor_right();
    }
    else if (n=="\u0015"){//下
        sentence.cursor_down();
    }
    else
    {
        if(caps == false)
                transform(n.begin(),n.end(),n.begin(),::tolower);
        strcpy(n1,n.c_str());
        sentence.edit(n1);
        //sentence.cursor.col += strlen(n1);
    }
    qDebug() << sentence.cursor.hang->cur_len <<sentence.cursor.hang->max_len;
    Row *temp = sentence.first_row;
    qsentence = "";
    while(temp)
    {
        qsentence.append(temp->row_text);
        temp = temp->Next_Row;
    }
    QLabel *label=new QLabel();
    label->setText(qsentence);
    //label->re
    s->setWidget(label);
    qDebug() << qsentence;

    qDebug() << "Ccccursorrrr hang：" << sentence.cursor.hang;
    qDebug() << "Ccccursorrrr column：" << sentence.cursor.col;

    update();
}
//void MainWindow::print_cursor()
//{
//    QTime time = QTime::currentTime();
//    QPixmap pixmap(":/image/cursor.png");
//    if(time.msec()%1000 < 700)
//    {
//        pLabel->setPixmap(pixmap);
//        pLabel->setFixedSize(100, 100);
////        pLabel->setScaledContents(true);
////        pLabel->setStyleSheet("qproperty-alignment: 'AlignTop | AlignLeft'; ");
////        pLabel->setText(qsentence);
//    }
//    else
//    {
//        pLabel->setStyleSheet("qproperty-alignment: 'AlignTop | AlignLeft'; ");
//        pLabel->setText(qsentence);
//    }
//    update();
//}
//void MainWindow::mousePressEvent(QMouseEvent *event)
//{
//    if(event->button()==Qt::LeftButton) //鼠标左键按下

//        qDebug() <<
// event->pos();
//}
//void MainWindow::paintEvent(QPaintEvent *event)
//{

//    Q_UNUSED(event);
//    QPainter painter(this->cenWid);
//    QPixmap textcursor;
//    QTime time = QTime::currentTime();
//    //QRect rc(100, 100, 1000000,1000000);//在一个700x700的矩形框内输入文字(实验)
//    QFont font;
//    font.setPixelSize(26);//设置字号
//    painter.setFont(font);//设置字体
//    painter.setPen(QColor(0, 0, 0));//画笔颜色
//    painter.drawText(100,100,qsentence);//绘制文字

//    textcursor.load(":/image/cursor.png");
//    if(time.msec()%1000 < 700)
//        painter.drawPixmap(84+13*sentence.cursor.col,97,textcursor);
//        //横向间距13
//    update();
//}

//void MainWindow::blink()
//{
//     cursorTimer ++;
//     if(cursorTimer%2==0)
//     {
//         if(cursorTimer==100)
//             cursorTimer=0;
//         delete s->widget();
//         QLabel *label=new QLabel();
//         label->setText(qsentence);
//         //label->re
//         s->setWidget(label);
//         //qDebug()<<"blink1";
//     }
//     else
//     {
//         delete s->widget();
//         QLabel *label=new QLabel();
//         label->setText(qsentence);
//         s->setWidget(label);
//         //qDebug()<<"blink2";
//     }
//}

MainWindow::~MainWindow()
{

}
