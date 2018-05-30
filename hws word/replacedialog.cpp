#include "replacedialog.h"
#include "ui_replacedialog.h"

replaceDialog::replaceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::replaceDialog)
{

    ui->setupUi(this);

    // 初始无效的按钮
    ui->findButton->setEnabled(false);
    ui->replaceAllButton->setEnabled(false);
    ui->replaceButton->setEnabled(false);


    setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);
    setWindowTitle("Replace");

    // 查找框文字改变
    connect(ui->findEdit,SIGNAL(textChanged(QString)),this,SLOT(textChangeSlot()));


    ui->findButton->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F));
    ui->replaceButton->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));
    ui->replaceAllButton->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_A));
   // ui->checkBox->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));
}

replaceDialog::~replaceDialog()
{
    delete ui;
}

//关闭窗口
void replaceDialog:: on_cancelButton_clicked(){
    this->close();
}

//文字改变
void replaceDialog:: textChangeSlot(){
    if(ui->findEdit->text().isEmpty()){
        ui->findButton->setEnabled(false);
        ui->replaceButton->setEnabled(false);
        ui->replaceAllButton->setEnabled(false);
    }
    else{
        ui->findButton->setEnabled(true);
        ui->replaceButton->setEnabled(true);
        ui->replaceAllButton->setEnabled(true);
    }
}

// 查找按钮
void replaceDialog:: on_findButton_clicked(){
    QString value = ui->findEdit->text(); //查找的文本

    emit this->findSignal(value); // 回调查找
}

void replaceDialog:: on_replaceButton_clicked(){
    QString target = ui->findEdit->text(); // 查找文本
    QString value  = ui->replaceEdit->text(); // 替换文本

    emit this->replaceSignal(target, value); // 回调替换
}


void replaceDialog::on_replaceAllButton_clicked(){
    QString target = ui->findEdit->text();
    QString value = ui->replaceEdit->text();

    emit this->replaceAllSignal(target, value);
}
