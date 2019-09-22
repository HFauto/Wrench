#ifndef CAMERA_H
#define CAMERA_H

#include <QMainWindow>
#include    <QCameraInfo>
#include    <QCamera>
#include    <QLabel>

#include    <QCameraViewfinder>
#include    <QCameraImageCapture>

namespace Ui {
class Camera;
}

class Camera : public QMainWindow
{
    Q_OBJECT

public:
    explicit Camera(QWidget *parent = nullptr);
    ~Camera();

private:
    Ui::Camera *ui;

    QCamera     *curCamera=Q_NULLPTR;//

    QCameraViewfinder *viewfinder=Q_NULLPTR;

    QCameraImageCapture *imageCapture; //抓图

    void    iniCamera();//初始化

    void    iniImageCapture();//初始化静态画图

private slots:



    void    on_imageCaptured(int id, const QImage &preview);


    void on_actPhoto_triggered();
    void on_actSave_triggered();
};

#endif // CAMERA_H


