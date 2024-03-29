#include "MainWindow.h"
#include "CustomerInfoDialog.h"
#include "CustomerInformation.h"
#include "SearchCustomerDialog.h"
#include "OrderDialog.h"
#include "SearchOrderDialog.h"
#include "FinancialRecordDialog.h"
#include "SearchFinancialRecordDialog.h"
#include <QMessageBox>
#include <QDebug>
#include <QDate>

void MainWindow::setStatusText(const QString& text)
{
    m_statusLabel->setText(text);
}

bool MainWindow::addNewOrder(const QString& customerName)
{
    bool ret = false;
    OrderDialog dialog;

    connect(&dialog, SIGNAL(sigAddFinancialRecord(QString,QString)), this, SLOT(onAddFinancialRecord(QString,QString)));

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
                //QMessageBox::information(this, QString(tr("提示")), QString(tr("客户\"%1\"订单添加成功，具体内容显示在表格中！")).arg(orderList[i].customerName), QString(tr("确定")));
                QString okStr = QString(tr("客户\"%1\"订单添加成功，具体内容显示在表格中！")).arg(orderList[i].customerName);
                setStatusText(okStr);
            }
            else
            {
                ret = false;
                QString errStr = QString(tr("客户\"%1\"订单添加失败！:%2")).arg(orderList[i].customerName, m_sqlDatabase->getErrorStr());
                QMessageBox::critical(this, QString(tr("错误")), errStr, QString(tr("确定")));
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

bool MainWindow::addNewFinancialRecord(const QString& customerName, const QString& contractID, bool flag)
{
    bool ret = false;
    FinancialRecordDialog dialog;

    if (!contractID.isEmpty())
    {
        dialog.setcontractID(contractID);
        dialog.disablecontractIDEdit();
    }

    if (!customerName.isEmpty())
    {
        dialog.setCustomerName(customerName);
        dialog.disableCustomerNameEdit();
    }

    if(dialog.exec() == QDialog::Accepted)
    {
        //qDebug() << "add order accepted!";

        FinancialRecordInfo info = dialog.getFinancialRecordInfo();

        /*qDebug() << "addNewFinancialRecord()";
        qDebug() << info.number;
        qDebug() << info.contractID;
        qDebug() << info.customerName;
        qDebug() << info.type;
        qDebug() << info.amount;
        qDebug() << info.payTime;
        qDebug() << info.payType;
        qDebug() << info.remarks;
        */

        if (true == m_sqlDatabase->insertFinacialInfo(info))
        {
            ret = true;
            if (true == flag)
            {
                m_tableWidget->clear();
                m_tableWidget->setDataTypeFinancialInfo();

                m_tableWidget->addFinancialInfo(info);
                //QMessageBox::information(this, QString(tr("提示")), QString(tr("订单\"%1\"收支记录添加成功，具体内容显示在表格中！")).arg(info.contractID), QString(tr("确定")));
            }

            QString okStr = QString(tr("订单\"%1\"收支记录添加成功！")).arg(info.contractID);
            setStatusText(okStr);
        }
        else
        {
            ret = false;
            QString errStr = QString(tr("订单\"%1\"收支记录添加失败！:%2")).arg(info.contractID, m_sqlDatabase->getErrorStr());
            QMessageBox::critical(this, QString(tr("错误")), errStr, QString(tr("确定")));
        }
    }
    else
    {
        ret = false;
        qDebug() << "add financial record reject!";
    }

    return ret;
}

void MainWindow::searchFinancialRecord(int type, const QString& keyword, bool showDataInNewDialog)
{
    bool ret = false;
    QList<FinancialRecordInfo> list;

    switch (type)
    {
    case SearchFinancialRecordDialog::SearchByCustomerName:
        ret = m_sqlDatabase->getFinacialInfoByCustomerName(keyword, list);
        break;
    case SearchFinancialRecordDialog::SearchByContractID:
        ret = m_sqlDatabase->getFinacialInfoByContractID(keyword, list);
        break;
    default:
        break;
    }

    QString okStr = QString(tr("查找到%1条交易记录信息，已显示在表格中！")).arg(list.size());

    if (showDataInNewDialog)
    {
        //TableWidgetDialog dialog;
        m_tableWidgetDialog->setWindowTitle(tr("查找收支记录"));
        m_tableWidgetDialog->getTableWidget()->clear();
        m_tableWidgetDialog->getTableWidget()->setDataTypeFinancialInfo();
        if (ret && list.size()>0)
        {
            for (int i=0; i<list.size(); i++)
            {
                m_tableWidgetDialog->getTableWidget()->addFinancialInfo(list[i]);
            }

            m_tableWidgetDialog->getTableWidget()->addFinancialStatistics();

            m_tableWidgetDialog->setStatusText(okStr);
            m_tableWidgetDialog->exec();
        }
        else
        {
            QMessageBox::critical(this, QString(tr("错误")), QString(tr("没有找到相关的交易记录信息！")), QString(tr("确定")));
        }
    }
    else
    {
        m_tableWidget->clear();
        m_tableWidget->setDataTypeFinancialInfo();

        if (ret && list.size()>0)
        {
            for (int i=0; i<list.size(); i++)
            {
                m_tableWidget->addFinancialInfo(list[i]);
            }

            m_tableWidget->addFinancialStatistics();
            //QMessageBox::information(this, QString(tr("提示")), okStr, QString(tr("确定")));
            setStatusText(okStr);
        }
        else
        {
            QMessageBox::critical(this, QString(tr("错误")), QString(tr("没有找到相关的交易记录信息！")), QString(tr("确定")));
        }
    }
}

double MainWindow::getRealIncome(const QString& contractID)
{
    if (contractID.isEmpty())
    {
        qDebug() << "MainWindow::getRealIncome " << "contractID is empty!";
        return -1;
    }

    SqlDatabase sql("record");
    QList<FinancialRecordInfo> list;
    if (false == sql.getFinacialInfoByContractID(contractID, list))
    {
        qDebug() << "MainWindow::getRealIncome " << "获取收支记录失败！";
        return -1;
    }

    double realIncome = 0;
    for (int i=0; i<list.size(); i++)
    {
        if (tr("收入") == list[i].type)
        {
            realIncome += list[i].amount;
        }
    }

    return realIncome;
}

void MainWindow::connectSlots()
{
    connect(m_addCustomerBtn, SIGNAL(clicked(bool)), this, SLOT(onAddCustomerBtn()));
    connect(m_searchCustomerBtn, SIGNAL(clicked(bool)), this, SLOT(onSearchCustomerBtn()));

    connect(m_addOrderBtn, SIGNAL(clicked(bool)), this, SLOT(onAddOrderBtn()));
    connect(m_searchOrderBtn, SIGNAL(clicked(bool)), this, SLOT(onSearchOrderBtn()));

    connect(m_addFinancialRecordBtn, SIGNAL(clicked(bool)), this, SLOT(onAddFinancialRecordBtn()));
    connect(m_searchFinancialRecordBtn, SIGNAL(clicked(bool)), this, SLOT(onSearchFinancialRecordBtn()));
    connect(m_tableWidget, SIGNAL(sigNewFinancialRecord(QString,QString)), this, SLOT(onAddFinancialRecord(QString,QString)));
    connect(m_tableWidget, SIGNAL(sigSearchFinancialByCustomerName(QString)), this, SLOT(onSearchFinancialByCustomerName(QString)));
    connect(m_tableWidget, SIGNAL(sigSearchFinancialByContractID(QString)), this, SLOT(onSearchFinancialBycontractID(QString)));

    connect(m_tableWidget, SIGNAL(sigEditCustomerInfo(int,QString)), this, SLOT(onEditCustomerInfo(int,QString)));
    connect(m_tableWidget, SIGNAL(sigEditOrderInfo(int,QString)), this, SLOT(onEditOrderInfo(int,QString)));
    connect(m_tableWidget, SIGNAL(sigEditFinancialInfo(int,int,TableWidget*)), this, SLOT(onEditFinancialInfo(int,int,TableWidget*)));

    connect(m_tableWidgetDialog->getTableWidget(), SIGNAL(sigEditFinancialInfo(int,int,TableWidget*)), this, SLOT(onEditFinancialInfo(int,int,TableWidget*)));
    connect(m_tableWidgetDialog->getTableWidget(), SIGNAL(sigNewFinancialRecord(QString,QString)), this, SLOT(onAddFinancialRecord(QString,QString)));
    connect(m_tableWidgetDialog->getTableWidget(), SIGNAL(sigSearchFinancialByCustomerName(QString)), this, SLOT(onSearchFinancialByCustomerName(QString)));
    connect(m_tableWidgetDialog->getTableWidget(), SIGNAL(sigSearchFinancialByContractID(QString)), this, SLOT(onSearchFinancialBycontractID(QString)));

    connect(m_tableWidget, SIGNAL(sigNewOrder(QString)), this, SLOT(onNewOrder(QString)));
    connect(m_tableWidget, SIGNAL(sigSearchHistoryOrder(QString)), this, SLOT(onSearchHistoryOrder(QString)));

    connect(m_tableWidget, SIGNAL(sigChangeOrderStatus(QStringList,QString)), this, SLOT(onChangeOrderStatus(QStringList,QString)));
}

void MainWindow::onAddCustomerBtn()
{
    CustomerInfoDialog dialog;

    if(dialog.exec() == QDialog::Accepted)
    {
        //CustomerInfoSql customerInfoSql("Customer");
        //qDebug() << "accepted!";
        m_tableWidget->clear();
        m_tableWidget->setDataTypeCustomerInfo();

        QList<CustomerInformation> customerInfoList = dialog.getCustomerInformationList();
        for (int i=0; i<customerInfoList.size(); i++)
        {
            // 插入前要先判断当前客户是否存在，若存在要提示
            if (m_sqlDatabase->customerInfoIsExist(customerInfoList[i].name))
            {
                QString errStr = QString(tr("数据库中已经存在客户\"%1\"的信息！")).arg(customerInfoList[i].name);
                QMessageBox::critical(this, QString(tr("错误")), errStr, QString(tr("确定")));
                continue;
            }

            if (true == m_sqlDatabase->insertCustomerInfo(customerInfoList[i]))
            {
                m_tableWidget->addCustomerInformation(customerInfoList[i]);
                //QMessageBox::information(this, QString(tr("提示")), QString(tr("客户\"%1\"信息添加成功，具体内容显示在表格中！")).arg(customerInfoList[i].name), QString(tr("确定")));
                QString okStr = QString(tr("客户\"%1\"信息添加成功，具体内容显示在表格中！")).arg(customerInfoList[i].name);
                setStatusText(okStr);
            }
            else
            {
                QString errStr = QString(tr("客户\"%1\"信息添加失败！:%2")).arg(customerInfoList[i].name, m_sqlDatabase->getErrorStr());
                QMessageBox::critical(this, QString(tr("错误")), errStr, QString(tr("确定")));
            }
        }
    }
    else
    {
        qDebug() << "<onAddCustomerBtn> reject!";
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
        QString keyword = dialog.getKeyWord().trimmed();
        switch (searchType)
        {
        case SearchCustomerDialog::SearchByStatus:
            qDebug() << dialog.getKeyWord();
            ret = m_sqlDatabase->getCustomerInfoByStatus(keyword, list);
            break;
        case SearchCustomerDialog::SearchByName:
            ret = m_sqlDatabase->getCustomerInfoByName(keyword, list);
            break;
        case SearchCustomerDialog::SearchByInquirySource:
            ret = m_sqlDatabase->getCustomerInfoByInquirySource(keyword, list);
            break;
        case SearchCustomerDialog::SearchByCountry:
            ret = m_sqlDatabase->getCustomerInfoByCountry(keyword, list);
            break;
        case SearchCustomerDialog::searchByCompany:
            ret = m_sqlDatabase->getCustomerInfoByCompany(keyword, list);
            break;
        case SearchCustomerDialog::SearchByEmail:
            ret = m_sqlDatabase->getCustomerInfoByEmail(keyword, list);
            break;
        case SearchCustomerDialog::SearchByPhoneNumber:
            ret = m_sqlDatabase->getCustomerInfoByPhoneNumber(keyword, list);
            break;
        case SearchCustomerDialog::SearchBySalesman:
            ret = m_sqlDatabase->getCustomerInfoBySalesman(keyword, list);
            break;
        case SearchCustomerDialog::SearchByKeyword:
            ret = m_sqlDatabase->getCustomerInfoByKeyword(keyword, list);
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
            //QMessageBox::information(this, QString(tr("提示")), QString(tr("查找到%1条客户信息，已显示在表格中！")).arg(list.size()), QString(tr("确定")));
            QString okStr = QString(tr("查找到%1条客户信息，已显示在表格中！")).arg(list.size());
            setStatusText(okStr);
        }
        else
        {
            QMessageBox::critical(this, QString(tr("错误")), QString(tr("没有找到相关的客户信息！")), QString(tr("确定")));
        }
    }
    else
    {
        qDebug() << "<onSearchCustomerBtn> reject!";
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
        case SearchOrderDialog::SearchAllOrder:     // 所有订单
            ret = m_sqlDatabase->getAllOrderInfo(list);
            break;
        case SearchOrderDialog::SearchByUnpayProfit:
            ret = m_sqlDatabase->getOrderInfoByStatus("未结算", list);
            break;
        case SearchOrderDialog::SearchByPayedProfit:
            ret = m_sqlDatabase->getOrderInfoByStatus("已结算", list);
            break;
        case SearchOrderDialog::SearchByLastMonthOrder:
            curDate = QDate::currentDate();
            curDate = curDate.addMonths(-1);
            startDate = QDate(curDate.year(), curDate.month(),1).toString("yyyy-MM-dd");
            endDate = QDate(curDate.year(), curDate.month(), curDate.daysInMonth()).toString("yyyy-MM-dd");

            ret = m_sqlDatabase->getOrderInfoByDateRange(startDate, endDate, list);
            break;
        case SearchOrderDialog::SearchByDateRange:
            startDate = dialog.getStartDate();
            endDate = dialog.getEndDate();
            ret = m_sqlDatabase->getOrderInfoByDateRange(startDate, endDate, list);
            break;
        case SearchOrderDialog::SearchBySalesman:
            keyword = dialog.getKeyWord();
            ret = m_sqlDatabase->getOrderInfoBySalesman(keyword, list);
            break;
        case SearchOrderDialog::SearchByContractid:
            keyword = dialog.getKeyWord();
            ret = m_sqlDatabase->getOrderInfoByContractID(keyword, list);
            break;
        case SearchOrderDialog::SearchByCustomerName:
            keyword = dialog.getKeyWord();
            ret = m_sqlDatabase->getOrderInfoByCustomerName(keyword, list);
            break;
        case SearchOrderDialog::SearchByNotShipped:
            ret = m_sqlDatabase->getOrderInfoByStatus("待发货", list);
            break;
        case SearchOrderDialog::SearchByShipped:
            ret = m_sqlDatabase->getOrderInfoByStatus("已发货", list);
            break;
        default:
            ret = false;
            break;
        }

        if (ret && list.size()>0)
        {
            for (int i=0; i<list.size(); i++)
            {
                double realIncome = getRealIncome(list[i].contractID);
                if (realIncome > 0)
                {
                    list[i].realIncome = realIncome;
                }
                list[i].calProfitIncomeAndExpenses();
                m_tableWidget->addOrderInformation(list[i]);
            }
            //qDebug() << OrderInformation::totalProfitSum << OrderInformation::partnerProfitSum;
            m_tableWidget->addOrderStatistics();

            //QMessageBox::information(this, QString(tr("提示")), QString(tr("查找到%1条订单信息，已显示在表格中！！").arg(list.size())), QString(tr("确定")));
            QString okStr = QString(tr("查找到%1条订单信息，已显示在表格中！！").arg(list.size()));
            setStatusText(okStr);
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


void MainWindow::onAddFinancialRecord(const QString& name, const QString& contractID)
{
    addNewFinancialRecord(name, contractID, false);
}

void MainWindow::onAddFinancialRecordBtn()
{
    addNewFinancialRecord("", "", true);
}

void MainWindow::onSearchFinancialRecordBtn()
{
    SearchFinancialRecordDialog dialog;    

    if(dialog.exec() == QDialog::Accepted)
    {
        int searchType = dialog.getSearchType();
        QString keyword = dialog.getKeyWord();

        searchFinancialRecord(searchType, keyword);
    }
}

void MainWindow::onSearchFinancialByCustomerName(const QString& name)
{
    searchFinancialRecord(SearchFinancialRecordDialog::SearchByCustomerName, name, true);
}

void MainWindow::onSearchFinancialBycontractID(const QString& contractID)
{
    searchFinancialRecord(SearchFinancialRecordDialog::SearchByContractID, contractID, true);
}

void MainWindow::onEditFinancialInfo(int row, int number, TableWidget* tableWidget)
{
    qDebug() << "onEditFinancialInfo" << row << number;
    QList<FinancialRecordInfo> list;

    if (false == m_sqlDatabase->getFinacialInfoByNumber(number, list))
    {
        QString errStr = QString(tr("从数据库中读取记录\"%1\"失败！")).arg(number);
        QMessageBox::critical(this, QString(tr("错误")), errStr, QString(tr("确定")));
        return;
    }

    FinancialRecordDialog dialog;
    dialog.setWindowTitle(tr("编辑收支记录"));
    dialog.setFinancialRecordInfo(list[0]);

    if(dialog.exec() == QDialog::Accepted)
    {
        //qDebug() << "edit order accepted!";

        FinancialRecordInfo info = dialog.getFinancialRecordInfo();
        info.number = number;
        if (true == m_sqlDatabase->updateFinacialInfo(info))
        {
            tableWidget->updateFinancialInfo(row, info);
            tableWidget->updateFinancialStatistics();
            QString okStr = QString(tr("订单\"%1\"收支记录更新成功，具体内容显示在表格中！")).arg(info.contractID);
            QMessageBox::information(tableWidget, QString(tr("提示")), okStr, QString(tr("确定")));
            //setStatusText(okStr);
        }
        else
        {
            QString errStr = QString(tr("订单\"%1\"收支记录更新失败:%2")).arg(info.contractID, m_sqlDatabase->getErrorStr());
            QMessageBox::critical(this, QString(tr("错误")), errStr, QString(tr("确定")));
        }
    }
}

void MainWindow::onEditCustomerInfo(int row, const QString& name)
{
    CustomerInfoDialog dialog;

    QList<CustomerInformation> list;

    if (false == m_sqlDatabase->getCustomerInfoByName(name, list))
    {
        QString errStr = QString(tr("从数据库中读取客户信息\"%1\"失败！")).arg(name);
        QMessageBox::critical(this, QString(tr("错误")), errStr, QString(tr("确定")));
        return;
    }

    dialog.setWindowTitle(tr("编辑客户资料"));
    dialog.setCustomerNameEditDisable();
    if (list.size() > 0)
    {
        for (int i=0; i<list.size(); i++)
        {
            if (name == list[i].name)
            {
                dialog.setCustomerInfomation(list[i]);
            }
            else
            {
                QString errStr = QString(tr("数据库中未找到客户\"%1\"信息！")).arg(name);
                QMessageBox::critical(this, QString(tr("错误")), errStr, QString(tr("确定")));
                return;
            }
        }
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
                //QMessageBox::information(this, QString(tr("提示")), QString(tr("更新客户\"%1\"信息到数据库成功！")).arg(customerInfoList[i].name), QString(tr("确定")));
                QString okStr = QString(tr("更新客户\"%1\"信息到数据库成功！")).arg(customerInfoList[i].name);
                setStatusText(okStr);
            }
            else
            {
                QString errStr = QString(tr("更新客户\"%1\"信息到数据库失败！")).arg(customerInfoList[i].name);
                QMessageBox::critical(this, QString(tr("错误")), errStr, QString(tr("确定")));
            }
        }
    }
}

void MainWindow::onEditOrderInfo(int row, const QString& orderID)
{
    //qDebug() << row << orderID;
    QList<OrderInformation> list;

    if (false == m_sqlDatabase->getOrderInfoByOrderID(orderID, list))
    {
        QString errStr = QString(tr("从数据库中读取订单\"%1\"失败！")).arg(orderID);
        QMessageBox::critical(this, QString(tr("错误")), errStr, QString(tr("确定")));
        return;
    }

    OrderDialog dialog;
    dialog.setWindowTitle(tr("编辑订单"));
    dialog.setOrderInfo(list[0]);

    connect(&dialog, SIGNAL(sigAddFinancialRecord(QString,QString)), this, SLOT(onAddFinancialRecord(QString,QString)));

    if(dialog.exec() == QDialog::Accepted)
    {
        qDebug() << "edit order accepted!";

        QList<OrderInformation> orderList = dialog.getOrderList();
        for (int i=0; i<orderList.size(); i++)
        {
            if (true == m_sqlDatabase->updateOrderInfo(orderList[i]))
            {
                m_tableWidget->updateOrderInformation(row, orderList[i]);
                m_tableWidget->updateOrderStatistics();
                //QMessageBox::information(this, QString(tr("提示")), QString(tr("客户\"%1\"订单更新成功，具体内容显示在表格中！")).arg(orderList[i].customerName), QString(tr("确定")));
                QString okStr = QString(tr("客户\"%1\"订单更新成功，具体内容显示在表格中！")).arg(orderList[i].customerName);
                setStatusText(okStr);
            }
            else
            {
                QString errStr = QString(tr("客户\"%1\"订单更新失败！:%2")).arg(orderList[i].customerName, m_sqlDatabase->getErrorStr());
                QMessageBox::critical(this, QString(tr("错误")), errStr, QString(tr("确定")));
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
            double realIncome = getRealIncome(list[i].contractID);
            if (realIncome > 0)
            {
                list[i].realIncome = realIncome;
            }

            list[i].calProfitIncomeAndExpenses();
            m_tableWidget->addOrderInformation(list[i]);
            //qDebug() << list[i].shouldIncome;
        }
        //QMessageBox::information(this, QString(tr("提示")), QString(tr("订单信息查找成功，具体内容显示在表格中！")), QString(tr("确定")));
        QString okStr = QString(tr("订单信息查找成功，具体内容显示在表格中！"));
        setStatusText(okStr);
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
            //QMessageBox::information(this, QString(tr("提示")), QString(tr("订单状态更新成功！")), QString(tr("确定")));
            QString okStr = QString(tr("订单状态更新成功！"));
            setStatusText(okStr);
        }
        else
        {
            QString errStr = QString(tr("订单状态更新失败：%1！")).arg(m_sqlDatabase->getErrorStr());
            QMessageBox::critical(this, QString(tr("错误")), errStr, QString(tr("确定")));
        }
    }
}
