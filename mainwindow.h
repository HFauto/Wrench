#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QPushButton"
#include "QToolBar"
#include "QStatusBar"
#include <QtSql>
#include <QDialog>
#include <QLabel>
#include <QDataWidgetMapper>
#include <QPrinter>


#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:



public slots:


private slots:

    void AddMenu_1();

    void AddMenu_2();

    void AddMenu_3();

    void on_acByTool_triggered();

    void on_actOpenDB_triggered();

    void on_actByJob_triggered();

    void on_actLanguage_triggered();

    void on_acExit_triggered();

    void on_actSetWorker_triggered();

    void on_actSetTool_triggered();

    void on_actSetCoed_triggered();

    void on_actNewDB_triggered();

    void on_actOutputExcel_triggered();

    void on_actPrint_triggered();

    void on_actArrange_triggered();

    void printPreviewSlot(QPrinter *printerPixmap);

    void on_actNewTemp_triggered();

    void on_actNewJob_triggered();

signals:


private:
    Ui::MainWindow *ui;

    void InterfaceInit();

    void closeEvent(QCloseEvent *event);//关闭窗口时间，可以询问是否退出

    QSqlDatabase  DB;//数据库连接

    QSqlRelationalTableModel  *tabModel;  //数据模型

    QItemSelectionModel *theSelection; //选择模型

    QDataWidgetMapper   *dataMapper; //数据映射

    QLabel *state;

    void openTable();//打开数据表





};

#endif // MAINWINDOW_H
