#ifndef ORDERDIALOG_H
#define ORDERDIALOG_H

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
#include "OrderInformation.h"
#include "TableWidget.h"
#include "Version.h"
//


class OrderDialog : public QDialog
{
    Q_OBJECT
private:
    QGridLayout* m_gridLayout;

    QList<OrderInformation> m_orderInfoList;

    QLineEdit* m_orderIDEdit;					// 订单编号
    QLineEdit* m_contractIDEdit;                // 合同编号
    QLineEdit* m_customerNameEdit;				// 客户
    QComboBox* m_orderStatusCbx;				// 订单状态
    //QDateTimeEdit* m_payTimeEdit;				// 付款时间
    //QComboBox* m_payTypeCbx;					// 付款方式
    QLineEdit* m_otherPayTypeEdit;              // 其他付款方式
    QLineEdit* m_realIncomeEdit;                // 实收
    QPushButton* m_financialRecordBtn;          // 收支记录
    QLineEdit* m_handlingFeeEdit;				// 平台手续费
    QPushButton* m_calHandlingFeeBtn;           // 自动计算平台手续费
    QLineEdit* m_freightCustomerEdit;			// 运费(客户)
    QLineEdit* m_freightFactoryToUsEdit;		// 运费(工厂→我司)
    QLineEdit* m_freightUsToForwardingEdit;		// 运费(我司→货代)
    QLineEdit* m_freightForeignEdit;			// 运费(国外)
    QLineEdit* m_exchangeRateEdit;				// 汇率
    QLineEdit* m_remarksEdit;					// 备注

    TableWidget* m_productTableWidget;         // 产品信息

    int m_newOrderFlag;

    QPushButton* m_okBtn;
    QPushButton* m_cancelBtn;

    void constructUI();
    void connectSlot();

public:
    enum {
        PayByBankOfChina         = 0,   // 中国银行
        PayByMerchantsBank,             // 招商银行
        PayByConstructionBank,          // 建设银行
        PayByAgriculturalBank,          // 农业银行
        PayByGuangfaBank,               // 广发银行
        PayByBankOfCommunications,      // 交通银行
        PayByXT,                        // XT
        PayByAlibaba,                   // 信保
        PayByWechat,                    // 微信
        PayByAlipay,                    // 支付宝
#ifdef REVI_FINANCIAL
        PayByYiWu,                      // 义乌个体户
#endif
        PayByChinaUSDAccount,           // 大陆美金账户
        PayByHKAccount,                 // 香港账户
        PayByWesternUnion,              // 西联汇款
        PayByPaypal,                    // Paypal
        PayByOther                      // 其他
    };

public:
    OrderDialog(QWidget *parent = 0);

    QList<OrderInformation> getOrderList();
    void setOrderInfo(const OrderInformation& orderInfo);

    void setCustomerName(const QString& name);
    void disableCustomerNameEdit();

    void setRealIncome();

signals:
    void sigAddFinancialRecord(const QString& name = "", const QString& contractID = "");

public slots:
    void onOKBtn();
    void onCancelBtn();
    void onCalHandlingFeeBtn();
    void onPayTypeCbxIndexChanged(int index);
    void onFinancialRecordBtn();
};

#endif // ADDCUSTOMERDIALOG_H
