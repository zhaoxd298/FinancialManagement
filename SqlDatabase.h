#ifndef SQLDATABASE_H
#define SQLDATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDriver>
#include <QSqlRecord>
#include "CustomerInformation.h"
#include "OrderInformation.h"
#include "FinancialRecordInfo.h"

class SqlDatabase
{
private:
    bool m_customerQueryIsOK;
    bool m_orderQueryIsOK;
    bool m_productQueryIsOK;
    bool m_financialQueryIsOK;

    QSqlDatabase m_db;                  // 数据库
    QSqlQuery* m_customerQuery;         // 客户信息数据库表
    QSqlQuery* m_orderQuery;            // 订单信息数据库表
    QSqlQuery* m_productQuery;          // 订单中产品信息数据库表
    QSqlQuery* m_financialQuery;        // 财务收支记录据库表

    QString m_errorStr;

    #define CUSTOMER_TABLE "customers"
    #define ORDER_TABLE "orders"
    #define PRODUCT_TABLE "products"
    #define FINANCIAL_TABLE "financial"

private:
    SqlDatabase(const SqlDatabase&);
    SqlDatabase& operator= (const SqlDatabase&);

    bool productInfoIsExist(int number);      // 产品信息是否存在
    bool insertProductInfo(const QString& orderID, const ProductInfo& productInfo);
    bool updateProductInfo(const ProductInfo& productInfo);
    bool getProductInfoByOrderID(const QString& orderID, QList<ProductInfo>& productInfoList);

    void getCustomerInfoList(QSqlQuery* query, QList<CustomerInformation>& customerInfoList);
    void getOrderInfoList(QSqlQuery* query, QList<OrderInformation>& orderInfoList);

    bool getFinancialInfoMaxNumber(int& number);

    bool deleteProductInfo(const QString& orderID);
public:
    explicit SqlDatabase(const QString& connectionName = "A");
    ~SqlDatabase();

    QString getErrorStr();

    bool customerInfoIsExist(const QString& name);      // 判断客户是否存在
    bool insertCustomerInfo(const CustomerInformation& customerInfo);
    bool updateCustomerInfo(const CustomerInformation& customerInfo);
    bool getCustomerInfoByName(const QString& name, QList<CustomerInformation>& customerInfoList);
    bool getCustomerInfoByInquirySource(const QString& source, QList<CustomerInformation>& customerInfoList);
    bool getCustomerInfoByCountry(const QString& country, QList<CustomerInformation>& customerInfoList);
    bool getCustomerInfoByCompany(const QString& company, QList<CustomerInformation>& customerInfoList);
    bool getCustomerInfoByEmail(const QString& email, QList<CustomerInformation>& customerInfoList);
    bool getCustomerInfoByPhoneNumber(const QString& phoneNumber, QList<CustomerInformation>& customerInfoList);
    bool getCustomerInfoBySalesman(const QString& salesman, QList<CustomerInformation>& customerInfoList);
    bool getCustomerInfoByKeyword(const QString& keyword, QList<CustomerInformation>& customerInfoList);
    bool getCustomerInfoByStatus(const QString& status, QList<CustomerInformation>& customerInfoList);

    bool orderInfoIsExist(const QString& orderID);      // 订单信息是否存在
    bool insertOrderInfo(const OrderInformation& orderInfo);
    bool updateOrderStatus(const QStringList& orderList, const QString& status);
    bool updateOrderInfo(const OrderInformation& orderInfo);
    bool getAllOrderInfo(QList<OrderInformation>& orderInfoList);
    bool getOrderInfoByOrderID(const QString& orderID, QList<OrderInformation>& orderInfoList);
    bool getOrderInfoByContractID(const QString& contractID, QList<OrderInformation>& orderInfoList);
    bool getOrderInfoByDateRange(const QString& startDate, const QString& endDate, QList<OrderInformation>& orderInfoList);
    bool getOrderInfoBySalesman(const QString& salesman, QList<OrderInformation>& orderInfoList);
    bool getOrderInfoByCustomerName(const QString& name, QList<OrderInformation>& orderInfoList);
    bool getOrderInfoByStatus(const QString& status, QList<OrderInformation>& orderInfoList);

    bool finacialInfoIsExist(int number);      // 收支记录是否存在
    bool insertFinacialInfo(FinancialRecordInfo &finacialInfo);
    bool updateFinacialInfo(const FinancialRecordInfo& finacialInfo);
    bool getFinacialInfoByNumber(int number, QList<FinancialRecordInfo>& financialRecordInfoList);
    bool getFinacialInfoByContractID(const QString& contractID, QList<FinancialRecordInfo>& financialRecordInfoList);
    bool getFinacialInfoByCustomerName(const QString& name, QList<FinancialRecordInfo>& financialRecordInfoList);
    bool getCustomerNameByContractID(const QString& contractID, QStringList& nameList);

    bool deleteCustomerInfo(const QString& name);
    bool deleteOrderInfo(const QString& orderID);
    bool deleteProductInfo(int number);
    bool deleteFinacialInfo(int number);
};

#endif // COOKIESQL_H
