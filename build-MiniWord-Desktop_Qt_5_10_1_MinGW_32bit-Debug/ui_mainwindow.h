/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionNew_N;
    QAction *actionOpen_O;
    QAction *actionSave_S;
    QAction *actionSave_as;
    QAction *actionPrint_P;
    QAction *actionExit;
    QAction *actionCut_X;
    QAction *actionCopy_C;
    QAction *actionPaste_P;
    QAction *actionUndo_Z;
    QAction *actionRedo_Y;
    QAction *actionFind_F;
    QAction *actionSelect_All_A;
    QAction *actionDocs;
    QAction *actionMiniword;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents_2;
    QLabel *label;
    QLabel *label_2;
    QMenuBar *menuBar;
    QMenu *menuFile_F;
    QMenu *menuEdit_E;
    QMenu *menuHelp_H;
    QMenu *menuAbout_A;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(831, 664);
        MainWindow->setCursor(QCursor(Qt::ArrowCursor));
        actionNew_N = new QAction(MainWindow);
        actionNew_N->setObjectName(QStringLiteral("actionNew_N"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/my_images/icons/new.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionNew_N->setIcon(icon);
        actionOpen_O = new QAction(MainWindow);
        actionOpen_O->setObjectName(QStringLiteral("actionOpen_O"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/my_images/icons/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen_O->setIcon(icon1);
        actionSave_S = new QAction(MainWindow);
        actionSave_S->setObjectName(QStringLiteral("actionSave_S"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/my_images/icons/Save.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave_S->setIcon(icon2);
        actionSave_as = new QAction(MainWindow);
        actionSave_as->setObjectName(QStringLiteral("actionSave_as"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/my_images/icons/saveas.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave_as->setIcon(icon3);
        actionPrint_P = new QAction(MainWindow);
        actionPrint_P->setObjectName(QStringLiteral("actionPrint_P"));
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/my_images/icons/exit.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionExit->setIcon(icon4);
        actionCut_X = new QAction(MainWindow);
        actionCut_X->setObjectName(QStringLiteral("actionCut_X"));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/my_images/icons/cut.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCut_X->setIcon(icon5);
        actionCopy_C = new QAction(MainWindow);
        actionCopy_C->setObjectName(QStringLiteral("actionCopy_C"));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/my_images/icons/copy.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCopy_C->setIcon(icon6);
        actionPaste_P = new QAction(MainWindow);
        actionPaste_P->setObjectName(QStringLiteral("actionPaste_P"));
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/my_images/icons/paste.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPaste_P->setIcon(icon7);
        actionUndo_Z = new QAction(MainWindow);
        actionUndo_Z->setObjectName(QStringLiteral("actionUndo_Z"));
        QIcon icon8;
        icon8.addFile(QStringLiteral(":/my_images/icons/undo_256.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionUndo_Z->setIcon(icon8);
        actionRedo_Y = new QAction(MainWindow);
        actionRedo_Y->setObjectName(QStringLiteral("actionRedo_Y"));
        QIcon icon9;
        icon9.addFile(QStringLiteral(":/my_images/icons/redo.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRedo_Y->setIcon(icon9);
        actionFind_F = new QAction(MainWindow);
        actionFind_F->setObjectName(QStringLiteral("actionFind_F"));
        actionSelect_All_A = new QAction(MainWindow);
        actionSelect_All_A->setObjectName(QStringLiteral("actionSelect_All_A"));
        QIcon icon10;
        icon10.addFile(QStringLiteral(":/my_images/icons/changefont.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSelect_All_A->setIcon(icon10);
        actionDocs = new QAction(MainWindow);
        actionDocs->setObjectName(QStringLiteral("actionDocs"));
        QIcon icon11;
        icon11.addFile(QStringLiteral(":/my_images/icons/aboutqt.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDocs->setIcon(icon11);
        actionMiniword = new QAction(MainWindow);
        actionMiniword->setObjectName(QStringLiteral("actionMiniword"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        scrollArea = new QScrollArea(centralWidget);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(scrollArea->sizePolicy().hasHeightForWidth());
        scrollArea->setSizePolicy(sizePolicy1);
        scrollArea->setMinimumSize(QSize(0, 0));
        scrollArea->viewport()->setProperty("cursor", QVariant(QCursor(Qt::IBeamCursor)));
        scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName(QStringLiteral("scrollAreaWidgetContents_2"));
        scrollAreaWidgetContents_2->setEnabled(true);
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 815, 566));
        scrollAreaWidgetContents_2->setMinimumSize(QSize(250, 250));
        label = new QLabel(scrollAreaWidgetContents_2);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(3, 1, 851, 491));
        label->setStyleSheet(QStringLiteral("font: 30pt \"Calibri\";"));
        label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        label->setWordWrap(false);
        label_2 = new QLabel(scrollAreaWidgetContents_2);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(110, 0, 31, 31));
        label_2->setStyleSheet(QStringLiteral("border-image: url(:/my_images/icons/I.png);background: transparent;"));
        scrollArea->setWidget(scrollAreaWidgetContents_2);

        verticalLayout->addWidget(scrollArea);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 831, 21));
        menuFile_F = new QMenu(menuBar);
        menuFile_F->setObjectName(QStringLiteral("menuFile_F"));
        menuEdit_E = new QMenu(menuBar);
        menuEdit_E->setObjectName(QStringLiteral("menuEdit_E"));
        menuHelp_H = new QMenu(menuBar);
        menuHelp_H->setObjectName(QStringLiteral("menuHelp_H"));
        menuAbout_A = new QMenu(menuBar);
        menuAbout_A->setObjectName(QStringLiteral("menuAbout_A"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile_F->menuAction());
        menuBar->addAction(menuEdit_E->menuAction());
        menuBar->addAction(menuHelp_H->menuAction());
        menuBar->addAction(menuAbout_A->menuAction());
        menuFile_F->addAction(actionNew_N);
        menuFile_F->addAction(actionOpen_O);
        menuFile_F->addAction(actionSave_S);
        menuFile_F->addAction(actionSave_as);
        menuFile_F->addSeparator();
        menuFile_F->addAction(actionPrint_P);
        menuFile_F->addAction(actionExit);
        menuEdit_E->addAction(actionCut_X);
        menuEdit_E->addAction(actionCopy_C);
        menuEdit_E->addAction(actionPaste_P);
        menuEdit_E->addAction(actionUndo_Z);
        menuEdit_E->addAction(actionRedo_Y);
        menuEdit_E->addSeparator();
        menuEdit_E->addAction(actionFind_F);
        menuEdit_E->addAction(actionSelect_All_A);
        menuHelp_H->addAction(actionDocs);
        menuAbout_A->addAction(actionMiniword);
        mainToolBar->addAction(actionNew_N);
        mainToolBar->addAction(actionOpen_O);
        mainToolBar->addAction(actionSave_S);
        mainToolBar->addAction(actionSave_as);
        mainToolBar->addAction(actionPrint_P);
        mainToolBar->addAction(actionExit);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionCut_X);
        mainToolBar->addAction(actionCopy_C);
        mainToolBar->addAction(actionPaste_P);
        mainToolBar->addAction(actionUndo_Z);
        mainToolBar->addAction(actionRedo_Y);
        mainToolBar->addAction(actionFind_F);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionSelect_All_A);
        mainToolBar->addAction(actionDocs);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        actionNew_N->setText(QApplication::translate("MainWindow", "New(&N)", nullptr));
        actionOpen_O->setText(QApplication::translate("MainWindow", "Open(&O)", nullptr));
        actionSave_S->setText(QApplication::translate("MainWindow", "Save(&S)", nullptr));
        actionSave_as->setText(QApplication::translate("MainWindow", "Save as", nullptr));
        actionPrint_P->setText(QApplication::translate("MainWindow", "Print(&P)", nullptr));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", nullptr));
        actionCut_X->setText(QApplication::translate("MainWindow", "Cut(&X)", nullptr));
#ifndef QT_NO_SHORTCUT
        actionCut_X->setShortcut(QApplication::translate("MainWindow", "Ctrl+X", nullptr));
#endif // QT_NO_SHORTCUT
        actionCopy_C->setText(QApplication::translate("MainWindow", "Copy(&C)", nullptr));
#ifndef QT_NO_SHORTCUT
        actionCopy_C->setShortcut(QApplication::translate("MainWindow", "Ctrl+C", nullptr));
#endif // QT_NO_SHORTCUT
        actionPaste_P->setText(QApplication::translate("MainWindow", "Paste(&P)", nullptr));
#ifndef QT_NO_SHORTCUT
        actionPaste_P->setShortcut(QApplication::translate("MainWindow", "Ctrl+V", nullptr));
#endif // QT_NO_SHORTCUT
        actionUndo_Z->setText(QApplication::translate("MainWindow", "Undo(&Z)", nullptr));
#ifndef QT_NO_SHORTCUT
        actionUndo_Z->setShortcut(QApplication::translate("MainWindow", "Ctrl+Z", nullptr));
#endif // QT_NO_SHORTCUT
        actionRedo_Y->setText(QApplication::translate("MainWindow", "Redo(&Y)", nullptr));
#ifndef QT_NO_SHORTCUT
        actionRedo_Y->setShortcut(QApplication::translate("MainWindow", "Ctrl+Y", nullptr));
#endif // QT_NO_SHORTCUT
        actionFind_F->setText(QApplication::translate("MainWindow", "Find(&F)", nullptr));
#ifndef QT_NO_SHORTCUT
        actionFind_F->setShortcut(QApplication::translate("MainWindow", "Ctrl+F", nullptr));
#endif // QT_NO_SHORTCUT
        actionSelect_All_A->setText(QApplication::translate("MainWindow", "Select All(&A)", nullptr));
#ifndef QT_NO_SHORTCUT
        actionSelect_All_A->setShortcut(QApplication::translate("MainWindow", "Ctrl+A", nullptr));
#endif // QT_NO_SHORTCUT
        actionDocs->setText(QApplication::translate("MainWindow", "Docs", nullptr));
        actionMiniword->setText(QApplication::translate("MainWindow", "Miniword", nullptr));
        label->setText(QApplication::translate("MainWindow", "TextLabel", nullptr));
        label_2->setText(QString());
        menuFile_F->setTitle(QApplication::translate("MainWindow", "File(&F)", nullptr));
        menuEdit_E->setTitle(QApplication::translate("MainWindow", "Edit(&E\357\274\211", nullptr));
        menuHelp_H->setTitle(QApplication::translate("MainWindow", "Help(&H)", nullptr));
        menuAbout_A->setTitle(QApplication::translate("MainWindow", "About(&A)", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
