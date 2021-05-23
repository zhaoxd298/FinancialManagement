#-------------------------------------------------
#
# Project created by QtCreator 2019-11-01T22:27:18
#
#-------------------------------------------------

QT       += core gui
QT       += axcontainer
QT       += network
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FinancialManagement
TEMPLATE = app


SOURCES += main.cpp \
    MainWindowUI.cpp \
    TableWidget.cpp \
    MainWindowSlots.cpp \
    SearchOrderDialog.cpp \
    CustomerInfoDialog.cpp \
    OrderDialog.cpp \
    SqlDatabase.cpp \
    OrderInformation.cpp \
    SearchCustomerDialog.cpp \
    SearchFinancialRecordDialog.cpp \
    FinancialRecordDialog.cpp

HEADERS  += \
    MainWindow.h \
    TableWidget.h \
    Log.h \
    SearchCustomerDialog.h \
    OrderInformation.h \
    SearchOrderDialog.h \
    CustomerInformation.h \
    CustomerInfoDialog.h \
    OrderDialog.h \
    SqlDatabase.h \
    Version.h \
    SearchFinancialRecordDialog.h \
    FinancialRecordDialog.h \
    FinancialRecordInfo.h
