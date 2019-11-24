#ifndef USERACCOUNT_H
#define USERACCOUNT_H

#include <QString>

struct UserAccount {
    int rowNum;
    int num;
    int status;
    bool isChecked;
    QString name;
    QString loginPasswd;
    QString payPasswd;
    QString cookie;
    QString statusStr;
    QString productIDNum;

    UserAccount()
    {
        rowNum = 0;
        num = 0;
        status = 0;
        isChecked = false;
        name = "";
        loginPasswd = "";
        payPasswd = "";
        cookie = "";
        statusStr = "";
        productIDNum = "";
    }

    bool isEmpty()
    {
        return ((name.isEmpty()) || (loginPasswd.isEmpty()));
    }

};

#endif // USERACCOUNT_H
