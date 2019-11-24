#include <QJsonArray>
#include "TableWidget.h"
#include "CustomerInfoDialog.h"
#include "SqlDatabase.h"
#include "OrderDialog.h"
#include "Log.h"

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

    m_orderList.append(false);
    for (int i=1; i<11; i++) {
        m_orderList.append(true);
    }

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
           << "公司名" << "邮箱" << "联系方式" << "职位" << "跟进情况" << "业务员" << "备注";
    m_validClumnCnt = m_header.size();
    setColumnCount(m_header.size());
    setHorizontalHeaderLabels(m_header);

    if (NULL != mainMenu)
    {
        mainMenu->clear(); //清除原有菜单
        mainMenu->addSeparator();

        m_editAction->setText(tr("编辑当前客户信息"));
        mainMenu->addAction(m_editAction);

        m_deleteAction->setText(tr("删除当前客户信息"));
        mainMenu->addAction(m_deleteAction);
        mainMenu->addSeparator();

        mainMenu->addAction(m_checkAllAction);
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
    m_header << "订单编号" << "客户" << "订单状态" << "品名" << "单价"
           << "成本单价" << "数量" << "付款时间" << "付款方式" << "实收金额"
           << "应收金额" << "运费（客户）" << "运费(厂家→我司)" << "运费(我司→货代)"
           << "运费(国外)" << "包装费" << "平台手续费" << "总支出" << "总利润"
           << "合伙人利润" << "备注";
    m_validClumnCnt = m_header.size();
    setColumnCount(m_header.size());
    setHorizontalHeaderLabels(m_header);

    if (NULL != mainMenu)
    {
        mainMenu->clear(); //清除原有菜单
        mainMenu->addSeparator();

        m_editAction->setText(tr("编辑当前订单信息"));
        mainMenu->addAction(m_editAction);

        m_deleteAction->setText(tr("删除该订单"));
        mainMenu->addAction(m_deleteAction);
        mainMenu->addSeparator();

        mainMenu->addAction(m_checkAllAction);
        mainMenu->addSeparator();

        mainMenu->addAction(m_exportToXlsAction);
        mainMenu->addAction(m_copyToXlsAction);
        mainMenu->addSeparator();
    }

    setEditTriggers(QAbstractItemView::NoEditTriggers);
}


void TableWidget::setDataTypeProductInfo()
{
    m_dataType = DATA_IS_PRODUCT_INFO;

    m_header.clear();
    m_header << "商品名称" << "单价" << "成本单价" << "数量";
    m_validClumnCnt = m_header.size();
    setColumnCount(m_header.size());
    setHorizontalHeaderLabels(m_header);

    if (NULL != mainMenu)
    {
        mainMenu->clear(); //清除原有菜单
        mainMenu->addAction(m_addProductAction);
        m_deleteAction->setText(tr("删除该产品"));
        mainMenu->addAction(m_deleteAction);
    }

    setEditTriggers(QAbstractItemView::DoubleClicked);
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

    setCellData(m_validRowCnt, 0, customerInfo.name);
    setCellData(m_validRowCnt, 1, customerInfo.level);
    setCellData(m_validRowCnt, 2, customerInfo.status);
    setCellData(m_validRowCnt, 3, customerInfo.enterDate);
    setCellData(m_validRowCnt, 4, customerInfo.inquirySource);
    setCellData(m_validRowCnt, 5, customerInfo.background);
    setCellData(m_validRowCnt, 6, customerInfo.address);
    setCellData(m_validRowCnt, 7, customerInfo.companyName);
    setCellData(m_validRowCnt, 8, customerInfo.email);
    setCellData(m_validRowCnt, 9, customerInfo.phoneNumber);
    setCellData(m_validRowCnt, 10, customerInfo.position);
    setCellData(m_validRowCnt, 11, customerInfo.schedule);
    setCellData(m_validRowCnt, 12, customerInfo.salesman);
    setCellData(m_validRowCnt, 13, customerInfo.remarks);

    m_validRowCnt++;
}


