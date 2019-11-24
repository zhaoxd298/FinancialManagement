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
    SearchCustomerDialog(QWidget *parent = 0);
    ~SearchCustomerDialog();

    QString getKeyWord();
    int getSearchType();
public slots:
    void onOkBtn();
    void onCbxIndexChanged(int index);
};

#endif // FINDCUSTOMERDIALOG_H
