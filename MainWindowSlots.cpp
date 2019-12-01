#include "MainWindow.h"
#include "CustomerInfoDialog.h"
#include "customerinformation.h"
#include "SearchCustomerDialog.h"
#include "OrderDialog.h"
#include "SearchOrderDialog.h"
#include <QMessageBox>
#include <QDebug>
#include <QDate>

bool MainWindow::addNewOrder(const QString& customerName)
{
    bool ret = false;
    OrderDialog dialog;
    if (!customerName.isEmpty())
    {
        dialog.setCustomerName(customerName);
        dialog.disableCustomerNameEdit();
    }

    if(dialog.exec() == QDialog::Accepted)
    {
        qDebug() << "add order accepted!";
        m_tableWidget->clear();
        m_tableWidget->setDataTypeOrderInfo();

        QList<OrderInformation> orderList = dialog.getOrderList();
        for (int i=0; i<orderList.size(); i++)
        {
            if (true == m_sqlDatabase->insertOrderInfo(orderList[i]))
            {
                ret = true;
                m_tableWidget->addOrderInformation(orderList[i]);
                QMessageBox::information(this, QString(tr("提示")), QString(tr("客户\"%1\"订单添加成功，具体内容显示在表格中！")).arg(orderList[i].customerName), QString(tr("确定")));
            }
            else
            {
                ret = false;
                QMessageBox::critical(this, QString(tr("错误")), QString(tr("客户\"%1\"订单添加失败！:%2")).arg(orderList[i].customerName).arg(m_sqlDatabase->getErrorStr()), QString(tr("确定")));
            }
        }
    }
    else
    {
        ret = false;
        qDebug() << "add order reject!";
    }

    return ret;
}

void MainWindow::connectSlots()
{
    connect(m_addCustomerBtn, SIGNAL(clicked(bool)), this, SLOT(onAddCustomerBtn()));
    connect(m_searchCustomerBtn, SIGNAL(clicked(bool)), this, SLOT(onSearchCustomerBtn()));
    connect(m_addOrderBtn, SIGNAL(clicked(bool)), this, SLOT(onAddOrderBtn()));
    connect(m_searchOrderBtn, SIGNAL(clicked(bool)), this, SLOT(onSearchOrderBtn()));

    connect(m_tableWidget, SIGNAL(sigEditCustomerInfo(int, QString)), this, SLOT(onEditCustomerInfo(int, QString)));
    connect(m_tableWidget, SIGNAL(sigEditOrderInfo(int, QString)), this, SLOT(onEditOrderInfo(int, QString)));

    connect(m_tableWidget, SIGNAL(sigNewOrder(QString)), this, SLOT(onNewOrder(QString)));
    connect(m_tableWidget, SIGNAL(sigSearchHistoryOrder(QString)), this, SLOT(onSearchHistoryOrder(QString)));

    connect(m_tableWidget, SIGNAL(sigChangeOrderStatus(QStringList, QString)), this, SLOT(onChangeOrderStatus(QStringList, QString)));
}

void MainWindow::onAddCustomerBtn()
{
    CustomerInfoDialog dialog;

    if(dialog.exec() == QDialog::Accepted)
    {
        //CustomerInfoSql customerInfoSql("Customer");
        qDebug() << "accepted!";
        m_tableWidget->clear();
        m_tableWidget->setDataTypeCustomerInfo();

        QList<CustomerInformation> customerInfoList = dialog.getCustomerInformationList();
        for (int i=0; i<customerInfoList.size(); i++)
        {
            // 插入前要先判断当前客户是否存在，若存在要提示
            if (m_sqlDatabase->customerInfoIsExist(customerInfoList[i].name))
            {
                QMessageBox::critical(this, QString(tr("错误")), QString(tr("数据库中已经存在客户\"%1\"的信息！")).arg(customerInfoList[i].name), QString(tr("确定")));
                continue;
            }

            if (true == m_sqlDatabase->insertCustomerInfo(customerInfoList[i]))
            {
                m_tableWidget->addCustomerInformation(customerInfoList[i]);
                QMessageBox::information(this, QString(tr("提示")), QString(tr("客户\"%1\"信息添加成功，具体内容显示在表格中！")).arg(customerInfoList[i].name), QString(tr("确定")));
            }
            else
            {
                QMessageBox::critical(this, QString(tr("错误")), QString(tr("客户\"%1\"信息添加失败！:%2")).arg(customerInfoList[i].name).arg(m_sqlDatabase->getErrorStr()), QString(tr("确定")));
            }
        }
    }
    else
    {
        qDebug() << "reject!";
    }
}

