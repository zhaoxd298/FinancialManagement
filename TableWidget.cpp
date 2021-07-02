#include <QJsonArray>
#include "TableWidget.h"
#include "CustomerInfoDialog.h"
#include "SqlDatabase.h"
#include "OrderDialog.h"
#include "Log.h"
#include "Version.h"

#include <QClipboard>
#include <QHeaderView>
#include <QCheckBox>
#include <QTableWidgetItem>
#include <QApplication>
#include <QContextMenuEvent>
#include <QTextCodec>
#include <QMessageBox>
#include <QDesktopServices>

#include <QDebug>

TableWidget::TableWidget()
{
    m_validRowCnt = 0;
    setRowCount(DEFAULT_ROW_CNT);

    createActions();

    setDataTypeCustomerInfo();

    //horizontalHeader()->setStretchLastSection(true);    // 设置充满表宽度
    horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}"); //设置表头背景色
    setStyleSheet("selection-background-color:lightblue;"); //设置选中背景色

    //verticalHeader()->setVisible(false);    // 隐藏纵坐标表头

    //setSelectionBehavior(QAbstractItemView::SelectRows);  //整行选中的方式

    connect(horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(onHeaderClicked(int)));
}

void TableWidget::clear()
{
    for (int i=m_validRowCnt-1; i>=0; i--)
    {
        removeRow(i);
    }

    m_validRowCnt = 0;
}


