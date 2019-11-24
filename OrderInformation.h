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
    int count;                   // 数量
    double curProfit;            // 利润
    double totalIncome;          // 总收如
    double totalCost;            // 总成本

    ProductInfo()
    {
        number = 0;
        productName = "";
        price = 0;
        costPrice = 0;
        count = 0;
        curProfit = 0;
        totalIncome = 0;
        totalCost = 0;
    }

    void calProfitIncomeAndCost()
    {
        curProfit = (costPrice > price) * count;
        //qDebug() << "curProfit:" << curProfit;

        totalIncome = price * count;
        //qDebug() << "totalIncome:" << totalIncome;

        totalCost = costPrice * count;
        //qDebug() << "totalCost:" << totalCost;
    }
};

struct OrderInformation
{
    QString orderID;                    // 订单编号
    QString customerName;               // 客户
    QString orderStatus;              // 订单状态
    QString payTime;                    // 付款时间
    QString payType;                    // 付款方式
    double realIncome;                  // 实收
    double shouldIncome;                // 应收
    double freightCustomer;             // 运费(客户)
    double freightFactoryToUs;          // 运费(工厂→我司)
    double freightUsToForwarding;       // 运费(我司→货代)
    double freightForeign;              // 运费(国外)
    double packageFee;                  // 包装费
    double handlingFee;                 // 平台手续费
    double tatolExpenses;               // 总支出
    double totalProfit;                 // 总利润
    double partnerProfit;               // 合伙人利润
    QString remarks;                    // 备注
    QString salesman;                   // 业务员

    QList<ProductInfo> productList;           // 商品列表

    OrderInformation()
    {
        orderID = "";
        customerName = "";
        orderStatus = "";
        payTime = "";
        payType = "";
        realIncome = 0;
        shouldIncome = 0;
        handlingFee = 0;
        freightCustomer = 0;
        freightFactoryToUs = 0;
        freightUsToForwarding = 0;
        freightForeign = 0;
        packageFee = 0;
        tatolExpenses = 0;
        totalProfit = 0;
        partnerProfit = 0;
        remarks = "";
    }

    void calProfitIncomeAndExpenses()
    {
        for (int i=0; i<productList.size(); i++)
        {
            productList[i].calProfitIncomeAndCost();
        }

        // 应收
        shouldIncome = freightCustomer;
        for (int i=0; i<productList.size(); i++)
        {
            shouldIncome += productList[i].totalIncome;
        }
        //qDebug() << "shouldIncome:" << shouldIncome;

        // 总支出
        tatolExpenses = handlingFee;
        tatolExpenses += freightFactoryToUs;
        tatolExpenses += freightUsToForwarding;
        tatolExpenses += freightForeign;
        tatolExpenses += packageFee;
        for (int i=0; i<productList.size(); i++)
        {
            tatolExpenses += productList[i].totalCost;
        }
        //qDebug() << "tatolExpenses:" << tatolExpenses;

        // 总利润
        totalProfit = realIncome - tatolExpenses;
        //qDebug() << "totalProfit:" << totalProfit;

        // 合伙人利润
        partnerProfit = totalProfit * 0.4;
        //qDebug() << "partnerProfit:" << partnerProfit;
    }
};

#endif // CUSTOMERINFORMATION_H
