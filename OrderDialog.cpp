#include "OrderDialog.h"
#include <QGridLayout>
#include <QMessageBox>
#include <QDateTime>
#include <QDebug>

OrderDialog::OrderDialog(QWidget *parent)
    : QDialog(parent, Qt::WindowCloseButtonHint)
{
    constructUI();

    connect(m_okBtn, SIGNAL(clicked(bool)), this, SLOT(onOKBtn()));
    connect(m_cancelBtn, SIGNAL(clicked(bool)), this, SLOT(onCancelBtn()));
    connect(m_calHandlingFeeBtn, SIGNAL(clicked(bool)), this, SLOT(onCalHandlingFeeBtn()));
}

void OrderDialog::constructUI()
{
    QGridLayout* gridLayout = new QGridLayout(this);

    QLabel* orderIDLabel = new QLabel(tr("订单编号："));
    m_orderIDEdit = new QLineEdit;
    m_orderIDEdit->setEnabled(false);
    QDateTime dateTime(QDateTime::currentDateTime());
    m_orderIDEdit->setText(dateTime.toString("yyyyMMddhhmmsszzz"));
    gridLayout->addWidget(orderIDLabel, 0, 0, 1, 1);
    gridLayout->addWidget(m_orderIDEdit, 0, 1, 1, 2);

    QLabel* contractIDLabel = new QLabel(tr("合同编号："));
    m_contractIDEdit = new QLineEdit;
    gridLayout->addWidget(contractIDLabel, 1, 0, 1, 1);
    gridLayout->addWidget(m_contractIDEdit, 1, 1, 1, 2);

    QLabel* customerNameLabel = new QLabel(tr("客户："));
    m_customerNameEdit = new QLineEdit;
    gridLayout->addWidget(customerNameLabel, 2, 0, 1, 1);
    gridLayout->addWidget(m_customerNameEdit, 2, 1, 1, 2);

    QLabel* orderStatusLabel = new QLabel(tr("订单状态："));
    m_orderStatusCbx = new QComboBox;
    QStringList items;
    items << "已付定金" << "已付全款" << "待发货" << "已发货" << "未结算利润" << "已结算利润";
    m_orderStatusCbx->addItems(items);
    gridLayout->addWidget(orderStatusLabel, 3, 0, 1, 1);
    gridLayout->addWidget(m_orderStatusCbx, 3, 1, 1, 2);

    QLabel* payTimeLabel = new QLabel(tr("付款时间："));
    m_payTimeEdit = new QDateTimeEdit(QDateTime::currentDateTime());
    m_payTimeEdit->setCalendarPopup(true);
    m_payTimeEdit->setDisplayFormat("yyyy-MM-dd");
    gridLayout->addWidget(payTimeLabel, 4, 0, 1, 1);
    gridLayout->addWidget(m_payTimeEdit, 4, 1, 1, 2);

    QLabel* payTypeLabel = new QLabel(tr("付款方式："));
    m_payTypeCbx = new QComboBox;
    items.clear();
    items << "信保" << "微信" << "支付宝" << "大陆美金账户" << "香港账户" << "西联汇款" << "Paypal" << "其他";
    m_payTypeCbx->addItems(items);
    gridLayout->addWidget(payTypeLabel, 5, 0, 1, 1);
    gridLayout->addWidget(m_payTypeCbx, 5, 1, 1, 2);

    QLabel* realIncomeLabel = new QLabel(tr("实收："));
    m_realIncomeEdit = new QLineEdit;
    m_realIncomeEdit->setText("0");
    gridLayout->addWidget(realIncomeLabel, 6, 0, 1, 1);
    gridLayout->addWidget(m_realIncomeEdit, 6, 1, 1, 2);

    QLabel* freightCustomerLabel = new QLabel(tr("运费(客户)："));
    m_freightCustomerEdit = new QLineEdit;
    m_freightCustomerEdit->setText("0");
    gridLayout->addWidget(freightCustomerLabel, 7, 0, 1, 1);
    gridLayout->addWidget(m_freightCustomerEdit, 7, 1, 1, 2);

    QLabel* freightFactoryToUsLabel = new QLabel(tr("运费(工厂→我司)："));
    m_freightFactoryToUsEdit = new QLineEdit;
    m_freightFactoryToUsEdit->setText("0");
    gridLayout->addWidget(freightFactoryToUsLabel, 8, 0, 1, 1);
    gridLayout->addWidget(m_freightFactoryToUsEdit, 8, 1, 1, 2);

    QLabel* freightUsToForwardingLabel = new QLabel(tr("运费(我司→货代)："));
    m_freightUsToForwardingEdit = new QLineEdit;
    m_freightUsToForwardingEdit->setText("0");
    gridLayout->addWidget(freightUsToForwardingLabel, 9, 0, 1, 1);
    gridLayout->addWidget(m_freightUsToForwardingEdit, 9, 1, 1, 2);

    QLabel* freightForeignLabel = new QLabel(tr("运费(国外)："));
    m_freightForeignEdit = new QLineEdit;
    m_freightForeignEdit->setText("0");
    gridLayout->addWidget(freightForeignLabel, 10, 0, 1, 1);
    gridLayout->addWidget(m_freightForeignEdit, 10, 1, 1, 2);

    QLabel* exchangeRateLabel = new QLabel(tr("汇率："));
    m_exchangeRateEdit = new QLineEdit;
    m_exchangeRateEdit->setText("0");
    gridLayout->addWidget(exchangeRateLabel, 11, 0, 1, 1);
    gridLayout->addWidget(m_exchangeRateEdit, 11, 1, 1, 2);


    QLabel* handlingFeeLabel = new QLabel(tr("平台手续费："));
    m_handlingFeeEdit = new QLineEdit;
    m_handlingFeeEdit->setText("0");
    gridLayout->addWidget(handlingFeeLabel, 12, 0, 1, 1);
    gridLayout->addWidget(m_handlingFeeEdit, 12, 1, 1, 2);

    m_calHandlingFeeBtn = new QPushButton(tr("自动计算"));
    gridLayout->addWidget(m_calHandlingFeeBtn, 12, 3, 1, 1);

    QLabel* remarksLabel = new QLabel(tr("备注："));
    m_remarksEdit = new QLineEdit;
    gridLayout->addWidget(remarksLabel, 13, 0, 1, 1);
    gridLayout->addWidget(m_remarksEdit, 13, 1, 1, 2);

    m_productTableWidget = new TableWidget;
    m_productTableWidget->setDataTypeProductInfo();
    gridLayout->addWidget(m_productTableWidget, 14, 0, 1, 5);

    m_okBtn = new QPushButton(tr("确认"));
    m_cancelBtn = new QPushButton(tr("取消"));

    gridLayout->addWidget(m_okBtn, 15, 3, 1, 1);
    gridLayout->addWidget(m_cancelBtn, 15, 4, 1, 1);

    setWindowTitle(tr("新增订单"));
    this->setMinimumWidth(420);
    //this->setFixedHeight(280);
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
    m_payTypeCbx->setCurrentText(orderInfo.payType);
    m_realIncomeEdit->setText(QString::number(orderInfo.realIncome));
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
}

void OrderDialog::setCustomerName(const QString& name)
{
    m_customerNameEdit->setText(name);
}

void OrderDialog::disableCustomerNameEdit()
{
    m_customerNameEdit->setEnabled(false);
}

void OrderDialog::onOKBtn()
{
    m_orderInfoList.clear();

    OrderInformation orderInfo;

    orderInfo.orderID = m_orderIDEdit->text();
    orderInfo.contractID = m_contractIDEdit->text();
    orderInfo.customerName = m_customerNameEdit->text();
    orderInfo.orderStatus = m_orderStatusCbx->currentText();
    orderInfo.payTime = m_payTimeEdit->text();      // ???
    orderInfo.payType = m_payTypeCbx->currentText();
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
    }
    else
    {
        m_orderInfoList.append(orderInfo);

        this->accept();
    }
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
        m_handlingFeeEdit->setText(QString::number(totalIncome*0.04));
    }
}
