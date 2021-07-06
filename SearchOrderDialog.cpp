#include "SearchOrderDialog.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QDate>
#include <QDebug>
#include "Version.h"

SearchOrderDialog::SearchOrderDialog(QWidget *parent)
    : QDialog(parent, Qt::WindowCloseButtonHint)
{
    m_lastSearchType = 0;
    constructUI();
    connectSlots();
}

void SearchOrderDialog::constructUI()
{
    QLabel* searchTypeLabel = new QLabel(tr("查找方式："));
    //searchTypeLabel->setFixedWidth(60);
    //searchTypeLabel->setSizePolicy(QSizePolicy::Fixed);
    m_searchTypeCBox = new QComboBox;
    //m_searchTypeCBox->setSizePolicy(QSizePolicy::Expanding);
    QStringList items;
    items << "所有订单" << "待发货" << "已发货" << "上一月订单" << "指定日期" << "按业务员" << "按合同编号" << "按客户名";
    m_searchTypeCBox->addItems(items);

    m_kerWordLabel = new QLabel(tr("关键字："));
    //m_kerWordLabel->setFixedWidth(60);
    m_keyWordEdit = new QLineEdit;

    m_dateRangeLabel = new QLabel(tr("日期范围："));
    m_startDataEdit = new QDateEdit(QDate::currentDate());
    m_startDataEdit->setDisplayFormat("yyyy-MM-dd");
    m_startDataEdit->setCalendarPopup(true);
    m_endDataEdit = new QDateEdit(QDate::currentDate());
    m_endDataEdit->setDisplayFormat("yyyy-MM-dd");
    m_endDataEdit->setCalendarPopup(true);
    m_connectLabel = new QLabel("    -");

    m_gridLayout = new QGridLayout;
    m_gridLayout->addWidget(searchTypeLabel, 0, 0, 1, 1);
    m_gridLayout->addWidget(m_searchTypeCBox, 0, 1, 1, 5);
    //m_gridLayout->addWidget(m_kerWordLabel, 1, 0, 1, 1);
    //m_gridLayout->addWidget(m_keyWordEdit, 1, 1, 1, 3);

    m_okBtn = new QPushButton(tr("确认"));
    QHBoxLayout* onBtnHLayout = new QHBoxLayout;
    onBtnHLayout->addStretch();
    onBtnHLayout->addWidget(m_okBtn);

    QVBoxLayout* mainVLayout = new QVBoxLayout(this);
    mainVLayout->addLayout(m_gridLayout);
    mainVLayout->addStretch();
    mainVLayout->addLayout(onBtnHLayout);

    setWindowTitle(tr("查找订单"));

    this->setFixedWidth(600);
    this->setFixedHeight(200);
}

void SearchOrderDialog::connectSlots()
{
    connect(m_okBtn, SIGNAL(clicked(bool)), this, SLOT(onOkBtn()));
    connect(m_searchTypeCBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onCbxIndexChanged(int)));
}

int SearchOrderDialog::searchTypeTextToType(const QString& text)
{
    int index = -1;

    if ("所有订单" == text)
    {
        index = SearchAllOrder;
    }
    else if ("未结算利润订单" == text)
    {
        index = SearchByUnpayProfit;
    }
    else if ("已结算利润订单" == text)
    {
        index = SearchByPayedProfit;
    }
    else if ("上一月订单" == text)
    {
        index = SearchByLastMonthOrder;
    }
    else if ("指定日期" == text)
    {
        index = SearchByDateRange;
    }
    else if ("按业务员" == text)
    {
        index = SearchBySalesman;
    }
    else if ("按合同编号" == text)
    {
        index = SearchByContractid;
    }
    else if ("按客户名" == text)
    {
        index = SearchByCustomerName;
    }
    else if ("待发货" == text)
    {
        index = SearchByNotShipped;
    }
    else if ("已发货" == text)
    {
        index = SearchByShipped;
    }

    return index;
}

QString SearchOrderDialog::getKeyWord()
{
    return m_keyWord;
}

int SearchOrderDialog::getSearchType()
{
    return m_searchType;
}


QString SearchOrderDialog::getStartDate()
{
    return m_startDate;
}

QString SearchOrderDialog::getEndDate()
{
    return m_endDate;
}