void TableWidget::setDataTypeCustomerInfo()
{
    m_dataType = DATA_IS_CUSTOMER_INFO;

    m_header.clear();
    m_header << "姓名" << "客户等级" <<  "客户状态" <<  "登记日期" << "询盘来源" << "背景" << "地址"
           << "公司名" << "网址" << "邮箱" << "联系方式" << "职位" << "跟进情况" << "业务员" << "备注";
    m_validClumnCnt = m_header.size();
    setColumnCount(m_header.size());
    setHorizontalHeaderLabels(m_header);
    for (int i=0; i<m_header.size(); i++)
    {
        setColumnWidth(i, 80);
    }

    m_orderList.clear();
    m_orderList.append(false);
    for (int i=1; i<m_header.size(); i++) {
        m_orderList.append(true);
    }

    if (NULL != mainMenu)
    {
        mainMenu->clear(); //清除原有菜单
        mainMenu->addSeparator();

        m_editAction->setText(tr("编辑"));
        mainMenu->addAction(m_editAction);

        m_deleteAction->setText(tr("删除"));
        mainMenu->addAction(m_deleteAction);
        mainMenu->addSeparator();

        mainMenu->addAction(m_copyAction);
        mainMenu->addAction(m_checkAllAction);
        mainMenu->addSeparator();

        mainMenu->addAction(m_newOrderAction);
        mainMenu->addAction(m_searchHistoryOrderAction);
        mainMenu->addSeparator();

        mainMenu->addAction(m_newFinancialRecordAction);
        mainMenu->addAction(m_searchFinancialRecordAction);
        mainMenu->addSeparator();

        mainMenu->addAction(m_exportToXlsAction);
        mainMenu->addAction(m_copyToXlsAction);
        mainMenu->addSeparator();
    }

    setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void TableWidget::setDataTypeOrderInfo()
{
    m_dataType = DATA_IS_ORDER_INFO;

    m_header.clear();
    m_header << "合同编号" << "客户" << "订单状态" << "品名" << "单价"
           << "成本单价" << "数量" << "规格"  << "唛头" << "实收金额"
           << "应收金额" << "运费（客户）" << "运费(厂家→我司)" << "运费(我司→货代)"
           << "运费(国外)" << "汇率" << "平台手续费" << "总支出" << "总利润" << "备注";

    m_validClumnCnt = m_header.size();
    setColumnCount(m_header.size());
    setHorizontalHeaderLabels(m_header);

    setColumnWidth(0, 80);
    setColumnWidth(1, 100);
    //setColumnWidth(2, 80);
    for (int i=2; i<m_header.size(); i++)
    {
        setColumnWidth(i, 80);
    }

    if (NULL != mainMenu)
    {
        mainMenu->clear(); //清除原有菜单
        mainMenu->addSeparator();

        m_editAction->setText(tr("编辑"));
        mainMenu->addAction(m_editAction);

        m_deleteAction->setText(tr("删除"));
        mainMenu->addAction(m_deleteAction);
        mainMenu->addSeparator();

        mainMenu->addAction(m_copyAction);
        mainMenu->addAction(m_checkAllAction);
        mainMenu->addSeparator();

        mainMenu->addAction(m_newFinancialRecordAction);
        mainMenu->addAction(m_searchFinancialRecordAction);
        mainMenu->addSeparator();

        mainMenu->addAction(m_exportToXlsAction);
        mainMenu->addAction(m_copyToXlsAction);
        mainMenu->addSeparator();

        mainMenu->addAction(m_changeOrderStatusToUnpayedAction);
        mainMenu->addAction(m_changeOrderStatusToPayedAction);
        mainMenu->addSeparator();
    }

    setEditTriggers(QAbstractItemView::NoEditTriggers);
}


void TableWidget::setDataTypeProductInfo()
{
    m_dataType = DATA_IS_PRODUCT_INFO;

    m_header.clear();
    m_header << "商品名称" << "单价" << "成本单价" << "数量" << "规格" << "唛头";
    m_validClumnCnt = m_header.size();
    setColumnCount(m_header.size());
    setHorizontalHeaderLabels(m_header);

    for (int i=0; i<m_header.size(); i++)
    {
        setColumnWidth(i, 80);
    }

    if (NULL != mainMenu)
    {
        mainMenu->clear(); //清除原有菜单

        mainMenu->addAction(m_pasteAction);
        mainMenu->addSeparator();

        mainMenu->addAction(m_addProductAction);
        m_deleteAction->setText(tr("删除该产品"));
        mainMenu->addAction(m_deleteAction);
    }

    //setEditTriggers(QAbstractItemView::AllEditTriggers);
    setEditTriggers(QAbstractItemView::AnyKeyPressed | QAbstractItemView::DoubleClicked);
}


void TableWidget::setDataTypeFinancialInfo()
{
    m_dataType = DATA_IS_FINANCIAL_INFO;

    m_header.clear();
    m_header << "客户" << "合同编号" << "收支" << "金额" << "付款时间" << "付款方式" << "备注";
    m_validClumnCnt = m_header.size();
    setColumnCount(m_header.size());
    setHorizontalHeaderLabels(m_header);

    setColumnWidth(0, 80);
    setColumnWidth(1, 80);
    setColumnWidth(2, 80);
    setColumnWidth(3, 80);
    setColumnWidth(4, 120);
    setColumnWidth(5, 120);
    setColumnWidth(6, 120);

    if (NULL != mainMenu)
    {
        mainMenu->clear(); //清除原有菜单
        mainMenu->addSeparator();

        m_editAction->setText(tr("编辑"));
        mainMenu->addAction(m_editAction);

        m_deleteAction->setText(tr("删除"));
        mainMenu->addAction(m_deleteAction);
        mainMenu->addSeparator();

        mainMenu->addAction(m_newFinancialRecordAction);
        mainMenu->addAction(m_searchFinancialRecordAction);
        mainMenu->addSeparator();

        mainMenu->addAction(m_copyAction);
        mainMenu->addAction(m_checkAllAction);
        mainMenu->addSeparator();

        mainMenu->addAction(m_exportToXlsAction);
        mainMenu->addAction(m_copyToXlsAction);
        mainMenu->addSeparator();
    }

    setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void TableWidget::setCustomerInfo(int row, const CustomerInformation& customerInfo)
{
    setCellData(row, 0, customerInfo.name);
    setCellData(row, 1, customerInfo.level);
    setCellData(row, 2, customerInfo.status);
    setCellData(row, 3, customerInfo.enterDate);
    setCellData(row, 4, customerInfo.inquirySource);
    setCellData(row, 5, customerInfo.background);
    setCellData(row, 6, customerInfo.address);
    setCellData(row, 7, customerInfo.companyName);
    setCellData(row, 8, customerInfo.websit);
    setCellData(row, 9, customerInfo.email);
    setCellData(row, 10, customerInfo.phoneNumber);
    setCellData(row, 11, customerInfo.position);
    setCellData(row, 12, customerInfo.schedule);
    setCellData(row, 13, customerInfo.salesman);
    setCellData(row, 14, customerInfo.remarks);
}

QString TableWidget::doubleToStr(double d)
{
    QString str = QString::number(d, 'f');

    if (str.contains('.'))
    {
        for (int i=str.size()-1; i>0; i--)
        {
            if ('0' == str[i])
            {
                str.remove(i, 1);
            }
            else if ('.' == str[i])
            {
                str.remove(i, 1);
                break;
            }
            else
            {
                break;
            }
        }
    }

    return str;
}

bool TableWidget::getSelectedOrderIDList(QStringList &orderList)
{
    bool ret = false;
    orderList.clear();

    QList<QTableWidgetSelectionRange> rangeList = selectedRanges();
    for (int i=0; i<rangeList.size(); i++)
    {
        for (int row=rangeList[i].topRow(); row<=rangeList[i].bottomRow(); row++)
        {
            QString orderID = itemStrData(row, 0);
            QString status = itemText(row, 2);
            if ((!orderID.isEmpty()) && (!status.isEmpty()))
            {
                orderList.append(orderID);
            }
        }
    }

    if (!orderList.isEmpty())
    {
        ret = true;
    }

    return ret;
}

void TableWidget::setTableHeader(const QStringList& header)
{
    m_header = header;
    m_validClumnCnt = header.size();
    setColumnCount(m_validClumnCnt);
    setHorizontalHeaderLabels(header);

    //qDebug() << m_validClumnCnt << header;
}


void TableWidget::addCustomerInformation(const CustomerInformation& customerInfo)
{
    insertRow(m_validRowCnt);

    setCustomerInfo(m_validRowCnt, customerInfo);

    m_validRowCnt++;
}


void TableWidget::updateCustomerInformation(int row, const CustomerInformation& customerInfo)
{
    if ((row < 0) || (row > m_validRowCnt))
    {
        return;
    }

    setCustomerInfo(row, customerInfo);
}

void TableWidget::addProductInfo(const ProductInfo& productInfo)
{
    if (DATA_IS_PRODUCT_INFO != m_dataType)
    {
        return;
    }

    insertRow(m_validRowCnt);
    setCellData(m_validRowCnt, 0, productInfo.productName);
    setCellData(m_validRowCnt, 1, QString::number(productInfo.price));
    setCellData(m_validRowCnt, 2, QString::number(productInfo.costPrice));
    setCellData(m_validRowCnt, 3, QString::number(productInfo.count));
    setCellData(m_validRowCnt, 4, productInfo.spec);
    setCellData(m_validRowCnt, 5, productInfo.mark);

    setItemData(m_validRowCnt, 0, productInfo.number);

    m_validRowCnt++;
}

void TableWidget::addOrderInformation(int row, const OrderInformation& orderInfo)
{
    if (orderInfo.productList.size() <= 0)
    {
        return;
    }

    int startRowCnt = row;
    int endRowCnt = row;
    insertRow(row);

    setCellData(row, orderInfo.ContractIDColumn, orderInfo.contractID);
    setItemStrData(row, orderInfo.ContractIDColumn, orderInfo.orderID);
    setItemData(row, orderInfo.ContractIDColumn, orderInfo.productList.size());

    setCellData(row, orderInfo.CustomerNameColumn, orderInfo.customerName);
    setCellData(row, orderInfo.OrderStatusColumn, orderInfo.orderStatus);
    setCellData(row, orderInfo.ProductNameColumn, orderInfo.productList[0].productName);
    setCellData(row, orderInfo.PriceColumn, doubleToStr(orderInfo.productList[0].price));
    setCellData(row, orderInfo.CostPriceColumn, doubleToStr(orderInfo.productList[0].costPrice));
    setCellData(row, orderInfo.CountColumn, QString::number(orderInfo.productList[0].count));
    setCellData(row, orderInfo.SpecColumn, orderInfo.productList[0].spec);
    setCellData(row, orderInfo.MarkColumn, orderInfo.productList[0].mark);
    //setCellData(row, orderInfo.PayTimeColumn, orderInfo.payTime);
    //setCellData(row, orderInfo.PayTypeColumn, orderInfo.payType);
    setCellData(row, orderInfo.RealIncomeColumn, doubleToStr(orderInfo.realIncome));
    setCellData(row, orderInfo.ShouldIncomeColumn, doubleToStr(orderInfo.shouldIncome));
    setCellData(row, orderInfo.FreightCustomerColumn, doubleToStr(orderInfo.freightCustomer));
    setCellData(row, orderInfo.FreightFactoryToUsColumn, doubleToStr(orderInfo.freightFactoryToUs));
    setCellData(row, orderInfo.FreightUsToForwardingColumn, doubleToStr(orderInfo.freightUsToForwarding));
    setCellData(row, orderInfo.FreightForeignColumn, doubleToStr(orderInfo.freightForeign));
    setCellData(row, orderInfo.ExchangeRateColumn, doubleToStr(orderInfo.exchangeRate));
    setCellData(row, orderInfo.HandlingFeeColumn, doubleToStr(orderInfo.handlingFee));
    setCellData(row, orderInfo.TotalExpensesColumn, doubleToStr(orderInfo.tatolExpenses));
    setCellData(row, orderInfo.TotalProfitColumn, doubleToStr(orderInfo.totalProfit));
#if defined(EXSUN_LIGHTING_FINANCIAL)   // 亿生有合伙人利润
    setCellData(row, orderInfo.PartnerProfitColumn, doubleToStr(orderInfo.partnerProfit));
    setCellData(row, orderInfo.RemarksColumn, orderInfo.remarks);
#elif defined(REVI_FINANCIAL)           // 睿为没有合伙人利润
    setCellData(row, orderInfo.RemarksColumn, orderInfo.remarks);
#endif

    m_validRowCnt++;
    for (int i=1; i<orderInfo.productList.size(); i++)
    {
        insertRow(row+i);
        setCellData(row+i, orderInfo.ProductNameColumn, orderInfo.productList[i].productName);
        setCellData(row+i, orderInfo.PriceColumn, doubleToStr(orderInfo.productList[i].price));
        setCellData(row+i, orderInfo.CostPriceColumn, doubleToStr(orderInfo.productList[i].costPrice));
        setCellData(row+i, orderInfo.CountColumn, QString::number(orderInfo.productList[i].count));
        setCellData(row+i, orderInfo.SpecColumn, orderInfo.productList[i].spec);
        setCellData(row+i, orderInfo.MarkColumn, orderInfo.productList[i].mark);
        m_validRowCnt++;
        endRowCnt = row + i;
    }

    int productCnt = orderInfo.productList.size();
    if (endRowCnt-startRowCnt >= 1)
    {
        for (int i=0; i<3; i++)
        {
            setSpan(startRowCnt, i, productCnt, 1);
        }

        for (int i=9; i<m_validClumnCnt; i++)
        {
            setSpan(startRowCnt, i, productCnt, 1);
        }
    }
}

void TableWidget::addOrderInformation(const OrderInformation& orderInfo)
{
    addOrderInformation(m_validRowCnt, orderInfo);
}

void TableWidget::updateOrderStatistics(int row)
{
    double realIncomeSum = 0;
    double shouldIncomeSum = 0;
    double freightCustomerSum = 0;
    double freightFactoryToUsSum = 0;
    double freightUsToForwardingSum = 0;
    double freightForeignSum = 0;
    double handlingFeeSum = 0;
    double tatolExpensesSum = 0;
    double totalProfitSum = 0;

    //qDebug() << "***********row:" << row;

    for (int i=0; i<row; i++)
    {
        realIncomeSum += itemText(i, OrderInformation::RealIncomeColumn).toDouble();
        shouldIncomeSum += itemText(i, OrderInformation::ShouldIncomeColumn).toDouble();
        freightCustomerSum += itemText(i, OrderInformation::FreightCustomerColumn).toDouble();
        freightFactoryToUsSum += itemText(i, OrderInformation::FreightFactoryToUsColumn).toDouble();
        freightUsToForwardingSum += itemText(i, OrderInformation::FreightUsToForwardingColumn).toDouble();
        freightForeignSum += itemText(i, OrderInformation::FreightForeignColumn).toDouble();
        handlingFeeSum += itemText(i, OrderInformation::HandlingFeeColumn).toDouble();
        tatolExpensesSum += itemText(i, OrderInformation::TotalExpensesColumn).toDouble();
        totalProfitSum += itemText(i, OrderInformation::TotalProfitColumn).toDouble();
    }

    setCellData(row, 0, tr("合计"));
    setCellData(row, OrderInformation::RealIncomeColumn, doubleToStr(realIncomeSum));
    setCellData(row, OrderInformation::ShouldIncomeColumn, doubleToStr(shouldIncomeSum));
    setCellData(row, OrderInformation::FreightCustomerColumn, doubleToStr(freightCustomerSum));
    setCellData(row, OrderInformation::FreightFactoryToUsColumn, doubleToStr(freightFactoryToUsSum));
    setCellData(row, OrderInformation::FreightUsToForwardingColumn, doubleToStr(freightUsToForwardingSum));
    setCellData(row, OrderInformation::FreightForeignColumn, doubleToStr(freightForeignSum));
    setCellData(row, OrderInformation::HandlingFeeColumn, doubleToStr(handlingFeeSum));
    setCellData(row, OrderInformation::TotalExpensesColumn, doubleToStr(tatolExpensesSum));
    setCellData(row, OrderInformation::TotalProfitColumn, doubleToStr(totalProfitSum));
}

void TableWidget::updateOrderStatistics()
{
    for (int i=0; i<m_validRowCnt; i++)
    {
        if (tr("合计") == itemText(i, 0))
        {
            updateOrderStatistics(i);
        }
    }
}

void TableWidget::addOrderStatistics()
{
    insertRow(m_validRowCnt++);
    insertRow(m_validRowCnt);

    //qDebug() << "########row:" << m_validRowCnt;
    updateOrderStatistics(m_validRowCnt);

    m_validRowCnt++;
}

void TableWidget::updateOrderInformation(int row, const OrderInformation& orderInfo)
{
    if ((row < 0) || (row > m_validRowCnt) || (orderInfo.productList.size() <= 0))
    {
        return;
    }

    int productCnt = itemData(row, 0);
    //qDebug() << "row:" << row << "productCnt:" << productCnt;

    for (int i=row+productCnt-1; i>=row; i--)
    {
        //qDebug() << "remove:" << i;
        removeRow(i);
        m_validRowCnt--;
    }

    addOrderInformation(row, orderInfo);
}

void TableWidget::updateOrderStatus(const QStringList& orderList, const QString& status)
{
    for (int i=0; i<orderList.size(); i++)
    {
        for (int row=0; row<m_validRowCnt; row++)
        {
            if (orderList[i] == itemStrData(row, 0))
            {
                setCellData(row, 2, status);
            }
        }
    }
}

QList<ProductInfo> TableWidget::getProductList()
{
    QList<ProductInfo> productList;

    if (DATA_IS_PRODUCT_INFO == m_dataType)
    {
        for (int i=0; i<rowCount(); i++)
        {
            ProductInfo product;
            bool priceOk, costPriceOk, countOk;

            product.number = itemData(i, 0);
            product.productName = itemText(i, 0);
            product.price = itemText(i, 1).toDouble(&priceOk);
            product.costPrice = itemText(i, 2).toDouble(&costPriceOk);
            product.count = itemText(i, 3).toLong(&countOk);
            product.spec = itemText(i, 4);
            product.mark = itemText(i, 5);

            //qDebug() << "mark:" << product.mark;
            if (product.productName.isEmpty() ||
                false == priceOk ||
                false == costPriceOk ||
                false == countOk)
            {
                break;
            }

            productList.append(product);
        }
    }

    return productList;
}

bool TableWidget::setItemData(int row, int column, int data)
{
    QTableWidgetItem* it = item(row, column);
    if (NULL != it) {
        it->setData(Qt::UserRole+1, QVariant(data));
        return true;
    }

    return false;
}

void TableWidget::setFinancialInfo(int row, const FinancialRecordInfo& financialInfo)
{
    setCellData(row, 0, financialInfo.customerName);
    setCellData(row, 1, financialInfo.contractID);
    setCellData(row, 2, financialInfo.type);
    setCellData(row, 3, doubleToStr(financialInfo.amount));
    setCellData(row, 4, financialInfo.payTime);
    setCellData(row, 5, financialInfo.payType);
    setCellData(row, 6, financialInfo.remarks);

    setItemData(row, 0, financialInfo.number);
}

void TableWidget::addFinancialInfo(const FinancialRecordInfo& financialInfo)
{
    if (DATA_IS_FINANCIAL_INFO != m_dataType)
    {
        return;
    }

    insertRow(m_validRowCnt);

    setFinancialInfo(m_validRowCnt, financialInfo);

    m_validRowCnt++;
}

void TableWidget::updateFinancialInfo(int row, const FinancialRecordInfo& financialInfo)
{
    if ((row < 0) || (row > m_validRowCnt))
    {
        return;
    }

    setFinancialInfo(row, financialInfo);
}

void TableWidget::addFinancialStatistics()
{
    insertRow(m_validRowCnt++);
    insertRow(m_validRowCnt++);
    insertRow(m_validRowCnt++);
    insertRow(m_validRowCnt++);
    insertRow(m_validRowCnt);

    updateFinancialStatistics(m_validRowCnt-3);

    m_validRowCnt++;
}

void TableWidget::updateFinancialStatistics()
{
    for (int i=0; i<m_validRowCnt; i++)
    {
        if (tr("总收入") == itemText(i, 0))
        {
            updateFinancialStatistics(i);
        }
    }
}

void TableWidget::updateFinancialStatistics(int row)
{
    double totalIncome = 0;
    double totaloutlay = 0;
    double remain = 0;

    for (int i=0; i<row; i++)
    {
        QString type = itemText(i, 2);
        double amount = itemText(i, 3).toDouble();

        if (tr("收入") == type)
        {
            totalIncome += amount;
        }
        else if (tr("支出") == type)
        {
            totaloutlay += amount;
        }
    }

    remain = totalIncome - totaloutlay;

    setCellData(row, 0, tr("总收入"));
    setCellData(row, 1, doubleToStr(totalIncome));

    setCellData(row+1, 0, tr("总支出"));
    setCellData(row+1, 1, doubleToStr(totaloutlay));

    setCellData(row+2, 0, tr("结余"));
    setCellData(row+2, 1, doubleToStr(remain));
}

int TableWidget::itemData(int row, int column)
{
    int data = -1;

    QTableWidgetItem* it = item(row, column);
    if (NULL != it) {
        data = it->data(Qt::UserRole+1).toInt();
    }

    return data;
}

bool TableWidget::setItemStrData(int row, int column, QString str)
{
    QTableWidgetItem* it = item(row, column);
    if (NULL != it) {
        it->setData(Qt::UserRole+2, QVariant(str.toLatin1()));
        return true;
    }

    return false;
}

QString TableWidget::itemStrData(int row, int column)
{
    QString str = "";

    QTableWidgetItem* it = item(row, column);
    if (NULL != it) {
        str = it->data(Qt::UserRole+2).toString();
    }

    return str;
}

QString TableWidget::itemText(int row, int column)
{
    QString str = "";

    /*if ((2 == row) || (3 == row)) {
        QLineEdit* lineEdit = dynamic_cast<QLineEdit*>(cellWidget(row, column));
        if (NULL != lineEdit) {
            str = lineEdit->text();
        }
    } else {
        QTableWidgetItem* it = item(row, column);
        if (NULL != it) {
            str = it->text();
        }
    }*/

    QTableWidgetItem* it = item(row, column);
    if (NULL != it) {
        str = it->text();
    }

    return str;
}

void TableWidget::setCellData(int row, int column, QString data)
{
    /*if ((2==column) || (3==column)) {       // 密码需要显示为*，故嵌入QLineEdit
        if (NULL == cellWidget(row, column)) {
            QLineEdit* lineEdit = new QLineEdit(data);
            lineEdit->setEchoMode(QLineEdit::Password);
            setCellWidget(row, column, lineEdit);
        } else {
            dynamic_cast<QLineEdit*>(cellWidget(row, column))->setText(data);
        }
    } else {
        if (NULL == item(row, column)) {
            QTableWidgetItem* item = new QTableWidgetItem(data);
            setItem(row, column, item);
        } else {
            item(row, column)->setText(data);
        }
    }*/

    QTableWidgetItem* it = item(row, column);
    if (NULL == it) {
        it = new QTableWidgetItem(data);
        it->setToolTip(data);
        //it->setData(Qt::DisplayRole, QVariant(data));
        setItem(row, column, it);
    } else {
        it->setText(data);
        it->setToolTip(data);
        //it->setData(Qt::DisplayRole, QVariant(data));
    }
}


bool TableWidget::productIsEmpty(int row)
{
    bool ret = true;

    if (itemText(row, 6).isEmpty()) {
        ret = true;
    } else {
        ret = false;
    }

    return ret;
}


// 创建Action
void TableWidget::createActions()
{
    mainMenu = new QMenu(this);

    m_editAction = new QAction(tr("编辑该行信息"), this);
    connect(m_editAction, SIGNAL(triggered(bool)), this, SLOT(onEdit()));

    m_deleteAction = new QAction(tr("删除该产品"), this);
    connect(m_deleteAction, SIGNAL(triggered(bool)), this, SLOT(onDelete()));
    m_deleteAction->setShortcut(QKeySequence::Delete);
    addAction(m_deleteAction);

    m_copyAction = new QAction(tr("复制"), this);
    connect(m_copyAction, SIGNAL(triggered(bool)), this, SLOT(onCopy()));
    m_copyAction->setShortcut(QKeySequence::Copy);
    addAction(m_copyAction);

    m_pasteAction = new QAction(tr("粘贴"), this);
    connect(m_pasteAction, SIGNAL(triggered(bool)), this, SLOT(onPaste()));
    m_pasteAction->setShortcut(QKeySequence::Paste);
    addAction(m_pasteAction);

    m_changeOrderStatusToUnpayedAction  = new QAction(tr("设置订单状态为\"未结算利润\""), this);
    connect(m_changeOrderStatusToUnpayedAction, SIGNAL(triggered(bool)), this, SLOT(onChangeOrderStatusToUnpayed()));
    m_changeOrderStatusToPayedAction  = new QAction(tr("设置订单状态为\"已结算利润\""), this);
    connect(m_changeOrderStatusToPayedAction, SIGNAL(triggered(bool)), this, SLOT(onChangeOrderStatusToPayed()));

    m_checkAllAction = new QAction(tr("全选"), this);
    connect(m_checkAllAction, SIGNAL(triggered(bool)), this, SLOT(onCheckAll()));
    m_checkAllAction->setShortcut(QKeySequence::SelectAll);
    addAction(m_checkAllAction);  // 将checkAllAction添加到当前窗体以响应快捷键

    m_searchHistoryOrderAction = new QAction(tr("查找历史订单"), this);
    connect(m_searchHistoryOrderAction, SIGNAL(triggered(bool)), this, SLOT(onSearchHistoryOrder()));

    m_newOrderAction = new QAction(tr("新建订单"), this);
    connect(m_newOrderAction, SIGNAL(triggered(bool)), this, SLOT(onNewOrder()));

    m_exportToXlsAction = new QAction(tr("导出列表.xls"), this);         // 导出列表.xls
    connect(m_exportToXlsAction, SIGNAL(triggered(bool)), this, SLOT(onExportToXls()));
    m_copyToXlsAction = new QAction(tr("复制列表.xls"), this);           // 复制列表.xls
    connect(m_copyToXlsAction, SIGNAL(triggered(bool)), this, SLOT(onCopyToXls()));


    m_addProductAction = new QAction(tr("新增一行"), this);
    connect(m_addProductAction, SIGNAL(triggered(bool)), this, SLOT(onAddOneLine()));

    m_newFinancialRecordAction = new QAction(tr("新增收支记录"), this);
    connect(m_newFinancialRecordAction, SIGNAL(triggered(bool)), this, SLOT(onNewFinancialRecordAction()));
    m_searchFinancialRecordAction = new QAction(tr("查找收支记录"), this);;
    connect(m_searchFinancialRecordAction, SIGNAL(triggered(bool)), this, SLOT(onSearchFinancialRecordAction()));

    mainMenu->clear(); //清除原有菜单
    mainMenu->addSeparator();

    mainMenu->addAction(m_editAction);
    mainMenu->addSeparator();

    mainMenu->addAction(m_checkAllAction);
    mainMenu->addSeparator();

    mainMenu->addAction(m_exportToXlsAction);
    mainMenu->addAction(m_copyToXlsAction);
    mainMenu->addSeparator();
}


void TableWidget::contextMenuEvent(QContextMenuEvent *event)
{
    //菜单出现的位置为当前鼠标的位置
    mainMenu->exec(QCursor::pos());
    event->accept();
}

QJsonArray TableWidget::saveCfg()
{
    QJsonArray array;
/*
    QList<UserAccount> userAccountList = getAllUserAccount();
    for (int i=0; i<userAccountList.size(); i++) {

        QJsonArray subArray;
        subArray.append(userAccountList[i].num);
        subArray.append(userAccountList[i].isChecked);

        QByteArray tmp = userAccountList[i].name.toLocal8Bit();
        subArray.append(QString::fromLocal8Bit(tmp));

        subArray.append(userAccountList[i].loginPasswd);
        subArray.append(userAccountList[i].payPasswd);
        subArray.append(userAccountList[i].cookie);

        tmp = userAccountList[i].statusStr.toLocal8Bit();
        subArray.append(QString::fromLocal8Bit(tmp));

        subArray.append(userAccountList[i].productIDNum);

        array.append(QJsonValue(subArray));
    }
*/
    return array;
}

void TableWidget::loadCfg(const QJsonArray &array)
{
    (void)array;
/*    QList<UserAccount> userAccountList;

    int size = array.size();
    for (int i=0; i<size; ++i) {
        QJsonValue value = array.at(i);
        if (value.isArray()) {
            QJsonArray subArray = value.toArray();
            int subSize = subArray.size();
            UserAccount userAccount;
            for (int j=0; j<subSize; ++j) {
                QJsonValue subValue = subArray.at(j);
                if (subValue.isDouble()) {
                    userAccount.num = subValue.toInt();
                    //qDebug() << "num:" << userAccount.num;
                }

                if (subValue.isBool()) {
                    userAccount.isChecked = subValue.toBool();
                    //qDebug() << "isChecked:" << userAccount.isChecked;
                }

                if (subValue.isString()) {
                    if (2 == j) {
                        userAccount.name = subValue.toString();
                        //qDebug() << "name:" << subValue.toString();
                    }
                    if (3 == j) {
                        userAccount.loginPasswd = subValue.toString();
                        //qDebug() << "loginPasswd:" << userAccount.loginPasswd;
                    }
                    if (4 == j) {
                        userAccount.payPasswd = subValue.toString();
                        //qDebug() << "payPasswd:" << userAccount.payPasswd;
                    }
                    if (5 == j) {
                        userAccount.cookie = subValue.toString();
                        //qDebug() << "cookie:" << userAccount.cookie;
                    }
                    if (6 == j) {
                        userAccount.statusStr = subValue.toString();
                        //qDebug() << "status:" << userAccount.status;
                    }
                    if (7 == j) {
                        userAccount.productIDNum = subValue.toString();
                        //qDebug() << "productIDNum:" << userAccount.productIDNum;
                    }
                }
            }
            userAccountList.append(userAccount);
        }
    }

    setUserAccount(userAccountList);
    */
}

void TableWidget::checkAll()
{
    for (int i=0; i<m_validRowCnt; i++) {
        item(i, 0)->setCheckState(Qt::Checked);
    }
}

void TableWidget::uncheckAll()
{
    for (int i=0; i<m_validRowCnt; i++) {
        item(i, 0)->setCheckState(Qt::Unchecked);
    }
}

void TableWidget::onHeaderClicked(int i)
{
    if (DATA_IS_CUSTOMER_INFO == m_dataType)
    {
        if (m_orderList[i]) {
            this->sortItems(i, Qt::AscendingOrder);
        } else {
            this->sortItems(i, Qt::DescendingOrder);
        }

        m_orderList[i] = !m_orderList[i];
    }
}

void TableWidget::onEdit()                  // 编辑
{
    if (DATA_IS_CUSTOMER_INFO == m_dataType)
    {
        editCustomerInfo();
    }
    else if (DATA_IS_ORDER_INFO == m_dataType)
    {
        editOrderInfo();
    }
    else if (DATA_IS_FINANCIAL_INFO == m_dataType)
    {
        editFinancialInfo();
    }
}

void TableWidget::onCopy()
{
    /*QList<QTableWidgetSelectionRange> rangeList = selectedRanges();
    for (int i=0; i<rangeList.size(); i++)
    {
        qDebug() << "onCopy" << rangeList[i].topRow() << rangeList[i].bottomRow() << rangeList[i].leftColumn() << rangeList[i].rightColumn();

    }*/

    QTableWidgetSelectionRange range = selectedRanges().first();
    QString str = "";
    qDebug() << "onCopy" << range.topRow() << range.bottomRow() << range.leftColumn() << range.rightColumn();

    for (int i=range.topRow(); i<=range.bottomRow(); i++)
    {
        for (int j=range.leftColumn(); j<=range.rightColumn(); j++)
        {
            str += itemText(i, j);
            if (j < range.rightColumn())
            {
                str += "\t";
            }
        }

        str += "\n";
    }

    QApplication::clipboard()->setText(str);
}

void TableWidget::onPaste()
{
    QString str = QApplication::clipboard()->text();

    QTableWidgetSelectionRange range = selectedRanges().first();
    int topRow = range.topRow();
    int leftColumn = range.leftColumn();

    QStringList rowList = str.split('\n');
    for (int i=0; i<rowList.size(); i++)
    {
        QStringList itemList = rowList[i].split('\t');
        int size = itemList.size();
        if (size > columnCount())
        {
            size = columnCount();
        }

        for (int j=0; j<size; j++)
        {
            if (leftColumn+j < columnCount())
            {
                setCellData(topRow+i, leftColumn+j, itemList[j]);
            }
        }
    }
}

void TableWidget::onChangeOrderStatusToUnpayed()  // 修改订单状态为“未结算利润”
{
    QStringList orderList;

    bool ret = getSelectedOrderIDList(orderList);

    if (ret)
    {
        QString status = QString(tr("未结算利润"));
        emit sigChangeOrderStatus(orderList, status);
    }
}

void TableWidget::onChangeOrderStatusToPayed()
{
    QStringList orderList;

    bool ret = getSelectedOrderIDList(orderList);

    if (ret)
    {
        QString status = QString(tr("已结算利润"));
        emit sigChangeOrderStatus(orderList, status);
    }
}

void TableWidget::onCheckAll()              // CTRL+A勾选全部用户
{
    //for (int i=0; i<rowCount(); i++) {
    /*for (int i=0; i<m_validRowCnt; i++) {
        item(i, 0)->setCheckState(Qt::Checked);
    }*/
    selectAll();
}

void TableWidget::onSearchHistoryOrder()      // 查找历史订单
{
    QString name = itemText(currentRow(), 0);
    if (name.isEmpty())
    {
        QMessageBox::critical(this, QString(tr("错误")), QString(tr("获取客户名失败！")), QString(tr("确定")));
        return;
    }

    for (int i=name.size()-1; i>0; i--)
    {
        if (' ' == name[i])
        {
            name.remove(i, 1);
        }
        else
        {
            break;
        }
    }

    emit sigSearchHistoryOrder(name);
}

void TableWidget::onNewOrder()                // 新建订单
{
    qDebug() << "onNewOrder";
    QString name = itemText(currentRow(), 0);
    if (name.isEmpty())
    {
        QMessageBox::critical(this, QString(tr("错误")), QString(tr("获取客户名失败！")), QString(tr("确定")));
        return;
    }

    emit sigNewOrder(name);
}

QString TableWidget::convertLineToStr(int rowNum, int startColumn, int endColumn, QString split)
{
    QString str = "";

    for (int i=startColumn; i<=endColumn; i++) {
        QString s = itemText(rowNum, i);

        if ((i > startColumn) && (!s.isEmpty())) {
            str += split;
        }
        str += s;
    }

    //str += tr("\n");

    return str;
}

QString TableWidget::convertLineToXlsStr(int rowNum, int startColumn, int endColumn, QString split)
{
    QString str = "";

    for (int i=startColumn; i<=endColumn; i++) {
        QString s = itemText(rowNum, i);
        s.replace('\n', ' ');
        if (i > startColumn) {
            str += split;
        }
        str += s;
    }

    str += tr("\n");

    return str;
}

void TableWidget::editCustomerInfo()
{
    QString name = itemText(currentRow(), 0);
    if (name.isEmpty())
    {
        QMessageBox::critical(this, QString(tr("错误")), QString(tr("获取客户名失败！")), QString(tr("确定")));
        return;
    }

    emit sigEditCustomerInfo(currentRow(), name);
}

void TableWidget::editOrderInfo()
{
    QString orderID = itemStrData(currentRow(), 0);
    if (orderID.isEmpty())
    {
        QMessageBox::critical(this, QString(tr("错误")), QString(tr("获取订单编号失败失败！")), QString(tr("确定")));
        return;
    }

    emit sigEditOrderInfo(currentRow(), orderID);
}

void TableWidget::editFinancialInfo()
{
    int number = itemData(currentRow(), 0);
    if (number < 0)
    {
        QMessageBox::critical(this, QString(tr("错误")), QString(tr("获取收支记录编号失败失败！")), QString(tr("确定")));
        return;
    }

    emit sigEditFinancialInfo(currentRow(), number, this);
}

void TableWidget::deleteCustomerInfo()
{
    QString name = itemText(currentRow(), 0);
    if (name.isEmpty())
    {
        QMessageBox::critical(this, QString(tr("错误")), QString(tr("获取客户名失败！")), QString(tr("确定")));
        return;
    }

    if (0 == QMessageBox::question(this, QString(tr("删除")), QString(tr("客户信息删除后无法恢复，是否删除？")), QString(tr("是")), QString(tr("否"))))
    {
        SqlDatabase sql("delete");
        if (sql.deleteCustomerInfo(name))
        {
            removeRow(currentRow());
            //QMessageBox::information(this, QString(tr("提示")), QString(tr("删除客户\"%1\"信息成功！")).arg(name), QString(tr("确定")));
        }
        else
        {
            QMessageBox::critical(this, QString(tr("错误")), QString(tr("删除客户\"%1\"信息失败:%2")).arg(name).arg(sql.getErrorStr()), QString(tr("确定")));
        }
    }
}

void TableWidget::deleteOrderInfo()
{
    QString orderID = itemStrData(currentRow(), 0);
    QString contractID = itemText(currentRow(), 0);
    if (orderID.isEmpty())
    {
        QMessageBox::critical(this, QString(tr("错误")), QString(tr("获取订单号失败！")), QString(tr("确定")));
        return;
    }

    if (0 == QMessageBox::question(this, QString(tr("删除")), QString(tr("订单信息删除后无法恢复，是否删除？")), QString(tr("是")), QString(tr("否"))))
    {
        SqlDatabase sql("delete");
        if (sql.deleteOrderInfo(orderID))
        {
            int startRow = currentRow();
            int n = this->rowSpan(startRow, 0);
            for (int i=n+startRow-1; i>=startRow; i--)
            {
                qDebug() << "remove:" << i;
                removeRow(i);
            }
            //QMessageBox::information(this, QString(tr("提示")), QString(tr("订单\"%1\"删除成功！")).arg(contractID), QString(tr("确定")));
        }
        else
        {
            QMessageBox::critical(this, QString(tr("错误")), QString(tr("订单\"%1\"删除失败:%2")).arg(contractID).arg(sql.getErrorStr()), QString(tr("确定")));
        }
    }
}

void TableWidget::deleteProductInfo()
{
    int row = currentRow();
    int number = itemData(row, 0);
    QString name = itemText(row, 0);

    qDebug() << number;

    SqlDatabase sql("delete");
    if (false == sql.deleteProductInfo(number))
    {
        QMessageBox::critical(this, QString(tr("错误")), QString(tr("产品\"%1\"删除失败:%2")).arg(name).arg(sql.getErrorStr()), QString(tr("确定")));
    }

    removeRow(row);
}

void TableWidget::deleteFinancialInfo()
{
    int row = currentRow();
    int number = itemData(row, 0);

    qDebug() << number;

    SqlDatabase sql("delete");
    if (false == sql.deleteFinacialInfo(number))
    {
        QMessageBox::critical(this, QString(tr("错误")), QString(tr("收支记录\"%1\"删除失败:%2")).arg(number).arg(sql.getErrorStr()), QString(tr("确定")));
    }

    removeRow(row);
}

void TableWidget::onExportToXls()           // 导出列表.xls
{
    QString filepath = QFileDialog::getSaveFileName(NULL, QObject::tr("选择保存路径"), "/untitled.xls", QObject::tr("Microsoft Office 97-2003 (*.xls)")); //获取保存路径

    QList<QVariant> allRowsData;    //保存所有行数据
    allRowsData.clear();

    if (filepath.isEmpty()) {
        qDebug() << "filepath is NULL!";
        return;
    }

    QAxObject* excel = new QAxObject();
    if (!excel->setControl("Excel.Application")) //连接Excel控件
    {
        QMessageBox::critical(NULL,"错误","未能创建 Excel 对象，请安装 Microsoft Excel。",QMessageBox::Apply);
        return;
    }

    excel->dynamicCall("SetVisible (bool Visible)", false);   //不显示窗体
    excel->setProperty("DisplayAlerts", false);               //不显示任何警告信息。如果为true那么在关闭是会出现类似“文件已修改，是否保存”的提示

    QAxObject* workbooks = excel->querySubObject("WorkBooks");//获取工作簿集合
    workbooks->dynamicCall("Add");  //新建一个工作簿
    QAxObject* workbook = excel->querySubObject("ActiveWorkBook");  //获取当前工作簿
    QAxObject* worksheets = workbook->querySubObject("Sheets");     //获取工作表集合
    QAxObject* worksheet = worksheets->querySubObject("Item(int)", 1);//获取工作表集合的工作表1，即sheet1

    QList<QVariant> aRowData;
    for (int i=0; i<m_header.size(); i++)
    {
        aRowData.append(QVariant(m_header[i]));
    }

    //qDebug() << aRowData;

    allRowsData.append(QVariant(aRowData));

    //for (int row=0; row <rowCount(); row++) {
    for (int row=0; row <m_validRowCnt; row++) {
        QList<QVariant> aRowData;   //保存一行数据
        for (int column=0; column<m_validClumnCnt; column++) {
            if (0 == column)
            {
                aRowData.append(QVariant("'" + itemText(row, column)));
            }
            else
            {
                aRowData.append(QVariant(itemText(row, column)));
            }
        }
        allRowsData.append(QVariant(aRowData));
    }

    //QString rangeStr = "A1:G" + QString::number(rowCount()+1);
    char a = 'A' + m_validClumnCnt - 1;
    QString rangeStr = QString("A1:%1").arg(a) + QString::number(m_validRowCnt + 1);
    //qDebug() << rangeStr << m_validClumnCnt;

    QAxObject *range = worksheet->querySubObject("Range(const QString )", rangeStr);
    range->dynamicCall("SetValue(const QVariant&)", QVariant(allRowsData)); //存储所有数据到 excel 中,批量操作,速度极快
    range->setProperty("RowHeight", 15); //设置单元格行高
    //range->setProperty("ColumnWidth", 50); //设置单元格列宽

    // 保存为后缀为xls
    //保存至filepath，注意一定要用QDir::toNativeSeparators将路径中的"/"转换为"\"，不然一定保存不了。
    workbook->dynamicCall("SaveAs(const QString&,int)", QDir::toNativeSeparators(filepath), 56);
    // 保存为后缀为xlsx
    //workbook->dynamicCall("SaveAs(const QString&)", QDir::toNativeSeparators(filepath));//保存至filepath，注意一定要用QDir::toNativeSeparators将路径中的"/"转换为"\"，不然一定保存不了。

    workbook->dynamicCall("Close()");   //关闭工作簿
    excel->dynamicCall("Quit()");       //关闭excel
    delete excel;
    excel=NULL;

    if (0 == QMessageBox::question(this,"完成","文件已经导出，是否现在打开？", QString(tr("是")), QString(tr("否"))))
    {
        QDesktopServices::openUrl(QUrl("file:///" + QDir::toNativeSeparators(filepath)));
    }
}

void TableWidget::onCopyToXls()             // 复制列表.xls
{
    QString str = "";
    QApplication::clipboard()->clear();

    for (int i=0; i<m_header.size(); i++)
    {
        str += m_header[i];
        if (i < m_header.size()-1)
        {
            str += "\t";
        }
        else
        {
           str += "\n";
        }
    }
    qDebug() << m_header.size() << m_validClumnCnt;
    for (int i=0; i<m_validRowCnt; i++) {
        str += convertLineToXlsStr(i, 0, m_validClumnCnt-1, tr("\t"));
    }

    QApplication::clipboard()->setText(str);
}

void TableWidget::onAddOneLine()            // 新增一行
{

}

void TableWidget::onDelete()      // 删除
{
    if (DATA_IS_CUSTOMER_INFO == m_dataType)
    {
        deleteCustomerInfo();
    }
    else if (DATA_IS_ORDER_INFO == m_dataType)
    {
        deleteOrderInfo();
    }
    else if (DATA_IS_PRODUCT_INFO == m_dataType)
    {
        deleteProductInfo();
    }
    else if (DATA_IS_FINANCIAL_INFO == m_dataType)
    {
        deleteFinancialInfo();
    }
}

void TableWidget::onNewFinancialRecordAction()  // 新增收支记录
{
    QString name = "";
    QString contractID = "";

    if (DATA_IS_CUSTOMER_INFO == m_dataType)
    {
        name = itemText(currentRow(), 0);
        if (name.isEmpty())
        {
            QMessageBox::critical(this, QString(tr("错误")), QString(tr("获取客户名失败！")), QString(tr("确定")));
            return;
        }
    }
    else if (DATA_IS_ORDER_INFO == m_dataType)
    {
        contractID = itemText(currentRow(), 0);
        if (contractID.isEmpty())
        {
            QMessageBox::critical(this, QString(tr("错误")), QString(tr("获取合同编号失败！")), QString(tr("确定")));
            return;
        }

        name = itemText(currentRow(), 1);
        if (name.isEmpty())
        {
            QMessageBox::critical(this, QString(tr("错误")), QString(tr("获取客户名失败！")), QString(tr("确定")));
            return;
        }
    } else if (DATA_IS_FINANCIAL_INFO == m_dataType)
    {
        name = itemText(currentRow(), 0);
        if (name.isEmpty())
        {
            QMessageBox::critical(this, QString(tr("错误")), QString(tr("获取客户名失败！")), QString(tr("确定")));
            return;
        }

        contractID = itemText(currentRow(), 1);
        if (contractID.isEmpty())
        {
            QMessageBox::critical(this, QString(tr("错误")), QString(tr("获取合同编号失败！")), QString(tr("确定")));
            return;
        }
    }

    emit sigNewFinancialRecord(name, contractID);
}

void TableWidget::onSearchFinancialRecordAction()   // 查找收支记录
{
    if (DATA_IS_CUSTOMER_INFO == m_dataType)
    {
        QString name = itemText(currentRow(), 0);
        if (name.isEmpty())
        {
            QMessageBox::critical(this, QString(tr("错误")), QString(tr("获取客户名失败！")), QString(tr("确定")));
            return;
        }

        emit sigSearchFinancialByCustomerName(name);
    }
    else if (DATA_IS_ORDER_INFO == m_dataType)
    {
        QString contractID = itemText(currentRow(), 0);
        if (contractID.isEmpty())
        {
            QMessageBox::critical(this, QString(tr("错误")), QString(tr("获取合同编号失败！")), QString(tr("确定")));
            return;
        }

        emit sigSearchFinancialByContractID(contractID);
    }
    else if (DATA_IS_FINANCIAL_INFO == m_dataType)
    {
        QString contractID = itemText(currentRow(), 1);
        if (contractID.isEmpty())
        {
            QMessageBox::critical(this, QString(tr("错误")), QString(tr("获取合同编号失败！")), QString(tr("确定")));
            return;
        }

        emit sigSearchFinancialByContractID(contractID);
    }
}
