#ifndef SEARCHFINANCIALRECORDDIALOG_H
#define SEARCHFINANCIALRECORDDIALOG_H
#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QDialog>
#include <QString>
#include <QComboBox>
#include <QGridLayout>

class SearchFinancialRecordDialog : public QDialog
{
    Q_OBJECT
private:
    QString m_keyWord;
    int m_searchType;

    QComboBox* m_searchTypeCBox;
    QLineEdit* m_keyWordEdit;
    QPushButton* m_okBtn;
    QGridLayout* m_gridLayout;

    void constructUI();
    void connectSlots();

public:
    enum {
        SearchByContractID          = 0,
        SearchByCustomerName        = 1,
    };

public:
    SearchFinancialRecordDialog(QWidget *parent = 0);

    QString getKeyWord();
    int getSearchType();

public slots:
    void onOkBtn();
};

#endif // SEARCHFINANCIALRECORDDIALOG_H
