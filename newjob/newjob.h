#ifndef NEWJOB_H
#define NEWJOB_H

#include <QMainWindow>
#include <QtSql>
#include <QDataWidgetMapper>

namespace Ui {
class Newjob;
}

class Newjob : public QMainWindow
{
    Q_OBJECT

public:
    explicit Newjob(QWidget *parent = nullptr);
    ~Newjob();

private slots:
    void on_btnAdd_clicked();

    void on_currentChanged(const QModelIndex &current, const QModelIndex &previous);

    //QTableView的SelectionModel的行发生了变化，进行处理
    void on_currentRowChanged(const QModelIndex &current, const QModelIndex &previous);

    void on_currentRowChanged2(const QModelIndex &current, const QModelIndex &previous);

    void on_btnDelete_clicked();

    void on_btnSave_clicked();

    void on_btnCancel_clicked();

    void on_btnSetPhoto_clicked();

    void on_btnCancelPhoto_clicked();

    void on_btnPhoto_clicked();

private:
    Ui::Newjob *ui;

    QSqlDatabase  DB;//数据库连接

    QSqlRelationalTableModel  *newModel;  //数据模型

    QSqlRelationalTableModel  *newjobModel;  //数据模型

    QItemSelectionModel *theSelection; //选择模型

    QItemSelectionModel *theSelection2; //选择模型

    QDataWidgetMapper   *dataMapper; //数据映射

public:

    static QByteArray data(QByteArray);
};

#endif // NEWJOB_H
