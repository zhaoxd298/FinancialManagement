#include "MainWindow.h"
#include "CustomerInfoDialog.h"
#include "customerinformation.h"
#include "SearchCustomerDialog.h"
#include "OrderDialog.h"
#include "SearchOrderDialog.h"
#include <QMessageBox>
#include <QDebug>
#include <QDate>

void MainWindow::connectSlots()
{
    connect(m_addCustomerBtn, SIGNAL(clicked(bool)), this, SLOT(onAddCustomerBtn()));
    connect(m_searchCustomerBtn, SIGNAL(clicked(bool)), this, SLOT(onSearchCustomerBtn()));
    connect(m_addOrderBtn, SIGNAL(clicked(bool)), this, SLOT(onAddOrderBtn()));
    connect(m_searchOrderBtn, SIGNAL(clicked(bool)), this, SLOT(onSearchOrderBtn()));
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
        case 0:
            qDebug() << dialog.getKeyWord();
            ret = m_sqlDatabase->getCustomerInfoByStatus(dialog.getKeyWord(), list);
            break;
        case 1:
            ret = m_sqlDatabase->getCustomerInfoByName(dialog.getKeyWord(), list);
            break;
        case 2:
            ret = m_sqlDatabase->getCustomerInfoByInquirySource(dialog.getKeyWord(), list);
            break;
        case 3:
            ret = m_sqlDatabase->getCustomerInfoByCountry(dialog.getKeyWord(), list);
            break;
        case 4:
            ret = m_sqlDatabase->getCustomerInfoByCompany(dialog.getKeyWord(), list);
            break;
        case 5:
            ret = m_sqlDatabase->getCustomerInfoByEmail(dialog.getKeyWord(), list);
            break;
        case 6:
            ret = m_sqlDatabase->getCustomerInfoByPhoneNumber(dialog.getKeyWord(), list);
            break;
        case 7:
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
            QMessageBox::information(this, QString(tr("提示")), QString(tr("客户信息查找成功，具体内容显示在表格中！")), QString(tr("确定")));
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
    OrderDialog dialog;

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
                m_tableWidget->addOrderInformation(orderList[i]);
                QMessageBox::information(this, QString(tr("提示")), QString(tr("客户\"%1\"订单添加成功，具体内容显示在表格中！")).arg(orderList[i].customerName), QString(tr("确定")));
            }
            else
            {
                QMessageBox::critical(this, QString(tr("错误")), QString(tr("客户\"%1\"订单添加失败！:%2")).arg(orderList[i].customerName).arg(m_sqlDatabase->getErrorStr()), QString(tr("确定")));
            }
        }
    }
    else
    {
        qDebug() << "add order reject!";
    }
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
        case 0:
            ret = m_sqlDatabase->getAllOrderInfo(list);
            break;
        case 1:
            curDate = QDate::currentDate();
            curDate = curDate.addMonths(-1);
            startDate = QDate(curDate.year(), curDate.month(),1).toString("yyyy-MM-dd");
            endDate = QDate(curDate.year(), curDate.month(), curDate.daysInMonth()).toString("yyyy-MM-dd");

            ret = m_sqlDatabase->getOrderInfoByDateRange(startDate, endDate, list);
            break;
        case 2:
            startDate = dialog.getStartDate();
            endDate = dialog.getEndDate();
            ret = m_sqlDatabase->getOrderInfoByDateRange(startDate, endDate, list);
            break;
        case 3:
            keyword = dialog.getKeyWord();
            ret = m_sqlDatabase->getOrderInfoBySalesman(keyword, list);
            break;
        case 4:
            keyword = dialog.getKeyWord();
            ret = m_sqlDatabase->getOrderInfoByOrderID(keyword, list);
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
                //qDebug() << list[i].orderID;
            }
            QMessageBox::information(this, QString(tr("提示")), QString(tr("订单信息查找成功，具体内容显示在表格中！")), QString(tr("确定")));
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
