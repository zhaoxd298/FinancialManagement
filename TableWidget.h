#ifndef TABLEWIDGET_H
#define TABLEWIDGET_H

#include <QTableWidget>
#include <QAction>
#include <QMenu>
#include <QList>
#include <QAxObject>
#include <QAxWidget>
#include <qaxselect.h>
#include <QFileDialog>
#include <QMetaType>

#include "CustomerInformation.h"
#include "OrderInformation.h"

class TableWidget : public QTableWidget
{
    Q_OBJECT
private:
    int m_validRowCnt;
    int m_validClumnCnt;
    QStringList m_header;
    QList<bool> m_orderList;

    QAction* m_editAction;
    QAction* m_deleteAction;
    QAction* m_copyAction;
    QAction* m_changeOrderStatusToUnpayedAction;
    QAction* m_changeOrderStatusToPayedAction;
    QAction* m_checkAllAction;

    QAction* m_searchHistoryOrderAction;      // 查找历史订单
    QAction* m_newOrderAction;                // 新建订单

    QAction* m_exportToXlsAction;
    QAction* m_copyToXlsAction;
    QAction* m_addProductAction;
    QMenu* mainMenu;

    enum {DEFAULT_ROW_CNT = 100};

    enum data_type {
        DATA_IS_CUSTOMER_INFO,
        DATA_IS_ORDER_INFO,
        DATA_IS_PRODUCT_INFO
    } m_dataType;
private:
    bool setItemData(int row, int column, int data);
    int itemData(int row, int column);

    bool setItemStrData(int row, int column, QString str);
    QString itemStrData(int row, int column);

    QString itemText(int row, int column);

    bool productIsEmpty(int row);           // 当前行商品编号是否为空

    void setCellData(int row, int column, QString data);    // 设置指定单元格内容

    void createActions();

    QString convertLineToStr(int rowNum, int startColumn, int endColumn, QString split = tr("----"));
    QString convertLineToXlsStr(int rowNum, int startColumn, int endColumn, QString split = tr("----"));

    void editCustomerInfo();
    void editOrderInfo();

    void deleteCustomerInfo();
    void deleteOrderInfo();
    void deleteProductInfo();

    void setCustomerInfo(int row, const CustomerInformation& customerInfo);

    QString doubleToStr(double d);

    bool getSelectedOrderIDList(QStringList& orderList);
protected:
    void contextMenuEvent(QContextMenuEvent *event);

public:
    TableWidget();

    void clear();

    void setDataTypeCustomerInfo();
    void setDataTypeOrderInfo();
    void setDataTypeProductInfo();

    void setTableHeader(const QStringList& header);
    void addCustomerInformation(const CustomerInformation& customerInfo);
    void updateCustomerInformation(int row, const CustomerInformation& customerInfo);

    void addProductInfo(const ProductInfo& productInfo);
    void addOrderInformation(int row, const OrderInformation& orderInfo);
    void addOrderInformation(const OrderInformation& orderInfo);
    void addOrderStatistics(const OrderInformation& orderInfo);
    void updateOrderInformation(int row, const OrderInformation& orderInfo);
    void updateOrderStatus(const QStringList& orderList, const QString& status);

    QList<ProductInfo> getProductList();

    QJsonArray saveCfg();
    void loadCfg(const QJsonArray& array);

    void checkAll();
    void uncheckAll();

signals:
    void sigEditCustomerInfo(int row, const QString& name);
    void sigEditOrderInfo(int row, const QString& orderID);

    void sigSearchHistoryOrder(const QString& name);
    void sigNewOrder(const QString& name);

    void sigChangeOrderStatus(const QStringList& orderList, const QString& status);

public slots:
    void onHeaderClicked(int i);    // 单击表头槽函数

    void onEdit();                  // 编辑
    void onCopy();

    void onChangeOrderStatusToUnpayed();  // 修改订单状态为“未结算利润”
    void onChangeOrderStatusToPayed();  // 修改订单状态为“已结算利润”

    void onCheckAll();                  // CTRL+A勾选全部用户

    void onSearchHistoryOrder();        // 查找历史订单
    void onNewOrder();                  // 新建订单

    void onExportToXls();               // 导出列表.xls
    void onCopyToXls();                 // 复制列表.xls

    void onAddOneLine();                // 新增一行
    void onDelete();                    // 删除
};

#endif // TABLEWIDGET_H
