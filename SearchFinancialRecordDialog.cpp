#include "SearchFinancialRecordDialog.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QDebug>

SearchFinancialRecordDialog::SearchFinancialRecordDialog(QWidget *parent)
    : QDialog(parent, Qt::WindowCloseButtonHint)
{
    constructUI();
    connectSlots();
}

QString SearchFinancialRecordDialog::getKeyWord()
{
    return m_keyWord;
}

int SearchFinancialRecordDialog::getSearchType()
{
    return m_searchType;
}

void SearchFinancialRecordDialog::onOkBtn()
{
    m_searchType = m_searchTypeCBox->currentIndex();
    m_keyWord = m_keyWordEdit->text();

    if (m_keyWord.isEmpty())
    {
        QMessageBox::critical(this, tr("错误"), tr("请输入关键字！"), QString(tr("确认")));
    }
    else
    {
        this->accept();
    }

    //qDebug() << m_searchType << m_keyWord;
}

void SearchFinancialRecordDialog::constructUI()
{
    QLabel* searchTypeLabel = new QLabel(tr("查找方式："));
    //searchTypeLabel->setFixedWidth(60);
    //searchTypeLabel->setSizePolicy(QSizePolicy::Fixed);
    m_searchTypeCBox = new QComboBox;
    //m_searchTypeCBox->setSizePolicy(QSizePolicy::Expanding);
    QStringList items;
    items << "按合同编号" << "按客户名";

    m_searchTypeCBox->addItems(items);

    QLabel* kerWordLabel = new QLabel(tr("关键字："));
    //kerWordLabel->setFixedWidth(60);
    m_keyWordEdit = new QLineEdit;

    m_gridLayout = new QGridLayout;
    m_gridLayout->addWidget(searchTypeLabel, 0, 0, 1, 1);
    m_gridLayout->addWidget(m_searchTypeCBox, 0, 1, 1, 4);
    m_gridLayout->addWidget(kerWordLabel, 1, 0, 1, 1);
    m_gridLayout->addWidget(m_keyWordEdit, 1, 1, 1, 4);

    m_okBtn = new QPushButton(tr("确认"));
    QHBoxLayout* onBtnHLayout = new QHBoxLayout;
    onBtnHLayout->addStretch();
    onBtnHLayout->addWidget(m_okBtn);

    QVBoxLayout* mainVLayout = new QVBoxLayout(this);
    mainVLayout->addLayout(m_gridLayout);
    mainVLayout->addStretch();
    mainVLayout->addLayout(onBtnHLayout);

    m_keyWordEdit->setFocus();

    setWindowTitle(tr("查找记录"));

    this->setFixedWidth(600);
    this->setFixedHeight(200);
}

void SearchFinancialRecordDialog::connectSlots()
{
    connect(m_okBtn, SIGNAL(clicked(bool)), this, SLOT(onOkBtn()));
}