void SearchOrderDialog::onOkBtn()
{
    bool ret = true;
    m_searchType = searchTypeTextToType(m_searchTypeCBox->currentText());

    if (SearchOrderDialog::SearchByDateRange == m_searchType)
    {
        m_startDate = m_startDataEdit->text();
        m_endDate = m_endDataEdit->text();
        ret = true;
    }
    else if ((SearchOrderDialog::SearchBySalesman == m_searchType) ||
             (SearchOrderDialog::SearchByContractid == m_searchType) ||
             (SearchOrderDialog::SearchByCustomerName == m_searchType))
    {
        m_keyWord = m_keyWordEdit->text();
        if (m_keyWord.isEmpty())
        {
            ret = false;
            QMessageBox::critical(this, tr("错误"), tr("请输入关键字！"), QString(tr("确认")));
        }
        else
        {
            ret = true;
        }
    }

    if (true == ret)
    {
        this->accept();
    }
}


void SearchOrderDialog::onCbxIndexChanged(int)
{
    int searchType = searchTypeTextToType(m_searchTypeCBox->currentText());

    if (SearchOrderDialog::SearchByDateRange == m_lastSearchType)
    {
        m_gridLayout->removeWidget(m_dateRangeLabel);
        m_gridLayout->removeWidget(m_startDataEdit);
        m_gridLayout->removeWidget(m_connectLabel);
        m_gridLayout->removeWidget(m_endDataEdit);
        m_dateRangeLabel->setParent(NULL);
        m_startDataEdit->setParent(NULL);
        m_connectLabel->setParent(NULL);
        m_endDataEdit->setParent(NULL);
    }
    else if ((SearchOrderDialog::SearchBySalesman == m_lastSearchType) ||
             (SearchOrderDialog::SearchByContractid == m_lastSearchType) ||
             (SearchOrderDialog::SearchByCustomerName == m_lastSearchType))
    {
        m_gridLayout->removeWidget(m_kerWordLabel);
        m_gridLayout->removeWidget(m_keyWordEdit);
        m_kerWordLabel->setParent(NULL);
        m_keyWordEdit->setParent(NULL);
    }

    if (SearchOrderDialog::SearchByDateRange == searchType)
    {
        m_gridLayout->addWidget(m_dateRangeLabel, 1, 0, 1, 1);
        m_gridLayout->addWidget(m_startDataEdit, 1, 1, 1, 2);
        m_gridLayout->addWidget(m_connectLabel, 1, 3, 1, 1);
        m_gridLayout->addWidget(m_endDataEdit, 1, 4, 1, 2);
    }
    else if (SearchOrderDialog::SearchBySalesman == searchType)
    {
        m_kerWordLabel->setText(tr("业务员："));
        m_gridLayout->addWidget(m_kerWordLabel, 1, 0, 1, 1);
        m_gridLayout->addWidget(m_keyWordEdit, 1, 1, 1, 5);
    }
    else if (SearchOrderDialog::SearchByContractid == searchType)
    {
        m_kerWordLabel->setText(tr("合同编号："));
        m_gridLayout->addWidget(m_kerWordLabel, 1, 0, 1, 1);
        m_gridLayout->addWidget(m_keyWordEdit, 1, 1, 1, 5);
    }
    else if (SearchOrderDialog::SearchByCustomerName == searchType)
    {
        m_kerWordLabel->setText(tr("客户名："));
        m_gridLayout->addWidget(m_kerWordLabel, 1, 0, 1, 1);
        m_gridLayout->addWidget(m_keyWordEdit, 1, 1, 1, 5);
    }

    m_lastSearchType = searchType;
}

SearchOrderDialog::~SearchOrderDialog()
{
    if (m_dateRangeLabel->parent() == NULL)
    {
        delete m_dateRangeLabel;
        m_dateRangeLabel = NULL;
    }

    if (m_startDataEdit->parent() == NULL)
    {
        delete m_startDataEdit;
        m_startDataEdit = NULL;
    }

    if (m_connectLabel->parent() == NULL)
    {
        delete m_connectLabel;
        m_connectLabel = NULL;
    }

    if (m_endDataEdit->parent() == NULL)
    {
        delete m_endDataEdit;
        m_endDataEdit = NULL;
    }

    if (m_kerWordLabel->parent() == NULL)
    {
        delete m_kerWordLabel;
        m_kerWordLabel = NULL;
    }

    if (m_keyWordEdit->parent() == NULL)
    {
        delete m_keyWordEdit;
        m_keyWordEdit = NULL;
    }
}
