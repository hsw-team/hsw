#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->label->setStyleSheet("qproperty-alignment: 'AlignTop | AlignLeft'; ");
    ui->label->setText(qsentence);
    ui->scrollArea->setWidgetResizable(true);
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");//情况2
    QTextCodec::setCodecForLocale(codec);
    this->setAttribute(Qt::WA_InputMethodEnabled);
    this->setAttribute(Qt::WA_KeyCompression);
    this->setFocusPolicy(Qt::WheelFocus);
    ui->scrollArea->setStyleSheet("background:white");

    // 一些初始化时应该默认关闭的操作
    ui->actionUndo_Z->setEnabled(false);
    ui->actionRedo_Y->setEnabled(false);
    // 连接
     connect(ui->actionFind_F,SIGNAL(triggered()),this,SLOT(Find_Text()));

}

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
    f.exec(); // 使用exec是因为打开查找窗口后，应该无法对主窗口进行操作
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
void MainWindow::keyPressEvent(QKeyEvent *ev)//按键事件
{
    this->setFocus();
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
    else if(ev->modifiers()== Qt::ControlModifier)
    {
        qDebug()<<"Discard";
    }

    else if(n == "\u0004")//输入为回车
    {
        int i,j = 0;
        int col = sentence.cursor.col;
        Row *hang = sentence.cursor.hang;
        sentence.add_row(sentence.cursor.hang);
        sentence.cursor.hang = sentence.cursor.hang->Next_Row;
        for(i = col; i < hang->cur_len; i++)
        {
                sentence.cursor.hang->row_text[j++] = hang->row_text[i];
                hang->row_text[i] = '\0';
        }
        hang->cur_len = col;
        sentence.cursor.hang->cur_len = j;
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
        if(sentence.cursor.col > 0)//当指针不在行首的时候(之前有字符可以删除)
        {
            //TODO：
            //判断中文字符

            //后面字符依次往前挪一个位置
            for(int i = sentence.cursor.col - 1; i < sentence.cursor.hang->cur_len; i++)
            {
                sentence.cursor.hang->row_text[i] = sentence.cursor.hang->row_text[i+1];
            }
            sentence.cursor.col--;
            sentence.cursor.hang->cur_len--;
        }
        else if(sentence.cursor.col == 0 && sentence.cursor.hang != sentence.first_row)//当在行首，并且不是第一行(第一行行首删除就不用操作了)
        {
            Row *temp = sentence.cursor.hang;
            sentence.cursor.hang = temp->Prev_Row;
            if(temp->Next_Row){
                temp->Next_Row->Prev_Row = sentence.cursor.hang;
            }
            sentence.cursor.hang->Next_Row = temp->Next_Row;
            sentence.cursor.col = sentence.cursor.hang->cur_len;
            //需要把之后行的加到后面
            int i = 0;
            int col = sentence.cursor.col;
            while(i < temp->cur_len)
            {
                sentence.cursor.hang->row_text[col++] =  temp->row_text[i++];
                sentence.cursor.hang->cur_len++;
            }
            delete temp;
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
        qsentence.append('\n');//MODIFIED
        temp = temp->Next_Row;
    }
    ui->label->setText(qsentence);
    ui->label->adjustSize();
    ui->scrollAreaWidgetContents_2->setMinimumSize(ui->label->width(),ui->label->height());
    qDebug() << qsentence;

    qDebug() << "Ccccursorrrr hang：" << sentence.cursor.hang;
    qDebug() << "Ccccursorrrr column：" << sentence.cursor.col;
}
void MainWindow::inputMethodEvent(QInputMethodEvent *a)
{
    qDebug()<<"当前输入为"<<a->commitString().toUtf8().data();
    sentence.edit(a->commitString().toUtf8().data());
    Row *temp = sentence.first_row;
    qsentence = "";
    while(temp)
    {
        qsentence.append(temp->row_text);
        qsentence.append('\n');//MODIFIED
        temp = temp->Next_Row;
    }
    ui->label->setText(qsentence);
    ui->label->adjustSize();
    ui->scrollAreaWidgetContents_2->setMinimumSize(ui->label->width(),ui->label->height());

}

MainWindow::~MainWindow()
{
    delete ui;
}
