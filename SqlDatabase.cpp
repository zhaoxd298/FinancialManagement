#include "SqlDatabase.h"

#include <QVariant>
#include <QDebug>
#include <QMutex>

QMutex mutex;
SqlDatabase::SqlDatabase(const QString &connectionName)
{
    mutex.lock();
    m_customerQueryIsOK = false;

    if (QSqlDatabase::contains(connectionName)) {

        m_db = QSqlDatabase::database(connectionName);
    } else {
        m_db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
    }

    m_db.setHostName("easybook-3313b0");          //设置数据库主机名
    m_db.setDatabaseName("data.db");              //设置数据库名
    m_db.setUserName("root");                     //设置数据库用户名
    m_db.setPassword("123456");                   //设置数据库密码
    m_db.open();

    int count = 0;

    m_customerQuery = new QSqlQuery(QSqlDatabase::database(connectionName));
    bool ret = m_customerQuery->exec(QString("select count(*) from sqlite_master where type='table' and name='%1'").arg(CUSTOMER_TABLE));    //关键的判断
    if (ret) {
        while (m_customerQuery->next()) {
            count = m_customerQuery->value(0).toString().toInt();
        }
    }
    if (0 == count) {
        QString cmd = QString("create table %1(name varchar primary key,"
                                  "level varchar,"
                                  "status varchar,"
                                  "enterDate DATE,"
                                  "inquirySource varchar,"
                                  "background varchar,"
                                  "address varchar,"
                                  "companyName varchar,"
                                  "websit varchar,"
                                  "email varchar,"
                                  "phoneNumber varchar,"
                                  "position varchar,"
                                  "schedule varchar,"
                                  "salesman varchar,"
                                  "remarks varchar)").arg(CUSTOMER_TABLE);
        m_customerQueryIsOK = m_customerQuery->exec(cmd);
        m_errorStr = m_customerQuery->lastError().text();
    } else {
        m_customerQueryIsOK = true;
    }


    m_orderQuery = new QSqlQuery(QSqlDatabase::database(connectionName));
    ret = m_orderQuery->exec(QString("select count(*) from sqlite_master where type='table' and name='%1'").arg(ORDER_TABLE));    //关键的判断
    if (ret) {
        while (m_orderQuery->next()) {
            count = m_orderQuery->value(0).toString().toInt();
        }
    }
    if (0 == count) {
        QString cmd = QString("create table %1(orderID varchar primary key,"
                                  "customerName varchar,"
                                  "orderStatus varchar,"
                                  "payTime DATE,"
                                  "payType varchar,"
                                  "realIncome DOUBLE,"
                                  "freightCustomer DOUBLE,"
                                  "freightFactoryToUs DOUBLE,"
                                  "freightUsToForwarding DOUBLE,"
                                  "freightForeign DOUBLE,"
                                  "exchangeRate DOUBLE,"
                                  "handlingFee DOUBLE,"
                                  "remarks varchar,"
                                  "salesman varchar,"
                                  "contractID varchar)").arg(ORDER_TABLE);
        m_orderQueryIsOK = m_orderQuery->exec(cmd);
        m_errorStr = m_orderQuery->lastError().text();
    } else {
        m_orderQueryIsOK = true;
    }

    m_productQuery = new QSqlQuery(QSqlDatabase::database(connectionName));
    ret = m_productQuery->exec(QString("select count(*) from sqlite_master where type='table' and name='%1'").arg(PRODUCT_TABLE));    //关键的判断
    if (ret) {
        while (m_productQuery->next()) {
            count = m_productQuery->value(0).toString().toInt();
        }
    }
    if (0 == count) {
        QString cmd = QString("create table %1("
                                  "number INTEGER PRIMARY KEY AUTOINCREMENT,"
                                  "orderID varchar,"
                                  "productName varchar,"
                                  "price DOUBLE,"
                                  "costPrice DOUBLE,"
                                  "count INT,"
                                  "spec varchar)").arg(PRODUCT_TABLE);
        m_productQueryIsOK = m_productQuery->exec(cmd);
        m_errorStr = m_productQuery->lastError().text();
        qDebug() << m_errorStr;
    } else {
        m_productQueryIsOK = true;
    }

    //qDebug() << m_customerQueryIsOK << m_orderQueryIsOK << m_productQueryIsOK;
    mutex.unlock();
}

