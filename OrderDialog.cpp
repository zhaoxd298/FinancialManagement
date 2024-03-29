#include "OrderDialog.h"
#include <QMessageBox>
#include <QDateTime>
#include <QList>
#include <QDebug>
#include "Version.h"
#include "SqlDatabase.h"
#include "FinancialRecordInfo.h"

OrderDialog::OrderDialog(QWidget *parent)
    : QDialog(parent, Qt::WindowCloseButtonHint)
{
    m_newOrderFlag = 1;

    constructUI();
    connectSlot();

    setRealIncome();
}

void OrderDialog::constructUI()
{
    m_gridLayout = new QGridLayout(this);

    QLabel* orderIDLabel = new QLabel(tr("订单编号："));
    m_orderIDEdit = new QLineEdit;
    m_orderIDEdit->setEnabled(false);
    QDateTime dateTime(QDateTime::currentDateTime());
    m_orderIDEdit->setText(dateTime.toString("yyyyMMddhhmmsszzz"));
    m_gridLayout->addWidget(orderIDLabel, 0, 0, 1, 1);
    m_gridLayout->addWidget(m_orderIDEdit, 0, 1, 1, 4);

    QLabel* contractIDLabel = new QLabel(tr("合同编号："));
    m_contractIDEdit = new QLineEdit;
    m_gridLayout->addWidget(contractIDLabel, 1, 0, 1, 1);
    m_gridLayout->addWidget(m_contractIDEdit, 1, 1, 1, 4);

    QLabel* customerNameLabel = new QLabel(tr("客户："));
    m_customerNameEdit = new QLineEdit;
    m_gridLayout->addWidget(customerNameLabel, 2, 0, 1, 1);
    m_gridLayout->addWidget(m_customerNameEdit, 2, 1, 1, 4);

    QLabel* orderStatusLabel = new QLabel(tr("订单状态："));
    m_orderStatusCbx = new QComboBox;
    QStringList items;
    items << "已付定金" << "已付全款" << "待发货" << "已发货" << "未结算利润" << "已结算利润";
    m_orderStatusCbx->addItems(items);
    m_gridLayout->addWidget(orderStatusLabel, 3, 0, 1, 1);
    m_gridLayout->addWidget(m_orderStatusCbx, 3, 1, 1, 4);

#if 0
    QLabel* payTimeLabel = new QLabel(tr("付款时间："));
    m_payTimeEdit = new QDateTimeEdit(QDateTime::currentDateTime());
    m_payTimeEdit->setCalendarPopup(true);
    m_payTimeEdit->setDisplayFormat("yyyy-MM-dd");
    m_gridLayout->addWidget(payTimeLabel, 4, 0, 1, 1);
    m_gridLayout->addWidget(m_payTimeEdit, 4, 1, 1, 3);

    QLabel* payTypeLabel = new QLabel(tr("付款方式："));
    m_payTypeCbx = new QComboBox;
    items.clear();
#if defined(EXSUN_LIGHTING_FINANCIAL)
    items << "信保" << "微信" << "支付宝" << "大陆美金账户" << "香港账户" << "西联汇款" << "Paypal" << "其他";
#elif defined(REVI_FINANCIAL)
    items << "中国银行" << "招商银行" << "建设银行" << "农业银行" << "广发银行" << "交通银行" << "XT"
          << "信保" << "微信" << "支付宝" << "义乌个体户" << "大陆美金账户" << "香港账户"
          << "西联汇款" << "Paypal" << "其他";
#endif
    m_payTypeCbx->addItems(items);
    m_payTypeCbx->setMaxVisibleItems(20);
    m_otherPayTypeEdit = new QLineEdit;
    //m_otherPayTypeEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    m_gridLayout->addWidget(payTypeLabel, 5, 0, 1, 1);
    m_gridLayout->addWidget(m_payTypeCbx, 5, 1, 1, 3);
    m_gridLayout->addWidget(m_otherPayTypeEdit, 5, 4, 1, 2);
#endif

    QLabel* realIncomeLabel = new QLabel(tr("实收(RMB)："));
    m_realIncomeEdit = new QLineEdit;
    m_realIncomeEdit->setText("0");
    m_realIncomeEdit->setEnabled(false);
    m_gridLayout->addWidget(realIncomeLabel, 6, 0, 1, 1);
    m_gridLayout->addWidget(m_realIncomeEdit, 6, 1, 1, 4);

    m_financialRecordBtn = new QPushButton(tr("新增收支记录"));
    m_gridLayout->addWidget(m_financialRecordBtn, 6, 5, 1, 1);

    QLabel* freightCustomerLabel = new QLabel(tr("运费(应收客户)(RMB)："));
    m_freightCustomerEdit = new QLineEdit;
    m_freightCustomerEdit->setText("0");
    m_gridLayout->addWidget(freightCustomerLabel, 7, 0, 1, 1);
    m_gridLayout->addWidget(m_freightCustomerEdit, 7, 1, 1, 4);

    QLabel* freightFactoryToUsLabel = new QLabel(tr("运费(工厂→我司)(RMB)："));
    m_freightFactoryToUsEdit = new QLineEdit;
    m_freightFactoryToUsEdit->setText("0");
    m_gridLayout->addWidget(freightFactoryToUsLabel, 8, 0, 1, 1);
    m_gridLayout->addWidget(m_freightFactoryToUsEdit, 8, 1, 1, 4);

    QLabel* freightUsToForwardingLabel = new QLabel(tr("运费(我司→货代)(RMB)："));
    m_freightUsToForwardingEdit = new QLineEdit;
    m_freightUsToForwardingEdit->setText("0");
    m_gridLayout->addWidget(freightUsToForwardingLabel, 9, 0, 1, 1);
    m_gridLayout->addWidget(m_freightUsToForwardingEdit, 9, 1, 1, 4);

    QLabel* freightForeignLabel = new QLabel(tr("运费(国外)(RMB)："));
    m_freightForeignEdit = new QLineEdit;
    m_freightForeignEdit->setText("0");
    m_gridLayout->addWidget(freightForeignLabel, 10, 0, 1, 1);
    m_gridLayout->addWidget(m_freightForeignEdit, 10, 1, 1, 4);

    QLabel* exchangeRateLabel = new QLabel(tr("汇率："));
    m_exchangeRateEdit = new QLineEdit;
    m_exchangeRateEdit->setText("0");
    m_gridLayout->addWidget(exchangeRateLabel, 11, 0, 1, 1);
    m_gridLayout->addWidget(m_exchangeRateEdit, 11, 1, 1, 4);


    QLabel* handlingFeeLabel = new QLabel(tr("平台手续费(2.17%)(RMB)："));
    m_handlingFeeEdit = new QLineEdit;
    m_handlingFeeEdit->setText("0");
    m_gridLayout->addWidget(handlingFeeLabel, 12, 0, 1, 1);
    m_gridLayout->addWidget(m_handlingFeeEdit, 12, 1, 1, 4);

    m_calHandlingFeeBtn = new QPushButton(tr("自动计算"));
    m_gridLayout->addWidget(m_calHandlingFeeBtn, 12, 5, 1, 1);

    QLabel* remarksLabel = new QLabel(tr("备注："));
    m_remarksEdit = new QLineEdit;
    m_gridLayout->addWidget(remarksLabel, 13, 0, 1, 1);
    m_gridLayout->addWidget(m_remarksEdit, 13, 1, 1, 4);

    m_productTableWidget = new TableWidget;
    m_productTableWidget->setDataTypeProductInfo();
    m_gridLayout->addWidget(m_productTableWidget, 14, 0, 1, 6);

    m_okBtn = new QPushButton(tr("确认"));
    m_cancelBtn = new QPushButton(tr("取消"));

    m_gridLayout->addWidget(m_okBtn, 15, 4, 1, 1);
    m_gridLayout->addWidget(m_cancelBtn, 15, 5, 1, 1);

    setWindowTitle(tr("新增订单"));
    this->setMinimumWidth(800);
    //this->setFixedHeight(280);
}

