#ifndef PERSONNEL_H
#define PERSONNEL_H

#include <QMainWindow>
#include <QtSql>
#include <QDataWidgetMapper>
#include <QCameraInfo>
#include <QCamera>

namespace Ui {
class Personnel;
}

class Personnel : public QMainWindow
{
    Q_OBJECT

public:
    explicit Personnel(QWidget *parent = nullptr);
    ~Personnel();

private:
    Ui::Personnel *ui;

    QSqlDatabase  DB;//数据库连接

    QSqlRelationalTableModel  *perModel;  //数据模型

    QSqlRelationalTableModel  *perModel2;  //数据模型

    QItemSelectionModel *theSelection; //选择模型

    QItemSelectionModel *theSelection2; //选择模型

    //QDataWidgetMapper   *dataMapper; //数据映射

     void    fillComboBox();//获取字段名称,填充“排序字段”的comboBox

     QCamera     *curCamera=Q_NULLPTR;


private slots:


    void on_currentChanged(const QModelIndex &current, const QModelIndex &previous);

    //QTableView的SelectionModel的行发生了变化，进行处理
    void on_currentRowChanged(const QModelIndex &current, const QModelIndex &previous);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_btnAdd_clicked();

    void on_btnDelete_clicked();

    void on_btnSave_clicked();

    void on_btnCancel_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_pushButton_8_clicked();
};

#endif // PERSONNEL_H