QString SqlDatabase::getErrorStr()
{
    return m_errorStr;
}

bool SqlDatabase::insertCustomerInfo(const CustomerInformation& customerInfo)
{
    bool ret = false;

    if (!m_customerQueryIsOK) {
        m_errorStr = "m_customerQueryIsOK is false";
        return ret;
    }

    if (customerInfoIsExist(customerInfo.name)) {  // 要插入的数据已经存在
        m_errorStr = "data is exist, please update!";
        ret = false;
        //updateCustomerInfo(customerInfo);
    }
    else    // 插入数据
    {
        m_customerQuery->prepare(QString("insert into %1 values(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)").arg(CUSTOMER_TABLE));

        m_customerQuery->bindValue(0, customerInfo.name);
        m_customerQuery->bindValue(1, customerInfo.level);
        m_customerQuery->bindValue(2, customerInfo.status);
        m_customerQuery->bindValue(3, customerInfo.enterDate);
        m_customerQuery->bindValue(4, customerInfo.inquirySource);
        m_customerQuery->bindValue(5, customerInfo.background);
        m_customerQuery->bindValue(6, customerInfo.address);
        m_customerQuery->bindValue(7, customerInfo.companyName);
        m_customerQuery->bindValue(8, customerInfo.websit);
        m_customerQuery->bindValue(9, customerInfo.email);
        m_customerQuery->bindValue(10, customerInfo.phoneNumber);
        m_customerQuery->bindValue(11, customerInfo.position);
        m_customerQuery->bindValue(12, customerInfo.schedule);
        m_customerQuery->bindValue(13, customerInfo.salesman);
        m_customerQuery->bindValue(14, customerInfo.remarks);
        ret = m_customerQuery->exec();
        if(!ret)
        {
            m_errorStr = "插入数据失败：" + m_customerQuery->lastError().text();
        } else {
            qDebug() << "插入数据成功！";
            m_errorStr = "success!";
        }
    }

    return ret;
}


bool SqlDatabase::updateCustomerInfo(const CustomerInformation& customerInfo)
{
    bool ret = false;

    if (!m_customerQueryIsOK)
    {
        return ret;
    }

    if (customerInfoIsExist(customerInfo.name))     // 要插入的数据已经存在
    {
        QString cmd = QString("update %1 set "
                              "level = :level,"
                              "status = :status,"
                              "enterDate = :enterDate,"
                              "inquirySource = :inquirySource,"
                              "background = :background,"
                              "address = :address,"
                              "companyName = :companyName,"
                              "websit = :websit,"
                              "email = :email,"
                              "phoneNumber = :phoneNumber,"
                              "position = :position,"
                              "schedule = :schedule,"
                              "salesman = :salesman,"
                              "remarks = :remarks "
                              "where name = :name").arg(CUSTOMER_TABLE);
        m_customerQuery->prepare(cmd);

        m_customerQuery->bindValue(":level", customerInfo.level);
        m_customerQuery->bindValue(":status", customerInfo.status);
        m_customerQuery->bindValue(":enterDate", customerInfo.enterDate);
        m_customerQuery->bindValue(":inquirySource", customerInfo.inquirySource);
        m_customerQuery->bindValue(":background", customerInfo.background);
        m_customerQuery->bindValue(":address", customerInfo.address);
        m_customerQuery->bindValue(":companyName", customerInfo.companyName);
        m_customerQuery->bindValue(":websit", customerInfo.websit);
        m_customerQuery->bindValue(":email", customerInfo.email);
        m_customerQuery->bindValue(":phoneNumber", customerInfo.phoneNumber);
        m_customerQuery->bindValue(":position", customerInfo.position);
        m_customerQuery->bindValue(":schedule", customerInfo.schedule);
        m_customerQuery->bindValue(":salesman", customerInfo.salesman);
        m_customerQuery->bindValue(":remarks", customerInfo.remarks );
        m_customerQuery->bindValue(":name", customerInfo.name);
        ret = m_customerQuery->exec();
        if (!ret) {
            m_errorStr = "更新数据失败：" + m_customerQuery->lastError().text();
        }/* else {
            qDebug() << "更新记录成功！";
        }*/
    }
    else
    {
        m_errorStr = "data is not exist!";
        ret = false;
    }

    return ret;
}