void OrderDialog::connectSlot()
{
    connect(m_okBtn, SIGNAL(clicked(bool)), this, SLOT(onOKBtn()));
    connect(m_cancelBtn, SIGNAL(clicked(bool)), this, SLOT(onCancelBtn()));
    connect(m_calHandlingFeeBtn, SIGNAL(clicked(bool)), this, SLOT(onCalHandlingFeeBtn()));
    connect(m_financialRecordBtn, SIGNAL(clicked(bool)), this, SLOT(onFinancialRecordBtn()));
    //connect(m_payTypeCbx, SIGNAL(currentIndexChanged(int)), this, SLOT(onPayTypeCbxIndexChanged(int)));
}

QList<OrderInformation> OrderDialog::getOrderList()
{
    return m_orderInfoList;
}


void OrderDialog::setOrderInfo(const OrderInformation& orderInfo)
{
    m_orderIDEdit->setText(orderInfo.orderID);
    m_contractIDEdit->setText(orderInfo.contractID);
    m_customerNameEdit->setText(orderInfo.customerName);
    m_orderStatusCbx->setCurrentText(orderInfo.orderStatus);

#if 0
    QStringList list = orderInfo.payTime.split('-');
    if (list.size() >= 3)
    {
        QDate date = QDate(list[0].toInt(), list[1].toInt(), list[2].toInt());
        m_payTimeEdit->setDate(date);
    }
    else
    {
        m_payTimeEdit->setEnabled(false);
    }

    if (orderInfo.payType.contains(tr("其他")))
    {
        m_payTypeCbx->setCurrentText(tr("其他"));
        QStringList list = orderInfo.payType.split('-');
        if (list.size() >= 2)
        {
            m_otherPayTypeEdit->setText(list[1]);
            m_gridLayout->addWidget(m_otherPayTypeEdit, 5, 4, 1, 2);
        }
    }
    else
    {
        m_payTypeCbx->setCurrentText(orderInfo.payType);
    }
#endif

    m_realIncomeEdit->setText(QString::number(orderInfo.realIncome));
    if (orderInfo.realIncome > 0)
    {
        m_realIncomeEdit->setEnabled(true);
    }
    m_handlingFeeEdit->setText(QString::number(orderInfo.handlingFee));
    m_freightCustomerEdit->setText(QString::number(orderInfo.freightCustomer));
    m_freightFactoryToUsEdit->setText(QString::number(orderInfo.freightFactoryToUs));
    m_freightUsToForwardingEdit->setText(QString::number(orderInfo.freightUsToForwarding));
    m_freightForeignEdit->setText(QString::number(orderInfo.freightForeign));
    m_exchangeRateEdit->setText(QString::number(orderInfo.exchangeRate));
    m_remarksEdit->setText(orderInfo.remarks);

    for (int i=0; i<orderInfo.productList.size(); i++)
    {
        m_productTableWidget->addProductInfo(orderInfo.productList[i]);
    }

    setRealIncome();

    m_newOrderFlag = 0;
}

