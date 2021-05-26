#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include <QLabel>

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
    QPushButton* m_addFinancialRecordBtn;
    QPushButton* m_searchFinancialRecordBtn;

    QWidget* m_mainWidget;
    TableWidget* m_tableWidget;

    QLabel* m_statusLabel;

    void constructUI();
    void connectSlots();

    bool addNewOrder(const QString& customerName="");
    bool addNewFinancialRecord(const QString& customerName="", const QString &contractID = "");
    void searchFinancialRecord(int type, const QString& keyword, bool showDataInNewDialog = false);

public:
    MainWindow(QWidget *parent = 0);

    void setStatusText(const QString& text);

    ~MainWindow();

public slots:
    void onAddCustomerBtn();
    void onSearchCustomerBtn();
    void onEditCustomerInfo(int row, const QString& name);

    void onAddOrderBtn();
    void onSearchOrderBtn();
    void onEditOrderInfo(int row, const QString& orderID);
    void onSearchHistoryOrder(const QString& name);      // 查找历史订单
    void onNewOrder(const QString& name);                // 新建订单

    void onAddFinancialRecord(const QString& name = "", const QString& contractID = "");
    void onSearchFinancialRecordBtn();
    void onSearchFinancialByCustomerName(const QString& name);
    void onSearchFinancialBycontractID(const QString& contractID);
    void onEditFinancialInfo(int row, int number);

    void onChangeOrderStatus(const QStringList& orderList, const QString& status);  // 修改订单状态
};

#endif // MAINWINDOW_H
