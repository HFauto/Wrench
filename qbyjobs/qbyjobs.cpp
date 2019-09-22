#include "qbyjobs.h"
#include "ui_qbyjobs.h"
#include <QMessageBox>
#include <QPainter>
#include <QPrinter>
#include <QPrintDialog>
#include <QFileDialog>
#include <QPrintPreviewDialog>
#include <QDebug>


#define cout qDebug() << "[" <<__FILE__ <<":"<<__FUNCTION__<<":"<<__LINE__ <<"]"



QByJobs::QByJobs(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QByJobs)
{
    ui->setupUi(this);
    QDateTime curDateTime=QDateTime::currentDateTime(); //读取当前日期时间
    ui->start->setDateTime(curDateTime);//设置日期时间
    ui->end->setDateTime(curDateTime);
    //隐藏保存和打印按钮
    ui->pushButton_2->setVisible(false);
    ui->pushButton_3->setVisible(false);
}

QByJobs::~QByJobs()
{
    delete ui;
}
/*
*@projectName   WrenchApp
*@brief         查询按钮
*@auther        Li
*@date          2019-08-28
*/
void QByJobs::on_pushButton_clicked()
{
    qryModel=new QSqlRelationalTableModel(this,DB);//数据表
    qryModel->setTable("jobtbl"); //设置数据表
    qryModel->setEditStrategy(QSqlTableModel::OnRowChange);//数据保存方式，OnManualSubmit , OnRowChange
    qryModel->setSort(qryModel->fieldIndex("jid"),Qt::AscendingOrder); //排序

    //设置表头

    qryModel->setHeaderData(qryModel->fieldIndex("code"),Qt::Horizontal,tr("工单号"));
    qryModel->setHeaderData(qryModel->fieldIndex("name"),Qt::Horizontal,tr("名称"));
    qryModel->setHeaderData(qryModel->fieldIndex("seqno"),Qt::Horizontal,tr("扳手"));
    qryModel->setHeaderData(qryModel->fieldIndex("dtss"),Qt::Horizontal,tr("开始时间"));
    qryModel->setHeaderData(qryModel->fieldIndex("dtee"),Qt::Horizontal,tr("结束时间"));
    qryModel->setHeaderData(qryModel->fieldIndex("uid"),Qt::Horizontal,tr("姓名"));

    //关联数据表  不能有缺省值
    qryModel->setRelation(qryModel->fieldIndex("uid"),QSqlRelation("worker","uid","workerName"));//工具

    theSelection=new QItemSelectionModel(qryModel);
    ui->tableView->setModel(qryModel);
    ui->tableView->setSelectionModel(theSelection);
    ui->tableView->setItemDelegate(new QSqlRelationalDelegate(ui->tableView)); //为关系型字段设置缺省代理组件
    if (!(qryModel->select()))//查询数据
    {
        QMessageBox::critical(this,tr("错误信息"), tr("打开数据表错误,错误信息\n")+qryModel->lastError().text(), QMessageBox::Ok,QMessageBox::NoButton);
        return;
    }

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//不可编辑
    ui->tableView->setAlternatingRowColors(true);//使用交替颜色绘制背景
    //隐藏其他字段
    //ui->tableView->setColumnHidden(tabModel->fieldIndex("Memo"),true);//隐藏列
    for(int i = 0;i < 2;i++)
    {
        ui->tableView->hideColumn(i);

    }
    for(int j = 8;j < 18;j++)
    {
        ui->tableView->hideColumn(j);
    }
    //获取内容    注意数据表不能取相同的名字，否则查不到

    QString name =ui->comboBox->currentText();
    QString tool =ui->toolBox->currentText();
    QString startTime =ui->start->dateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");
    QString endTime =ui->end->dateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");
    QString worker =ui->workBox->currentText();


    QString str = QString("code='%1'and seqno='%2' and dtss<'%3' and dtee>'%4' and  workerName='%5'").arg(name).arg(tool)
            .arg(startTime).arg(endTime).arg(worker);
    //QString str = QString(" workerName='%5' ").arg(worker);

    qryModel->setFilter(str);
    qryModel->select();
    //当数据表有数据才显示保存和打印按钮
    if(ui->tableView->verticalHeader()->count())
    {

        ui->pushButton_2->setVisible(true);
        ui->pushButton_3->setVisible(true);
    }
    else
    {

        ui->pushButton_2->setVisible(false);
        ui->pushButton_3->setVisible(false);
    }
}
/*
*@projectName   WrenchApp
*@brief         保存按钮
*@auther        Li
*@date          2019-08-28
*/
void QByJobs::on_pushButton_2_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("保存PDF文件"), QString(), "*.pdf");
    if (!fileName.isEmpty())
    {
        // 如果文件后缀为空，则默认使用.pdf
        if (QFileInfo(fileName).suffix().isEmpty())
        {
            fileName.append(".pdf");
        }
        QPrinter printerPixmap(QPrinter::HighResolution);
        //自定义纸张大小，这里要打印的内容都在stackedWidget上
        printerPixmap.setPageSize(QPrinter::Custom);
        printerPixmap.setPaperSize(QSizeF(ui->tableView->height(), ui->tableView->width()), QPrinter::Point);
        //设置纸张大小为A4，这里注释掉了，建议自定义纸张 ，否则保存的就会有很多空白
        //printerPixmap.setPageSize(QPrinter::A4);
        //横向打印
        printerPixmap.setOrientation(QPrinter::Landscape);
        //设置输出格式为pdf
        printerPixmap.setOutputFormat(QPrinter::PdfFormat);
        //设置输出路径
        printerPixmap.setOutputFileName(fileName);
        //获取界面的图片
        QPixmap pixmap = QPixmap::grabWidget(ui->tableView, ui->tableView->rect());

        QPainter painterPixmap;
        painterPixmap.begin(&printerPixmap);
        QRect rect = painterPixmap.viewport();
        int x = rect.width() / pixmap.width();
        int y = rect.height() / pixmap.height();
        //将图像(所有要画的东西)在pdf上按比例尺缩放
        painterPixmap.scale(x, y);
        //画图
        painterPixmap.drawPixmap(0, 0, pixmap);
        painterPixmap.end();

        QMessageBox::information(this, tr("生成PDF"), tr("保存PDF文件成功"), QMessageBox::Ok);
    }
}
/*
*@projectName   WrenchApp
*@brief         打印预览
*@auther        Li
*@date          2019-08-28
*/
void QByJobs::printPreviewSlot(QPrinter *printerPixmap)
{
    printerPixmap->setOrientation(QPrinter::Landscape);
    //获取界面的图片
    QPixmap pixmap = QPixmap::grabWidget(ui->tableView, ui->tableView->rect());
    QPainter painterPixmap(this);
    painterPixmap.begin(printerPixmap);
    QRect rect = painterPixmap.viewport();
    int x = rect.width() / pixmap.width();
    int y = rect.height() / pixmap.height();
    painterPixmap.scale(x, y);
    painterPixmap.drawPixmap(0, 0, pixmap);
    painterPixmap.end();
}
/*
*@projectName   WrenchApp
*@brief         打印按钮
*@auther        Li
*@date          2019-08-28
*/
void QByJobs::on_pushButton_3_clicked()
{
    //设置分辨率为屏幕分辨率
    QPrinter printer(QPrinter::ScreenResolution);

    //自定义纸张大小
    printer.setPageSize(QPrinter::Custom);
    printer.setPaperSize(QSizeF(ui->tableView->height(),ui->tableView->width()),
                         QPrinter::Point);
    //设置打印范围
    // printer.setPrintRange(QPrinter::Selection);
    QPrintPreviewDialog preview(&printer,this);
    //设置打印预览窗口大小
    preview.setMinimumSize(1000,1000);
    connect(&preview, SIGNAL(paintRequested(QPrinter *)),this,SLOT(printPreviewSlot(QPrinter* )));
    preview.exec();
}
