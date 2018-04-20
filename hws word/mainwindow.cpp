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

    CreateMenu();
    ColorSelect();
    update();
//    connect(timer,SIGNAL(timeout()),this,SLOT(update()));
//    timer->start(1000);
    //调用控件，仅试用
    /*
    textedit = new QPlainTextEdit;
    setCentralWidget(textedit);
    */

    //申明一个Document对象
    //读入Hello World!
    //并从控制台输出Document的内容
    /*
    Document M;
    char a[20]={"Hello World!"};
    M.first_row->edit(a);
    M.show_doc();
    */
}

void MainWindow::CreateMenu()
{
    // File 菜单栏
    File = menuBar()->addMenu(tr("File"));
    File_New = new QAction(tr("新建"),this);
    File_New->setShortcut(QKeySequence::New);
    File_Open=new QAction(tr("打开"),this);
    File_Open->setShortcut(QKeySequence::Open);
    File_Save=new QAction(tr("保存"),this);
    File_Save->setShortcut(QKeySequence::Save);

    File->addAction(File_New);
    File->addAction(File_Open);
    File->addAction(File_Save);


    // Edit 菜单栏
    Edit = menuBar()->addMenu(tr("Edit"));
    Edit_Search=new QAction(tr("查找文本"),this);
    Edit_Search->setShortcut(QKeySequence::Find);

    Edit->addAction(Edit_Search);


    // Help 菜单栏
    Help = menuBar()->addMenu(tr("Help"));
    Help_Help=new QAction(tr("帮助文档"),this);

    Help->addAction(Help_Help);


    // About 菜单栏
    About = menuBar()->addMenu(tr("About"));
    About_About=new QAction(tr("关于软件"),this);

    About->addAction(About_About);

    //动作 connect
    connect(File_Open,SIGNAL(triggered(bool)),this,SLOT(Open_File()));
    /*
    connect(this->,SIGNAL(triggered(bool)),this,SLOT());*/
    connect(this->Edit_Search,SIGNAL(triggered(bool)),this,SLOT(Find_Text()));
    connect(this->Help_Help,SIGNAL(triggered(bool)),this,SLOT(Show_Help()));
    connect(About_About,SIGNAL(triggered(bool)),this,SLOT(Show_About()));
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
    qDebug() << qsentence;

    qDebug() << "Ccccursorrrr hang：" << sentence.cursor.hang;
    qDebug() << "Ccccursorrrr column：" << sentence.cursor.col;

    update();
}
//void MainWindow::mousePressEvent(QMouseEvent *event)
//{
//    if(event->button()==Qt::LeftButton) //鼠标左键按下

//        qDebug() <<
// event->pos();
//}
void MainWindow::paintEvent(QPaintEvent *event)
{

    Q_UNUSED(event);
    QPainter painter(this);
    QPixmap textcursor;
    QTime time = QTime::currentTime();
    QRect rc(100, 100, 800, 800);//在一个700x700的矩形框内输入文字(实验)
    QFont font;
    font.setPixelSize(26);//设置字号
    painter.setFont(font);//设置字体
    painter.setPen(QColor(0, 0, 0));//画笔颜色
    painter.drawText(rc,Qt::TextWrapAnywhere,qsentence);//绘制文字

    textcursor.load(":/image/cursor.png");
    if(time.msec()%1000 < 700)
        painter.drawPixmap(84+13*sentence.cursor.col,97,textcursor);
        //横向间距13
    update();
}

MainWindow::~MainWindow()
{

}
