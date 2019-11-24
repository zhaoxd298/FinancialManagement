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
    QAction* m_checkAllAction;
    QAction* m_exportToXlsAction;
    QAction* m_copyToXlsAction;
    QAction* m_addProductAction;
    QAction* m_deleteAction;
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

    void addOrderInformation(const OrderInformation& orderInfo);
    void addProductInfo(const ProductInfo& productInfo);
    void updateOrderInformation(int row, const OrderInformation& orderInfo);

    QList<ProductInfo> getProductList();

    QJsonArray saveCfg();
    void loadCfg(const QJsonArray& array);

    void checkAll();
    void uncheckAll();

public slots:
    void onHeaderClicked(int i);    // 单击表头槽函数

    void onEdit();                  // 编辑

    void onCheckAll();              // CTRL+A勾选全部用户

    void onExportToXls();           // 导出列表.xls
    void onCopyToXls();             // 复制列表.xls

    void onAddOneLine();            // 新增一行
    void onDelete();      // 删除
};

#endif // TABLEWIDGET_H
