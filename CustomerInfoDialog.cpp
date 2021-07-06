#include "CustomerInfoDialog.h"
#include "SqlDatabase.h"
#include "CustomerInformation.h"
#include <QGridLayout>
#include <QMessageBox>
#include <QDebug>

CustomerInfoDialog::CustomerInfoDialog(QWidget *parent)
    : QDialog(parent, Qt::WindowCloseButtonHint)
{
    constructUI();

    connect(m_okBtn, SIGNAL(clicked(bool)), this, SLOT(onOKBtn()));
    connect(m_cancelBtn, SIGNAL(clicked(bool)), this, SLOT(onCancelBtn()));
}

void CustomerInfoDialog::constructUI()
{
    QGridLayout* layout = new QGridLayout(this);

    QLabel* customerNameLabel = new QLabel(tr("姓名："));
    m_customerName = new QLineEdit;
    layout->addWidget(customerNameLabel, 0, 0, 1, 1);
    layout->addWidget(m_customerName, 0, 1, 1, 4);

    QLabel* customerLevelLabel = new QLabel(tr("客户等级："));
    m_customerLevel = new QComboBox;
    QStringList items;
    items << "*" << "**" << "***" << "****" << "*****";
    m_customerLevel->addItems(items);
    layout->addWidget(customerLevelLabel, 1, 0, 1, 1);
    layout->addWidget(m_customerLevel, 1, 1, 1, 4);

    QLabel* statusLabel = new QLabel(tr("状态："));
    m_statusCbx = new QComboBox;
    items.clear();
    items << "需跟进" << "未报价" << "已报价" << "待确认" << "已成交";
    m_statusCbx->addItems(items);
    layout->addWidget(statusLabel, 2, 0, 1, 1);
    layout->addWidget(m_statusCbx, 2, 1, 1, 4);

    QLabel* dateLabel = new QLabel(tr("登记日期："));
    m_enterDateEdit = new QDateEdit(QDate::currentDate());          // 添加客户的日期
    m_enterDateEdit->setCalendarPopup(true);
    m_enterDateEdit->setDisplayFormat("yyyy-MM-dd");
    layout->addWidget(dateLabel, 3, 0, 1, 1);
    layout->addWidget(m_enterDateEdit, 3, 1, 1, 4);

    QLabel* inquirySourceLabel = new QLabel(tr("询盘来源："));
    m_inquirySource = new QComboBox;
    items.clear();
    items << "Alibaba" << "Facebook" << "Linkin";
    m_inquirySource->addItems(items);
    layout->addWidget(inquirySourceLabel, 4, 0, 1, 1);
    layout->addWidget(m_inquirySource, 4, 1, 1, 4);

    QLabel* backgroundLabel = new QLabel(tr("背景："));
    m_backgroundEdit = new QLineEdit;
    layout->addWidget(backgroundLabel, 5, 0, 1, 1);
    layout->addWidget(m_backgroundEdit, 5, 1, 1, 4);

    QLabel* addressLabel = new QLabel(tr("地址："));
    m_addressEdit = new QLineEdit;
    layout->addWidget(addressLabel, 6, 0, 1, 1);
    layout->addWidget(m_addressEdit, 6, 1, 1, 4);

    QLabel* companyNameLabel = new QLabel(tr("公司名："));
    m_companyName = new QLineEdit;
    layout->addWidget(companyNameLabel, 7, 0, 1, 1);
    layout->addWidget(m_companyName, 7, 1, 1, 4);

    QLabel* websitLabel = new QLabel(tr("网址："));
    m_websiteEdit = new QLineEdit;
    layout->addWidget(websitLabel, 8, 0, 1, 1);
    layout->addWidget(m_websiteEdit, 8, 1, 1, 4);

    QLabel* customerEmailLabel = new QLabel(tr("邮箱："));
    m_customerEmail = new QLineEdit;
    layout->addWidget(customerEmailLabel, 9, 0, 1, 1);
    layout->addWidget(m_customerEmail, 9, 1, 1, 4);

    QLabel* phoneNumberLabel = new QLabel(tr("联系方式："));
    m_phoneNumber = new QLineEdit;
    layout->addWidget(phoneNumberLabel, 10, 0, 1, 1);
    layout->addWidget(m_phoneNumber, 10, 1, 1, 4);

    QLabel* customerPositionLabel = new QLabel(tr("职位："));
    m_customerPosition = new QLineEdit;
    layout->addWidget(customerPositionLabel, 11, 0, 1, 1);
    layout->addWidget(m_customerPosition, 11, 1, 1, 4);

    QLabel* salesmanLabel = new QLabel(tr("业务员："));
    m_salesman = new QLineEdit("Amy");
    layout->addWidget(salesmanLabel, 12, 0, 1, 1);
    layout->addWidget(m_salesman, 12, 1, 1, 4);

    QLabel* m_remarksLabel = new QLabel(tr("备注："));
    m_remarksEdit = new QLineEdit;
    layout->addWidget(m_remarksLabel, 13, 0, 1, 1);
    layout->addWidget(m_remarksEdit, 13, 1, 1, 4);

    QLabel* scheduleLabel = new QLabel(tr("跟进情况："));
    m_schedule = new QPlainTextEdit;
    layout->addWidget(scheduleLabel, 14, 0, 1, 1);
    layout->addWidget(m_schedule, 14, 1, 1, 4);

    m_okBtn = new QPushButton(tr("确认"));
    m_cancelBtn = new QPushButton(tr("取消"));

    layout->addWidget(m_okBtn, 15, 4, 1, 1);
    layout->addWidget(m_cancelBtn, 15, 5, 1, 1);

    setWindowTitle(tr("新增客户"));

    setMinimumWidth(800);
    //this->resize(420, 280);
    //this->setFixedWidth(320);
    //this->setFixedHeight(280);
}

