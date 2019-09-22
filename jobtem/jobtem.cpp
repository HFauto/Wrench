#include "jobtem.h"
#include "ui_jobtem.h"
#include "camera/camera.h"
#include <QMessageBox>
#include <QFileDialog>

Jobtem::Jobtem(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Jobtem)
{
    ui->setupUi(this);

    jobtemModel=new QSqlRelationalTableModel(this,DB);//数据表

    jobtemModel->setTable("jobdef"); //设置数据表

    //数据保存方式，OnManualSubmit点击保存按钮保存记录, OnRowChange
    jobtemModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    jobtemModel->setSort(jobtemModel->fieldIndex("pid"),Qt::AscendingOrder); //排序
    if (!(jobtemModel->select()))//查询数据
    {
        QMessageBox::critical(this,tr("错误信息"), tr("打开数据表错误,错误信息\n")+jobtemModel->lastError().text(), QMessageBox::Ok,QMessageBox::NoButton);
        return;
    }
    //设置表头
    jobtemModel->setHeaderData(jobtemModel->fieldIndex("pid"),Qt::Horizontal,tr("进程号"));
    jobtemModel->setHeaderData(jobtemModel->fieldIndex("code"),Qt::Horizontal,tr("代码"));
    jobtemModel->setHeaderData(jobtemModel->fieldIndex("name"),Qt::Horizontal,tr("名称"));
    jobtemModel->setHeaderData(jobtemModel->fieldIndex("dt0"),Qt::Horizontal,tr("开始时间"));
    jobtemModel->setHeaderData(jobtemModel->fieldIndex("mfgflag"),Qt::Horizontal,tr("标志"));
    jobtemModel->setHeaderData(jobtemModel->fieldIndex("uid"),Qt::Horizontal,tr("工人"));
    jobtemModel->setHeaderData(jobtemModel->fieldIndex("dept"),Qt::Horizontal,tr("部门"));
    jobtemModel->setHeaderData(jobtemModel->fieldIndex("part"),Qt::Horizontal,tr("部位"));
    jobtemModel->setHeaderData(jobtemModel->fieldIndex("loc"),Qt::Horizontal,tr("地点"));
    jobtemModel->setHeaderData(jobtemModel->fieldIndex("note"),Qt::Horizontal,tr("备注"));
    jobtemModel->setHeaderData(jobtemModel->fieldIndex("picpng"),Qt::Horizontal,tr("照片"));
    jobtemModel->setHeaderData(jobtemModel->fieldIndex("excel"),Qt::Horizontal,tr("表格"));



    //关联选择模型
    theSelection=new QItemSelectionModel(jobtemModel);

    //theSelection当前项变化时触发currentChanged信号
    connect(theSelection,SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this,SLOT(on_currentChanged(QModelIndex,QModelIndex)));

    //选择行变化时
    connect(theSelection,SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this,SLOT(on_currentRowChanged(QModelIndex,QModelIndex)));


    ui->tableView->setModel(jobtemModel);//设置数据模型
    ui->tableView->setSelectionModel(theSelection);//设置选择模型
    ui->tableView->setColumnHidden(jobtemModel->fieldIndex("picpng"),true);//隐藏列
    ui->tableView->setColumnHidden(jobtemModel->fieldIndex("excel"),true);//隐藏列
    //ui->tableView->setColumnHidden(perModel->fieldIndex("note"),true);//隐藏列
    //ui->tableView->setItemDelegate(new QSqlRelationalDelegate(ui->tableView)); //为关系型字段设置缺省代理组件


    //tableView显示属性设置
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setAlternatingRowColors(true);//使用交替颜色绘制背景
    theSelection->setCurrentIndex(jobtemModel->index(0,0),QItemSelectionModel::Select);//默认选择第一行数据
}

Jobtem::~Jobtem()
{
    delete ui;
}
/*
* @projectName   WrenchApp
* @brief         选择项变化
* @author        Li
* @date          2019-09-14
*/
void Jobtem::on_currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(current)
    Q_UNUSED(previous)
    ui->btnSave->setEnabled(jobtemModel->isDirty()); //有未保存修改时可用
    ui->btnCancel->setEnabled(jobtemModel->isDirty());

}
/*
* @projectName   WrenchApp
* @brief         选择行变化
* @author        Li
* @date          2019-09-14
*/
void Jobtem::on_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous)
    //行切换时的状态控制
