#ifndef JOB_H
#define JOB_H

#include <QMainWindow>
#include <QtSql>

namespace Ui {
class Job;
}

class Job : public QMainWindow
{
    Q_OBJECT

public:
    explicit Job(QWidget *parent = nullptr);
    ~Job();

private:
    Ui::Job *ui;

    QSqlDatabase  DB;//数据库连接

    QSqlRelationalTableModel  *jobModel;  //数据模型

    QItemSelectionModel *theSelection; //选择模型


private slots:


    void on_currentChanged(const QModelIndex &current, const QModelIndex &previous);

    void on_btnAdd_clicked();

    void on_btnInsert_clicked();

    void on_btnDelete_clicked();

    void on_btnSave_clicked();

    void on_btnCancel_clicked();

    void on_radioButton_clicked();

    void on_radioButton_2_clicked();
};

#endif // JOB_H
