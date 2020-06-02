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

    realIncomeSum += realIncome;
    shouldIncomeSum += shouldIncome;
    freightCustomerSum += freightCustomer;
    freightFactoryToUsSum += freightFactoryToUs;
    freightUsToForwardingSum += freightUsToForwarding;
    freightForeignSum += freightForeign;
    handlingFeeSum += handlingFee;
    tatolExpensesSum += tatolExpenses;
    totalProfitSum += totalProfit;
    partnerProfitSum += partnerProfit;
}

void OrderInformation::clearSumItem()
{
    realIncomeSum = 0;
    shouldIncomeSum = 0;
    freightCustomerSum = 0;
    freightFactoryToUsSum = 0;
    freightUsToForwardingSum = 0;
    freightForeignSum = 0;
    handlingFeeSum = 0;
    tatolExpensesSum = 0;
    totalProfitSum = 0;
    partnerProfitSum = 0;
}

double OrderInformation::realIncomeSum = 0;
double OrderInformation::shouldIncomeSum = 0;
double OrderInformation::freightCustomerSum = 0;
double OrderInformation::freightFactoryToUsSum = 0;
double OrderInformation::freightUsToForwardingSum = 0;
double OrderInformation::freightForeignSum = 0;
double OrderInformation::handlingFeeSum = 0;
double OrderInformation::tatolExpensesSum = 0;
double OrderInformation::totalProfitSum = 0;
double OrderInformation::partnerProfitSum = 0;
