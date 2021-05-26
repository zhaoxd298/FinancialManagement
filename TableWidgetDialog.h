#ifndef TABLEWIDGETDIALOG_H
#define TABLEWIDGETDIALOG_H
#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QDialog>
#include <QString>
#include <QComboBox>
#include <QGridLayout>
#include "TableWidget.h"
#include "FinancialRecordInfo.h"

class TableWidgetDialog : public QDialog
{
    Q_OBJECT
private:
    QPushButton* m_okBtn;
    QLabel* m_statusLabel;
    TableWidget* m_tableWidget;

    void constructUI();
    void connectSlots();

public:
    TableWidgetDialog(QWidget *parent = 0);

    TableWidget* getTableWidget();

    void setStatusText(const QString& text);

public slots:
    void onOkBtn();
};

#endif // TABLEWIDGETDIALOG_H