//    ui->pushButton_3->setEnabled(current.isValid());
//    ui->pushButton_6->setEnabled(current.isValid());
//    ui->pushButton_7->setEnabled(current.isValid());

    if (!current.isValid())
    {
        ui->labPhoto->clear(); //清除图片显示
        return;
    }

    //dataMapper->setCurrentIndex(current.row()); //更新数据映射的行号

    //QModelIndex curIndex=theSelection->currentIndex();//获取当前选择单元格的模型索引
    int curRecNo=current.row();//获取行号
    QSqlRecord  curRec=jobtemModel->record(curRecNo); //获取当前记录

    if (curRec.isNull("picpng"))  //图片字段内容为空
        ui->labPhoto->clear();
    else
    {
        QByteArray data=curRec.value("picpng").toByteArray();
        QPixmap pic;
        pic.loadFromData(data);
        ui->labPhoto->setPixmap(pic.scaledToWidth(150));//设置图片大小
    }


}
/*
* @projectName   WrenchApp
* @brief         添加记录
* @author        Li
* @date          2019-09-14
*/
void Jobtem::on_btnAdd_clicked()
{
    jobtemModel->insertRow(jobtemModel->rowCount(),QModelIndex()); //在末尾添加一个记录
    QModelIndex curIndex=jobtemModel->index(jobtemModel->rowCount()-1,1);//创建最后一行的ModelIndex
    theSelection->clearSelection();//清空选择项
    theSelection->setCurrentIndex(curIndex,QItemSelectionModel::Select);//设置刚插入的行为当前选择行

    QDateTime curDateTime=QDateTime::currentDateTime(); //读取当前日期时间
    QString dtss =curDateTime.toString("yyyy-MM-dd HH:mm:ss.zzz");


    int currow=curIndex.row(); //获得当前行
    jobtemModel->setData(jobtemModel->index(currow,jobtemModel->fieldIndex("pid")),jobtemModel->rowCount()); //自动生成编号
    jobtemModel->setData(jobtemModel->index(currow,jobtemModel->fieldIndex("code")),"ZZ"); //自动生成编号
    jobtemModel->setData(jobtemModel->index(currow,jobtemModel->fieldIndex("name")),"Example"); //自动生成编号
    jobtemModel->setData(jobtemModel->index(currow,jobtemModel->fieldIndex("dt0")),dtss); //自动生成编号
    jobtemModel->setData(jobtemModel->index(currow,jobtemModel->fieldIndex("mfgflag")),"0"); //自动生成编号
    jobtemModel->setData(jobtemModel->index(currow,jobtemModel->fieldIndex("uid")),"Z0001"); //自动生成编号
    jobtemModel->setData(jobtemModel->index(currow,jobtemModel->fieldIndex("dept")),"ZY1"); //自动生成编号
    jobtemModel->setData(jobtemModel->index(currow,jobtemModel->fieldIndex("part")),"ZX1"); //自动生成编号
    jobtemModel->setData(jobtemModel->index(currow,jobtemModel->fieldIndex("loc")),"ZZ1"); //自动生成编号
}
/*
* @projectName   WrenchApp
* @brief         删除记录
* @author        Li
* @date          2019-09-14
*/
void Jobtem::on_btnDelete_clicked()
{
    //删除当前记录
    ui->btnSave->setEnabled(true);
    ui->btnCancel->setEnabled(true);

    QModelIndex curIndex=theSelection->currentIndex();//获取当前选择单元格的模型索引
    jobtemModel->removeRow(curIndex.row()); //删除最后一行
}
/*
* @projectName   WrenchApp
* @brief         保存记录
* @author        Li
* @date          2019-09-14
*/
void Jobtem::on_btnSave_clicked()
{
    //保存修改
    bool res= jobtemModel->submitAll();

    if (!res)
        QMessageBox::information(this, tr("消息"), tr("数据保存错误,错误信息\n")+ jobtemModel->lastError().text(),
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
* @date          2019-09-14
*/
void Jobtem::on_btnCancel_clicked()
{
    //取消修改
    jobtemModel->revertAll();
    ui->btnSave->setEnabled(false);
    ui->btnCancel->setEnabled(false);
}
/*
* @projectName   WrenchApp
* @brief         设置照片
* @author        Li
* @date          2019-09-14
*/
void Jobtem::on_btnSetPhoto_clicked()
{
    QString aFile=QFileDialog::getOpenFileName(this,tr("选择图片文件"),"",tr("照片(*.jpg *.png)"));
    if (aFile.isEmpty())
        return;

    QByteArray data;
    QFile* file=new QFile(aFile); //fileName为二进制数据文件名
    file->open(QIODevice::ReadOnly);
    data = file->readAll();
    file->close();
    //QModelIndex curIndex=theSelection->currentIndex();//获取当前选择单元格的模型索引
    int curRecNo=theSelection->currentIndex().row();
    QSqlRecord  curRec=jobtemModel->record(curRecNo); //获取当前记录
    curRec.setValue("picpng",data); //设置字段数据
    jobtemModel->setRecord(curRecNo,curRec);
    //tabModel->submit(); //并不能保存到数据库

    QPixmap pic;
    pic.load(aFile);  //在界面上显示
    ui->labPhoto->setPixmap(pic.scaledToWidth(150));


}
/*
* @projectName   WrenchApp
* @brief         取消照片
* @author        Li
* @date          2019-09-14
*/
void Jobtem::on_btnCancelPhoto_clicked()
{
    int curRecNo=theSelection->currentIndex().row();
    QSqlRecord  curRec=jobtemModel->record(curRecNo); //获取当前记录
    curRec.setNull("picpng");//设置为空值
    jobtemModel->setRecord(curRecNo,curRec);
    ui->labPhoto->clear();
}
/*
* @projectName   WrenchApp
* @brief         拍照
* @author        Li
* @date          2019-09-14
*/
void Jobtem::on_pushButton_clicked()
{
    Camera* camera = new Camera(this);
    camera->setAttribute(Qt::WA_DeleteOnClose); //对话框关闭时自动删除对话框对象,用于不需要读取返回值的对话框
    camera->setWindowTitle(tr("拍照"));
    camera->show();
}
