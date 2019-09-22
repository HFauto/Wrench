#include "language.h"
#include "ui_language.h"
#include <QTranslator>
#include <QSettings>
#include <QProcess>
extern QTranslator * m_translator;


Language::Language(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Language)
{
    ui->setupUi(this);
    m_translator = new QTranslator(this);
    if(ui->label->text()=="语言设置")
    {
        ui->comboBox->setCurrentIndex(0);
    }
    else
    {
         ui->comboBox->setCurrentIndex(1);
    }
}

Language::~Language()
{
    delete ui;
}
/*
* @projectName   WrenchApp
* @brief         中英文切换
* @author        Li
* @date          2019-08-30
*/
void Language::on_comboBox_activated(int index)
{
    if (index==0)
    {
        //中文界面
        qApp->removeTranslator( m_translator);
        delete  m_translator;
        m_translator=new QTranslator;
        m_translator->load("wrenchApp_cn.qm");
        qApp->installTranslator(m_translator);//安装翻译器
        ui->retranslateUi(this);//更新界面语言

        QSettings   settings("WWB-Qt","WrenchApp"); //注册表键组
        settings.setValue("Language","CN"); //界面语言，汉语
        //退出程序
        qApp->quit();
        QProcess::startDetached(qApp->applicationFilePath(),QStringList());
    }

   else
    {
        //英文界面
        qApp->removeTranslator( m_translator);
        delete  m_translator;
        m_translator=new QTranslator;
        m_translator->load("wrenchApp_en.qm");
        qApp->installTranslator(m_translator);//安装翻译器
        ui->retranslateUi(this);//更新界面语言

        QSettings   settings("WWB-Qt","WrenchApp"); //注册表键组
        settings.setValue("Language","EN"); //界面语言，英语
        //退出程序
        qApp->quit();
        QProcess::startDetached(qApp->applicationFilePath(),QStringList());
    }
}


