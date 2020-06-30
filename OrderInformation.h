#ifndef ORDERINFORMATION_H
#define ORDERINFORMATION_H

#include <QString>
#include <QList>
#include <QDebug>

struct ProductInfo
{
    int number;                  // 编号，用于数据库
    QString productName;         // 商品名称
    double price;                // 单价
    double costPrice;            // 成本单价
    QString spec;                // 产品规格
    int count;                   // 数量
    double curProfit;            // 利润
    double totalIncome;          // 总收如
    double totalCost;            // 总成本

    ProductInfo();
    void calProfitIncomeAndCost();
};

struct OrderInformation
{
    QString orderID;                    // 订单编号
    QString contractID;                 // 合同编号
    QString customerName;               // 客户
    QString orderStatus;                // 订单状态
    QString payTime;                    // 付款时间
    QString payType;                    // 付款方式
    double realIncome;                  // 实收
    double shouldIncome;                // 应收
    double freightCustomer;             // 运费(客户)
    double freightFactoryToUs;          // 运费(工厂→我司)
    double freightUsToForwarding;       // 运费(我司→货代)
    double freightForeign;              // 运费(国外)
    double exchangeRate;                // 汇率
    double handlingFee;                 // 平台手续费
    double tatolExpenses;               // 总支出
    double totalProfit;                 // 总利润
    double partnerProfit;               // 合伙人利润
    QString remarks;                    // 备注
    QString salesman;                   // 业务员

    QList<ProductInfo> productList;           // 商品列表

    OrderInformation();

    void calProfitIncomeAndExpenses();
};

#endif // CUSTOMERINFORMATION_H