bool SqlDatabase::getCustomerInfoByName(const QString& name, QList<CustomerInformation>& customerInfoList)
{
    bool ret = false;

    if (!m_customerQueryIsOK) {
        return ret;
    }

    ret = m_customerQuery->exec(QString("select * from %1 where name like '%%2%'").arg(CUSTOMER_TABLE).arg(name));
    m_errorStr = m_customerQuery->lastError().text();

    if (true == ret)
    {
        getCustomerInfoList(m_customerQuery, customerInfoList);

        if (customerInfoList.isEmpty())
        {
            ret = false;
        }
    }

    return ret;
}

bool SqlDatabase::getCustomerInfoByInquirySource(const QString& source, QList<CustomerInformation>& customerInfoList)
{
    bool ret = false;

    if (!m_customerQueryIsOK) {
        return ret;
    }

    ret = m_customerQuery->exec(QString("select * from %1 where inquirySource like '%%2%'").arg(CUSTOMER_TABLE).arg(source));
    m_errorStr = m_customerQuery->lastError().text();

    if (true == ret)
    {
        getCustomerInfoList(m_customerQuery, customerInfoList);

        if (customerInfoList.isEmpty())
        {
            ret = false;
        }
    }

    return ret;
}

bool SqlDatabase::getCustomerInfoByCountry(const QString& country, QList<CustomerInformation>& customerInfoList)
{
    bool ret = false;

    if (!m_customerQueryIsOK) {
        return ret;
    }

    ret = m_customerQuery->exec(QString("select * from %1 where address like '%%2%'").arg(CUSTOMER_TABLE).arg(country));
    m_errorStr = m_customerQuery->lastError().text();

    if (true == ret)
    {
        getCustomerInfoList(m_customerQuery, customerInfoList);

        if (customerInfoList.isEmpty())
        {
            ret = false;
        }
    }

    return ret;
}

bool SqlDatabase::getCustomerInfoByCompany(const QString& company, QList<CustomerInformation>& customerInfoList)
{
    bool ret = false;

    if (!m_customerQueryIsOK) {
        return ret;
    }

    ret = m_customerQuery->exec(QString("select * from %1 where companyName like '%%2%'").arg(CUSTOMER_TABLE).arg(company));
    m_errorStr = m_customerQuery->lastError().text();

    if (true == ret)
    {
        getCustomerInfoList(m_customerQuery, customerInfoList);

        if (customerInfoList.isEmpty())
        {
            ret = false;
        }
    }

    return ret;
}

bool SqlDatabase::getCustomerInfoByEmail(const QString& email, QList<CustomerInformation>& customerInfoList)
{
    bool ret = false;

    if (!m_customerQueryIsOK) {
        return ret;
    }

    ret = m_customerQuery->exec(QString("select * from %1 where email like '%%2%'").arg(CUSTOMER_TABLE).arg(email));
    m_errorStr = m_customerQuery->lastError().text();

    if (true == ret)
    {
        getCustomerInfoList(m_customerQuery, customerInfoList);

        if (customerInfoList.isEmpty())
        {
            ret = false;
        }
    }

    return ret;
}

bool SqlDatabase::getCustomerInfoByPhoneNumber(const QString& phoneNumber, QList<CustomerInformation>& customerInfoList)
{
    bool ret = false;

    if (!m_customerQueryIsOK) {
        return ret;
    }

    ret = m_customerQuery->exec(QString("select * from %1 where phoneNumber like '%%2%'").arg(CUSTOMER_TABLE).arg(phoneNumber));
    m_errorStr = m_customerQuery->lastError().text();

    if (true == ret)
    {
        getCustomerInfoList(m_customerQuery, customerInfoList);

        if (customerInfoList.isEmpty())
        {
            ret = false;
        }
    }

    return ret;
}

bool SqlDatabase::getCustomerInfoBySalesman(const QString& salesman, QList<CustomerInformation>& customerInfoList)
{
    bool ret = false;

    if (!m_customerQueryIsOK) {
        return ret;
    }

    ret = m_customerQuery->exec(QString("select * from %1 where salesman like '%%2%'").arg(CUSTOMER_TABLE).arg(salesman));
    m_errorStr = m_customerQuery->lastError().text();

    if (true == ret)
    {
        getCustomerInfoList(m_customerQuery, customerInfoList);

        if (customerInfoList.isEmpty())
        {
            ret = false;
        }
    }

    return ret;
}

