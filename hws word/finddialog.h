#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QDebug>
namespace Ui {
class findDialog;
}

class findDialog : public QDialog
{
    Q_OBJECT

   // bool found = false; // 当前查找前是否查找成功

public:
    explicit findDialog(QWidget *parent = 0);
    ~findDialog();
private slots:    
    void on_pushButton_2_clicked(); // 点击 Cancel

    void on_findButton_clicked(); // 点击 Find Next

    void textChangeSlot(); // Line Text 中文字改变

signals:
    void FindSignal(QString);

private:
    Ui::findDialog *ui;


    void showMessage(QString title);  //显示信息
};

#endif // FINDDIALOG_H
