#include "OrderDialog.h"
#include <QMessageBox>
#include <QDateTime>
#include <QDebug>
#include "Version.h"

OrderDialog::OrderDialog(QWidget *parent)
    : QDialog(parent, Qt::WindowCloseButtonHint)
{
    constructUI();

    connect(m_okBtn, SIGNAL(clicked(bool)), this, SLOT(onOKBtn()));
    connect(m_cancelBtn, SIGNAL(clicked(bool)), this, SLOT(onCancelBtn()));
    connect(m_calHandlingFeeBtn, SIGNAL(clicked(bool)), this, SLOT(onCalHandlingFeeBtn()));
    connect(m_payTypeCbx, SIGNAL(currentIndexChanged(int)), this, SLOT(onPayTypeCbxIndexChanged(int)));
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
    m_gridLayout->addWidget(m_orderIDEdit, 0, 1, 1, 3);

    QLabel* contractIDLabel = new QLabel(tr("合同编号："));
    m_contractIDEdit = new QLineEdit;
    m_gridLayout->addWidget(contractIDLabel, 1, 0, 1, 1);
    m_gridLayout->addWidget(m_contractIDEdit, 1, 1, 1, 3);

    QLabel* customerNameLabel = new QLabel(tr("客户："));
    m_customerNameEdit = new QLineEdit;
    m_gridLayout->addWidget(customerNameLabel, 2, 0, 1, 1);
    m_gridLayout->addWidget(m_customerNameEdit, 2, 1, 1, 3);

    QLabel* orderStatusLabel = new QLabel(tr("订单状态："));
    m_orderStatusCbx = new QComboBox;
    QStringList items;
    items << "需跟进" << "已付定金" << "已付全款" << "待发货" << "已发货" << "未结算利润" << "已结算利润";
    m_orderStatusCbx->addItems(items);
    m_gridLayout->addWidget(orderStatusLabel, 3, 0, 1, 1);
    m_gridLayout->addWidget(m_orderStatusCbx, 3, 1, 1, 3);

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
    items << "信保" << "微信" << "支付宝" << "义乌个体户" << "大陆美金账户" << "香港账户" << "西联汇款" << "Paypal" << "其他";
#endif
    m_payTypeCbx->addItems(items);
    m_otherPayTypeEdit = new QLineEdit;
    //m_otherPayTypeEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    m_gridLayout->addWidget(payTypeLabel, 5, 0, 1, 1);
    m_gridLayout->addWidget(m_payTypeCbx, 5, 1, 1, 3);

    QLabel* realIncomeLabel = new QLabel(tr("实收(RMB)："));
    m_realIncomeEdit = new QLineEdit;
    m_realIncomeEdit->setText("0");
    m_gridLayout->addWidget(realIncomeLabel, 6, 0, 1, 1);
    m_gridLayout->addWidget(m_realIncomeEdit, 6, 1, 1, 3);

    QLabel* freightCustomerLabel = new QLabel(tr("运费(客户)(RMB)："));
    m_freightCustomerEdit = new QLineEdit;
    m_freightCustomerEdit->setText("0");
    m_gridLayout->addWidget(freightCustomerLabel, 7, 0, 1, 1);
    m_gridLayout->addWidget(m_freightCustomerEdit, 7, 1, 1, 3);

    QLabel* freightFactoryToUsLabel = new QLabel(tr("运费(工厂→我司)(RMB)："));
    m_freightFactoryToUsEdit = new QLineEdit;
    m_freightFactoryToUsEdit->setText("0");
    m_gridLayout->addWidget(freightFactoryToUsLabel, 8, 0, 1, 1);
    m_gridLayout->addWidget(m_freightFactoryToUsEdit, 8, 1, 1, 3);

    QLabel* freightUsToForwardingLabel = new QLabel(tr("运费(我司→货代)(RMB)："));
    m_freightUsToForwardingEdit = new QLineEdit;
    m_freightUsToForwardingEdit->setText("0");
    m_gridLayout->addWidget(freightUsToForwardingLabel, 9, 0, 1, 1);
    m_gridLayout->addWidget(m_freightUsToForwardingEdit, 9, 1, 1, 3);

    QLabel* freightForeignLabel = new QLabel(tr("运费(国外)(RMB)："));
    m_freightForeignEdit = new QLineEdit;
    m_freightForeignEdit->setText("0");
    m_gridLayout->addWidget(freightForeignLabel, 10, 0, 1, 1);
    m_gridLayout->addWidget(m_freightForeignEdit, 10, 1, 1, 3);

    QLabel* exchangeRateLabel = new QLabel(tr("汇率："));
    m_exchangeRateEdit = new QLineEdit;
    m_exchangeRateEdit->setText("0");
    m_gridLayout->addWidget(exchangeRateLabel, 11, 0, 1, 1);
    m_gridLayout->addWidget(m_exchangeRateEdit, 11, 1, 1, 3);


    QLabel* handlingFeeLabel = new QLabel(tr("平台手续费(2.17%)(RMB)："));
    m_handlingFeeEdit = new QLineEdit;
    m_handlingFeeEdit->setText("0");
    m_gridLayout->addWidget(handlingFeeLabel, 12, 0, 1, 1);
    m_gridLayout->addWidget(m_handlingFeeEdit, 12, 1, 1, 3);

    m_calHandlingFeeBtn = new QPushButton(tr("自动计算"));
    m_gridLayout->addWidget(m_calHandlingFeeBtn, 12, 4, 1, 1);

    QLabel* remarksLabel = new QLabel(tr("备注："));
    m_remarksEdit = new QLineEdit;
    m_gridLayout->addWidget(remarksLabel, 13, 0, 1, 1);
    m_gridLayout->addWidget(m_remarksEdit, 13, 1, 1, 3);

    m_productTableWidget = new TableWidget;
    m_productTableWidget->setDataTypeProductInfo();
    m_gridLayout->addWidget(m_productTableWidget, 14, 0, 1, 6);

    m_okBtn = new QPushButton(tr("确认"));
    m_cancelBtn = new QPushButton(tr("取消"));

    m_gridLayout->addWidget(m_okBtn, 15, 4, 1, 1);
    m_gridLayout->addWidget(m_cancelBtn, 15, 5, 1, 1);

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
    if (OrderDialog::PayByOther == m_payTypeCbx->currentIndex())
    {
        orderInfo.payType = m_payTypeCbx->currentText() + "-" + m_otherPayTypeEdit->text();
    }
    else
    {
        orderInfo.payType = m_payTypeCbx->currentText();
    }
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
