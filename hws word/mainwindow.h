#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QInputMethod>
#include <iostream>
#include <algorithm>
#include <QEvent>
#include <QAbstractScrollArea>
#include <QClipboard>
#include <QGraphicsOpacityEffect>
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
#include <QVector>
#include <QVBoxLayout>
#include <windows.h>
#include "document.h"
#include "finddialog.h"
#include "replacedialog.h"

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
    QVector<QLabel *> test;
    //QLabel *test[1000];

    int chi_num = 0;
    int start_x = -1, start_y = -1;//拖动鼠标时的起始坐标
    int end_x=-1,end_y=-1;//拖动鼠标后释放时的坐标
    int start_col=-1,start_row=-1;//拖动鼠标后的起始数据结构位置
    int end_col=-1, end_row=-1;//拖动鼠标后释放时的数据结构位置
    findDialog f; // 查找框
    bool found = false; // 当前查找是否已经成功
    bool notExist = false; // 查找失败标记
    replaceDialog r; // 替换框
    bool isSelectedblock = false;//是否选中块
    bool isFirstReplace = true; // 是否第一次替换

private slots:
    void Open_File();
    void Save_All();
    void Save_As();//MODIFIED
    void Show_Help();
    void Show_About();

    void Find_Text();
    void Find(QString text);
    void Replace_Window();
    void Replace(QString, QString);
    void ReplaceAll(QString, QString);

    void Block_Copy();
    void Block_Paste();
    void Block_Delete();

    void refresh();//MODIFIED
    void cnt_chinese();//MODIFIED
    //void blink();
    //void paintEvent(QPaintEvent *event);
    //void print_cursor();
    void keyPressEvent(QKeyEvent *event);
    //void mousePressEvent(QMouseEvent *event);
public:
    explicit MainWindow(QWidget *parent = 0);
    void pos_to_coordinate(int x, int y, bool flag);
    virtual void inputMethodEvent(QInputMethodEvent *a);
    void add_blockshadow();
    void mouseReleaseEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
protected:
    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