void TableWidget::updateCustomerInformation(int row, const CustomerInformation& customerInfo)
{
    if ((row < 0) || (row > m_validRowCnt))
    {
        return;
    }

    setCellData(row, 0, customerInfo.name);
    setCellData(row, 1, customerInfo.level);
    setCellData(row, 2, customerInfo.status);
    setCellData(row, 3, customerInfo.enterDate);
    setCellData(row, 4, customerInfo.inquirySource);
    setCellData(row, 5, customerInfo.background);
    setCellData(row, 6, customerInfo.address);
    setCellData(row, 7, customerInfo.companyName);
    setCellData(row, 8, customerInfo.email);
    setCellData(row, 9, customerInfo.phoneNumber);
    setCellData(row, 10, customerInfo.position);
    setCellData(row, 11, customerInfo.schedule);
    setCellData(row, 12, customerInfo.salesman);
    setCellData(row, 13, customerInfo.remarks);
}

void TableWidget::addOrderInformation(const OrderInformation& orderInfo)
{
    if (orderInfo.productList.size() <= 0)
    {
        return;
    }

    int startRowCnt = m_validRowCnt;
    insertRow(m_validRowCnt);

    setCellData(m_validRowCnt, 0, orderInfo.orderID);
    setCellData(m_validRowCnt, 1, orderInfo.customerName);
    setCellData(m_validRowCnt, 2, orderInfo.orderStatus);
    setCellData(m_validRowCnt, 3, orderInfo.productList[0].productName);
    setCellData(m_validRowCnt, 4, QString::number(orderInfo.productList[0].price));
    setCellData(m_validRowCnt, 5, QString::number(orderInfo.productList[0].costPrice));
    setCellData(m_validRowCnt, 6, QString::number(orderInfo.productList[0].count));
    setCellData(m_validRowCnt, 7, orderInfo.payTime);
    setCellData(m_validRowCnt, 8, orderInfo.payType);
    setCellData(m_validRowCnt, 9, QString::number(orderInfo.realIncome));
    setCellData(m_validRowCnt, 10, QString::number(orderInfo.shouldIncome));
    setCellData(m_validRowCnt, 11, QString::number(orderInfo.freightCustomer));
    setCellData(m_validRowCnt, 12, QString::number(orderInfo.freightFactoryToUs));
    setCellData(m_validRowCnt, 13, QString::number(orderInfo.freightUsToForwarding));
    setCellData(m_validRowCnt, 14, QString::number(orderInfo.freightForeign));
    setCellData(m_validRowCnt, 15, QString::number(orderInfo.packageFee));
    setCellData(m_validRowCnt, 16, QString::number(orderInfo.handlingFee));
    setCellData(m_validRowCnt, 17, QString::number(orderInfo.tatolExpenses));
    setCellData(m_validRowCnt, 18, QString::number(orderInfo.totalProfit));
    setCellData(m_validRowCnt, 19, QString::number(orderInfo.partnerProfit));
    setCellData(m_validRowCnt, 20, orderInfo.remarks);

    m_validRowCnt++;
    for (int i=1; i<orderInfo.productList.size(); i++)
    {
        setCellData(m_validRowCnt, 3, orderInfo.productList[i].productName);
        setCellData(m_validRowCnt, 4, QString::number(orderInfo.productList[i].price));
        setCellData(m_validRowCnt, 5, QString::number(orderInfo.productList[i].costPrice));
        setCellData(m_validRowCnt, 6, QString::number(orderInfo.productList[i].count));
        m_validRowCnt++;
    }

    int productCnt = orderInfo.productList.size();
    if (m_validRowCnt-startRowCnt > 1)
    {
        for (int i=0; i<3; i++)
        {
            setSpan(startRowCnt, i, productCnt, 1);
        }

        for (int i=7; i<m_validClumnCnt; i++)
        {
            setSpan(startRowCnt, i, productCnt, 1);
        }
    }
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

    setItemData(m_validRowCnt, 0, productInfo.number);

    m_validRowCnt++;
}


