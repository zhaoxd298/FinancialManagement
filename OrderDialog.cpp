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

    QLabel* customerNameLabel = new QLabel(tr("客户："));
    m_customerNameEdit = new QLineEdit;
    gridLayout->addWidget(customerNameLabel, 1, 0, 1, 1);
    gridLayout->addWidget(m_customerNameEdit, 1, 1, 1, 2);

    QLabel* orderStatusLabel = new QLabel(tr("订单状态："));
    m_orderStatusCbx = new QComboBox;
    QStringList item;
    item << "已付定金" << "已付全款" << "待发货" << "已发货" << "已结算利润";
    m_orderStatusCbx->addItems(item);
    gridLayout->addWidget(orderStatusLabel, 2, 0, 1, 1);
    gridLayout->addWidget(m_orderStatusCbx, 2, 1, 1, 2);

    QLabel* payTimeLabel = new QLabel(tr("付款时间："));
    m_payTimeEdit = new QDateTimeEdit(QDateTime::currentDateTime());
    m_payTimeEdit->setCalendarPopup(true);
    m_payTimeEdit->setDisplayFormat("yyyy-MM-dd");
    gridLayout->addWidget(payTimeLabel, 3, 0, 1, 1);
    gridLayout->addWidget(m_payTimeEdit, 3, 1, 1, 2);

    QLabel* payTypeLabel = new QLabel(tr("付款方式："));
    m_payTypeEdit = new QLineEdit;
    gridLayout->addWidget(payTypeLabel, 4, 0, 1, 1);
    gridLayout->addWidget(m_payTypeEdit, 4, 1, 1, 2);

    QLabel* realIncomeLabel = new QLabel(tr("实收："));
    m_realIncomeEdit = new QLineEdit;
    m_realIncomeEdit->setText("0");
    gridLayout->addWidget(realIncomeLabel, 5, 0, 1, 1);
    gridLayout->addWidget(m_realIncomeEdit, 5, 1, 1, 2);

    QLabel* freightCustomerLabel = new QLabel(tr("运费(客户)："));
    m_freightCustomerEdit = new QLineEdit;
    m_freightCustomerEdit->setText("0");
    gridLayout->addWidget(freightCustomerLabel, 6, 0, 1, 1);
    gridLayout->addWidget(m_freightCustomerEdit, 6, 1, 1, 2);

    QLabel* freightFactoryToUsLabel = new QLabel(tr("运费(工厂→我司)："));
    m_freightFactoryToUsEdit = new QLineEdit;
    m_freightFactoryToUsEdit->setText("0");
    gridLayout->addWidget(freightFactoryToUsLabel, 7, 0, 1, 1);
    gridLayout->addWidget(m_freightFactoryToUsEdit, 7, 1, 1, 2);

    QLabel* freightUsToForwardingLabel = new QLabel(tr("运费(我司→货代)："));
    m_freightUsToForwardingEdit = new QLineEdit;
    m_freightUsToForwardingEdit->setText("0");
    gridLayout->addWidget(freightUsToForwardingLabel, 8, 0, 1, 1);
    gridLayout->addWidget(m_freightUsToForwardingEdit, 8, 1, 1, 2);

    QLabel* freightForeignLabel = new QLabel(tr("运费(国外)："));
    m_freightForeignEdit = new QLineEdit;
    m_freightForeignEdit->setText("0");
    gridLayout->addWidget(freightForeignLabel, 9, 0, 1, 1);
    gridLayout->addWidget(m_freightForeignEdit, 9, 1, 1, 2);

    QLabel* packageFeeLabel = new QLabel(tr("包装费："));
    m_packageFeeEdit = new QLineEdit;
    m_packageFeeEdit->setText("0");
    gridLayout->addWidget(packageFeeLabel, 10, 0, 1, 1);
    gridLayout->addWidget(m_packageFeeEdit, 10, 1, 1, 2);


    QLabel* handlingFeeLabel = new QLabel(tr("平台手续费："));
    m_handlingFeeEdit = new QLineEdit;
    m_handlingFeeEdit->setText("0");
    gridLayout->addWidget(handlingFeeLabel, 11, 0, 1, 1);
    gridLayout->addWidget(m_handlingFeeEdit, 11, 1, 1, 2);

    QLabel* remarksLabel = new QLabel(tr("备注："));
    m_remarksEdit = new QLineEdit;
    gridLayout->addWidget(remarksLabel, 12, 0, 1, 1);
    gridLayout->addWidget(m_remarksEdit, 12, 1, 1, 2);

    m_productTableWidget = new TableWidget;
    m_productTableWidget->setDataTypeProductInfo();
    gridLayout->addWidget(m_productTableWidget, 13, 0, 1, 5);

    m_okBtn = new QPushButton(tr("确认"));
    m_cancelBtn = new QPushButton(tr("取消"));

    gridLayout->addWidget(m_okBtn, 14, 3, 1, 1);
    gridLayout->addWidget(m_cancelBtn, 14, 4, 1, 1);

    setWindowTitle(tr("新增订单"));
    this->setMinimumWidth(480);
    //this->setFixedHeight(280);
}

QList<OrderInformation> OrderDialog::getOrderList()
{
    return m_orderInfoList;
}


void OrderDialog::setOrderInfo(const OrderInformation& orderInfo)
{
    m_orderIDEdit->setText(orderInfo.orderID);
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
    m_payTypeEdit->setText(orderInfo.payType);
    m_realIncomeEdit->setText(QString::number(orderInfo.realIncome));
    m_handlingFeeEdit->setText(QString::number(orderInfo.handlingFee));
    m_freightCustomerEdit->setText(QString::number(orderInfo.freightCustomer));
    m_freightFactoryToUsEdit->setText(QString::number(orderInfo.freightFactoryToUs));
    m_freightUsToForwardingEdit->setText(QString::number(orderInfo.freightUsToForwarding));
    m_freightForeignEdit->setText(QString::number(orderInfo.freightForeign));
    m_packageFeeEdit->setText(QString::number(orderInfo.packageFee));
    m_remarksEdit->setText(orderInfo.remarks);

    for (int i=0; i<orderInfo.productList.size(); i++)
    {
        m_productTableWidget->addProductInfo(orderInfo.productList[i]);
    }
}

void OrderDialog::onOKBtn()
{
    m_orderInfoList.clear();

    OrderInformation orderInfo;

    orderInfo.orderID = m_orderIDEdit->text();
    orderInfo.customerName = m_customerNameEdit->text();
    orderInfo.orderStatus = m_orderStatusCbx->currentText();
    orderInfo.payTime = m_payTimeEdit->text();      // ???
    orderInfo.payType = m_payTypeEdit->text();
    orderInfo.realIncome = m_realIncomeEdit->text().toDouble();
    orderInfo.handlingFee = m_handlingFeeEdit->text().toDouble();
    orderInfo.freightCustomer = m_freightCustomerEdit->text().toDouble();
    orderInfo.freightFactoryToUs = m_freightFactoryToUsEdit->text().toDouble();
    orderInfo.freightUsToForwarding = m_freightUsToForwardingEdit->text().toDouble();
    orderInfo.freightForeign = m_freightForeignEdit->text().toDouble();
    orderInfo.packageFee = m_packageFeeEdit->text().toDouble();
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
