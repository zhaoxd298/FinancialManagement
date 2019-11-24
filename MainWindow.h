#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QPushButton>

#include "SqlDatabase.h"
#include "TableWidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    SqlDatabase* m_sqlDatabase;

    QPushButton* m_addCustomerBtn;
    QPushButton* m_searchCustomerBtn;
    QPushButton* m_addOrderBtn;
    QPushButton* m_searchOrderBtn;
    QWidget* m_mainWidget;
    TableWidget* m_tableWidget;

    void constructUI();
    void connectSlots();

public:
    MainWindow(QWidget *parent = 0);

    ~MainWindow();

public slots:
    void onAddCustomerBtn();
    void onSearchCustomerBtn();
    void onAddOrderBtn();
    void onSearchOrderBtn();
};

#endif // MAINWINDOW_H