bool SqlDatabase::getCustomerInfoByStatus(const QString& status, QList<CustomerInformation>& customerInfoList)
{
    bool ret = false;

    if (!m_customerQueryIsOK) {
        return ret;
    }

    ret = m_customerQuery->exec(QString("select * from %1 where status like '%%2%'").arg(CUSTOMER_TABLE).arg(status));
    m_errorStr = m_customerQuery->lastError().text();

    if (true == ret)
    {
        getCustomerInfoList(m_customerQuery, customerInfoList);

        if (customerInfoList.isEmpty())
        {
            ret = false;
        }
    }

    return ret;
}


bool SqlDatabase::orderInfoIsExist(const QString& orderID)
{
    int count = 0;

    if (m_orderQueryIsOK) {
        m_orderQuery->exec(QString("select count(*) from %1 where orderID='%2'").arg(ORDER_TABLE).arg(orderID));

        while (m_orderQuery->next()) {
            count = m_orderQuery->value(0).toString().toInt();
        }
    }

    return (0 != count) ? true : false;
}


bool SqlDatabase::insertOrderInfo(const OrderInformation& orderInfo)
{
    bool ret = false;

    if (!m_orderQueryIsOK) {
        return ret;
    }

    if (customerInfoIsExist(orderInfo.orderID)) {  // 要插入的数据已经存在
        qDebug() << "need update data!";
        ret = false;
        //updateCustomerInfo(customerInfo);
    }
    else    // 插入数据
    {
        m_orderQuery->prepare(QString("insert into %1 values(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)").arg(ORDER_TABLE));

        m_orderQuery->bindValue(0, orderInfo.orderID);
        m_orderQuery->bindValue(1, orderInfo.customerName);
        m_orderQuery->bindValue(2, orderInfo.orderStatus);
        m_orderQuery->bindValue(3, orderInfo.payTime);
        m_orderQuery->bindValue(4, orderInfo.payType);
        m_orderQuery->bindValue(5, orderInfo.realIncome);
        m_orderQuery->bindValue(6, orderInfo.freightCustomer);
        m_orderQuery->bindValue(7, orderInfo.freightFactoryToUs);
        m_orderQuery->bindValue(8, orderInfo.freightUsToForwarding);
        m_orderQuery->bindValue(9, orderInfo.freightForeign);
        m_orderQuery->bindValue(10, orderInfo.exchangeRate);
        m_orderQuery->bindValue(11, orderInfo.handlingFee);
        m_orderQuery->bindValue(12, orderInfo.remarks);
        m_orderQuery->bindValue(13, orderInfo.salesman);
        m_orderQuery->bindValue(14, orderInfo.contractID);
        ret = m_orderQuery->exec();
        if(!ret)
        {
            m_errorStr = m_customerQuery->lastError().text();
        } else {
            m_errorStr = "插入数据成功！";
        }

        for (int i=0; i<orderInfo.productList.size(); i++)
        {
            qDebug() << i;
            ret = insertProductInfo(orderInfo.orderID, orderInfo.productList[i]);
            if (false == ret)
            {
                m_errorStr = QString("insert \"%1\" error!").arg(orderInfo.productList[i].productName);
                qDebug() << m_errorStr;
            }
        }
    }

    return ret;
}

bool SqlDatabase::updateOrderStatus(const QStringList& orderList, const QString& status)
{
    bool ret = false;

    if (!m_orderQueryIsOK || orderList.isEmpty())
    {
        return ret;
    }

    for (int i=0; i<orderList.size(); i++)
    {
        if (orderInfoIsExist(orderList[i]))     // 要插入的数据已经存在
        {
            QString cmd = QString("update %1 set "
                                  "orderStatus = :orderStatus "
                                  "where orderID = :orderID").arg(ORDER_TABLE);
            m_orderQuery->prepare(cmd);

            m_orderQuery->bindValue(":orderStatus", status);
            m_orderQuery->bindValue(":orderID", orderList[i]);

            ret = m_orderQuery->exec();
            if (!ret)
            {
                m_errorStr = "更新订单状态失败：" + m_orderQuery->lastError().text();
                break;
            }
            else
            {
                m_errorStr = "更新订单状态成功！";
            }
        }
        else
        {
            m_errorStr = "data is not exist!";
            ret = false;
        }
    }

    return ret;
}

