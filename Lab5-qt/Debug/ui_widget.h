/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "../../qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QCustomPlot *customPlot;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *Puertos;
    QPushButton *ABRIR;
    QWidget *horizontalLayoutWidget_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *RPM;
    QLabel *label_3;
    QLineEdit *lineEdit_2;
    QPushButton *ASIGNAR;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QPushButton *ARRANCAR;
    QPushButton *DETENERSE;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *rpmdat;
    QLabel *segdat;
    QWidget *gridLayoutWidget_2;
    QGridLayout *gridLayout_2;
    QLabel *label_7;
    QLabel *rpmmax;
    QLabel *segmax;
    QLabel *label_6;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(905, 662);
        customPlot = new QCustomPlot(Widget);
        customPlot->setObjectName(QString::fromUtf8("customPlot"));
        customPlot->setGeometry(QRect(60, 280, 741, 321));
        horizontalLayoutWidget = new QWidget(Widget);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(30, 90, 233, 80));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(horizontalLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setFamily(QString::fromUtf8("Siemens Sans"));
        font.setPointSize(10);
        label->setFont(font);
        label->setTextFormat(Qt::MarkdownText);

        horizontalLayout->addWidget(label);

        Puertos = new QComboBox(horizontalLayoutWidget);
        Puertos->setObjectName(QString::fromUtf8("Puertos"));

        horizontalLayout->addWidget(Puertos);

        ABRIR = new QPushButton(horizontalLayoutWidget);
        ABRIR->setObjectName(QString::fromUtf8("ABRIR"));

        horizontalLayout->addWidget(ABRIR);

        horizontalLayoutWidget_2 = new QWidget(Widget);
        horizontalLayoutWidget_2->setObjectName(QString::fromUtf8("horizontalLayoutWidget_2"));
        horizontalLayoutWidget_2->setGeometry(QRect(30, 10, 387, 80));
        horizontalLayout_2 = new QHBoxLayout(horizontalLayoutWidget_2);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(horizontalLayoutWidget_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setFont(font);

        horizontalLayout_2->addWidget(label_2);

        RPM = new QLineEdit(horizontalLayoutWidget_2);
        RPM->setObjectName(QString::fromUtf8("RPM"));

        horizontalLayout_2->addWidget(RPM);

        label_3 = new QLabel(horizontalLayoutWidget_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_2->addWidget(label_3);

        lineEdit_2 = new QLineEdit(horizontalLayoutWidget_2);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));

        horizontalLayout_2->addWidget(lineEdit_2);

        ASIGNAR = new QPushButton(horizontalLayoutWidget_2);
        ASIGNAR->setObjectName(QString::fromUtf8("ASIGNAR"));

        horizontalLayout_2->addWidget(ASIGNAR);

        verticalLayoutWidget = new QWidget(Widget);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(690, 10, 160, 80));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        ARRANCAR = new QPushButton(verticalLayoutWidget);
        ARRANCAR->setObjectName(QString::fromUtf8("ARRANCAR"));

        verticalLayout->addWidget(ARRANCAR);

        DETENERSE = new QPushButton(verticalLayoutWidget);
        DETENERSE->setObjectName(QString::fromUtf8("DETENERSE"));

        verticalLayout->addWidget(DETENERSE);

        gridLayoutWidget = new QWidget(Widget);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(690, 110, 160, 80));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        label_4 = new QLabel(gridLayoutWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 1, 0, 1, 1);

        label_5 = new QLabel(gridLayoutWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 0, 0, 1, 1);

        rpmdat = new QLabel(gridLayoutWidget);
        rpmdat->setObjectName(QString::fromUtf8("rpmdat"));

        gridLayout->addWidget(rpmdat, 0, 1, 1, 1);

        segdat = new QLabel(gridLayoutWidget);
        segdat->setObjectName(QString::fromUtf8("segdat"));

        gridLayout->addWidget(segdat, 1, 1, 1, 1);

        gridLayoutWidget_2 = new QWidget(Widget);
        gridLayoutWidget_2->setObjectName(QString::fromUtf8("gridLayoutWidget_2"));
        gridLayoutWidget_2->setGeometry(QRect(530, 110, 160, 80));
        gridLayout_2 = new QGridLayout(gridLayoutWidget_2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        label_7 = new QLabel(gridLayoutWidget_2);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout_2->addWidget(label_7, 1, 0, 1, 1);

        rpmmax = new QLabel(gridLayoutWidget_2);
        rpmmax->setObjectName(QString::fromUtf8("rpmmax"));

        gridLayout_2->addWidget(rpmmax, 0, 1, 1, 1);

        segmax = new QLabel(gridLayoutWidget_2);
        segmax->setObjectName(QString::fromUtf8("segmax"));

        gridLayout_2->addWidget(segmax, 1, 1, 1, 1);

        label_6 = new QLabel(gridLayoutWidget_2);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_2->addWidget(label_6, 0, 0, 1, 1);


        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        label->setText(QCoreApplication::translate("Widget", "Puerto:", nullptr));
        ABRIR->setText(QCoreApplication::translate("Widget", "ABRIR", nullptr));
        label_2->setText(QCoreApplication::translate("Widget", "RPM:", nullptr));
        label_3->setText(QCoreApplication::translate("Widget", "Velocidad (m/s*s)", nullptr));
        ASIGNAR->setText(QCoreApplication::translate("Widget", "Asignar", nullptr));
        ARRANCAR->setText(QCoreApplication::translate("Widget", "Arrancar", nullptr));
        DETENERSE->setText(QCoreApplication::translate("Widget", "Detenerse", nullptr));
        label_4->setText(QCoreApplication::translate("Widget", "Tiempo", nullptr));
        label_5->setText(QCoreApplication::translate("Widget", "RPM", nullptr));
        rpmdat->setText(QCoreApplication::translate("Widget", "TextLabel", nullptr));
        segdat->setText(QCoreApplication::translate("Widget", "TextLabel", nullptr));
        label_7->setText(QCoreApplication::translate("Widget", "TiempoMax", nullptr));
        rpmmax->setText(QCoreApplication::translate("Widget", "TextLabel", nullptr));
        segmax->setText(QCoreApplication::translate("Widget", "TextLabel", nullptr));
        label_6->setText(QCoreApplication::translate("Widget", "RPMmax", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
