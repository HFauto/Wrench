#include "newjob.h"
#include "ui_newjob.h"
#include "camera/camera.h"
#include <QMessageBox>
#include <QPixmap>
#include <QFileDialog>

Newjob::Newjob(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Newjob)
{
    ui->setupUi(this);

    newModel=new QSqlRelationalTableModel(this,DB);//数据表
    newjobModel=new QSqlRelationalTableModel(this,DB);//数据表

    newModel->setTable("jobdef"); //设置数据表
    newjobModel->setTable("jobtbl"); //设置数据表

    newjobModel->setEditStrategy(QSqlTableModel::OnManualSubmit);//数据保存方式，OnManualSubmit , OnRowChange
    newModel->setEditStrategy(QSqlTableModel::OnManualSubmit);//数据保存方式，OnManualSubmit , OnRowChange

    newjobModel->setSort(newjobModel->fieldIndex("jid"),Qt::AscendingOrder); //排序
    newModel->setSort(newModel->fieldIndex("pid"),Qt::AscendingOrder);


    if (!(newModel->select()))//查询数据
    {
        QMessageBox::critical(this,tr("错误信息"), tr("打开数据表错误,错误信息\n")+newModel->lastError().text(), QMessageBox::Ok,QMessageBox::NoButton);
        return;
    }

    if (!(newjobModel->select()))//查询数据
    {
        QMessageBox::critical(this,tr("错误信息"), tr("打开数据表错误,错误信息\n")+newjobModel->lastError().text(), QMessageBox::Ok,QMessageBox::NoButton);
        return;
    }


    //设置表头
    newModel->setHeaderData(newModel->fieldIndex("pid"),Qt::Horizontal,tr("端口号"));
    newModel->setHeaderData(newModel->fieldIndex("code"),Qt::Horizontal,tr("代码"));
    newModel->setHeaderData(newModel->fieldIndex("name"),Qt::Horizontal,tr("名称"));

    //设置表头
    newjobModel->setHeaderData(newjobModel->fieldIndex("jid"),Qt::Horizontal,tr("工单号"));
    newjobModel->setHeaderData(newjobModel->fieldIndex("pid"),Qt::Horizontal,tr("端口号"));
    newjobModel->setHeaderData(newjobModel->fieldIndex("code"),Qt::Horizontal,tr("代码"));
    newjobModel->setHeaderData(newjobModel->fieldIndex("name"),Qt::Horizontal,tr("工单名称"));
    newjobModel->setHeaderData(newjobModel->fieldIndex("seqno"),Qt::Horizontal,tr("工单代码"));
    newjobModel->setHeaderData(newjobModel->fieldIndex("dtss"),Qt::Horizontal,tr("开始时间"));
    newjobModel->setHeaderData(newjobModel->fieldIndex("dtee"),Qt::Horizontal,tr("结束时间"));
    newjobModel->setHeaderData(newjobModel->fieldIndex("uid"),Qt::Horizontal,tr("工人代号"));
    newjobModel->setHeaderData(newjobModel->fieldIndex("dept"),Qt::Horizontal,tr("部门代号"));
    newjobModel->setHeaderData(newjobModel->fieldIndex("part"),Qt::Horizontal,tr("部位代号"));
    newjobModel->setHeaderData(newjobModel->fieldIndex("loc"),Qt::Horizontal,tr("地点代号"));
    newjobModel->setHeaderData(newjobModel->fieldIndex("note"),Qt::Horizontal,tr("备注"));

    newjobModel->setHeaderData(newjobModel->fieldIndex("todos"),Qt::Horizontal,tr("最大扭矩"));
    newjobModel->setHeaderData(newjobModel->fieldIndex("oks"),Qt::Horizontal,tr("奥卡丝"));
    newjobModel->setHeaderData(newjobModel->fieldIndex("ngs"),Qt::Horizontal,tr("安全漏洞"));
    newjobModel->setHeaderData(newjobModel->fieldIndex("mfgflag"),Qt::Horizontal,tr("标志"));
    newjobModel->setHeaderData(newjobModel->fieldIndex("excel"),Qt::Horizontal,tr("表格"));

    newjobModel->setHeaderData(newjobModel->fieldIndex("picpng"),Qt::Horizontal,tr("照片"));

    //关联选择模型
    theSelection2=new QItemSelectionModel(newModel);
    theSelection=new QItemSelectionModel(newjobModel);

    connect(theSelection2,SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this,SLOT(on_currentRowChanged2(QModelIndex,QModelIndex)));

    //当前行变化时
    connect(theSelection2,SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this,SLOT(on_currentRowChanged2(QModelIndex,QModelIndex)));

    //theSelection当前项变化时触发currentChanged信号
    connect(theSelection,SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this,SLOT(on_currentChanged(QModelIndex,QModelIndex)));

    //选择行变化时
    connect(theSelection,SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this,SLOT(on_currentRowChanged(QModelIndex,QModelIndex)));


    ui->tableView2->setModel(newModel);//设置数据模型
    ui->tableView2->setSelectionModel(theSelection2);//设置选择模型

    ui->tableView2->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->tableView2->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView2->setEditTriggers(QAbstractItemView::NoEditTriggers);//不可编辑
    ui->tableView2->setAlternatingRowColors(true);//使用交替颜色绘制背景

    //隐藏其他字段
    for(int i = 3;i < 12;i++)
    {
        ui->tableView2->hideColumn(i);

    }

    ui->tableView->setModel(newjobModel);
    ui->tableView->setSelectionModel(theSelection);


    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setAlternatingRowColors(true);//使用交替颜色绘制背景
    theSelection->setCurrentIndex(newjobModel->index(0,0),QItemSelectionModel::Select);//默认选择第一行数据
    ui->tableView->setColumnHidden(newjobModel->fieldIndex("picpng"),true);//隐藏列

    //数据映射
    dataMapper= new QDataWidgetMapper();
    dataMapper->setModel(newModel);//设置数据模型
    dataMapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    dataMapper->addMapping(ui->line_pid,newModel->fieldIndex("pid"));
    dataMapper->addMapping(ui->line_code,newModel->fieldIndex("code"));
    dataMapper->addMapping(ui->line_name,newModel->fieldIndex("name"));
    dataMapper->addMapping(ui->line_dt,newModel->fieldIndex("dt0"));
    dataMapper->addMapping(ui->line_flag,newModel->fieldIndex("mfgflag"));
    dataMapper->addMapping(ui->line_uid,newModel->fieldIndex("uid"));
    dataMapper->addMapping(ui->line_dept,newModel->fieldIndex("dept"));
    dataMapper->addMapping(ui->line_part,newModel->fieldIndex("part"));
    dataMapper->addMapping(ui->line_loc,newModel->fieldIndex("loc"));
    dataMapper->addMapping(ui->text_note,newModel->fieldIndex("note"));
    dataMapper->toFirst();//移动到首记录
    theSelection2->setCurrentIndex(newModel->index(0,0),QItemSelectionModel::Select);//默认选择第一行数据

}

