#include "SearchCustomerDialog.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QMessageBox>

SearchCustomerDialog::SearchCustomerDialog(QWidget *parent)
    : QDialog(parent, Qt::WindowCloseButtonHint)
{
    m_lastStatusCbxIndex = 0;
    constructUI();
    connectSlots();
}

void SearchCustomerDialog::constructUI()
{
    QLabel* searchTypeLabel = new QLabel(tr("查找方式："));
    //searchTypeLabel->setFixedWidth(60);
    //searchTypeLabel->setSizePolicy(QSizePolicy::Fixed);
    m_searchTypeCBox = new QComboBox;
    //m_searchTypeCBox->setSizePolicy(QSizePolicy::Expanding);
    QStringList items;
    items << "按姓名" << "按状态" << "按邮箱" << "按电话" << "按客户公司名" << "按国家" << "按询盘来源" << "按业务员" << "按关键字";

    m_searchTypeCBox->addItems(items);

    QLabel* kerWordLabel = new QLabel(tr("关键字："));
    //kerWordLabel->setFixedWidth(60);
    m_keyWordEdit = new QLineEdit;


    m_statusCBox = new QComboBox;
    items.clear();
    items << "需跟进" << "未报价" << "已报价" << "待确认" << "已成交";
    m_statusCBox->addItems(items);

    m_gridLayout = new QGridLayout;
    m_gridLayout->addWidget(searchTypeLabel, 0, 0, 1, 1);
    m_gridLayout->addWidget(m_searchTypeCBox, 0, 1, 1, 5);
    m_gridLayout->addWidget(kerWordLabel, 1, 0, 1, 1);
    m_gridLayout->addWidget(m_keyWordEdit, 1, 1, 1, 5);

    m_okBtn = new QPushButton(tr("确认"));
    QHBoxLayout* onBtnHLayout = new QHBoxLayout;
    onBtnHLayout->addStretch();
    onBtnHLayout->addWidget(m_okBtn);

    QVBoxLayout* mainVLayout = new QVBoxLayout(this);
    mainVLayout->addLayout(m_gridLayout);
    mainVLayout->addStretch();
    mainVLayout->addLayout(onBtnHLayout);

    m_keyWordEdit->setFocus();

    setWindowTitle(tr("查找客户"));

    this->setFixedWidth(500);
    this->setFixedHeight(150);
}

void SearchCustomerDialog::connectSlots()
{
    connect(m_okBtn, SIGNAL(clicked(bool)), this, SLOT(onOkBtn()));
    connect(m_searchTypeCBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onCbxIndexChanged(int)));
}

QString SearchCustomerDialog::getKeyWord()
{
    return m_keyWord;
}


int SearchCustomerDialog::getSearchType()
{
    return m_searchType;
}

void SearchCustomerDialog::onOkBtn()
{
    m_searchType = m_searchTypeCBox->currentIndex();

    if (SearchByStatus == m_searchType)
    {
        m_keyWord = m_statusCBox->currentText();
    }
    else
    {
        m_keyWord = m_keyWordEdit->text();
    }
    if (m_keyWord.isEmpty())
    {
        QMessageBox::critical(this, tr("错误"), tr("请输入关键字！"), QString(tr("确认")));
    }
    else
    {
        this->accept();
    }
}

void SearchCustomerDialog::onCbxIndexChanged(int index)
{
    if (SearchByStatus == m_lastStatusCbxIndex)
    {
        m_gridLayout->removeWidget(m_statusCBox);
        m_statusCBox->setParent(NULL);
    }
    else
    {
        m_gridLayout->removeWidget(m_keyWordEdit);
        m_keyWordEdit->setParent(NULL);
    }

    if (SearchByStatus == index)
    {
        m_gridLayout->addWidget(m_statusCBox, 1, 1, 1, 5);
    }
    else
    {

        m_gridLayout->addWidget(m_keyWordEdit, 1, 1, 1, 5);
        m_keyWordEdit->setFocus();
    }

    m_lastStatusCbxIndex = index;
}

SearchCustomerDialog::~SearchCustomerDialog()
{
    if (NULL == m_statusCBox->parent())
    {
        delete m_statusCBox;
        m_statusCBox = NULL;
    }

    if (NULL == m_keyWordEdit->parent())
    {
        delete m_keyWordEdit;
        m_keyWordEdit = NULL;
    }
}