bool SqlDatabase::updateOrderInfo(const OrderInformation& orderInfo)
{
    bool ret = false;

    if (!m_orderQueryIsOK)
    {
        return ret;
    }

    if (orderInfoIsExist(orderInfo.orderID))     // 要插入的数据已经存在
    {
        QString cmd = QString("update %1 set "
                              "customerName = :customerName,"
                              "orderStatus = :orderStatus,"
                              "payTime = :payTime,"
                              "payType = :payType,"
                              "realIncome = :realIncome,"
                              "freightCustomer = :freightCustomer,"
                              "freightFactoryToUs = :freightFactoryToUs,"
                              "freightUsToForwarding = :freightUsToForwarding,"
                              "freightForeign = :freightForeign,"
                              "exchangeRate = :exchangeRate,"
                              "handlingFee = :handlingFee,"
                              "remarks = :remarks,"
                              "salesman = :salesman,"
                              "contractID = :contractID "
                              "where orderID = :orderID").arg(ORDER_TABLE);
        m_orderQuery->prepare(cmd);

        m_orderQuery->bindValue(":customerName", orderInfo.customerName);
        m_orderQuery->bindValue(":orderStatus", orderInfo.orderStatus);
        m_orderQuery->bindValue(":payTime", orderInfo.payTime);
        m_orderQuery->bindValue(":payType", orderInfo.payType);
        m_orderQuery->bindValue(":realIncome", orderInfo.realIncome);
        m_orderQuery->bindValue(":freightCustomer", orderInfo.freightCustomer);
        m_orderQuery->bindValue(":freightFactoryToUs", orderInfo.freightFactoryToUs);
        m_orderQuery->bindValue(":freightUsToForwarding", orderInfo.freightUsToForwarding);
        m_orderQuery->bindValue(":freightForeign", orderInfo.freightForeign);
        m_orderQuery->bindValue(":exchangeRate", orderInfo.exchangeRate);
        m_orderQuery->bindValue(":handlingFee", orderInfo.handlingFee);
        m_orderQuery->bindValue(":remarks", orderInfo.remarks);
        m_orderQuery->bindValue(":salesman", orderInfo.salesman);
        m_orderQuery->bindValue(":orderID", orderInfo.orderID);
        m_orderQuery->bindValue(":contractID", orderInfo.contractID);

        ret = m_orderQuery->exec();
        if (!ret) {
            m_errorStr = "更新数据失败：" + m_orderQuery->lastError().text();
        } else {
            m_errorStr = "更新记录成功！";
        }

        // update product info
        for (int i=0; i<orderInfo.productList.size(); i++)
        {
            ret = updateProductInfo(orderInfo.productList[i]);
            if (false == ret)
            {
                m_errorStr = QString("update \"%1\" error!").arg(orderInfo.productList[i].productName);
                //qDebug() << m_errorStr;
            }
        }
    }
    else
    {
        m_errorStr = "data is not exist!";
        ret = false;
    }

    return ret;
}


bool SqlDatabase::getAllOrderInfo(QList<OrderInformation>& orderInfoList)
{
    bool ret = false;

    if (!m_orderQueryIsOK) {
        return ret;
    }

    orderInfoList.clear();

    ret = m_orderQuery->exec(QString("select * from %1").arg(ORDER_TABLE));
    m_errorStr = m_orderQuery->lastError().text();

    if (true == ret)
    {
        getOrderInfoList(m_orderQuery, orderInfoList);

        if (orderInfoList.isEmpty())
        {
            ret = false;
        }
    }

    for (int i=0; i<orderInfoList.size(); i++)
    {
        if (false == getProductInfoByOrderID(orderInfoList[i].orderID, orderInfoList[i].productList))
        {
            m_errorStr = "Get product list error";
            ret = false;
        }

        //orderInfoList[i].calProfitIncomeAndExpenses();
    }

    return ret;
}