void MainWindow::onSearchCustomerBtn()
{
    bool ret = false;
    SearchCustomerDialog dialog;

    if(dialog.exec() == QDialog::Accepted)
    {
        QList<CustomerInformation> list;

        m_tableWidget->clear();
        m_tableWidget->setDataTypeCustomerInfo();

        qDebug() << "search accepted!";
        //qDebug() << dialog.getKeyWord() << dialog.getSearchType();
        int searchType = dialog.getSearchType();
        switch (searchType)
        {
        case SearchCustomerDialog::SearchByStatus:
            qDebug() << dialog.getKeyWord();
            ret = m_sqlDatabase->getCustomerInfoByStatus(dialog.getKeyWord(), list);
            break;
        case SearchCustomerDialog::SearchByName:
            ret = m_sqlDatabase->getCustomerInfoByName(dialog.getKeyWord(), list);
            break;
        case SearchCustomerDialog::SearchByInquirySource:
            ret = m_sqlDatabase->getCustomerInfoByInquirySource(dialog.getKeyWord(), list);
            break;
        case SearchCustomerDialog::SearchByCountry:
            ret = m_sqlDatabase->getCustomerInfoByCountry(dialog.getKeyWord(), list);
            break;
        case SearchCustomerDialog::searchByCompany:
            ret = m_sqlDatabase->getCustomerInfoByCompany(dialog.getKeyWord(), list);
            break;
        case SearchCustomerDialog::SearchByEmail:
            ret = m_sqlDatabase->getCustomerInfoByEmail(dialog.getKeyWord(), list);
            break;
        case SearchCustomerDialog::SearchByPhoneNumber:
            ret = m_sqlDatabase->getCustomerInfoByPhoneNumber(dialog.getKeyWord(), list);
            break;
        case SearchCustomerDialog::SearchBySalesman:
            ret = m_sqlDatabase->getCustomerInfoBySalesman(dialog.getKeyWord(), list);
            break;

        default:
            ret = false;
            break;
        }

        if (ret && list.size()>0)
        {
            for (int i=0; i<list.size(); i++)
            {
                m_tableWidget->addCustomerInformation(list[i]);
            }
            QMessageBox::information(this, QString(tr("提示")), QString(tr("查找到%1条客户信息，已显示在表格中！")).arg(list.size()), QString(tr("确定")));
        }
        else
        {
            QMessageBox::critical(this, QString(tr("错误")), QString(tr("没有找到相关的客户信息！")), QString(tr("确定")));
        }
    }
    else
    {
        qDebug() << "search reject!";
    }
}
void MainWindow::onAddOrderBtn()
{
    addNewOrder();
}
void MainWindow::onSearchOrderBtn()
{
    bool ret = false;
    SearchOrderDialog dialog;
    QString startDate, endDate, keyword;
    QDate curDate;

    if(dialog.exec() == QDialog::Accepted)
    {
        qDebug() << "search order accepted!";
        m_tableWidget->clear();
        m_tableWidget->setDataTypeOrderInfo();

        QList<OrderInformation> list;

        int searchType = dialog.getSearchType();
        switch (searchType) {
        case SEARCH_ALL_ORDER:     // 所有订单
            ret = m_sqlDatabase->getAllOrderInfo(list);
            break;
        case SEARCH_BY_UNPAY_PROFIT:
            ret = m_sqlDatabase->getOrderInfoByStatus("未结算", list);
            break;
        case SEARCH_BY_PAYED_PROFIT:
            ret = m_sqlDatabase->getOrderInfoByStatus("已结算", list);
            break;
        case SEARCH_BY_LAST_MONTH_ORDER:
            curDate = QDate::currentDate();
            curDate = curDate.addMonths(-1);
            startDate = QDate(curDate.year(), curDate.month(),1).toString("yyyy-MM-dd");
            endDate = QDate(curDate.year(), curDate.month(), curDate.daysInMonth()).toString("yyyy-MM-dd");

            ret = m_sqlDatabase->getOrderInfoByDateRange(startDate, endDate, list);
            break;
        case SEARCH_BY_DATE_RANGE:
            startDate = dialog.getStartDate();
            endDate = dialog.getEndDate();
            ret = m_sqlDatabase->getOrderInfoByDateRange(startDate, endDate, list);
            break;
        case SEARCH_BY_SALESMAN:
            keyword = dialog.getKeyWord();
            ret = m_sqlDatabase->getOrderInfoBySalesman(keyword, list);
            break;
        case SEARCH_BY_ORDERID:
            keyword = dialog.getKeyWord();
            ret = m_sqlDatabase->getOrderInfoByOrderID(keyword, list);
            break;
        case SEARCH_BY_CUSTOMER_NAME:
            keyword = dialog.getKeyWord();
            ret = m_sqlDatabase->getOrderInfoByCustomerName(keyword, list);
            break;

        default:
            ret = false;
            break;
        }

        if (ret && list.size()>0)
        {
            for (int i=0; i<list.size(); i++)
            {
                list[i].calProfitIncomeAndExpenses();
                m_tableWidget->addOrderInformation(list[i]);
            }
            QMessageBox::information(this, QString(tr("提示")), QString(tr("查找到%1条订单信息，已显示在表格中！！").arg(list.size())), QString(tr("确定")));
        }
        else
        {
            QMessageBox::critical(this, QString(tr("错误")), QString(tr("没有找到相关的订单信息！")), QString(tr("确定")));
        }
    }
    else
    {
        qDebug() << "search order reject!";
    }
}

