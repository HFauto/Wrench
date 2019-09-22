#ifndef TOOLINTERFACE_H
#define TOOLINTERFACE_H

#include <QMainWindow>
#include <QtSql>

namespace Ui {
class ToolInterface;
}

class ToolInterface : public QMainWindow
{
    Q_OBJECT

public:
    explicit ToolInterface(QWidget *parent = nullptr);
    ~ToolInterface();

private:
    Ui::ToolInterface *ui;

    QSqlDatabase  DB;//数据库连接

    QSqlRelationalTableModel  *toolModel;  //数据模型

    QItemSelectionModel *theSelection; //选择模型


private slots:


    void on_currentChanged(const QModelIndex &current, const QModelIndex &previous);

    //QTableView的SelectionModel的行发生了变化，进行处理
    void on_currentRowChanged(const QModelIndex &current, const QModelIndex &previous);

    void on_btnAdd_clicked();

    void on_btnInsert_clicked();

    void on_btnDelete_clicked();

    void on_btnSave_clicked();

    void on_btnCancel_clicked();

    void on_btnSetPhoto_clicked();

    void on_btnCancelPhoto_clicked();

    void on_pushButton_clicked();
};

#endif // TOOLINTERFACE_H
