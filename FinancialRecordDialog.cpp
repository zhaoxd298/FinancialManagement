#include <QMessageBox>
#include "FinancialRecordDialog.h"
#include "SqlDatabase.h"

FinancialRecordDialog::FinancialRecordDialog(QWidget *parent)
    : QDialog(parent, Qt::WindowCloseButtonHint)
{
    constructUI();

    connect(m_okBtn, SIGNAL(clicked(bool)), this, SLOT(onOKBtn()));
    connect(m_cancelBtn, SIGNAL(clicked(bool)), this, SLOT(onCancelBtn()));
    connect(m_contractIDEdit, SIGNAL(textChanged(QString)), this, SLOT(onContractIDEditChanged(QString)));
}


FinancialRecordInfo FinancialRecordDialog::getFinancialRecordInfo()
{
    return m_financialRecordInfo;
}

void FinancialRecordDialog::setFinancialRecordInfo(const FinancialRecordInfo& record)
{
    m_contractIDEdit->setText(record.contractID);
    m_customerNameEdit->setText(record.customerName);
    m_typeCbx->setCurrentText(record.type);
    m_amountEdit->setText(QString::number(record.amount));
    QStringList list = record.payTime.split('-');
    if (list.size() >= 3)
    {
        QDate date = QDate(list[0].toInt(), list[1].toInt(), list[2].toInt());
        m_payTimeEdit->setDate(date);
    }
    else
    {
        m_payTimeEdit->setEnabled(false);
    }


    list = record.payType.split('-');
    m_payTypeCbx->setCurrentText(list[0]);
    if (list.size() >= 2)
    {
        m_payTypeRemarksEdit->setText(list[1]);
    }

    m_remarksEdit->setText(record.remarks);
}

void FinancialRecordDialog::setCustomerName(const QString& name)
{
    m_customerNameEdit->setText(name);
}

void FinancialRecordDialog::disableCustomerNameEdit()
{
    m_customerNameEdit->setEnabled(false);
}

void FinancialRecordDialog::constructUI()
{
    m_gridLayout = new QGridLayout(this);

    QLabel* contractIDLabel = new QLabel(tr("合同编号："));
    m_contractIDEdit = new QLineEdit;
    m_gridLayout->addWidget(contractIDLabel, 0, 0, 1, 1);
    m_gridLayout->addWidget(m_contractIDEdit, 0, 1, 1, 3);

    QLabel* customerNameLabel = new QLabel(tr("客户："));
    m_customerNameEdit = new QLineEdit;
    m_gridLayout->addWidget(customerNameLabel, 1, 0, 1, 1);
    m_gridLayout->addWidget(m_customerNameEdit, 1, 1, 1, 3);

    QLabel* typeLabel = new QLabel(tr("收支："));
    m_typeCbx = new QComboBox;
    QStringList items;
    items << "" << "收入" << "支出";
    m_typeCbx->addItems(items);
    m_gridLayout->addWidget(typeLabel, 2, 0, 1, 1);
    m_gridLayout->addWidget(m_typeCbx, 2, 1, 1, 3);

    QLabel* amountLabel = new QLabel(tr("金额："));
    m_amountEdit = new QLineEdit;
    m_gridLayout->addWidget(amountLabel, 3, 0, 1, 1);
    m_gridLayout->addWidget(m_amountEdit, 3, 1, 1, 3);

    QLabel* payTimeLabel = new QLabel(tr("付款时间："));
    m_payTimeEdit = new QDateTimeEdit(QDateTime::currentDateTime());
    m_payTimeEdit->setCalendarPopup(true);
    m_payTimeEdit->setDisplayFormat("yyyy-MM-dd");
    m_gridLayout->addWidget(payTimeLabel, 4, 0, 1, 1);
    m_gridLayout->addWidget(m_payTimeEdit, 4, 1, 1, 3);

    QLabel* payTypeLabel = new QLabel(tr("付款方式："));
    m_payTypeCbx = new QComboBox;
    items.clear();
    items << "中国银行" << "招商银行" << "建设银行" << "农业银行" << "广发银行" << "交通银行" << "XT"
          << "信保" << "微信" << "支付宝" << "义乌个体户" << "大陆美金账户" << "香港账户"
          << "西联汇款" << "Paypal" << "其他";
    m_payTypeCbx->addItems(items);
    m_payTypeCbx->setMaxVisibleItems(20);
    m_payTypeRemarksEdit = new QLineEdit;
    m_payTypeRemarksEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    m_gridLayout->addWidget(payTypeLabel, 5, 0, 1, 1);
    m_gridLayout->addWidget(m_payTypeCbx, 5, 1, 1, 3);
    m_gridLayout->addWidget(m_payTypeRemarksEdit, 5, 4, 1, 2);

    QLabel* remarksLabel = new QLabel(tr("备注："));
    m_remarksEdit = new QLineEdit;
    m_gridLayout->addWidget(remarksLabel, 6, 0, 1, 1);
    m_gridLayout->addWidget(m_remarksEdit, 6, 1, 1, 3);


    m_okBtn = new QPushButton(tr("确认"));
    m_cancelBtn = new QPushButton(tr("取消"));

    m_gridLayout->addWidget(m_okBtn, 12, 4, 1, 1);
    m_gridLayout->addWidget(m_cancelBtn, 12, 5, 1, 1);

    setWindowTitle(tr("新增记录"));
    this->setMinimumWidth(500);
    this->setMinimumHeight(300);
}

void FinancialRecordDialog::onOKBtn()
{
    m_financialRecordInfo.contractID = m_contractIDEdit->text();
    m_financialRecordInfo.customerName = m_customerNameEdit->text();
    m_financialRecordInfo.type = m_typeCbx->currentText();
    m_financialRecordInfo.amount = m_amountEdit->text().toDouble();
    m_financialRecordInfo.payTime = m_payTimeEdit->text();
    m_financialRecordInfo.payType = m_payTypeCbx->currentText();
    if (!m_payTypeRemarksEdit->text().isEmpty())
    {
        m_financialRecordInfo.payType += "-";
        m_financialRecordInfo.payType += m_payTypeRemarksEdit->text();
    }
    m_financialRecordInfo.remarks = m_remarksEdit->text();

    if (m_financialRecordInfo.contractID.isEmpty())
    {
        QMessageBox::critical(this, tr("错误"), tr("合同编号不能为空！"), QString(tr("确认")));
        return;
    }

    if (m_financialRecordInfo.customerName.isEmpty())
    {
        QMessageBox::critical(this, tr("错误"), tr("客户名称不能为空！"), QString(tr("确认")));
        return;
    }

    if (m_financialRecordInfo.type.isEmpty())
    {
        QMessageBox::critical(this, tr("错误"), tr("收支不能为空！"), QString(tr("确认")));
        return;
    }

    this->accept();
}

void FinancialRecordDialog::onCancelBtn()
{
    this->reject();
}

void FinancialRecordDialog::onContractIDEditChanged(QString contractID)
{
    SqlDatabase sql("search");
    QStringList nameList;

    if (true == sql.getCustomerNameByContractID(contractID, nameList))
    {
        if (nameList.size() > 0)
        {
            m_customerNameEdit->setText(nameList[0]);
        }
    }
    /*else
    {
        m_customerNameEdit->clear();
    }*/
}