void MainWindow::onEditCustomerInfo(int row, const QString& name)
{
    CustomerInfoDialog dialog;

    QList<CustomerInformation> list;

    if (false == m_sqlDatabase->getCustomerInfoByName(name, list))
    {
        QMessageBox::critical(this, QString(tr("错误")), QString(tr("从数据库中读取客户信息\"%1\"失败！")).arg(name), QString(tr("确定")));
        return;
    }

    dialog.setCustomerNameEditDisable();
    if (list.size() > 0)
    {
        dialog.setCustomerInfomation(list[0]);
    }

    if(dialog.exec() == QDialog::Accepted)
    {
        // save to database
        QList<CustomerInformation> customerInfoList = dialog.getCustomerInformationList();
        for (int i=0; i<customerInfoList.size(); i++)
        {
            if (true == m_sqlDatabase->updateCustomerInfo(customerInfoList[i]))
            {
                m_tableWidget->updateCustomerInformation(row, customerInfoList[i]);
                QMessageBox::information(this, QString(tr("提示")), QString(tr("更新客户\"%1\"信息到数据库成功！")).arg(customerInfoList[i].name), QString(tr("确定")));
            }
            else
            {
                QMessageBox::critical(this, QString(tr("错误")), QString(tr("更新客户\"%1\"信息到数据库失败！")).arg(customerInfoList[i].name), QString(tr("确定")));
            }
        }
    }
}

void MainWindow::onEditOrderInfo(int row, const QString& orderID)
{
    qDebug() << row << orderID;
    QList<OrderInformation> list;

    if (false == m_sqlDatabase->getOrderInfoByOrderID(orderID, list))
    {
        QMessageBox::critical(this, QString(tr("错误")), QString(tr("从数据库中读取订单\"%1\"失败！")).arg(orderID), QString(tr("确定")));
        return;
    }

    OrderDialog dialog;
    dialog.setOrderInfo(list[0]);

    if(dialog.exec() == QDialog::Accepted)
    {
        qDebug() << "edit order accepted!";

        QList<OrderInformation> orderList = dialog.getOrderList();
        for (int i=0; i<orderList.size(); i++)
        {
            if (true == m_sqlDatabase->updateOrderInfo(orderList[i]))
            {
                m_tableWidget->updateOrderInformation(row, orderList[i]);
                QMessageBox::information(this, QString(tr("提示")), QString(tr("客户\"%1\"订单更新成功，具体内容显示在表格中！")).arg(orderList[i].customerName), QString(tr("确定")));
            }
            else
            {
                QMessageBox::critical(this, QString(tr("错误")), QString(tr("客户\"%1\"订单更新失败！:%2")).arg(orderList[i].customerName).arg(m_sqlDatabase->getErrorStr()), QString(tr("确定")));
            }
        }
    }
}

void MainWindow::onSearchHistoryOrder(const QString &name)      // 查找历史订单
{
    QList<OrderInformation> list;

    bool ret = m_sqlDatabase->getOrderInfoByCustomerName(name, list);

    if (ret && list.size()>0)
    {
        m_tableWidget->clear();
        m_tableWidget->setDataTypeOrderInfo();

        for (int i=0; i<list.size(); i++)
        {
            list[i].calProfitIncomeAndExpenses();
            m_tableWidget->addOrderInformation(list[i]);
            //qDebug() << list[i].shouldIncome;
        }
        QMessageBox::information(this, QString(tr("提示")), QString(tr("订单信息查找成功，具体内容显示在表格中！")), QString(tr("确定")));
    }
    else
    {
        QMessageBox::critical(this, QString(tr("错误")), QString(tr("没有找到相关的订单信息！")), QString(tr("确定")));
    }
}

void MainWindow::onNewOrder(const QString &name)                // 新建订单
{
    qDebug() << name;
    addNewOrder(name);
}

void MainWindow::onChangeOrderStatus(const QStringList &orderList, const QString& status)  // 修改订单状态为“已结算利润”
{
    if (!orderList.isEmpty())
    {
        bool ret = m_sqlDatabase->updateOrderStatus(orderList, status);
        if (ret)
        {
            m_tableWidget->updateOrderStatus(orderList, status);
            QMessageBox::information(this, QString(tr("提示")), QString(tr("订单状态更新成功！")), QString(tr("确定")));
        }
        else
        {
            QMessageBox::critical(this, QString(tr("错误")), QString(tr("订单状态更新失败：%1！")).arg(m_sqlDatabase->getErrorStr()), QString(tr("确定")));
        }
    }
}
