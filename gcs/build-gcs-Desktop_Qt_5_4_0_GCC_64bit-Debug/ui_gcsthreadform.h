/********************************************************************************
** Form generated from reading UI file 'gcsthreadform.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GCSTHREADFORM_H
#define UI_GCSTHREADFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_gcsThreadForm
{
public:

    void setupUi(QWidget *gcsThreadForm)
    {
        if (gcsThreadForm->objectName().isEmpty())
            gcsThreadForm->setObjectName(QStringLiteral("gcsThreadForm"));
        gcsThreadForm->resize(400, 300);

        retranslateUi(gcsThreadForm);

        QMetaObject::connectSlotsByName(gcsThreadForm);
    } // setupUi

    void retranslateUi(QWidget *gcsThreadForm)
    {
        gcsThreadForm->setWindowTitle(QApplication::translate("gcsThreadForm", "Form", 0));
    } // retranslateUi

};

namespace Ui {
    class gcsThreadForm: public Ui_gcsThreadForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GCSTHREADFORM_H
