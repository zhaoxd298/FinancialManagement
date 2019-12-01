#ifndef CUSTOMERINFODIALOG_H
#define CUSTOMERINFODIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QList>
#include <QComboBox>
#include <QPlainTextEdit>
#include <QDateEdit>
#include "CustomerInformation.h"

class CustomerInfoDialog : public QDialog
{
    Q_OBJECT
private:
    QList<CustomerInformation> m_customerInfoList;

    QLineEdit* m_customerName;
    QComboBox* m_customerLevel;         // 客户等级
    QComboBox* m_statusCbx;             // 状态
    QDateEdit* m_enterDateEdit;         // 添加客户的日期
    QComboBox* m_inquirySource;
    QLineEdit* m_backgroundEdit;        // 背景
    QLineEdit* m_addressEdit;
    QLineEdit* m_companyName;
    QLineEdit* m_websiteEdit;               // 网址
    QLineEdit* m_customerEmail;
    QLineEdit* m_phoneNumber;
    QLineEdit* m_customerPosition;
    QPlainTextEdit* m_schedule;
    QLineEdit* m_salesman;
    QLineEdit* m_remarksEdit;           // 备注

    QPushButton* m_okBtn;
    QPushButton* m_cancelBtn;

    void constructUI();
public:
    CustomerInfoDialog(QWidget *parent = 0);

    QList<CustomerInformation> getCustomerInformationList();
    void setCustomerInfomation(const CustomerInformation& customerInfo);
    void setCustomerNameEditDisable();
public slots:
    void onOKBtn();
    void onCancelBtn();
};

#endif // ADDCUSTOMERDIALOG_H