Newjob::~Newjob()
{
    delete ui;
}
/*
* @projectName   WrenchApp
* @brief         当前项变化
* @author        Li
* @date          2019-09-15
*/
void Newjob::on_currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(current)
    Q_UNUSED(previous)
    ui->btnSave->setEnabled(newjobModel->isDirty()); //有未保存修改时可用
    ui->btnCancel->setEnabled(newjobModel->isDirty());

}
/*
* @projectName   WrenchApp
* @brief         当前行变化
* @author        Li
* @date          2019-09-15
*/
void Newjob::on_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous)
    //行切换时的状态控制
    ui->btnDelete->setEnabled(current.isValid());
    ui->btnSetPhoto->setEnabled(current.isValid());

    if (!current.isValid())
    {
        ui->labPhoto->clear(); //清除图片显示
        return;
    }


    //dataMapper->setCurrentIndex(current.row()); //更新数据映射的行号
    //QModelIndex curIndex=theSelection->currentIndex();//获取当前选择单元格的模型索引
    int curRecNo=current.row();//获取行号
    QSqlRecord  curRec=newjobModel->record(curRecNo); //获取当前记录

    if (curRec.isNull("picpng"))  //图片字段内容为空
    {
        ui->labPhoto->clear();
        ui->btnCancelPhoto->setEnabled(false);
    }
    else
    {
        ui->btnCancelPhoto->setEnabled(true);
        QByteArray data=curRec.value("picpng").toByteArray();
        QPixmap pic;
        pic.loadFromData(data);
        ui->labPhoto->setPixmap(pic.scaledToWidth(150));//设置图片大小
    }

}
/*
* @projectName   WrenchApp
* @brief         获取图片数据
* @author        Li
* @date          2019-09-16
*/
QByteArray Newjob::data(QByteArray datae)
{
    static QByteArray data;
    if(datae!=nullptr)
        data = datae;
    return data;
}
/*
* @projectName   WrenchApp
* @brief         模板当前项变化
* @author        Li
* @date          2019-09-15
*/
void Newjob::on_currentRowChanged2(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous)

    if (!current.isValid())
    {
        ui->lab_pic->clear(); //清除图片显示
        return;
    }
    dataMapper->setCurrentModelIndex(current); //更新数据映射的行号

    //QModelIndex curIndex=theSelection->currentIndex();//获取当前选择单元格的模型索引
    int curRecNo=current.row();//获取行号
    QSqlRecord  curRec=newModel->record(curRecNo); //获取当前记录

    if (curRec.isNull("picpng"))  //图片字段内容为空
    {
        ui->lab_pic->clear();
    }
    else
    {
        QByteArray data=curRec.value("picpng").toByteArray();
        Newjob::data(data);//获取图片数据
        QPixmap pic;
        pic.loadFromData(data);
        ui->lab_pic->setPixmap(pic.scaledToWidth(150));//设置图片大小
    }

}
/*
* @projectName   WrenchApp
* @brief         新建工单
* @author        Li
* @date          2019-09-15
*/
void Newjob::on_btnAdd_clicked()
{
    QString pid =ui->line_pid->text();
    QString code =ui->line_code->text();
    QString name =ui->line_name->text();
    QString dt0 =ui->line_dt->text();
    QString mfg =ui->line_flag->text();
    QString uid =ui->line_uid->text();
    QString dept =ui->line_dept->text();
    QString part =ui->line_part->text();
    QString loc =ui->line_loc->text();
    QString note =ui->text_note->toPlainText();
    const QPixmap *pic = ui->lab_pic->pixmap();
    QByteArray data = nullptr;
    //    const QPixmap *exc = ui->lab_ex->pixmap();
    QDateTime curDateTime=QDateTime::currentDateTime(); //读取当前日期时间

    QString seqno =curDateTime.toString( code + "yyyyMMdd001");
    QString dtss =curDateTime.toString("yyyy-MM-dd HH:mm:ss.zzz");


    newjobModel->insertRow(newjobModel->rowCount(),QModelIndex()); //在末尾添加一个记录
    QModelIndex curIndex=newjobModel->index(newjobModel->rowCount()-1,1);//创建最后一行的ModelIndex
    theSelection->clearSelection();//清空选择项
    theSelection->setCurrentIndex(curIndex,QItemSelectionModel::Select);//设置刚插入的行为当前选择行
    int currow=curIndex.row(); //获得当前行
    newjobModel->setData(newjobModel->index(currow,newjobModel->fieldIndex("jid")),newjobModel->rowCount()); //自动生成编号
    newjobModel->setData(newjobModel->index(currow,newjobModel->fieldIndex("pid")),pid); //自动生成编号
    newjobModel->setData(newjobModel->index(currow,newjobModel->fieldIndex("code")),code); //自动生成编号
    newjobModel->setData(newjobModel->index(currow,newjobModel->fieldIndex("name")),name); //自动生成编号
    newjobModel->setData(newjobModel->index(currow,newjobModel->fieldIndex("seqno")),seqno); //自动生成编号
    newjobModel->setData(newjobModel->index(currow,newjobModel->fieldIndex("dtss")),dtss); //自动生成编号
    newjobModel->setData(newjobModel->index(currow,newjobModel->fieldIndex("mfgflag")),mfg); //自动生成编号
    newjobModel->setData(newjobModel->index(currow,newjobModel->fieldIndex("uid")),uid); //自动生成编号
    newjobModel->setData(newjobModel->index(currow,newjobModel->fieldIndex("dept")),dept); //自动生成编号
    newjobModel->setData(newjobModel->index(currow,newjobModel->fieldIndex("part")),part); //自动生成编号
    newjobModel->setData(newjobModel->index(currow,newjobModel->fieldIndex("loc")),loc); //自动生成编号
    newjobModel->setData(newjobModel->index(currow,newjobModel->fieldIndex("note")),note); //自动生成编号
    newjobModel->setData(newjobModel->index(currow,newjobModel->fieldIndex("picpng")),Newjob::data(data));

    //    newjobModel->setData(newjobModel->index(currow,newjobModel->fieldIndex("excel")),exc); //自动生成编号
    ui->labPhoto->setPixmap(pic->scaledToWidth(150));//设置图片大小

}
/*
* @projectName   WrenchApp
* @brief         删除数据
* @author        Li
* @date          2019-09-15
*/
void Newjob::on_btnDelete_clicked()
{
    //删除当前记录
    ui->btnSave->setEnabled(true);
    ui->btnCancel->setEnabled(true);

    QModelIndex curIndex=theSelection->currentIndex();//获取当前选择单元格的模型索引
    newjobModel->removeRow(curIndex.row()); //删除最后一行
}
/*
* @projectName   WrenchApp
* @brief         保存
* @author        Li
* @date          2019-09-15
*/
void Newjob::on_btnSave_clicked()
{
    //保存修改
    bool res= newjobModel->submitAll();

    if (!res)
        QMessageBox::information(this, tr("消息"), tr("数据保存错误,错误信息\n")+ newjobModel->lastError().text(),
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
* @date          2019-09-15
*/
void Newjob::on_btnCancel_clicked()
{
    //取消修改
    newjobModel->revertAll();
    ui->btnSave->setEnabled(false);
    ui->btnCancel->setEnabled(false);
}
/*
* @projectName   WrenchApp
* @brief         设置照片
* @author        Li
* @date          2019-09-15
*/
void Newjob::on_btnSetPhoto_clicked()
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
    QSqlRecord  curRec=newjobModel->record(curRecNo); //获取当前记录
    curRec.setValue("picpng",data); //设置字段数据
    newjobModel->setRecord(curRecNo,curRec);
    //tabModel->submit(); //并不能保存到数据库

    QPixmap pic;
    pic.load(aFile);  //在界面上显示
    ui->labPhoto->setPixmap(pic.scaledToWidth(150));
}
/*
* @projectName   WrenchApp
* @brief         删除照片
* @author        Li
* @date          2019-09-15
*/
void Newjob::on_btnCancelPhoto_clicked()
{
    int curRecNo=theSelection->currentIndex().row();
    QSqlRecord  curRec=newjobModel->record(curRecNo); //获取当前记录
    curRec.setNull("picpng");//设置为空值
    newjobModel->setRecord(curRecNo,curRec);
    ui->labPhoto->clear();
}
/*
* @projectName   WrenchApp
* @brief         拍照
* @author        Li
* @date          2019-09-15
*/
void Newjob::on_btnPhoto_clicked()
{
    Camera* camera = new Camera(this);
    camera->setAttribute(Qt::WA_DeleteOnClose); //对话框关闭时自动删除对话框对象,用于不需要读取返回值的对话框
    camera->setWindowTitle(tr("拍照"));
    camera->show();
}
