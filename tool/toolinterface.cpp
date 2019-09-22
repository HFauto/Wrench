#include "toolinterface.h"
#include "ui_toolinterface.h"
#include <QMessageBox>
#include <QFileDialog>
#include "camera/camera.h"

ToolInterface::ToolInterface(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ToolInterface)
{
    ui->setupUi(this);

    toolModel=new QSqlRelationalTableModel(this,DB);//数据表

    toolModel->setTable("tool"); //设置数据表

    //数据保存方式，OnManualSubmit点击保存按钮保存记录, OnRowChange
    toolModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    toolModel->setSort(toolModel->fieldIndex("rid"),Qt::AscendingOrder); //排序
    if (!(toolModel->select()))//查询数据
    {
        QMessageBox::critical(this,tr("错误信息"), tr("打开数据表错误,错误信息\n")+toolModel->lastError().text(), QMessageBox::Ok,QMessageBox::NoButton);
        return;
    }
    //设置表头
    toolModel->setHeaderData(toolModel->fieldIndex("rid"),Qt::Horizontal,tr("行号"));
    toolModel->setHeaderData(toolModel->fieldIndex("tid"),Qt::Horizontal,tr("工具代码"));
    toolModel->setHeaderData(toolModel->fieldIndex("toolName"),Qt::Horizontal,tr("工具名"));
    toolModel->setHeaderData(toolModel->fieldIndex("dts"),Qt::Horizontal,tr("开始时间"));
    toolModel->setHeaderData(toolModel->fieldIndex("dte"),Qt::Horizontal,tr("结束时间"));
    toolModel->setHeaderData(toolModel->fieldIndex("dtc"),Qt::Horizontal,tr("校正时间"));
    toolModel->setHeaderData(toolModel->fieldIndex("type"),Qt::Horizontal,tr("工具类型"));
    toolModel->setHeaderData(toolModel->fieldIndex("direction"),Qt::Horizontal,tr("角度"));
    toolModel->setHeaderData(toolModel->fieldIndex("fixpoint"),Qt::Horizontal,tr("小数位数"));
    toolModel->setHeaderData(toolModel->fieldIndex("mintorq"),Qt::Horizontal,tr("最小扭矩"));
    toolModel->setHeaderData(toolModel->fieldIndex("maxtorq"),Qt::Horizontal,tr("最大扭矩"));
    toolModel->setHeaderData(toolModel->fieldIndex("note"),Qt::Horizontal,tr("备注"));
    toolModel->setHeaderData(toolModel->fieldIndex("picpng"),Qt::Horizontal,tr("照片"));

    //关联选择模型
    theSelection=new QItemSelectionModel(toolModel);

    //theSelection当前项变化时触发currentChanged信号
    connect(theSelection,SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this,SLOT(on_currentChanged(QModelIndex,QModelIndex)));

    //选择行变化时
    connect(theSelection,SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this,SLOT(on_currentRowChanged(QModelIndex,QModelIndex)));


    ui->tableView->setModel(toolModel);//设置数据模型
    ui->tableView->setSelectionModel(theSelection);//设置选择模型
    ui->tableView->setColumnHidden(toolModel->fieldIndex("picpng"),true);//隐藏列
    //ui->tableView->setColumnHidden(perModel->fieldIndex("note"),true);//隐藏列
    //ui->tableView->setItemDelegate(new QSqlRelationalDelegate(ui->tableView)); //为关系型字段设置缺省代理组件


    //tableView显示属性设置
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setAlternatingRowColors(true);//使用交替颜色绘制背景
    theSelection->setCurrentIndex(toolModel->index(0,0),QItemSelectionModel::Select);//默认选择第一行数据

}

