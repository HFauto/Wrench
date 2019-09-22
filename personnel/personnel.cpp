#include "personnel.h"
#include "ui_personnel.h"
#include <QMessageBox>
#include <QFileDialog>
#include "camera/camera.h"
#define cout qDebug() << "[" <<__FILE__ <<":"<<__FUNCTION__<<":"<<__LINE__ <<"]"



Personnel::Personnel(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Personnel)
{
    ui->setupUi(this);

    perModel=new QSqlRelationalTableModel(this,DB);//数据表
    perModel2=new QSqlRelationalTableModel(this,DB);//数据表

    perModel->setTable("worker"); //设置数据表
    perModel2->setTable("workerTool"); //设置数据表

    //数据保存方式，OnManualSubmit点击保存按钮保存记录, OnRowChange
    perModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    //数据保存方式，OnManualSubmit点击保存按钮保存记录, OnRowChange
    perModel2->setEditStrategy(QSqlTableModel::OnManualSubmit);

    perModel->setSort(perModel->fieldIndex("uid"),Qt::AscendingOrder); //排序
    perModel2->setSort(perModel->fieldIndex("wid"),Qt::AscendingOrder); //排序
    if (!(perModel->select()))//查询数据
    {
        QMessageBox::critical(this,tr("错误信息"), tr("打开数据表错误,错误信息\n")+perModel->lastError().text(), QMessageBox::Ok,QMessageBox::NoButton);
        return;
    }
    //设置表头
    perModel->setHeaderData(perModel->fieldIndex("rid"),Qt::Horizontal,tr("行号"));
    perModel->setHeaderData(perModel->fieldIndex("uid"),Qt::Horizontal,tr("工号"));
    perModel->setHeaderData(perModel->fieldIndex("workerName"),Qt::Horizontal,tr("姓名"));
    perModel->setHeaderData(perModel->fieldIndex("departMent"),Qt::Horizontal,tr("部门"));
    perModel->setHeaderData(perModel->fieldIndex("position"),Qt::Horizontal,tr("职位"));
    perModel->setHeaderData(perModel->fieldIndex("registration"),Qt::Horizontal,tr("注册时间"));
    perModel->setHeaderData(perModel->fieldIndex("phone"),Qt::Horizontal,tr("电话"));
    perModel->setHeaderData(perModel->fieldIndex("IDNumber"),Qt::Horizontal,tr("身份证号"));
    perModel->setHeaderData(perModel->fieldIndex("address"),Qt::Horizontal,tr("家庭住址"));
    perModel->setHeaderData(perModel->fieldIndex("note"),Qt::Horizontal,tr("备注"));
    perModel->setHeaderData(perModel->fieldIndex("photo"),Qt::Horizontal,tr("照片"));


    perModel2->setHeaderData(perModel2->fieldIndex("wid"),Qt::Horizontal,tr("姓名代号"));
    perModel2->setHeaderData(perModel2->fieldIndex("tid"),Qt::Horizontal,tr("工具代号"));
    perModel2->setHeaderData(perModel2->fieldIndex("dts"),Qt::Horizontal,tr("开始使用时间"));
    perModel2->setHeaderData(perModel2->fieldIndex("dte"),Qt::Horizontal,tr("还回时间"));


    //关联选择模型
    theSelection=new QItemSelectionModel(perModel);
    theSelection2=new QItemSelectionModel(perModel2);

    //theSelection当前项变化时触发currentChanged信号
    connect(theSelection,SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this,SLOT(on_currentChanged(QModelIndex,QModelIndex)));

    connect(theSelection2,SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this,SLOT(on_currentChanged(QModelIndex,QModelIndex)));
    //选择行变化时
    connect(theSelection,SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this,SLOT(on_currentRowChanged(QModelIndex,QModelIndex)));

    connect(theSelection2,SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this,SLOT(on_currentRowChanged(QModelIndex,QModelIndex)));


    ui->tableView->setModel(perModel);//设置数据模型
    ui->tableView->setSelectionModel(theSelection);//设置选择模型
    ui->tableView->setColumnHidden(perModel->fieldIndex("photo"),true);//隐藏列
    //ui->tableView->setColumnHidden(perModel->fieldIndex("note"),true);//隐藏列


    ui->tableView2->setModel(perModel2);//设置数据模型
    ui->tableView2->setSelectionModel(theSelection2);//设置选择模型

//    ui->tableView2->setItemDelegate(new QSqlRelationalDelegate(ui->tableView2)); //为关系型字段设置缺省代理组件

    //tableView显示属性设置
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setAlternatingRowColors(true);//使用交替颜色绘制背景
    theSelection->setCurrentIndex(perModel->index(0,0),QItemSelectionModel::Select);//默认选择第一行数据

    ui->tableView2->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->tableView2->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView2->setAlternatingRowColors(true);//使用交替颜色绘制背景

    //关联数据表   使用关系数据表不能保存新的数据
//    perModel2->setRelation(perModel2->fieldIndex("wid"),QSqlRelation("worker","uid","workerName"));//工人
//    perModel2->setRelation(perModel2->fieldIndex("tid"),QSqlRelation("tool","tid","toolName"));//工具

    //comboBox组件显示数据库内容


    //创建界面组件与数据模型的字段之间的数据映射
//    dataMapper= new QDataWidgetMapper();
//    dataMapper->setModel(perModel2);//设置数据模型
//    dataMapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);//


    //界面组件与tabModel的具体字段之间的联系
    //dataMapper->setItemDelegate(new QSqlRelationalDelegate(this)); //含有外键的
//    dataMapper->addMapping(ui->lineEdit,perModel->fieldIndex("uid"));
//    dataMapper->addMapping(ui->workerName,perModel->fieldIndex("workerName"));
//    dataMapper->addMapping(ui->departMent,perModel->fieldIndex("departMent"));
//    dataMapper->addMapping(ui->position,perModel->fieldIndex("position"));
//    dataMapper->addMapping(ui->registration,perModel->fieldIndex("registration"));
//    dataMapper->addMapping(ui->phone,perModel->fieldIndex("phone"));
//    dataMapper->addMapping(ui->IDNumber,perModel->fieldIndex("IDNumber"));
//    dataMapper->addMapping(ui->address,perModel->fieldIndex("address"));
//    dataMapper->addMapping(ui->lineEdit,perModel2->fieldIndex("wid"));
//    dataMapper->toFirst();//移动到首记录
    fillComboBox();

}