void TableWidget::updateOrderInformation(int row, const OrderInformation& orderInfo)
{
    if ((row < 0) || (row > m_validRowCnt) || (orderInfo.productList.size() <= 0))
    {
        return;
    }

    //setCellData(m_validRowCnt, 0, orderInfo.orderID);
    setCellData(row, 1, orderInfo.customerName);
    setCellData(row, 2, orderInfo.orderStatus);
    setCellData(row, 3, orderInfo.productList[0].productName);
    setCellData(row, 4, QString::number(orderInfo.productList[0].price));
    setCellData(row, 5, QString::number(orderInfo.productList[0].costPrice));
    setCellData(row, 6, QString::number(orderInfo.productList[0].count));
    setCellData(row, 7, orderInfo.payTime);
    setCellData(row, 8, orderInfo.payType);
    setCellData(row, 9, QString::number(orderInfo.realIncome));
    setCellData(row, 10, QString::number(orderInfo.shouldIncome));
    setCellData(row, 11, QString::number(orderInfo.freightCustomer));
    setCellData(row, 12, QString::number(orderInfo.freightFactoryToUs));
    setCellData(row, 13, QString::number(orderInfo.freightUsToForwarding));
    setCellData(row, 14, QString::number(orderInfo.freightForeign));
    setCellData(row, 15, QString::number(orderInfo.packageFee));
    setCellData(row, 16, QString::number(orderInfo.handlingFee));
    setCellData(row, 17, QString::number(orderInfo.tatolExpenses));
    setCellData(row, 18, QString::number(orderInfo.totalProfit));
    setCellData(row, 19, QString::number(orderInfo.partnerProfit));
    setCellData(row, 20, orderInfo.remarks);

    for (int i=1; i<orderInfo.productList.size(); i++)
    {
        setCellData(row+i, 3, orderInfo.productList[i].productName);
        setCellData(row+i, 4, QString::number(orderInfo.productList[i].price));
        setCellData(row+i, 5, QString::number(orderInfo.productList[i].costPrice));
        setCellData(row+i, 6, QString::number(orderInfo.productList[i].count));
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

int TableWidget::itemData(int row, int column)
{
    int data = 0;

    QTableWidgetItem* it = item(row, column);
    if (NULL != it) {
        data = it->data(Qt::UserRole+1).toInt();
    }

    return data;
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
        setItem(row, column, it);
    } else {
        it->setText(data);
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

    m_checkAllAction = new QAction(tr("全选"), this);
    connect(m_checkAllAction, SIGNAL(triggered(bool)), this, SLOT(onCheckAll()));
    m_checkAllAction->setShortcut(QKeySequence::SelectAll);
    addAction(m_checkAllAction);  // 将checkAllAction添加到当前窗体以响应快捷键

    m_exportToXlsAction = new QAction(tr("导出列表.xls"), this);         // 导出列表.xls
    connect(m_exportToXlsAction, SIGNAL(triggered(bool)), this, SLOT(onExportToXls()));
    m_copyToXlsAction = new QAction(tr("复制列表.xls"), this);           // 复制列表.xls
    connect(m_copyToXlsAction, SIGNAL(triggered(bool)), this, SLOT(onCopyToXls()));


    m_addProductAction = new QAction(tr("新增一行"), this);
    connect(m_addProductAction, SIGNAL(triggered(bool)), this, SLOT(onAddOneLine()));

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
}

void TableWidget::onCheckAll()              // CTRL+A勾选全部用户
{
    //for (int i=0; i<rowCount(); i++) {
    for (int i=0; i<m_validRowCnt; i++) {
        item(i, 0)->setCheckState(Qt::Checked);
    }
    selectAll();
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

    SqlDatabase sql("dialog");
    QList<CustomerInformation> list;

    if (false == sql.getCustomerInfoByName(name, list))
    {
        QMessageBox::critical(this, QString(tr("错误")), QString(tr("从数据库中读取客户信息\"%1\"失败！")).arg(name), QString(tr("确定")));
        return;
    }

    CustomerInfoDialog dialog;
    dialog.setCustomerNameEditDisable();
    if (list.size() > 0)
    {
        dialog.setCustomerInfomation(list[0]);
    }

    if(dialog.exec() == QDialog::Accepted)
    {
        // save to database
        QList<CustomerInformation> customerInfoList = dialog.getCustomerInformationList();
        for (int i=0; i<customerInfoList.size(); i++)
        {
            if (true == sql.updateCustomerInfo(customerInfoList[i]))
            {
                updateCustomerInformation(currentRow(), customerInfoList[i]);
                QMessageBox::information(this, QString(tr("提示")), QString(tr("更新客户\"%1\"信息到数据库成功！")).arg(customerInfoList[i].name), QString(tr("确定")));
            }
            else
            {
                QMessageBox::critical(this, QString(tr("错误")), QString(tr("更新客户\"%1\"信息到数据库失败！")).arg(customerInfoList[i].name), QString(tr("确定")));
            }
        }
    }
}

void TableWidget::editOrderInfo()
{
    QString orderID = itemText(currentRow(), 0);
    if (orderID.isEmpty())
    {
        QMessageBox::critical(this, QString(tr("错误")), QString(tr("获取订单编号失败失败！")), QString(tr("确定")));
        return;
    }

    SqlDatabase sql("dialog");
    QList<OrderInformation> list;

    if (false == sql.getOrderInfoByOrderID(orderID, list))
    {
        QMessageBox::critical(this, QString(tr("错误")), QString(tr("从数据库中读取订单\"%1\"失败！")).arg(orderID), QString(tr("确定")));
        return;
    }

    OrderDialog dialog;
    dialog.setOrderInfo(list[0]);

    if(dialog.exec() == QDialog::Accepted)
    {
        qDebug() << "edit order accepted!";
        //m_tableWidget->clear();
        //m_tableWidget->setDataTypeOrderInfo();

        QList<OrderInformation> orderList = dialog.getOrderList();
        for (int i=0; i<orderList.size(); i++)
        {
            if (true == sql.updateOrderInfo(orderList[i]))
            {
                updateOrderInformation(currentRow(), orderList[i]);
                QMessageBox::information(this, QString(tr("提示")), QString(tr("客户\"%1\"订单更新成功，具体内容显示在表格中！")).arg(orderList[i].customerName), QString(tr("确定")));
            }
            else
            {
                QMessageBox::critical(this, QString(tr("错误")), QString(tr("客户\"%1\"订单更新失败！:%2")).arg(orderList[i].customerName).arg(sql.getErrorStr()), QString(tr("确定")));
            }
        }
    }
    qDebug() << orderID;
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
            QMessageBox::information(this, QString(tr("提示")), QString(tr("删除客户\"%1\"信息成功！")).arg(name), QString(tr("确定")));
        }
        else
        {
            QMessageBox::critical(this, QString(tr("错误")), QString(tr("删除客户\"%1\"信息失败:%2")).arg(name).arg(sql.getErrorStr()), QString(tr("确定")));
        }
    }
}

void TableWidget::deleteOrderInfo()
{
    QString orderID = itemText(currentRow(), 0);
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
            QMessageBox::information(this, QString(tr("提示")), QString(tr("订单\"%1\"删除成功！")).arg(orderID), QString(tr("确定")));
        }
        else
        {
            QMessageBox::critical(this, QString(tr("错误")), QString(tr("订单\"%1\"删除失败:%2")).arg(orderID).arg(sql.getErrorStr()), QString(tr("确定")));
        }
    }
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

    // 保存为后缀为xls
    workbook->dynamicCall("SaveAs(const QString&,int)", QDir::toNativeSeparators(filepath), 56);//保存至filepath，注意一定要用QDir::toNativeSeparators将路径中的"/"转换为"\"，不然一定保存不了。
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

void TableWidget::onDelete()      // 删除当前产品
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
        removeRow(currentRow());
    }
}
