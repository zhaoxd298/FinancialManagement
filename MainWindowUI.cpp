#include "MainWindow.h"
#include "version.h"
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "Version.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_sqlDatabase = new SqlDatabase("customer");
    constructUI();
    connectSlots();
}

void MainWindow::constructUI()
{
    setWindowTitle(tr(TITLE) + VERSION);

    m_mainWidget = new QWidget;

    QGroupBox* customerManagerGBox = new QGroupBox(tr("客户管理"));
    m_addCustomerBtn = new QPushButton(tr("新增客户"));
    m_searchCustomerBtn = new QPushButton(tr("查找客户"));
    QVBoxLayout* customerManagerLayout = new QVBoxLayout(customerManagerGBox);
    customerManagerLayout->addWidget(m_addCustomerBtn);
    customerManagerLayout->addWidget(m_searchCustomerBtn);
    customerManagerGBox->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    QGroupBox* orderManagerGBox = new QGroupBox(tr("订单管理"));
    m_addOrderBtn = new QPushButton(tr("新增订单"));
    m_searchOrderBtn = new QPushButton(tr("查找订单"));
    QVBoxLayout* orderManagerLayout = new QVBoxLayout(orderManagerGBox);
    orderManagerLayout->addWidget(m_addOrderBtn);
    orderManagerLayout->addWidget(m_searchOrderBtn);
    orderManagerGBox->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    QGroupBox* financialRecordGBox = new QGroupBox(tr("收支记录"));
    m_addFinancialRecordBtn = new QPushButton(tr("新增记录"));
    m_searchFinancialRecordBtn = new QPushButton(tr("查找记录"));
    QVBoxLayout* financialRecordLayout = new QVBoxLayout(financialRecordGBox);
    financialRecordLayout->addWidget(m_addFinancialRecordBtn);
    financialRecordLayout->addWidget(m_searchFinancialRecordBtn);
    financialRecordGBox->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    QHBoxLayout* menuHLayout = new QHBoxLayout;
    menuHLayout->addWidget(customerManagerGBox);
    menuHLayout->addWidget(orderManagerGBox);
    menuHLayout->addWidget(financialRecordGBox);
    menuHLayout->addStretch();

    m_tableWidget = new TableWidget;

    m_statusLabel = new QLabel;
    m_statusLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    QVBoxLayout* mainWidgetVLayout = new QVBoxLayout(m_mainWidget);
    mainWidgetVLayout->addLayout(menuHLayout);
    mainWidgetVLayout->addWidget(m_tableWidget);
    mainWidgetVLayout->addWidget(m_statusLabel);

    this->resize(1200, 600);

    setCentralWidget(m_mainWidget);
}

MainWindow::~MainWindow()
{

}
