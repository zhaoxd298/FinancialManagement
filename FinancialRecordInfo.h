#ifndef FINANCIALRECORDINFO_H
#define FINANCIALRECORDINFO_H

#include <QString>
#include <QList>
#include <QDebug>
#include "Version.h"

struct FinancialRecordInfo
{
    int number;                     // 编号，用于数据库主键
    QString contractID;             // 合同编号
    QString customerName;           // 客户
    QString type;                   // 收支（收入/支出）
    double amount;                  // 金额
    QString payTime;				// 付款时间
    QString payType;                // 付款方式
    QString remarks;				// 备注
};


#endif // FINANCIALRECORDINFO_H
