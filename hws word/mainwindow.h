#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include <QSize>
#include <QPlainTextEdit>

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QPlainTextEdit *textedit;

    QMenu *File;
    QMenu *Edit;
    QMenu *Help;
    QMenu *About;

    QAction *File_New;
    QAction *File_Open;
    QAction *File_Save;
    QAction *Edit_Search;
    QAction *About_About;
    QAction *Help_Help;

    void CreateMenu();

private slots:
    void Open_File();
    void Show_Help();
    void Show_About();
    void Find_Text();

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent);
};

#endif // MAINWINDOW_H