void OrderDialog::setCustomerName(const QString& name)
{
    m_customerNameEdit->setText(name);
}

void OrderDialog::disableCustomerNameEdit()
{
    m_customerNameEdit->setEnabled(false);
}

//    bool getFinacialInfoByContractID(const QString& contractID, QList<FinancialRecordInfo>& financialRecordInfoList);

void OrderDialog::setRealIncome()
{
    SqlDatabase sql("record");
    QList<FinancialRecordInfo> list;
    QString contractID = m_contractIDEdit->text();

    if (contractID.isEmpty())
    {
        //QMessageBox::critical(this, QString(tr("错误")), QString(tr("获取合同编号失败！")), QString(tr("确定")));
        qDebug() << "OrderDialog::setRealIncome " << "获取合同编号失败！";
        return;
    }

    if (false == sql.getFinacialInfoByContractID(contractID, list))
    {
        //QMessageBox::critical(this, QString(tr("错误")), QString(tr("获取收支记录失败！")), QString(tr("确定")));
        qDebug() << "OrderDialog::setRealIncome " << "获取收支记录失败！";
        return;
    }

    double realIncome = 0;
    for (int i=0; i<list.size(); i++)
    {
        if (tr("收入") == list[i].type)
        {
            realIncome += list[i].amount;
        }
    }

    if (realIncome > 0)
    {
        m_realIncomeEdit->setText(QString::number(realIncome));
        m_realIncomeEdit->setEnabled(false);
    }
}

