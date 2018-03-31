#include "mainwindow.h"
#include <qDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->resize(QSize(1024,768));
    CreateMenu();


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
void MainWindow::closeEvent()
{
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

MainWindow::~MainWindow()
{

}
