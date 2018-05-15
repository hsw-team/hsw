/********************************************************************************
** Form generated from reading UI file 'finddialog.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FINDDIALOG_H
#define UI_FINDDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_findDialog
{
public:
    QPushButton *findButton;
    QPushButton *pushButton_2;
    QLineEdit *lineEdit;
    QLabel *label;

    void setupUi(QDialog *findDialog)
    {
        if (findDialog->objectName().isEmpty())
            findDialog->setObjectName(QStringLiteral("findDialog"));
        findDialog->resize(400, 300);
        findButton = new QPushButton(findDialog);
        findButton->setObjectName(QStringLiteral("findButton"));
        findButton->setGeometry(QRect(150, 170, 91, 41));
        findButton->setStyleSheet(QStringLiteral("font: 9pt \"Consolas\";"));
        pushButton_2 = new QPushButton(findDialog);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(149, 232, 91, 41));
        pushButton_2->setStyleSheet(QStringLiteral("font: 9pt \"Consolas\";"));
        lineEdit = new QLineEdit(findDialog);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(80, 110, 241, 41));
        label = new QLabel(findDialog);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(140, 50, 111, 41));
        label->setStyleSheet(QLatin1String("\n"
"font: 10pt \"Consolas\";"));

        retranslateUi(findDialog);

        QMetaObject::connectSlotsByName(findDialog);
    } // setupUi

    void retranslateUi(QDialog *findDialog)
    {
        findDialog->setWindowTitle(QApplication::translate("findDialog", "Dialog", nullptr));
        findButton->setText(QApplication::translate("findDialog", "Find Next", nullptr));
        pushButton_2->setText(QApplication::translate("findDialog", "Cancel", nullptr));
        label->setText(QApplication::translate("findDialog", "Find Content:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class findDialog: public Ui_findDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FINDDIALOG_H
