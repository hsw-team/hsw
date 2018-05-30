#include "finddialog.h"
#include "mainwindow.h"
#include "ui_finddialog.h"



findDialog::findDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::findDialog)
{
    ui->setupUi(this);
    ui->findButton->setEnabled(false);// 开始时未输入，查找键无效
    setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);
    setWindowTitle("Find");

    connect(ui->lineEdit,SIGNAL(textChanged(QString)),this,SLOT(textChangeSlot())); //lineEdit中输入，则触发查找键有效


}

findDialog::~findDialog()
{
    delete ui;
}

//查找失败的弹窗
void findDialog::showMessage(QString title){
    QMessageBox box(QMessageBox::Question,"Miniword -",title);
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
    //允许搜索空格序列，包含空格不会自动去除
    QString value = ui->lineEdit->text();//.trimmed();
    qDebug() << "点击查找， 此时输入框内:" << value << endl;


     emit this->FindSignal(value);
}

//如果未输入内容，查找键无效
void findDialog::textChangeSlot(){
    // 根据与记事本的比较，空格是允许搜索的
    //if(ui->lineEdit->text().trimmed().isEmpty()){
    if(ui->lineEdit->text().isEmpty()){
        ui->findButton->setEnabled(false);
    }else{
       ui->findButton->setEnabled(true);
    }
}