bool SqlDatabase::getOrderInfoByOrderID(const QString& orderID, QList<OrderInformation> &orderInfoList)
{
    bool ret = false;

    if (!m_orderQueryIsOK) {
        return ret;
    }

    orderInfoList.clear();

    ret = m_orderQuery->exec(QString("select * from %1 where orderID=%2").arg(ORDER_TABLE).arg(orderID));
    m_errorStr = m_orderQuery->lastError().text();

    if (true == ret)
    {
        getOrderInfoList(m_orderQuery, orderInfoList);

        if (orderInfoList.isEmpty())
        {
            ret = false;
        }
    }

    for (int i=0; i<orderInfoList.size(); i++)
    {

        if (false == getProductInfoByOrderID(orderInfoList[i].orderID, orderInfoList[i].productList))
        {
            m_errorStr = "Get product list error";
            ret = false;
        }

        //orderInfoList[i].calProfitIncomeAndExpenses();
    }

    return ret;
}


bool SqlDatabase::getOrderInfoByDateRange(const QString& startDate, const QString& endDate, QList<OrderInformation>& orderInfoList)
{
    bool ret = false;

    if (!m_orderQueryIsOK) {
        return ret;
    }

    orderInfoList.clear();

    QString cmd = QString("select * from %1 "
                          "where Date(payTime) between '%2' and '%3'").arg(ORDER_TABLE).arg(startDate).arg(endDate);
    ret = m_orderQuery->exec(cmd);
    m_errorStr = m_orderQuery->lastError().text();

    if (true == ret)
    {
        getOrderInfoList(m_orderQuery, orderInfoList);

        if (orderInfoList.isEmpty())
        {
            ret = false;
        }
    }

    for (int i=0; i<orderInfoList.size(); i++)
    {
        if (false == getProductInfoByOrderID(orderInfoList[i].orderID, orderInfoList[i].productList))
        {
            m_errorStr = "Get product list error";
            ret = false;
        }

        //orderInfoList[i].calProfitIncomeAndExpenses();
    }

    return ret;
}
///????
bool SqlDatabase::getOrderInfoBySalesman(const QString& salesman, QList<OrderInformation>& orderInfoList)
{
    bool ret = false;

    if (!m_orderQueryIsOK) {
        return ret;
    }

    orderInfoList.clear();

    QString cmd = QString("select * from %1 where salesman like '%%2%'").arg(ORDER_TABLE).arg(salesman);
    ret = m_orderQuery->exec(cmd);
    m_errorStr = m_orderQuery->lastError().text();

    if (true == ret)
    {
        getOrderInfoList(m_orderQuery, orderInfoList);

        if (orderInfoList.isEmpty())
        {
            ret = false;
        }
    }

    for (int i=0; i<orderInfoList.size(); i++)
    {
        if (false == getProductInfoByOrderID(orderInfoList[i].orderID, orderInfoList[i].productList))
        {
            m_errorStr = "Get product list error";
            ret = false;
        }

        //orderInfoList[i].calProfitIncomeAndExpenses();
    }

    return ret;
}

bool SqlDatabase::getOrderInfoByCustomerName(const QString& name, QList<OrderInformation>& orderInfoList)
{
    bool ret = false;

    if (!m_orderQueryIsOK) {
        return ret;
    }

    orderInfoList.clear();

    QString cmd = QString("select * from %1 where customerName like '%%2%'").arg(ORDER_TABLE).arg(name);
    ret = m_orderQuery->exec(cmd);
    m_errorStr = m_orderQuery->lastError().text();

    if (true == ret)
    {
        getOrderInfoList(m_orderQuery, orderInfoList);

        if (orderInfoList.isEmpty())
        {
            ret = false;
        }
    }

    for (int i=0; i<orderInfoList.size(); i++)
    {
        if (false == getProductInfoByOrderID(orderInfoList[i].orderID, orderInfoList[i].productList))
        {
            m_errorStr = "Get product list error";
            ret = false;
        }

        //orderInfoList[i].calProfitIncomeAndExpenses();
    }

    return ret;
}

