#ifndef SEARCHCUSTOMERDIALOG_H
#define SEARCHCUSTOMERDIALOG_H

#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QDialog>
#include <QString>
#include <QComboBox>
#include <QGridLayout>

class SearchCustomerDialog : public QDialog
{
    Q_OBJECT
private:
    QString m_keyWord;
    int m_searchType;

    int m_lastStatusCbxIndex;

    QComboBox* m_searchTypeCBox;
    QComboBox* m_statusCBox;
    QLineEdit* m_keyWordEdit;
    QPushButton* m_okBtn;
    QGridLayout* m_gridLayout;

    void constructUI();
    void connectSlots();
public:
    enum {
        SearchByName            = 0,
        SearchByStatus          = 1,
        SearchByEmail           = 2,
        SearchByPhoneNumber     = 3,
        searchByCompany         = 4,
        SearchByCountry         = 5,
        SearchByInquirySource   = 6,
        SearchBySalesman        = 7
    };
public:
    SearchCustomerDialog(QWidget *parent = 0);
    ~SearchCustomerDialog();

    QString getKeyWord();
    int getSearchType();
public slots:
    void onOkBtn();
    void onCbxIndexChanged(int index);
};

#endif // FINDCUSTOMERDIALOG_H
