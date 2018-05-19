#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QInputMethod>
#include <iostream>
#include <algorithm>
#include <QEvent>
#include <QAbstractScrollArea>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QTextCodec>
#include <QFileDialog>
#include <QString>
#include <QLabel>
#include <QMessageBox>
#include <QBitmap>
#include <QSize>
#include <QPlainTextEdit>
#include <QPaintEvent>
#include <QPainter>
#include <QPen>
#include <QScrollArea>
#include <QScrollBar>
#include <QTime>
#include <QTimer>
#include <QThread>
#include <QWidget>
#include <QVBoxLayout>
#include <windows.h>
#include "document.h"
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QPalette pal;
    qint8 cursorTimer = 0;

    bool caps = false;
    Document sentence;
    QString qsentence;

private slots:
    void Open_File();
    void Save_All();
    void Show_Help();
    void Show_About();
    void Find_Text();
    //void blink();
    //void paintEvent(QPaintEvent *event);
    //void print_cursor();
    void keyPressEvent(QKeyEvent *event);
    //void mousePressEvent(QMouseEvent *event);
public:
    explicit MainWindow(QWidget *parent = 0);
    virtual void inputMethodEvent(QInputMethodEvent *a);
    void mousePressEvent(QMouseEvent *event);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
protected:
    void closeEvent(QCloseEvent);
};

#endif // MAINWINDOW_H