Personnel::~Personnel()
{
    delete ui;
}
/*
* @projectName   WrenchApp
* @brief         当前项变化
* @author        Li
* @date          2019-09-03
*/
void Personnel::on_currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(current)
    Q_UNUSED(previous)
    ui->pushButton_4->setEnabled(perModel->isDirty()); //有未保存修改时可用
    ui->pushButton_5->setEnabled(perModel->isDirty());

    ui->btnSave->setEnabled(perModel2->isDirty()); //有未保存修改时可用
    ui->btnCancel->setEnabled(perModel2->isDirty());

    //QString startTime =ui->start->dateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");
    //ui->tableView2->setItemDelegateForColumn(5,"yyyy-MM-dd HH:mm:ss.zzz");
}
/*
* @projectName   WrenchApp
* @brief         选择行变化
* @author        Li
* @date          2019-09-03
*/
void Personnel::on_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous)
    //行切换时的状态控制
    ui->pushButton_3->setEnabled(current.isValid());
    ui->pushButton_6->setEnabled(current.isValid());
    ui->pushButton_7->setEnabled(current.isValid());

    if (!current.isValid())
    {
        ui->photo->clear(); //清除图片显示
        return;
    }

   //dataMapper->setCurrentIndex(current.row()); //更新数据映射的行号

    //QModelIndex curIndex=theSelection->currentIndex();//获取当前选择单元格的模型索引
    int curRecNo=current.row();//获取行号
    QSqlRecord  curRec=perModel->record(curRecNo); //获取当前记录

    if (curRec.isNull("photo"))  //图片字段内容为空
        ui->photo->clear();
    else
    {
        QByteArray data=curRec.value("photo").toByteArray();
        QPixmap pic;
        pic.loadFromData(data);
        ui->photo->setPixmap(pic.scaledToWidth(150));//设置图片大小
    }

}
/*
* @projectName   WrenchApp
* @brief         添加记录
* @author        Li
* @date          2019-09-01
*/
void Personnel::on_pushButton_clicked()
{
    perModel->insertRow(perModel->rowCount(),QModelIndex()); //在末尾添加一个记录
    QModelIndex curIndex=perModel->index(perModel->rowCount()-1,1);//创建最后一行的ModelIndex
    theSelection->clearSelection();//清空选择项
    theSelection->setCurrentIndex(curIndex,QItemSelectionModel::Select);//设置刚插入的行为当前选择行
    int currow=curIndex.row(); //获得当前行
    perModel->setData(perModel->index(currow,0),perModel->rowCount()); //自动生成编号
}
/*
* @projectName   WrenchApp
* @brief         插入记录
* @author        Li
* @date          2019-09-01
*/
void Personnel::on_pushButton_2_clicked()
{
    QModelIndex curIndex=ui->tableView->currentIndex();
    perModel->insertRow(curIndex.row(),QModelIndex());
    theSelection->clearSelection();//清除已有选择
    theSelection->setCurrentIndex(curIndex,QItemSelectionModel::Select);
}
/*
* @projectName   WrenchApp
* @brief         删除记录
* @author        Li
* @date          2019-09-01
*/
void Personnel::on_pushButton_3_clicked()
{
    //删除当前记录
    ui->pushButton_4->setEnabled(true);
    ui->pushButton_5->setEnabled(true);

    QModelIndex curIndex=theSelection->currentIndex();//获取当前选择单元格的模型索引
    perModel->removeRow(curIndex.row()); //删除最后一行
}
/*
* @projectName   WrenchApp
* @brief         保存记录
* @author        Li
* @date          2019-09-01
*/
void Personnel::on_pushButton_4_clicked()
{
    //保存修改
    bool res= perModel->submitAll();

    if (!res)
        QMessageBox::information(this, tr("消息"), tr("数据保存错误,错误信息\n")+ perModel->lastError().text(),
                                 QMessageBox::Ok,QMessageBox::NoButton);
    else
    {
        ui->pushButton_4->setEnabled(false);
        ui->pushButton_5->setEnabled(false);
    }
    ui->comboBox->clear();
    fillComboBox();

}
/*
* @projectName   WrenchApp
* @brief         取消修改
* @author        Li
* @date          2019-09-01
*/
void Personnel::on_pushButton_5_clicked()
{
    //取消修改
    perModel->revertAll();
    ui->pushButton_4->setEnabled(false);
    ui->pushButton_5->setEnabled(false);
}
/*
* @projectName   WrenchApp
* @brief         设置照片
* @author        Li
* @date          2019-09-01
*/
void Personnel::on_pushButton_6_clicked()
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
    QSqlRecord  curRec=perModel->record(curRecNo); //获取当前记录
    curRec.setValue("photo",data); //设置字段数据
    perModel->setRecord(curRecNo,curRec);
    //tabModel->submit(); //并不能保存到数据库

    QPixmap pic;
    pic.load(aFile);  //在界面上显示
    ui->photo->setPixmap(pic.scaledToWidth(150));
}
/*
* @projectName   WrenchApp
* @brief         删除照片
* @author        Li
* @date          2019-09-01
*/
void Personnel::on_pushButton_7_clicked()
{
    int curRecNo=theSelection->currentIndex().row();
    QSqlRecord  curRec=perModel->record(curRecNo); //获取当前记录
    curRec.setNull("photo");//设置为空值
    perModel->setRecord(curRecNo,curRec);
    ui->photo->clear();
}
/*
* @projectName   WrenchApp
* @brief         填充comboBox
* @author        Li
* @date          2019-09-03
*/
void  Personnel::fillComboBox()
{
    QSqlQuery *sqlQuery = new QSqlQuery;
    sqlQuery->exec("select uid from worker");
    while(sqlQuery->next())
    {
        QString text = sqlQuery->value(0).toString();
        ui->comboBox->addItem(text);
    }
    delete sqlQuery;

}
/*
* @projectName   WrenchApp
* @brief         workerTool添加记录
* @author        Li
* @date          2019-09-03
*/
void Personnel::on_btnAdd_clicked()
{
    perModel2->insertRow(perModel2->rowCount(),QModelIndex()); //在末尾添加一个记录
    QModelIndex curIndex=perModel2->index(perModel2->rowCount()-1,1);//创建最后一行的ModelIndex
    theSelection2->clearSelection();//清空选择项
    theSelection2->setCurrentIndex(curIndex,QItemSelectionModel::Select);//设置刚插入的行为当前选择行
    int currow=curIndex.row(); //获得当前行
    perModel2->setData(perModel2->index(currow,0),ui->comboBox->currentText()); //自动生成工人代号
}
/*
* @projectName   WrenchApp
* @brief         workerTool删除记录
* @author        Li
* @date          2019-09-04
*/
void Personnel::on_btnDelete_clicked()
{
    //删除当前记录
    ui->btnSave->setEnabled(true);
    ui->btnCancel->setEnabled(true);

    QModelIndex curIndex=theSelection2->currentIndex();//获取当前选择单元格的模型索引
    perModel2->removeRow(curIndex.row()); //删除最后一行
}
/*
* @projectName   WrenchApp
* @brief         workerTool保存记录
* @author        Li
* @date          2019-09-04
*/
void Personnel::on_btnSave_clicked()
{
    bool res= perModel2->submitAll();

    if (!res)
        QMessageBox::information(this, tr("消息"), tr("数据保存错误,错误信息\n")+ perModel2->lastError().text(),
                                 QMessageBox::Ok,QMessageBox::NoButton);
    else
    {
        ui->btnSave->setEnabled(false);
        ui->btnCancel->setEnabled(false);
    }
}
/*
* @projectName   WrenchApp
* @brief         workerTool取消保存
* @author        Li
* @date          2019-09-04
*/
void Personnel::on_btnCancel_clicked()
{
    perModel2->revertAll();
    ui->btnSave->setEnabled(false);
    ui->btnCancel->setEnabled(false);
}
/*
* @projectName   WrenchApp
* @brief         过滤comboBox选中的内容
* @author        Li
* @date          2019-09-04
*/
void Personnel::on_comboBox_currentIndexChanged(int index)
{
    Q_UNUSED(index)
    QString name =ui->comboBox->currentText();
    QString str = QString(" wid='%1' ").arg(name);

    perModel2->setFilter(str);
    perModel2->select();
}
/*
* @projectName   WrenchApp
* @brief         调用摄像头
* @author        Li
* @date          2019-09-07
*/
void Personnel::on_pushButton_8_clicked()
{
    Camera* camera = new Camera(this);
    camera->setAttribute(Qt::WA_DeleteOnClose); //对话框关闭时自动删除对话框对象,用于不需要读取返回值的对话框
    camera->setWindowTitle(tr("拍照"));
    camera->show();
}
