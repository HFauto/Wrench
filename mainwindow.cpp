#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qbyjobs/qbyjobs.h"
#include "qbytools/qbytools.h"
#include "personnel/personnel.h"
#include "languageDialog/language.h"
#include "tool/toolinterface.h"
#include "code/codeinterface.h"
#include "job/job.h"
#include "jobtem/jobtem.h"
#include "newjob/newjob.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>
#include <QCloseEvent>
#include <QFile>
#include <QDir>
#include <QLabel>
#include <QStatusBar>
#include <QFileInfo>
#include <QPrintDialog>
#include <QPrinter>
#include <QPainter>
#include <QPrintPreviewDialog>

#define cout qDebug() << "[" <<__FILE__ <<":"<<__FUNCTION__<<":"<<__LINE__ <<"]"

/*
* @projectName   WrenchApp
* @brief         窗口关闭时询问是否退出
* @author        Li
* @date          2019-09-07
*/
void MainWindow::closeEvent(QCloseEvent *event)
{ //窗口关闭时询问是否退出
    QMessageBox::StandardButton result=QMessageBox::question(this, tr("确认"), tr("确定要退出本程序吗？"),
                                                             QMessageBox::Yes|QMessageBox::No |QMessageBox::Cancel,
                                                             QMessageBox::No);

    if (result==QMessageBox::Yes)
        event->accept();
    else
        event->ignore();
}
/*
* @projectName   WrenchApp
* @brief         构造函数
* @author        Li
* @date          2019-09-07
*/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowState(Qt::WindowMaximized);

    state = new QLabel("");

    statusBar()->addWidget(state);

    InterfaceInit();


}
/*
* @projectName   WrenchApp
* @brief         界面初始化
* @author        HFauto
* @date          2019-07-05
*/
void MainWindow::InterfaceInit()
{
    //action绑定
    ui->menuBar->hide();
    ui->actDBs->setMenu(ui->menuDB);
    ui->actQuery->setMenu(ui->menuQuery);
    ui->actSettings->setMenu(ui->menuSettings);

    //打开数据库
    DB=QSqlDatabase::addDatabase("QSQLITE"); //添加 SQL LITE数据库驱动
    DB.setDatabaseName("b.db"); //设置数据库名称

    if (!DB.open())   //打开数据库
    {
        QMessageBox::warning(this, tr("错误"), tr("打开数据库失败"), QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }

    openTable();//打开数据表

    //设置状态栏信息
    QString currentPath = QDir::currentPath();
    state->setText(currentPath + "/b.db");

    //添加 menuAction
    connect(ui->actDBs,SIGNAL(triggered()),this,SLOT(AddMenu_1()));
    connect(ui->actQuery,SIGNAL(triggered()),this,SLOT(AddMenu_2()));
    connect(ui->actSettings,SIGNAL(triggered()),this,SLOT(AddMenu_3()));


}

MainWindow::~MainWindow()
{
    delete  state;
    delete ui;
}
/*
* @projectName   WrenchApp
* @brief         添加 menu下拉菜单
* @author        HFauto
* @date          2019-07-05
*/
void MainWindow::AddMenu_1()
{
    QRect trec = ui->mainToolBar->actionGeometry(ui->actDBs);
    ui->actDBs->menu()->exec(this->mapToGlobal(QPoint(trec.left(),trec.bottom())));
}
void MainWindow::AddMenu_2()
{
    QRect trec = ui->mainToolBar->actionGeometry(ui->actQuery);
    ui->actQuery->menu()->exec(this->mapToGlobal(QPoint(trec.left(),trec.bottom())));
}
void MainWindow::AddMenu_3()
{
    QRect trec = ui->mainToolBar->actionGeometry(ui->actSettings);
    ui->actSettings->menu()->exec(this->mapToGlobal(QPoint(trec.left(),trec.bottom())));

}
/*
* @projectName   WrenchApp
* @brief         按工具查询
* @author        HFauto
* @date          2019-08-23
*/
void MainWindow::on_acByTool_triggered()
{
    QByTools* qByTools = new QByTools(this);
    qByTools->setAttribute(Qt::WA_DeleteOnClose); //对话框关闭时自动删除对话框对象,用于不需要读取返回值的对话框
    qByTools->setWindowTitle(tr("按工具查询"));
    qByTools->show();
}
/*
*@projectName   WrenchApp
*@brief         按工单查询
*@auther        Li
*@date          2019-08-26
*/
void MainWindow::on_actByJob_triggered()
{
    QByJobs* qByJobs = new QByJobs(this);
    qByJobs->setAttribute(Qt::WA_DeleteOnClose); //对话框关闭时自动删除对话框对象,用于不需要读取返回值的对话框
    qByJobs->setWindowTitle(tr("按工单查询"));
    qByJobs->show();
}

/*
* @projectName   WrenchApp
* @brief         打开数据库
* @author        HFauto
* @date          2019-08-23
*/
void MainWindow::on_actOpenDB_triggered()
{
    //打开数据表
    QString aFile=QFileDialog::getOpenFileName(this, tr("选择数据库文件"), "", tr("SQL Lite数据库(*.db)"));
    if (aFile.isEmpty()) return;

    //打开数据库
    DB=QSqlDatabase::addDatabase("QSQLITE"); //添加 SQL LITE数据库驱动
    DB.setDatabaseName(aFile); //设置数据库名称

    if (!DB.open())   //打开数据库
    {
        QMessageBox::warning(this, tr("错误"), tr("打开数据库失败"), QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }
    state->setText(aFile);

}
/*
* @projectName   WrenchApp
* @brief         语言设置窗口
* @author        Li
* @date          2019-08-30
*/
void MainWindow::on_actLanguage_triggered()
{
    Language* language = new Language(this);
    language->setAttribute(Qt::WA_DeleteOnClose); //对话框关闭时自动删除对话框对象,用于不需要读取返回值的对话框
    language->setWindowTitle(tr("语言设置"));
    language->show();
}
/*
* @projectName   WrenchApp
* @brief         退出按钮
* @author        Li
* @date          2019-08-30
*/
void MainWindow::on_acExit_triggered()
{
    //窗口关闭时询问是否退出
    QMessageBox::StandardButton result=QMessageBox::question(this, tr("确认"), tr("确定要退出本程序吗？"),
                                                             QMessageBox::Yes|QMessageBox::No |QMessageBox::Cancel,
                                                             QMessageBox::No);

    if (result==QMessageBox::Yes)
    {
        QApplication *app=nullptr;

        app->quit();
    }
    else
        return;

}
/*
* @projectName   WrenchApp
* @brief         人员窗口设置密码登录
* @author        Li
* @date          2019-08-30
*/
void MainWindow::on_actSetWorker_triggered()
{

    QString dlgTitle=tr("登陆密码");
    QString txtLabel=tr("请输入密码");
    //QLineEdit::EchoMode echoMode=QLineEdit::Normal;//正常文字输入
    QLineEdit::EchoMode echoMode=QLineEdit::Password;//密码输入

    bool ok=false;
    QString text = QInputDialog::getText(this, dlgTitle,txtLabel, echoMode,"", &ok);
    if (ok && ("0000"==text))
    {
        Personnel* personnel = new Personnel(this);
        personnel->setAttribute(Qt::WA_DeleteOnClose); //对话框关闭时自动删除对话框对象,用于不需要读取返回值的对话框
        personnel->setWindowTitle(tr("人员"));
        personnel->show();
    }
}
/*
* @projectName   WrenchApp
* @brief         工具窗口设置密码登录
* @author        Li
* @date          2019-09-04
*/
void MainWindow::on_actSetTool_triggered()
{
    QString dlgTitle=tr("登陆密码");
    QString txtLabel=tr("请输入密码");
    //QLineEdit::EchoMode echoMode=QLineEdit::Normal;//正常文字输入
    QLineEdit::EchoMode echoMode=QLineEdit::Password;//密码输入

    bool ok=false;
    QString text = QInputDialog::getText(this, dlgTitle,txtLabel, echoMode,"", &ok);
    if (ok && ("1111"==text))
    {
        ToolInterface* toolInterface = new ToolInterface(this);
        toolInterface->setAttribute(Qt::WA_DeleteOnClose); //对话框关闭时自动删除对话框对象,用于不需要读取返回值的对话框
        toolInterface->setWindowTitle(tr("工具"));
        toolInterface->show();
    }
}
/*
* @projectName   WrenchApp
* @brief         代码窗口设置密码登录
* @author        Li
* @date          2019-09-04
*/
void MainWindow::on_actSetCoed_triggered()
{
    QString dlgTitle=tr("登陆密码");
    QString txtLabel=tr("请输入密码");
    //QLineEdit::EchoMode echoMode=QLineEdit::Normal;//正常文字输入
    QLineEdit::EchoMode echoMode=QLineEdit::Password;//密码输入

    bool ok=false;
    QString text = QInputDialog::getText(this, dlgTitle,txtLabel, echoMode,"", &ok);
    if (ok && ("2222"==text))
    {
        CodeInterface* codeInterface = new CodeInterface(this);
        codeInterface->setAttribute(Qt::WA_DeleteOnClose); //对话框关闭时自动删除对话框对象,用于不需要读取返回值的对话框
        codeInterface->setWindowTitle(tr("代码"));
        codeInterface->show();
    }
}
/*
* @projectName   WrenchApp
* @brief         新建数据库
* @author        Li
* @date          2019-09-05
*/
void MainWindow::on_actNewDB_triggered()
{
    QString currentPath = QDir::currentPath();
    QString aFile=QFileDialog::getSaveFileName(this, tr("新建数据库文件"),currentPath,tr( "SQL Lite数据库(*.db)"));
    if (aFile.isEmpty()) return;
    QFile file(currentPath + "/debug/gg.db");
    //判断文件是否存在
    if(!file.exists())
    {
       QMessageBox::critical(this, tr("错误信息"),tr ("文件不存在！\n")+DB.lastError().text(), QMessageBox::Ok, QMessageBox::NoButton);
    }
    else
    {
        if(!file.copy(aFile))
        {
            QMessageBox::critical(this, tr("错误信息"),tr ("复制失败！\n")+DB.lastError().text(), QMessageBox::Ok, QMessageBox::NoButton);
        }
    }

    //打开数据库
    DB=QSqlDatabase::addDatabase("QSQLITE"); //添加 SQL LITE数据库驱动
    DB.setDatabaseName(aFile); //设置数据库名称

    if (!DB.open())
    {
        QMessageBox::critical(this, tr("错误信息"),tr ("无法打开数据库！\n")+DB.lastError().text(), QMessageBox::Ok, QMessageBox::NoButton);
    }
    state->setText(aFile);


}
/*
* @projectName   WrenchApp
* @brief         导出表格
* @author        Li
* @date          2019-09-05
*/
void MainWindow::on_actOutputExcel_triggered()
{

}
/*
*@projectName   WrenchApp
*@brief         打印预览
*@auther        Li
*@date          2019-08-28
*/
void MainWindow::printPreviewSlot(QPrinter *printerPixmap)
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
* @projectName   WrenchApp
* @brief         打印
* @author        Li
* @date          2019-09-05
*/
void MainWindow::on_actPrint_triggered()
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
/*
* @projectName   WrenchApp
* @brief         打开数据表
* @author        Li
* @date          2019-09-08
*/
void MainWindow::openTable()
{
    tabModel=new QSqlRelationalTableModel(this,DB);//数据表
    tabModel->setTable("scwtbl"); //设置数据表
    tabModel->setEditStrategy(QSqlTableModel::OnRowChange);//数据保存方式，OnManualSubmit , OnRowChange
    tabModel->setSort(tabModel->fieldIndex("jid"),Qt::AscendingOrder); //排序

//    tabModel->setRelation(tabModel->fieldIndex("nameID"),QSqlRelation("tasktbl","nameID","taskName"));//任务名称
    tabModel->setRelation(tabModel->fieldIndex("uid"),QSqlRelation("worker","uid","workerName"));//工人
    //tabModel->setRelation(tabModel->fieldIndex("tid"),QSqlRelation("tool","tid","type"));//工具号
    tabModel->setRelation(tabModel->fieldIndex("tid"),QSqlRelation("tool","tid","toolName"));//工具
//    tabModel->setRelation(tabModel->fieldIndex("dept"),QSqlRelation("codes","code","name"));//工具
//    tabModel->setRelation(tabModel->fieldIndex("part"),QSqlRelation("codes","code","name"));//工具
//    tabModel->setRelation(tabModel->fieldIndex("loc"),QSqlRelation("codes","code","name"));//工具
    if (!(tabModel->select()))//查询数据
    {
       QMessageBox::critical(this, tr("错误信息"), tr("打开数据表错误,错误信息\n")+tabModel->lastError().text(), QMessageBox::Ok,QMessageBox::NoButton);
       return;
    }

    //设置表头
    tabModel->setHeaderData(tabModel->fieldIndex("rid"),Qt::Horizontal,tr("行号"));
    tabModel->setHeaderData(tabModel->fieldIndex("jid"),Qt::Horizontal,tr("工单号"));
    tabModel->setHeaderData(tabModel->fieldIndex("dts"),Qt::Horizontal,tr("开始时间"));
    tabModel->setHeaderData(tabModel->fieldIndex("dte"),Qt::Horizontal,tr("结束时间"));
    tabModel->setHeaderData(tabModel->fieldIndex("target1"),Qt::Horizontal,tr("目标1"));

    tabModel->setHeaderData(tabModel->fieldIndex("tsq"),Qt::Horizontal,tr("工单号"));
    tabModel->setHeaderData(tabModel->fieldIndex("fsp"),Qt::Horizontal,tr("工单号"));
    tabModel->setHeaderData(tabModel->fieldIndex("pos"),Qt::Horizontal,tr("工单号"));

    tabModel->setHeaderData(tabModel->fieldIndex("toolName"),Qt::Horizontal,tr("工具"));
    tabModel->setHeaderData(tabModel->fieldIndex("mfgflag"),Qt::Horizontal,tr("工单号"));
    tabModel->setHeaderData(tabModel->fieldIndex("workerName"),Qt::Horizontal,tr("工人"));
    tabModel->setHeaderData(tabModel->fieldIndex("dept"),Qt::Horizontal,tr("部门"));
    tabModel->setHeaderData(tabModel->fieldIndex("part"),Qt::Horizontal,tr("部位"));
    tabModel->setHeaderData(tabModel->fieldIndex("loc"),Qt::Horizontal,tr("地点"));
    tabModel->setHeaderData(tabModel->fieldIndex("note"),Qt::Horizontal,tr("备注"));
    tabModel->setHeaderData(tabModel->fieldIndex("flag"),Qt::Horizontal,tr("标志"));
    tabModel->setHeaderData(tabModel->fieldIndex("mode"),Qt::Horizontal,tr("模型"));
    tabModel->setHeaderData(tabModel->fieldIndex("unit"),Qt::Horizontal,tr("工单号"));
    tabModel->setHeaderData(tabModel->fieldIndex("target2"),Qt::Horizontal,tr("目标2"));
    tabModel->setHeaderData(tabModel->fieldIndex("tolhi"),Qt::Horizontal,tr("最大值"));
    tabModel->setHeaderData(tabModel->fieldIndex("tollo"),Qt::Horizontal,tr("最小值"));
    tabModel->setHeaderData(tabModel->fieldIndex("cnt"),Qt::Horizontal,tr("小数位数"));
    tabModel->setHeaderData(tabModel->fieldIndex("picpng"),Qt::Horizontal,tr("照片"));
    tabModel->setHeaderData(tabModel->fieldIndex("xml"),Qt::Horizontal,tr("工单号"));



    //关联选择模型
    theSelection=new QItemSelectionModel(tabModel);

    //theSelection当前项变化时触发currentChanged信号
//    connect(theSelection,SIGNAL(currentChanged(QModelIndex,QModelIndex)),
//            this,SLOT(on_currentChanged(QModelIndex,QModelIndex)));

//    //选择行变化时
//    connect(theSelection,SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
//            this,SLOT(on_currentRowChanged(QModelIndex,QModelIndex)));


    ui->tableView->setModel(tabModel);//设置数据模型
    ui->tableView->setSelectionModel(theSelection);//设置选择模型

    //隐藏列
    ui->tableView->setColumnHidden(tabModel->fieldIndex("tsq"),true);//隐藏列
    ui->tableView->setColumnHidden(tabModel->fieldIndex("fsp"),true);//隐藏列
    ui->tableView->setColumnHidden(tabModel->fieldIndex("pos"),true);//隐藏列
    ui->tableView->setColumnHidden(tabModel->fieldIndex("unit"),true);//隐藏列
    ui->tableView->setColumnHidden(tabModel->fieldIndex("tolhi"),true);//隐藏列
    ui->tableView->setColumnHidden(tabModel->fieldIndex("tollo"),true);//隐藏列
    ui->tableView->setColumnHidden(tabModel->fieldIndex("cnt"),true);//隐藏列
    ui->tableView->setColumnHidden(tabModel->fieldIndex("xml"),true);//隐藏列
    ui->tableView->setColumnHidden(tabModel->fieldIndex("picpng"),true);//隐藏列
    //ui->tableView->setItemDelegate(new QSqlRelationalDelegate(ui->tableView)); //为关系型字段设置缺省代理组件


    //tableView显示属性设置
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setAlternatingRowColors(true);//使用交替颜色绘制背景
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//不可编辑
    ui->tableView->setTextElideMode(Qt::ElideMiddle);//文本居中

//    theSelection->setCurrentIndex(tabModel->index(0,0),QItemSelectionModel::Select);//默认选择第一行数据


}
/*
* @projectName   WrenchApp
* @brief         工单安排
* @author        Li
* @date          2019-09-08
*/
void MainWindow::on_actArrange_triggered()
{
    Job* job = new Job(this);
    job->setAttribute(Qt::WA_DeleteOnClose); //对话框关闭时自动删除对话框对象,用于不需要读取返回值的对话框
    job->setWindowTitle(tr("工单安排"));
    job->show();
}
/*
* @projectName   WrenchApp
* @brief         新建模板
* @author        Li
* @date          2019-09-12
*/
void MainWindow::on_actNewTemp_triggered()
{
    Jobtem* jobtem = new Jobtem(this);
    jobtem->setAttribute(Qt::WA_DeleteOnClose); //对话框关闭时自动删除对话框对象,用于不需要读取返回值的对话框
    jobtem->setWindowTitle(tr("新建模板"));
    jobtem->show();
}
/*
* @projectName   WrenchApp
* @brief         新建工单
* @author        Li
* @date          2019-09-14
*/
void MainWindow::on_actNewJob_triggered()
{
    Newjob* newjob = new Newjob(this);
    newjob->setAttribute(Qt::WA_DeleteOnClose); //对话框关闭时自动删除对话框对象,用于不需要读取返回值的对话框
    newjob->setWindowTitle(tr("新建工单"));
    newjob->show();
}
