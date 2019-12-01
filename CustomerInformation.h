#ifndef CUSTOMERINFORMATION_H
#define CUSTOMERINFORMATION_H

#include <QString>

struct CustomerInformation
{
    QString name;
    QString level;              // 客户等级
    QString status;             // 状态
    QString enterDate;          // 添加客户的日期
    QString inquirySource;
    QString background;
    QString address;
    QString companyName;
    QString websit;
    QString email;
    QString phoneNumber;
    QString position;
    QString schedule;
    QString salesman;
    QString remarks;           // 备注
};

#endif // CUSTOMERINFORMATION_H
