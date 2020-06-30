#include "OrderInformation.h"


ProductInfo::ProductInfo()
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

void ProductInfo::calProfitIncomeAndCost()
{
    curProfit = (costPrice > price) * count;
    //qDebug() << "curProfit:" << curProfit;

    totalIncome = price * count;
    //qDebug() << "totalIncome:" << totalIncome;

    totalCost = costPrice * count;
    //qDebug() << "totalCost:" << totalCost;
}


OrderInformation::OrderInformation()
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
    exchangeRate = 0;
    tatolExpenses = 0;
    totalProfit = 0;
    partnerProfit = 0;
    remarks = "";
}

void OrderInformation::calProfitIncomeAndExpenses()
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

