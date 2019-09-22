#ifndef QBYJOBS_H
#define QBYJOBS_H

#include <QMainWindow>
#include <QtSql>
#include <QPrinter>

namespace Ui {
class QByJobs;
}

class QByJobs : public QMainWindow
{
    Q_OBJECT

public:
    explicit QByJobs(QWidget *parent = nullptr);
    ~QByJobs();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void printPreviewSlot(QPrinter *printerPixmap);

    void on_pushButton_3_clicked();

private:
    Ui::QByJobs *ui;
    QSqlDatabase  DB;//数据库连接
    QSqlRelationalTableModel  *qryModel;  //数据模型
    QItemSelectionModel *theSelection; //选择模型
};

#endif // QBYJOBS_H