QList<CustomerInformation> CustomerInfoDialog::getCustomerInformationList()
{
    return m_customerInfoList;
}


void CustomerInfoDialog::setCustomerInfomation(const CustomerInformation& customerInfo)
{
    m_customerName->setText(customerInfo.name);
    m_customerLevel->setCurrentText(customerInfo.level);
    m_statusCbx->setCurrentText(customerInfo.status);
    QStringList list = customerInfo.enterDate.split('-');
    if (list.size() >= 3)
    {
        QDate date = QDate(list[0].toInt(), list[1].toInt(), list[2].toInt());
        m_enterDateEdit->setDate(date);
    }
    else
    {
        m_enterDateEdit->setEnabled(false);
    }
    m_inquirySource->setCurrentText(customerInfo.inquirySource);
    m_backgroundEdit->setText(customerInfo.background);
    m_addressEdit->setText(customerInfo.address);
    m_companyName->setText(customerInfo.companyName);
    m_websiteEdit->setText(customerInfo.websit);
    m_customerEmail->setText(customerInfo.email);
    m_phoneNumber->setText(customerInfo.phoneNumber);
    m_customerPosition->setText(customerInfo.position);
    m_schedule->setPlainText(customerInfo.schedule);
    m_salesman->setText(customerInfo.salesman);
    m_remarksEdit->setText(customerInfo.remarks);
}


void CustomerInfoDialog::setCustomerNameEditDisable()
{
    m_customerName->setEnabled(false);
}

void CustomerInfoDialog::onOKBtn()
{
    m_customerInfoList.clear();

    CustomerInformation customerInfo;

    customerInfo.name = m_customerName->text();
    customerInfo.level = m_customerLevel->currentText();
    customerInfo.status = m_statusCbx->currentText();
    customerInfo.enterDate = m_enterDateEdit->text();
    customerInfo.inquirySource = m_inquirySource->currentText();
    customerInfo.background = m_backgroundEdit->text();
    customerInfo.address = m_addressEdit->text();
    customerInfo.companyName = m_companyName->text();
    customerInfo.websit = m_websiteEdit->text();
    customerInfo.email = m_customerEmail->text();
    customerInfo.phoneNumber = m_phoneNumber->text();
    customerInfo.position = m_customerPosition->text();
    customerInfo.schedule = m_schedule->toPlainText();
    customerInfo.salesman = m_salesman->text();
    customerInfo.remarks = m_remarksEdit->text();

    if (customerInfo.name.isEmpty())
    {
        QMessageBox::critical(this, tr("错误"), tr("客户名称不能为空！"), QString(tr("确认")));
    }
    else
    {
        m_customerInfoList.append(customerInfo);

        this->accept();
    }
}

void CustomerInfoDialog::onCancelBtn()
{
    this->reject();
}
