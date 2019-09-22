#include "job.h"
#include "ui_job.h"
#include <QMessageBox>
#include <QFileDialog>


Job::Job(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Job)
{
    ui->setupUi(this);

    jobModel=new QSqlRelationalTableModel(this,DB);//数据表

    jobModel->setTable("jobtbl"); //设置数据表

    //数据保存方式，OnManualSubmit点击保存按钮保存记录, OnRowChange
    jobModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    jobModel->setSort(jobModel->fieldIndex("rid"),Qt::AscendingOrder); //排序
    if (!(jobModel->select()))//查询数据
    {
        QMessageBox::critical(this,tr("错误信息"), tr("打开数据表错误,错误信息\n")+jobModel->lastError().text(), QMessageBox::Ok,QMessageBox::NoButton);
        return;
    }
    //设置表头
    jobModel->setHeaderData(jobModel->fieldIndex("jid"),Qt::Horizontal,tr("工单号"));
    jobModel->setHeaderData(jobModel->fieldIndex("pid"),Qt::Horizontal,tr("端口号"));
    jobModel->setHeaderData(jobModel->fieldIndex("code"),Qt::Horizontal,tr("代码"));
    jobModel->setHeaderData(jobModel->fieldIndex("name"),Qt::Horizontal,tr("工单名称"));
    jobModel->setHeaderData(jobModel->fieldIndex("seqno"),Qt::Horizontal,tr("工单代码"));
    jobModel->setHeaderData(jobModel->fieldIndex("dtss"),Qt::Horizontal,tr("开始时间"));
    jobModel->setHeaderData(jobModel->fieldIndex("dtee"),Qt::Horizontal,tr("结束时间"));
    jobModel->setHeaderData(jobModel->fieldIndex("uid"),Qt::Horizontal,tr("工人代号"));
    jobModel->setHeaderData(jobModel->fieldIndex("dept"),Qt::Horizontal,tr("部门代号"));
    jobModel->setHeaderData(jobModel->fieldIndex("part"),Qt::Horizontal,tr("部位代号"));
    jobModel->setHeaderData(jobModel->fieldIndex("loc"),Qt::Horizontal,tr("地点代号"));
    jobModel->setHeaderData(jobModel->fieldIndex("note"),Qt::Horizontal,tr("备注"));

    jobModel->setHeaderData(jobModel->fieldIndex("todos"),Qt::Horizontal,tr("最大扭矩"));
    jobModel->setHeaderData(jobModel->fieldIndex("oks"),Qt::Horizontal,tr("备注"));
    jobModel->setHeaderData(jobModel->fieldIndex("ngs"),Qt::Horizontal,tr("照片"));
    jobModel->setHeaderData(jobModel->fieldIndex("mfgflag"),Qt::Horizontal,tr("照片"));
    jobModel->setHeaderData(jobModel->fieldIndex("excel"),Qt::Horizontal,tr("照片"));

    jobModel->setHeaderData(jobModel->fieldIndex("picpng"),Qt::Horizontal,tr("照片"));

    //关联选择模型
    theSelection=new QItemSelectionModel(jobModel);

    //theSelection当前项变化时触发currentChanged信号
    connect(theSelection,SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this,SLOT(on_currentChanged(QModelIndex,QModelIndex)));

    //选择行变化时
    connect(theSelection,SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this,SLOT(on_currentRowChanged(QModelIndex,QModelIndex)));


    ui->tableView->setModel(jobModel);//设置数据模型
    ui->tableView->setSelectionModel(theSelection);//设置选择模型


    ui->tableView->setColumnHidden(jobModel->fieldIndex("note"),true);//隐藏列
    ui->tableView->setColumnHidden(jobModel->fieldIndex("todos"),true);//隐藏列
    ui->tableView->setColumnHidden(jobModel->fieldIndex("oks"),true);//隐藏列
    ui->tableView->setColumnHidden(jobModel->fieldIndex("ngs"),true);//隐藏列
    ui->tableView->setColumnHidden(jobModel->fieldIndex("mfgflag"),true);//隐藏列
    ui->tableView->setColumnHidden(jobModel->fieldIndex("excel"),true);//隐藏列
    ui->tableView->setColumnHidden(jobModel->fieldIndex("picpng"),true);//隐藏列
    //ui->tableView->setItemDelegate(new QSqlRelationalDelegate(ui->tableView)); //为关系型字段设置缺省代理组件


    //tableView显示属性设置
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setAlternatingRowColors(true);//使用交替颜色绘制背景
    theSelection->setCurrentIndex(jobModel->index(0,0),QItemSelectionModel::Select);//默认选择第一行数据
}

Job::~Job()
{
    delete ui;
}
/*
* @projectName   WrenchApp
* @brief         当前项变化
* @author        Li
* @date          2019-09-03
*/
void Job::on_currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(current)
    Q_UNUSED(previous)
    ui->btnSave->setEnabled(jobModel->isDirty()); //有未保存修改时可用
    ui->btnCancel->setEnabled(jobModel->isDirty());

}
/*
* @projectName   WrenchApp
* @brief         添加数据
* @author        Li
* @date          2019-09-08
*/
void Job::on_btnAdd_clicked()
{
    jobModel->insertRow(jobModel->rowCount(),QModelIndex()); //在末尾添加一个记录
    QModelIndex curIndex=jobModel->index(jobModel->rowCount()-1,1);//创建最后一行的ModelIndex
    theSelection->clearSelection();//清空选择项
    theSelection->setCurrentIndex(curIndex,QItemSelectionModel::Select);//设置刚插入的行为当前选择行

}
/*
* @projectName   WrenchApp
* @brief         插入数据
* @author        Li
* @date          2019-09-08
*/
void Job::on_btnInsert_clicked()
{
    QModelIndex curIndex=ui->tableView->currentIndex();
    jobModel->insertRow(curIndex.row(),QModelIndex());
    theSelection->clearSelection();//清除已有选择
    theSelection->setCurrentIndex(curIndex,QItemSelectionModel::Select);
}
/*
* @projectName   WrenchApp
* @brief         删除记录
* @author        Li
* @date          2019-09-08
*/
void Job::on_btnDelete_clicked()
{
    //删除当前记录
    ui->btnSave->setEnabled(true);
    ui->btnCancel->setEnabled(true);

    QModelIndex curIndex=theSelection->currentIndex();//获取当前选择单元格的模型索引
    jobModel->removeRow(curIndex.row()); //删除最后一行
}
/*
* @projectName   WrenchApp
* @brief         保存记录
* @author        Li
* @date          2019-09-08
*/
void Job::on_btnSave_clicked()
{
    //保存修改
    bool res= jobModel->submitAll();

    if (!res)
        QMessageBox::information(this, tr("消息"), tr("数据保存错误,错误信息\n")+ jobModel->lastError().text(),
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
* @date          2019-09-08
*/
void Job::on_btnCancel_clicked()
{
    //取消修改
    jobModel->revertAll();
    ui->btnSave->setEnabled(false);
    ui->btnCancel->setEnabled(false);
}
/*
* @projectName   WrenchApp
* @brief         显示全部工单
* @author        Li
* @date          2019-09-10
*/
void Job::on_radioButton_clicked()
{
    jobModel->setFilter("");
}
/*
* @projectName   WrenchApp
* @brief         显示未完成的工单
* @author        Li
* @date          2019-09-10
*/
void Job::on_radioButton_2_clicked()
{
    jobModel->setFilter("dtee is null");
}
