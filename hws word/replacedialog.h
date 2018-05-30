#ifndef REPLACEDIALOG_H
#define REPLACEDIALOG_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class replaceDialog;
}

class replaceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit replaceDialog(QWidget *parent = 0);
    ~replaceDialog();
signals:
    void findSignal(QString);
    void replaceSignal(QString, QString);
    void replaceAllSignal(QString, QString);

private slots:
    void on_cancelButton_clicked();
    void textChangeSlot();
    void on_findButton_clicked();
    void on_replaceButton_clicked();
    void on_replaceAllButton_clicked();
private:
    Ui::replaceDialog *ui;
};

#endif // REPLACEDIALOG_H