ToolInterface::~ToolInterface()
{
    delete ui;
}
/*
* @projectName   WrenchApp
* @brief         当前项变化
* @author        Li
* @date          2019-09-03
*/
void ToolInterface::on_currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(current)
    Q_UNUSED(previous)
    ui->btnSave->setEnabled(toolModel->isDirty()); //有未保存修改时可用
    ui->btnCancel->setEnabled(toolModel->isDirty());

}
/*
* @projectName   WrenchApp
* @brief         选择行变化
* @author        Li
* @date          2019-09-03
*/
void ToolInterface::on_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
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
    QSqlRecord  curRec=toolModel->record(curRecNo); //获取当前记录

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
* @date          2019-09-04
*/
void ToolInterface::on_btnAdd_clicked()
{
    toolModel->insertRow(toolModel->rowCount(),QModelIndex()); //在末尾添加一个记录
    QModelIndex curIndex=toolModel->index(toolModel->rowCount()-1,1);//创建最后一行的ModelIndex
    theSelection->clearSelection();//清空选择项
    theSelection->setCurrentIndex(curIndex,QItemSelectionModel::Select);//设置刚插入的行为当前选择行
    int currow=curIndex.row(); //获得当前行
    toolModel->setData(toolModel->index(currow,0),toolModel->rowCount()); //自动生成编号
}
/*
* @projectName   WrenchApp
* @brief         插入记录
* @author        Li
* @date          2019-09-04
*/
void ToolInterface::on_btnInsert_clicked()
{
    QModelIndex curIndex=ui->tableView->currentIndex();
    toolModel->insertRow(curIndex.row(),QModelIndex());
    theSelection->clearSelection();//清除已有选择
    theSelection->setCurrentIndex(curIndex,QItemSelectionModel::Select);
}
/*
* @projectName   WrenchApp
* @brief         删除记录
* @author        Li
* @date          2019-09-04
*/
void ToolInterface::on_btnDelete_clicked()
{
    //删除当前记录
    ui->btnSave->setEnabled(true);
    ui->btnCancel->setEnabled(true);

    QModelIndex curIndex=theSelection->currentIndex();//获取当前选择单元格的模型索引
    toolModel->removeRow(curIndex.row()); //删除最后一行
}
/*
* @projectName   WrenchApp
* @brief         保存记录
* @author        Li
* @date          2019-09-04
*/
void ToolInterface::on_btnSave_clicked()
{
    //保存修改
    bool res= toolModel->submitAll();

    if (!res)
        QMessageBox::information(this, tr("消息"), tr("数据保存错误,错误信息\n")+ toolModel->lastError().text(),
                                 QMessageBox::Ok,QMessageBox::NoButton);
    else
    {
        ui->btnSave->setEnabled(false);
        ui->btnCancel->setEnabled(false);
    }
}
/*
* @projectName   WrenchApp
* @brief         取消保存
* @author        Li
* @date          2019-09-04
*/
void ToolInterface::on_btnCancel_clicked()
{
    //取消修改
    toolModel->revertAll();
    ui->btnSave->setEnabled(false);
    ui->btnCancel->setEnabled(false);
}
/*
* @projectName   WrenchApp
* @brief         保存照片
* @author        Li
* @date          2019-09-04
*/
void ToolInterface::on_btnSetPhoto_clicked()
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
    QSqlRecord  curRec=toolModel->record(curRecNo); //获取当前记录
    curRec.setValue("picpng",data); //设置字段数据
    toolModel->setRecord(curRecNo,curRec);
    //tabModel->submit(); //并不能保存到数据库

    QPixmap pic;
    pic.load(aFile);  //在界面上显示
    ui->labPhoto->setPixmap(pic.scaledToWidth(ui->labPhoto->width()));
}
/*
* @projectName   WrenchApp
* @brief         删除照片
* @author        Li
* @date          2019-09-04
*/
void ToolInterface::on_btnCancelPhoto_clicked()
{
    int curRecNo=theSelection->currentIndex().row();
    QSqlRecord  curRec=toolModel->record(curRecNo); //获取当前记录
    curRec.setNull("picpng");//设置为空值
    toolModel->setRecord(curRecNo,curRec);
    ui->labPhoto->clear();
}
/*
* @projectName   WrenchApp
* @brief         拍照
* @author        Li
* @date          2019-09-08
*/
void ToolInterface::on_pushButton_clicked()
{
    Camera* camera = new Camera(this);
    camera->setAttribute(Qt::WA_DeleteOnClose); //对话框关闭时自动删除对话框对象,用于不需要读取返回值的对话框
    camera->setWindowTitle(tr("拍照"));
    camera->show();
}
