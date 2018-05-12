#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <algorithm>
#include <QEvent>
#include <QAbstractScrollArea>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QString>
#include <QLabel>
#include <QMessageBox>
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

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QPlainTextEdit *textedit;
    QPalette pal;
    QMenu *File;
    QMenu *Edit;
    QMenu *Help;
    QMenu *About;

    QAction *File_New;                    // new file
    QAction *File_Open;                   // open file
    QAction *File_Save;                   // save file
    QAction *File_Save_as;                // save file as
    QAction *File_Print;                  // print the file
    QAction *File_Exit;                   // exit the file

    QAction *Edit_Search;                 // find text
    QAction *Edit_Cut;                    // cut text
    QAction *Edit_Copy;                   // copy text
    QAction *Edit_Paste;                  // paste text
    QAction *Edit_Undo;                   // undo
    QAction *Edit_Redo;                   // redo
    QAction *Edit_Select_all;             // Select all

    QAction *About_About;
    QAction *Help_Help;

    QToolBar *toolBar;
    QLabel *pLabel;
    QLabel *curlabel;

    QScrollArea *s;
    QWidget *cenWid;

    qint8 cursorTimer = 0;

    bool caps = false;
    Document sentence;
    QString qsentence;

    void CreateMenu();
    void initToolBar();
    void ColorSelect();
private slots:

    void Open_File();
    void Show_Help();
    void Show_About();
    void Find_Text();
    //void blink();
    //void paintEvent(QPaintEvent *event);
    //void print_cursor();
    void keyReleaseEvent(QKeyEvent *event);
    //void mousePressEvent(QMouseEvent *event);

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent);
};

//class MyThread : public QThread {
//    Q_OBJECT
//    private slots:
//        virtual void run();
//    };


#endif // MAINWINDOW_H
