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
#include "OrderInformation.h"
#include "TableWidget.h"
//

class OrderDialog : public QDialog
{
    Q_OBJECT
private:
    QList<OrderInformation> m_orderInfoList;

    QLineEdit* m_orderIDEdit;					// 订单编号
    QLineEdit* m_contractIDEdit;                // 合同编号
    QLineEdit* m_customerNameEdit;				// 客户
    QComboBox* m_orderStatusCbx;				// 订单状态
    QDateTimeEdit* m_payTimeEdit;				// 付款时间
    QComboBox* m_payTypeCbx;					// 付款方式
    QLineEdit* m_realIncomeEdit;                // 实收
    QLineEdit* m_handlingFeeEdit;				// 平台手续费
    QPushButton* m_calHandlingFeeBtn;           // 自动计算平台手续费
    QLineEdit* m_freightCustomerEdit;			// 运费(客户)
    QLineEdit* m_freightFactoryToUsEdit;		// 运费(工厂→我司)
    QLineEdit* m_freightUsToForwardingEdit;		// 运费(我司→货代)
    QLineEdit* m_freightForeignEdit;			// 运费(国外)
    QLineEdit* m_exchangeRateEdit;				// 汇率
    QLineEdit* m_remarksEdit;					// 备注

    TableWidget* m_productTableWidget;         // 产品信息

    QPushButton* m_okBtn;
    QPushButton* m_cancelBtn;

    void constructUI();
public:
    OrderDialog(QWidget *parent = 0);

    QList<OrderInformation> getOrderList();
    void setOrderInfo(const OrderInformation& orderInfo);

    void setCustomerName(const QString& name);
    void disableCustomerNameEdit();
public slots:
    void onOKBtn();
    void onCancelBtn();
    void onCalHandlingFeeBtn();
};

#endif // ADDCUSTOMERDIALOG_H
