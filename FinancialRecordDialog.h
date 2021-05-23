#ifndef FINANCIALRECORDDIALOG_H
#define FINANCIALRECORDDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QList>
#include <QTableWidget>
#include <QDateTimeEdit>
#include <QDateTime>
#include <QComboBox>
#include <QGridLayout>
#include "FinancialRecordInfo.h"

class FinancialRecordDialog : public QDialog
{
    Q_OBJECT
private:
    QGridLayout* m_gridLayout;

    FinancialRecordInfo m_financialRecordInfo;

    QLineEdit* m_contractIDEdit;                // 合同编号
    QLineEdit* m_customerNameEdit;				// 客户
    QComboBox* m_typeCbx;                       // 收支（收入/支出）
    QLineEdit* m_amountEdit;                    // 金额
    QDateTimeEdit* m_payTimeEdit;				// 付款时间
    QComboBox* m_payTypeCbx;					// 付款方式
    QLineEdit* m_payTypeRemarksEdit;			// 付款方式备注
    QLineEdit* m_remarksEdit;					// 备注

    QPushButton* m_okBtn;
    QPushButton* m_cancelBtn;

    void constructUI();
public:
    FinancialRecordDialog(QWidget *parent = 0);
    FinancialRecordInfo getFinancialRecordInfo();
    void setFinancialRecordInfo(const FinancialRecordInfo& record);

    void setCustomerName(const QString& name);
    void disableCustomerNameEdit();

public slots:
    void onOKBtn();
    void onCancelBtn();
    void onContractIDEditChanged(QString contractID);
};


#endif // ADDFINANCIALRECORDDIALOG_H
