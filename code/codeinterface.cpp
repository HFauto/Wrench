#include "codeinterface.h"
#include "ui_codeinterface.h"
#include <QDebug>
#include <QMessageBox>

CodeInterface::CodeInterface(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CodeInterface)
{
    ui->setupUi(this);

    codeModel=new QSqlRelationalTableModel(this,DB);//数据表

    codeModel->setTable("codes"); //设置数据表

    //数据保存方式，OnManualSubmit点击保存按钮保存记录, OnRowChange
    codeModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    codeModel->setSort(codeModel->fieldIndex("type"),Qt::AscendingOrder); //排序
    if (!(codeModel->select()))//查询数据
    {
        QMessageBox::critical(this,tr("错误信息"), tr("打开数据表错误,错误信息\n")+codeModel->lastError().text(), QMessageBox::Ok,QMessageBox::NoButton);
        return;
    }

    //设置表头
    codeModel->setHeaderData(codeModel->fieldIndex("type"),Qt::Horizontal,tr("类型号"));
    codeModel->setHeaderData(codeModel->fieldIndex("code"),Qt::Horizontal,tr("代码"));
    codeModel->setHeaderData(codeModel->fieldIndex("name"),Qt::Horizontal,tr("名字"));
    codeModel->setHeaderData(codeModel->fieldIndex("note"),Qt::Horizontal,tr("备注"));
    codeModel->setHeaderData(codeModel->fieldIndex("xml"),Qt::Horizontal,tr("文件"));

    //关联选择模型
    theSelection=new QItemSelectionModel(codeModel);

    //theSelection当前项变化时触发currentChanged信号
    connect(theSelection,SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this,SLOT(on_currentChanged(QModelIndex,QModelIndex)));

    //选择行变化时
    connect(theSelection,SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this,SLOT(on_currentRowChanged(QModelIndex,QModelIndex)));


    ui->tableView->setModel(codeModel);//设置数据模型
    ui->tableView->setSelectionModel(theSelection);//设置选择模型
    ui->tableView->setColumnHidden(codeModel->fieldIndex("xml"),true);//隐藏列

    //tableView显示属性设置
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setAlternatingRowColors(true);//使用交替颜色绘制背景




}

CodeInterface::~CodeInterface()
{
    delete ui;
}
/*
* @projectName   WrenchApp
* @brief         当前改变
* @author        Li
* @date          2019-09-05
*/
void CodeInterface::on_currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(current)
    Q_UNUSED(previous)
    ui->btnSave->setEnabled(codeModel->isDirty()); //有未保存修改时可用
    ui->btnCancel->setEnabled(codeModel->isDirty());

}
/*
* @projectName   WrenchApp
* @brief         添加记录
* @author        Li
* @date          2019-09-05
*/
void CodeInterface::on_btnAdd_clicked()
{
    codeModel->insertRow(codeModel->rowCount(),QModelIndex()); //在末尾添加一个记录
    QModelIndex curIndex=codeModel->index(codeModel->rowCount()-1,1);//创建最后一行的ModelIndex
    theSelection->clearSelection();//清空选择项
    theSelection->setCurrentIndex(curIndex,QItemSelectionModel::Select);//设置刚插入的行为当前选择行

}
/*
* @projectName   WrenchApp
* @brief         插入记录
* @author        Li
* @date          2019-09-05
*/
void CodeInterface::on_btnInsert_clicked()
{
    QModelIndex curIndex=ui->tableView->currentIndex();
    codeModel->insertRow(curIndex.row(),QModelIndex());
    theSelection->clearSelection();//清除已有选择
    theSelection->setCurrentIndex(curIndex,QItemSelectionModel::Select);
}
/*
* @projectName   WrenchApp
* @brief         删除记录
* @author        Li
* @date          2019-09-05
*/
void CodeInterface::on_btnDelete_clicked()
{
    //删除当前记录
    ui->btnSave->setEnabled(true);
    ui->btnCancel->setEnabled(true);

    QModelIndex curIndex=theSelection->currentIndex();//获取当前选择单元格的模型索引
    codeModel->removeRow(curIndex.row()); //删除最后一行
}
/*
* @projectName   WrenchApp
* @brief         保存记录
* @author        Li
* @date          2019-09-05
*/
void CodeInterface::on_btnSave_clicked()
{
    //保存修改
    bool res= codeModel->submitAll();

    if (!res)
        QMessageBox::information(this, tr("消息"), tr("数据保存错误,错误信息\n")+ codeModel->lastError().text(),
                                 QMessageBox::Ok,QMessageBox::NoButton);
    else
    {
        ui->btnSave->setEnabled(false);
        ui->btnCancel->setEnabled(false);
    }
}
/*
* @projectName   WrenchApp
* @brief         取消修改
* @author        Li
* @date          2019-09-05
*/
void CodeInterface::on_btnCancel_clicked()
{
    //取消修改
    codeModel->revertAll();
    ui->btnSave->setEnabled(false);
    ui->btnCancel->setEnabled(false);
}
