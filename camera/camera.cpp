#include "camera.h"
#include "ui_camera.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

/*
* @projectName   WrenchApp
* @brief         初始化相机
* @author        Li
* @date          2019-09-08
*/
void Camera::iniCamera()
{
    QCameraInfo curCameraInfo=QCameraInfo::defaultCamera(); //获取缺省摄像头

    if(!curCameraInfo.isNull())
    {
        curCamera=new QCamera(curCameraInfo,this); //创建摄像头对象

        curCamera->setCaptureMode(QCamera::CaptureStillImage);//设置为抓图

        curCamera->setViewfinder(ui->viewFinder); //设置取景框预览
    }
    else
    {
        QMessageBox::critical(this,tr("错误信息"), tr("无法获取摄像头信息\n"), QMessageBox::Ok,QMessageBox::NoButton);
        return;
    }

}
/*
* @projectName   WrenchApp
* @brief         初始化抓图
* @author        Li
* @date          2019-09-08
*/
void Camera::iniImageCapture()
{
    //创建 QCameraImageCapture对象
    imageCapture = new QCameraImageCapture(curCamera,this);

    imageCapture->setBufferFormat(QVideoFrame::Format_Jpeg); //缓冲区格式

    imageCapture->setCaptureDestination(QCameraImageCapture::CaptureToFile); //保存目标

    connect(imageCapture,SIGNAL(imageCaptured(int, const QImage &)),
            this,SLOT(on_imageCaptured(int, const QImage &)));

}

Camera::Camera(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::Camera)
{
    ui->setupUi(this);

    setWindowState(Qt::WindowMaximized);

    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    if (cameras.size()>0)
    {
        iniCamera();  //初始化摄像头

        iniImageCapture();//初始化静态画图

        curCamera->start();//打开摄像头

    }
}

Camera::~Camera()
{

    delete ui;
}
/*
* @projectName   WrenchApp
* @brief         显示图片
* @author        Li
* @date          2019-09-08
*/
void Camera::on_imageCaptured(int id, const QImage &preview)
{
    Q_UNUSED(id)
    QImage scaledImage = preview.scaled(ui->LabCapturedImage->size(),
                     Qt::KeepAspectRatio,  Qt::SmoothTransformation);
    ui->LabCapturedImage->setPixmap(QPixmap::fromImage(scaledImage));
    qDebug()<<5;
}
/*
* @projectName   WrenchApp
* @brief         拍照
* @author        Li
* @date          2019-09-08
*/
void Camera::on_actPhoto_triggered()
{
    //拍照 按钮
        if (curCamera->captureMode()!=QCamera::CaptureStillImage)
        {
            curCamera->setCaptureMode(QCamera::CaptureStillImage);          
            return;
        }

        imageCapture->capture();      
}
/*
* @projectName   WrenchApp
* @brief         保存图片
* @author        Li
* @date          2019-09-08
*/
void Camera::on_actSave_triggered()
{
    const QPixmap *pixmap = ui->LabCapturedImage->pixmap();
    QString currentPath = QDir::currentPath();
    QString aFile=QFileDialog::getSaveFileName(this, tr("保存图片"),currentPath,"*.png");
    pixmap->save(aFile);

}