bool SqlDatabase::getOrderInfoByStatus(const QString& status, QList<OrderInformation>& orderInfoList)
{
    bool ret = false;

    if (!m_orderQueryIsOK) {
        return ret;
    }

    orderInfoList.clear();

    QString cmd = QString("select * from %1 where orderStatus like '%%2%'").arg(ORDER_TABLE).arg(status);
    ret = m_orderQuery->exec(cmd);
    m_errorStr = m_orderQuery->lastError().text();

    if (true == ret)
    {
        getOrderInfoList(m_orderQuery, orderInfoList);

        if (orderInfoList.isEmpty())
        {
            ret = false;
        }
    }

    for (int i=0; i<orderInfoList.size(); i++)
    {
        if (false == getProductInfoByOrderID(orderInfoList[i].orderID, orderInfoList[i].productList))
        {
            m_errorStr = "Get product list error";
            ret = false;
        }

        //orderInfoList[i].calProfitIncomeAndExpenses();
    }

    return ret;
}

bool SqlDatabase::productInfoIsExist(int number)
{
    int count = 0;

    if (m_productQueryIsOK) {
        QString cmd = QString("select count(*) from %1 where number=%2").arg(PRODUCT_TABLE).arg(number);
        //qDebug() << cmd;
        m_productQuery->exec(cmd);
        //m_errorStr = m_productQuery->lastError().text();
        //qDebug() << m_errorStr;
        while (m_productQuery->next()) {
            count = m_productQuery->value(0).toString().toInt();
        }
    }

    return (0 != count) ? true : false;
}

bool SqlDatabase::insertProductInfo(const QString& orderID, const ProductInfo& productInfo)
{
    bool ret = false;

    if (!m_productQueryIsOK) {
        return ret;
    }

    QString cmd = QString("insert into %1 (orderID,productName,price,costPrice,count,spec) "
                          "VALUES('%2', '%3', %4, %5, %6, '%7')").arg(PRODUCT_TABLE).arg(orderID).arg(productInfo.productName).arg(productInfo.price).arg(productInfo.costPrice).arg(productInfo.count).arg(productInfo.spec);
    ret = m_productQuery->exec(cmd);
    if(!ret)
    {
        m_errorStr = m_productQuery->lastError().text();
    } else {
        m_errorStr = "插入数据成功！";
    }

    return ret;
}

bool SqlDatabase::updateProductInfo(const ProductInfo& productInfo)
{
    bool ret = false;

    if (!m_productQueryIsOK)
    {
        qDebug() << "m_productQueryIsOK:" << m_productQueryIsOK;
        return ret;
    }

    if (productInfoIsExist(productInfo.number))
    {
        QString cmd = QString("update %1 set "
                              "productName = :productName,"
                              "price = :price,"
                              "costPrice = :costPrice,"
                              "count = :count,"
                              "spec = :spec "
                              "where number = :number").arg(PRODUCT_TABLE);
        m_productQuery->prepare(cmd);

        m_productQuery->bindValue(":productName", productInfo.productName);
        m_productQuery->bindValue(":price", productInfo.price);
        m_productQuery->bindValue(":costPrice", productInfo.costPrice);
        m_productQuery->bindValue(":count", productInfo.count);
        m_productQuery->bindValue(":spec", productInfo.spec);
        m_productQuery->bindValue(":number", productInfo.number);

        ret = m_productQuery->exec();
        if (!ret) {
            m_errorStr = "更新数据失败：" + m_productQuery->lastError().text();
            qDebug() << m_errorStr;
        } else {
            m_errorStr = "更新记录成功！";
        }
    }
    else
    {
        m_errorStr = "data is not exist!";
        qDebug() << m_errorStr;
        ret = false;
    }

    return ret;
}


bool SqlDatabase::getProductInfoByOrderID(const QString& orderID, QList<ProductInfo>& productInfoList)
{
    bool ret = false;

    if (!m_productQueryIsOK) {
        return ret;
    }

    ret = m_productQuery->exec(QString("select * from %1 where orderID = %2").arg(PRODUCT_TABLE).arg(orderID));
    m_errorStr = m_productQuery->lastError().text();

    while (m_productQuery->next()) {
        ProductInfo info;

        info.number = m_productQuery->value(0).toInt();
        info.productName = m_productQuery->value(2).toString();
        info.price = m_productQuery->value(3).toDouble(&ret);
        info.costPrice = m_productQuery->value(4).toDouble(&ret);
        info.count = m_productQuery->value(5).toInt(&ret);
        info.spec = m_productQuery->value(6).toString();

        productInfoList.append(info);
    }

    return ret;
}

