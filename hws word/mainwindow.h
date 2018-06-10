#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QInputMethod>
#include <iostream>
#include <algorithm>
#include <QEvent>
#include <QAbstractScrollArea>
#include <QClipboard>
#include <QDebug>
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
#include <QStack>
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
#include <QDesktopServices>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrintPreviewDialog>
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

    Document *Doc;
    QLabel *status;//状态栏
    bool caps = false;//当前是否打开大写锁定
    QVector<QLabel *> block_label;//快选阴影的label数组

    QStack<Document*> Undo;
    QStack<Document*> Redo;

    int word_width = 12,word_height = 26;
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

    void window_init();
    void slot_init();

    void open_file();//打开文件
    void save_all();//保存
    void save_as();//另存为
    void show_help();//帮助
    void show_about();//关于
    void new_file();//新建文档
    void print_pdf();//打印文本
    void auto_save();//自动备份保存

    void find_window();//查找窗口
    void find(QString text);//查找函数
    void replace_window();//替换窗口
    void replace(QString, QString);//替换
    void replace_all(QString, QString);//全部替换
    void not_found_window();// 查找失败弹窗

    void block_copy();//块拷贝
    void block_selectall();//全选
    void paste_text(QString);
    void block_paste();//粘贴
    void block_delete();//块删除
    void block_cut();//剪切

    void undo();//撤销
    void redo();//重做
    void buffer_cache();//保存操作

    void enter_pressed();//按下回车
    void delete_pressed();//按下delete
    void backspace_pressed();//按下backspace

    void refresh_screen();//刷新
    void blink();//光标闪烁
    void cursor_blink();
    void english_count();//计算当前光标前英文字符数
    int english_judge(Row *row_ptr, int col);

    void chinese_count();//计算当前光标前中文字符数
    int chinese_judge(Row *row_ptr,int &col);

    void keyPressEvent(QKeyEvent *event);//键盘事件

public:
    explicit MainWindow(QWidget *parent = 0);
    void pos_to_coordinate(int x, int y, bool flag);//将坐标转换为对应行数和列数，并可选择是否要立即更新光标位置
    virtual void inputMethodEvent(QInputMethodEvent *a);
    void add_blockshadow();//添加块选阴影
    void scrollbarfollow();//滚动条跟随光标
    void contextMenuEvent(QContextMenuEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);//鼠标释放事件
    void mousePressEvent(QMouseEvent *event);//鼠标点击事件
    void mouseMoveEvent(QMouseEvent *event);//鼠标移动事件
    ~MainWindow();

    bool eventFilter(QObject *object, QEvent *event);

private:
    Ui::MainWindow *ui;
protected:
    void closeEvent(QCloseEvent *event);//关闭
};

#endif // MAINWINDOW_H
