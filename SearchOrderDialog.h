#ifndef SEARCHORDERDIALOG_H
#define SEARCHORDERDIALOG_H

#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QDateEdit>
#include <QGridLayout>
#include <QDialog>

#define SEARCH_ALL_ORDER              0
#define SEARCH_BY_UNPAY_PROFIT        1
#define SEARCH_BY_PAYED_PROFIT        2
#define SEARCH_BY_LAST_MONTH_ORDER    3
#define SEARCH_BY_DATE_RANGE          4
#define SEARCH_BY_SALESMAN            5
#define SEARCH_BY_ORDERID             6
#define SEARCH_BY_CUSTOMER_NAME       7

class SearchOrderDialog : public QDialog
{
    Q_OBJECT
private:
    QString m_keyWord;
    int m_searchType;
    int m_lastCbxIndex;
    QString m_startDate;
    QString m_endDate;

    QLabel* m_kerWordLabel;
    QComboBox* m_searchTypeCBox;
    QLineEdit* m_keyWordEdit;
    QDateEdit* m_startDataEdit;
    QDateEdit* m_endDataEdit;
    QLabel* m_connectLabel;
    QLabel* m_dateRangeLabel;
    QPushButton* m_okBtn;

    QGridLayout* m_gridLayout;

    void constructUI();
    void connectSlots();
public:
    SearchOrderDialog(QWidget *parent = 0);
    ~SearchOrderDialog();

    QString getKeyWord();
    int getSearchType();
    QString getStartDate();
    QString getEndDate();
public slots:
    void onOkBtn();
    void onCbxIndexChanged(int index);
};

#endif // FINDCUSTOMERDIALOG_H
