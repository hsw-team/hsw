#include "finddialog.h"
#include "ui_finddialog.h"



findDialog::findDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::findDialog)
{
    ui->setupUi(this);
    ui->findButton->setEnabled(false);
    setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);
    setWindowTitle("Find");

    connect(ui->lineEdit,SIGNAL(textChanged(QString)),this,SLOT(textChangeSlot()));

}

findDialog::~findDialog()
{
    delete ui;
}

//查找失败的弹窗
void findDialog::showMessage(QString title){
    QMessageBox box(QMessageBox::Question,"Search",title);
    box.setIcon(QMessageBox::NoIcon);// 暂时没有加上icon
    box.setStandardButtons (QMessageBox::Ok);
    box.setButtonText (QMessageBox::Ok,QString("OK"));
    box.setWindowFlags( Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint );
    box.exec();
}

void findDialog::on_pushButton_2_clicked()
{
    this->close(); // 点击取消，关闭窗口
}

void findDialog::on_findButton_clicked()
{
    //QDebug() << "find clicked" << endl;
    QString value = ui->lineEdit->text().trimmed();
    qDebug() << value << endl;
}

//如果未输入内容，查找键无效
void findDialog::textChangeSlot(){
    if(ui->lineEdit->text().trimmed().isEmpty()){
        ui->findButton->setEnabled(false);
    }else{
       ui->findButton->setEnabled(true);
    }
}
