#ifndef QBYTOOLS_H
#define QBYTOOLS_H

#include <QMainWindow>
#include <QtSql>
#include <QPrinter>

namespace Ui {
class QByTools;
}

class QByTools : public QMainWindow
{
    Q_OBJECT

public:
    explicit QByTools(QWidget *parent = nullptr);
    ~QByTools();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void printPreviewSlot(QPrinter *printerPixmap);




    void on_pushButton_3_clicked();

private:
    Ui::QByTools *ui;

    QSqlDatabase  DB;//数据库连接
    QSqlRelationalTableModel  *qryModel;  //数据模型
    QItemSelectionModel *theSelection; //选择模型
};

#endif // QBYTOOLS_H