void OrderDialog::onOKBtn()
{
    m_orderInfoList.clear();

    OrderInformation orderInfo;

    orderInfo.orderID = m_orderIDEdit->text();
    orderInfo.contractID = m_contractIDEdit->text();
    orderInfo.customerName = m_customerNameEdit->text();
    orderInfo.orderStatus = m_orderStatusCbx->currentText();
    orderInfo.payTime = "";      // ???
    orderInfo.payType = "";
    /*if (!m_otherPayTypeEdit->text().isEmpty())
    {
        orderInfo.payType += "-";
        orderInfo.payType += m_otherPayTypeEdit->text();
    }*/

    orderInfo.realIncome = m_realIncomeEdit->text().toDouble();
    orderInfo.handlingFee = m_handlingFeeEdit->text().toDouble();
    orderInfo.freightCustomer = m_freightCustomerEdit->text().toDouble();
    orderInfo.freightFactoryToUs = m_freightFactoryToUsEdit->text().toDouble();
    orderInfo.freightUsToForwarding = m_freightUsToForwardingEdit->text().toDouble();
    orderInfo.freightForeign = m_freightForeignEdit->text().toDouble();
    orderInfo.exchangeRate = m_exchangeRateEdit->text().toDouble();
    orderInfo.remarks = m_remarksEdit->text();
    orderInfo.salesman = "Amy";

    orderInfo.productList = m_productTableWidget->getProductList();

    orderInfo.calProfitIncomeAndExpenses();

    if (orderInfo.customerName.isEmpty() || orderInfo.productList.size() == 0)
    {
        QMessageBox::critical(this, tr("错误"), tr("客户名称、货品名称、数量、单价不能为空！"), QString(tr("确认")));
        return;
    }

    if (1 == m_newOrderFlag)
    {
        SqlDatabase sql("contractID");
        if (true == sql.contractIDIsExist(orderInfo.contractID))
        {
            QString str = QString(tr("合同编号：\"%1\"已经存在！")).arg(orderInfo.contractID);
            QMessageBox::critical(this, tr("错误"), str, QString(tr("确认")));
            return;
        }
    }

    m_orderInfoList.append(orderInfo);

    this->accept();
}

void OrderDialog::onCancelBtn()
{
    this->reject();
}

void OrderDialog::onCalHandlingFeeBtn()
{
    bool ret;
    double totalIncome = m_realIncomeEdit->text().toDouble(&ret);

    if (ret)
    {
        m_handlingFeeEdit->setText(QString::number(totalIncome*0.0217));
    }
}

void OrderDialog::onPayTypeCbxIndexChanged(int index)
{
    //qDebug() << index;
    if (index == OrderDialog::PayByOther)
    {
        //qDebug() << "Pay by other!";
        m_gridLayout->addWidget(m_otherPayTypeEdit, 5, 4, 1, 2);
    }
    else
    {
        m_gridLayout->removeWidget(m_otherPayTypeEdit);
        m_otherPayTypeEdit->setParent(NULL);
    }
}

void OrderDialog::onFinancialRecordBtn()
{   
    QString name = m_customerNameEdit->text();
    QString contractID = m_contractIDEdit->text();

    if (name.isEmpty() || contractID.isEmpty())
    {
        QMessageBox::critical(this, tr("错误"), tr("请下输入\"合同编号\"和\"客户名称\"！"), QString(tr("确认")));
        return;
    }

    emit sigAddFinancialRecord(name, contractID);

    setRealIncome();
}