void SqlDatabase::getCustomerInfoList(QSqlQuery* query, QList<CustomerInformation>& customerInfoList)
{
    while (query->next()) {
        CustomerInformation info;

        info.name = query->value(0).toString();
        info.level = query->value(1).toString();
        info.status = query->value(2).toString();
        info.enterDate = query->value(3).toString();
        info.inquirySource = query->value(4).toString();
        info.background = query->value(5).toString();
        info.address = query->value(6).toString();
        info.companyName = query->value(7).toString();
        info.websit = query->value(8).toString();
        info.email = query->value(9).toString();
        info.phoneNumber = query->value(10).toString();
        info.position = query->value(11).toString();
        info.schedule = query->value(12).toString();
        info.salesman = query->value(13).toString();
        info.remarks = query->value(14).toString();

        customerInfoList.append(info);
    }
}

void SqlDatabase::getOrderInfoList(QSqlQuery* query, QList<OrderInformation>& orderInfoList)
{
    while (query->next()) {
        OrderInformation orderInfo;

        orderInfo.orderID = m_orderQuery->value(0).toString();
        orderInfo.customerName = m_orderQuery->value(1).toString();
        orderInfo.orderStatus = m_orderQuery->value(2).toString();
        orderInfo.payTime = m_orderQuery->value(3).toString();
        orderInfo.payType = m_orderQuery->value(4).toString();
        orderInfo.realIncome = m_orderQuery->value(5).toDouble();
        orderInfo.freightCustomer = m_orderQuery->value(6).toDouble();
        orderInfo.freightFactoryToUs = m_orderQuery->value(7).toDouble();
        orderInfo.freightUsToForwarding = m_orderQuery->value(8).toDouble();
        orderInfo.freightForeign = m_orderQuery->value(9).toDouble();
        orderInfo.exchangeRate = m_orderQuery->value(10).toDouble();
        orderInfo.handlingFee = m_orderQuery->value(11).toDouble();
        orderInfo.remarks = m_orderQuery->value(12).toString();
        orderInfo.salesman = m_orderQuery->value(13).toString();
        orderInfo.contractID = m_orderQuery->value(14).toString();


        orderInfoList.append(orderInfo);
    }
}

bool SqlDatabase::deleteProductInfo(const QString& orderID)
{
    bool ret = false;

    if (m_productQueryIsOK) {
        m_productQuery->prepare(QString("delete from %1 where orderID = ?").arg(PRODUCT_TABLE));
        m_productQuery->addBindValue(orderID);
        ret = m_productQuery->exec();
        if (!ret) {
            m_errorStr = m_productQuery->lastError().text();
        } else {
            m_errorStr =  "删除产品信息成功！";
        }
    }

    return ret;
}

bool SqlDatabase::customerInfoIsExist(const QString& name)      // 判断记录是否存在
{
    int count = 0;

    if (m_customerQueryIsOK) {
        m_customerQuery->exec(QString("select count(*) from %1 where name='%2'").arg(CUSTOMER_TABLE).arg(name));

        while (m_customerQuery->next()) {
            count = m_customerQuery->value(0).toString().toInt();
        }
    }

    return (0 != count) ? true : false;
}

bool SqlDatabase::deleteCustomerInfo(const QString& name)
{
    bool ret = false;

    if (m_customerQueryIsOK) {
        m_customerQuery->prepare(QString("delete from %1 where name = ?").arg(CUSTOMER_TABLE));
        m_customerQuery->addBindValue(name);
        ret = m_customerQuery->exec();
        if (!ret) {
            m_errorStr = m_customerQuery->lastError().text();
        } else {
            m_errorStr =  "删除客户信息成功！";
        }
    }

    return ret;
}


bool SqlDatabase::deleteOrderInfo(const QString& orderID)
{
    bool ret = false;

    if (m_orderQueryIsOK) {
        m_orderQuery->prepare(QString("delete from %1 where orderID = ?").arg(ORDER_TABLE));
        m_orderQuery->addBindValue(orderID);
        ret = m_orderQuery->exec();
        if (!ret) {
            m_errorStr = m_orderQuery->lastError().text();
        } else {
            m_errorStr =  "删除订单信息成功！";
        }
    }

    ret = deleteProductInfo(orderID);

    return ret;
}

SqlDatabase::~SqlDatabase()
{
    m_db.close();
    delete m_customerQuery;
}
