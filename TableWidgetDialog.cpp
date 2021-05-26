#include "TableWidgetDialog.h"
#include <QHBoxLayout>
#include <QVBoxLayout>

TableWidgetDialog::TableWidgetDialog(QWidget *parent)
    : QDialog(parent, Qt::WindowCloseButtonHint)
{
    constructUI();
    connectSlots();
}

TableWidget* TableWidgetDialog::getTableWidget()
{
    return m_tableWidget;
}

void TableWidgetDialog::setStatusText(const QString& text)
{
    m_statusLabel->setText(text);
}

void TableWidgetDialog::constructUI()
{
    m_tableWidget = new TableWidget;
    m_statusLabel = new QLabel;
    m_statusLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_okBtn = new QPushButton(tr("确认"));
    m_tableWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QHBoxLayout *okBtnLayout = new QHBoxLayout;
    //okBtnLayout->addStretch();
    okBtnLayout->addWidget(m_statusLabel);
    okBtnLayout->addWidget(m_okBtn);
    okBtnLayout->addSpacing(20);

    mainLayout->addWidget(m_tableWidget);
    mainLayout->addSpacing(20);
    mainLayout->addLayout(okBtnLayout);

    setMinimumSize(800, 400);
}

void TableWidgetDialog::connectSlots()
{
    connect(m_okBtn, SIGNAL(clicked(bool)), this, SLOT(onOkBtn()));
}

void TableWidgetDialog::onOkBtn()
{
    this->accept();
}
