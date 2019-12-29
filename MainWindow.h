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

    bool addNewOrder(const QString& customerName="");

public:
    MainWindow(QWidget *parent = 0);

    ~MainWindow();

public slots:
    void onAddCustomerBtn();
    void onSearchCustomerBtn();

    void onAddOrderBtn();
    void onSearchOrderBtn();

    void onEditCustomerInfo(int row, const QString& name);
    void onEditOrderInfo(int row, const QString& orderID);

    void onSearchHistoryOrder(const QString& name);      // 查找历史订单
    void onNewOrder(const QString& name);                // 新建订单

    void onChangeOrderStatus(const QStringList& orderList, const QString& status);  // 修改订单状态
};

#endif // MAINWINDOW_H
