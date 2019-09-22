#ifndef CODEINTERFACE_H
#define CODEINTERFACE_H

#include <QMainWindow>
#include <QtSql>
#include <QStandardItemModel>
#include <QDataWidgetMapper>

namespace Ui {
class CodeInterface;
}

class CodeInterface : public QMainWindow
{
    Q_OBJECT

public:
    explicit CodeInterface(QWidget *parent = nullptr);
    ~CodeInterface();

private slots:


    void on_btnAdd_clicked();

    void on_btnInsert_clicked();

    void on_btnDelete_clicked();

    void on_btnSave_clicked();

    void on_btnCancel_clicked();

    void on_currentChanged(const QModelIndex &current, const QModelIndex &previous);

    //QTableView的SelectionModel的行发生了变化，进行处理
    //void on_currentRowChanged(const QModelIndex &current, const QModelIndex &previous);

private:
    Ui::CodeInterface *ui;

    QSqlDatabase  DB;//数据库连接

    QSqlRelationalTableModel  *codeModel;  //数据模型

    QItemSelectionModel *theSelection; //选择模型



};

#endif // CODEINTERFACE_H
