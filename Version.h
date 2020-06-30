#ifndef VERSION_H
#define VERSION_H

//#define REVI_FINANCIAL
#define EXSUN_LIGHTING_FINANCIAL

#if defined(EXSUN_LIGHTING_FINANCIAL)
#define TITLE "亿生财务管理系统 "
#elif defined(REVI_FINANCIAL)
#define TITLE "睿为财务管理系统 "
#endif

#define VERSION "V1.2.9"

/**
 * 1.2.3
 * 订单增加合同编号

 * 1.2.4
 * 平台手续费由4%改为2.17%算

 * 1.2.5
 * 1.按订单号查询  --->  按合同编号查询
 * 2.客户信息增加按关键字搜索（即全文搜索，只要任何一列满足关键字都会被检索出来）
 * 3.可对订单中的产品增加和删除

 * 1.2.6
 * 1.新增订单  --->  增加货币单位RMB

 * 1.2.7
 * 1.新增睿为财务管理系统
 * 2.睿为财务管理系统付款方式增加义乌个体户
 * 3.付款方式选“其他”时增加一个QLineEdit输入对应的付款方式
 * 4.删除订单信息和客户信息成功时不再弹出对话框

 * 1.2.8
 * 1.睿为不需要合伙人利润
 * 2.增加合计功能

 * 1.2.9
 * 1.更新“合计”功能，在编辑订单后也可以更新合计值
 * 2.睿为查找订单对话框去掉“按未结算利润查询”和“按结算利润查询”，增加“待发货”和“已发货”选项
 */


#endif // VERSION_H
