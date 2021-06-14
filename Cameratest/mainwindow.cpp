#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCamera>
#include <QCameraInfo>
#include <QCameraImageCapture>
#include <QCameraViewfinderSettings>
#include <QElapsedTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    image_label = ui->image_label;
    cam_box = ui->camBox;
    mode_box = ui->modeBox;
    settings_box = ui->settingsBox;
    connect(cam_box, &QComboBox::currentTextChanged, this, &MainWindow::cam_box_text_changed);
    connect(mode_box, &QComboBox::currentTextChanged, this, &MainWindow::mode_box_text_changed);
    connect(settings_box, &QComboBox::currentTextChanged, this, &MainWindow::settings_box_text_changed);
    camera = NULL;
    image_capture = NULL;
    for(QCameraInfo cam_info: QCameraInfo::availableCameras()){
        cam_box->addItem(cam_info.deviceName());
    }
    mode_box->addItem("CaptureStillImage");
    mode_box->addItem("CaptureVideo");
    mode_box->addItem("CaptureViewfinder");
    timer.start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::build_settings_string(QCameraViewfinderSettings settings)
{
    QString text;
    text.append(QString::number(settings.resolution().width()));
    text.append("x");
    text.append(QString::number(settings.resolution().height()));
    text.append(", ");
    text.append(pixelformat_to_string(settings.pixelFormat()));
    text.append(": ");
    text.append(QString::number(settings.minimumFrameRate()));
    text.append("-");
    text.append(QString::number(settings.maximumFrameRate()));
    return text;
}

QCameraViewfinderSettings MainWindow::build_settings_from_string(QString text)
{
    if(text.contains(" ")){
        return build_settings_from_string(text.replace(" ", ""));
    }
    QCameraViewfinderSettings settings;
    settings.setResolution(text.mid(0, text.indexOf("x")).toInt(), text.mid(text.indexOf("x")+1, text.indexOf(",")-text.indexOf("x")-1).toInt());
    settings.setPixelFormat(string_to_pixelformat(text.mid(text.indexOf(",")+1, text.indexOf(":")-text.indexOf(",")-1)));
    settings.setMinimumFrameRate(text.mid(text.indexOf(":")+1, text.indexOf("-")-text.indexOf(":")-1).toInt());
    settings.setMaximumFrameRate(text.mid(text.indexOf("-")+1, text.length()-text.indexOf("-")-1).toInt());
    return settings;
}

QString MainWindow::pixelformat_to_string(QVideoFrame::PixelFormat format)
{
    switch (format) {
        case QVideoFrame::Format_ARGB32: return ARGB32;
        case QVideoFrame::Format_ARGB32_Premultiplied: return ARGB32_Premultiplied;
        case QVideoFrame::Format_ARGB8565_Premultiplied: return ARGB8565_Premultiplied;
        case QVideoFrame::Format_AYUV444: return AYUV444;
        case QVideoFrame::Format_AYUV444_Premultiplied: return AYUV444_Premultiplied;
        case QVideoFrame::Format_AdobeDng: return AdobeDng;
        case QVideoFrame::Format_BGR24: return BGR24;
        case QVideoFrame::Format_BGR32: return BGR32;
        case QVideoFrame::Format_BGR555: return BGR555;
        case QVideoFrame::Format_BGR565: return BGR565;
        case QVideoFrame::Format_BGRA32: return  BGRA32;
        case QVideoFrame::Format_BGRA32_Premultiplied:return BGRA32_Premultiplied;
        case QVideoFrame::Format_BGRA5658_Premultiplied :return BGRA5658_Premultiplied;
        case QVideoFrame::Format_CameraRaw :return CameraRaw;
        case QVideoFrame::Format_IMC1 :return IMC1;
        case QVideoFrame::Format_IMC2 :return IMC2;
        case QVideoFrame::Format_IMC3 :return IMC3;
        case QVideoFrame::Format_IMC4 :return IMC4;
        case QVideoFrame::Format_Invalid :return Invalid;
        case QVideoFrame::Format_Jpeg :return Jpeg;
        case QVideoFrame::Format_NV12 :return NV12;
        case QVideoFrame::Format_NV21 :return NV21;
        case QVideoFrame::Format_RGB24 :return RGB24;
        case QVideoFrame::Format_RGB32 :return RGB32;
        case QVideoFrame::Format_RGB555 :return RGB555;
        case QVideoFrame::Format_RGB565 :return RGB565;
        case QVideoFrame::Format_UYVY :return UYVY;
        case QVideoFrame::Format_User :return User;
        case QVideoFrame::Format_Y16 :return Y16;
        case QVideoFrame::Format_Y8 :return Y8;
        case QVideoFrame::Format_YUV420P :return YUV420P;
        case QVideoFrame::Format_YUV444 :return YUV444;
        case QVideoFrame::Format_YUYV :return YUYV;
        case QVideoFrame::Format_YV12 :return YV12;
    }
    return "";
}

QVideoFrame::PixelFormat MainWindow::string_to_pixelformat(QString text)
{
    if(text.contains(ARGB32)){
        return QVideoFrame::Format_ARGB32;
    }else if(text.contains(ARGB32_Premultiplied)){
        return QVideoFrame::Format_ARGB32_Premultiplied;
    }else if(text.contains(ARGB8565_Premultiplied)){
        return QVideoFrame::Format_ARGB8565_Premultiplied;
    }else if(text.contains(AYUV444)){
        return QVideoFrame::Format_AYUV444;
    }else if(text.contains(AYUV444_Premultiplied)){
        return QVideoFrame::Format_AYUV444_Premultiplied;
    }else if(text.contains(AdobeDng)){
        return QVideoFrame::Format_AdobeDng;
    }else if(text.contains(BGR24)){
        return QVideoFrame::Format_BGR24;
    }else if(text.contains(BGR32)){
        return QVideoFrame::Format_BGR32;
    }else if(text.contains(BGR555)){
        return QVideoFrame::Format_BGR555;
    }else if(text.contains(BGR565)){
        return QVideoFrame::Format_BGR565;
    }else if(text.contains(BGRA32)){
        return QVideoFrame::Format_BGRA32;
    }else if(text.contains(BGRA32_Premultiplied)){
        return QVideoFrame::Format_BGRA32_Premultiplied;
    }else if(text.contains(BGRA5658_Premultiplied)){
        return QVideoFrame::Format_BGRA5658_Premultiplied;
    }else if(text.contains(CameraRaw)){
        return QVideoFrame::Format_CameraRaw;
    }else if(text.contains(IMC1)){
        return QVideoFrame::Format_IMC1;
    }else if(text.contains(IMC2)){
        return QVideoFrame::Format_IMC2;
    }else if(text.contains(IMC3)){
        return QVideoFrame::Format_IMC3;
    }else if(text.contains(IMC4)){
        return QVideoFrame::Format_IMC4;
    }else if(text.contains(Invalid)){
        return QVideoFrame::Format_Invalid;
    }else if(text.contains(Jpeg)){
        return QVideoFrame::Format_Jpeg;
    }else if(text.contains(NV12)){
        return QVideoFrame::Format_NV12;
    }else if(text.contains(NV21)){
        return QVideoFrame::Format_NV21;
    }else if(text.contains(RGB24)){
        return QVideoFrame::Format_RGB24;
    }else if(text.contains(RGB32)){
        return QVideoFrame::Format_RGB32;
    }else if(text.contains(RGB555)){
        return QVideoFrame::Format_RGB555;
    }else if(text.contains(RGB565)){
        return QVideoFrame::Format_RGB565;
    }else if(text.contains(UYVY)){
        return QVideoFrame::Format_UYVY;
    }else if(text.contains(User)){
        return QVideoFrame::Format_User;
    }else if(text.contains(Y16)){
        return QVideoFrame::Format_Y16;
    }else if(text.contains(Y8)){
        return QVideoFrame::Format_Y8;
    }else if(text.contains(YUV420P)){
        return QVideoFrame::Format_YUV420P;
    }else if(text.contains(YUV444)){
        return QVideoFrame::Format_YUV444;
    }else if(text.contains(YUYV)){
        return QVideoFrame::Format_YUYV;
    }else if(text.contains(YV12)){
        return QVideoFrame::Format_YV12;
    }
    return QVideoFrame::Format_Invalid;
}

void MainWindow::stateChanged(QCamera::State state)
{
    qDebug() << cam_name << " State " << state;
}

void MainWindow::statusChanged(QCamera::Status status)
{
    qDebug() << cam_name << " Status " << status;
    if(status == QCamera::LoadedStatus){
        settings_box->clear();
        for(QCameraViewfinderSettings settings : camera->supportedViewfinderSettings()){
            settings_box->addItem(build_settings_string(settings));
        }
        image_capture = new QCameraImageCapture(camera);
        //TODO?
        connect(image_capture, &QCameraImageCapture::bufferFormatChanged, this, &MainWindow::bufferFormatChanged);
        connect(image_capture, &QCameraImageCapture::captureDestinationChanged, this, &MainWindow::captureDestinationChanged);
        connect(image_capture, &QCameraImageCapture::imageAvailable, this, &MainWindow::imageAvailable);
        connect(image_capture, &QCameraImageCapture::imageCaptured, this, &MainWindow::imageCaptured);
        connect(image_capture, &QCameraImageCapture::imageExposed, this, &MainWindow::imageExposed);
        connect(image_capture, &QCameraImageCapture::readyForCaptureChanged, this, &MainWindow::readyForCaptureChanged);
        //TODO?
        QImageEncoderSettings encoder_settings;
        encoder_settings.setCodec("image/jpeg");
        encoder_settings.setResolution(camera->viewfinderSettings().resolution());
        image_capture->setEncodingSettings(encoder_settings);
        image_capture->setCaptureDestination(QCameraImageCapture::CaptureToBuffer);
        camera->start();
    }
}

void MainWindow::captureModeChanged(QCamera::CaptureModes mode)
{
    qDebug() << cam_name << " Changed mode to " << mode;
}

void MainWindow::cam_box_text_changed(const QString &text)
{
    qDebug() << "CameraBox changed to " << text;
    if(camera){
        camera->stop();
        camera->unload();
        disconnect(camera, &QCamera::stateChanged, this, &MainWindow::stateChanged);
        disconnect(camera, &QCamera::statusChanged, this, &MainWindow::statusChanged);
        disconnect(camera, &QCamera::captureModeChanged, this, &MainWindow::captureModeChanged);
    }
    if(image_capture){
        disconnect(image_capture, &QCameraImageCapture::bufferFormatChanged, this, &MainWindow::bufferFormatChanged);
        disconnect(image_capture, &QCameraImageCapture::captureDestinationChanged, this, &MainWindow::captureDestinationChanged);
        disconnect(image_capture, &QCameraImageCapture::imageAvailable, this, &MainWindow::imageAvailable);
        disconnect(image_capture, &QCameraImageCapture::imageCaptured, this, &MainWindow::imageCaptured);
        disconnect(image_capture, &QCameraImageCapture::imageExposed, this, &MainWindow::imageExposed);
        disconnect(image_capture, &QCameraImageCapture::readyForCaptureChanged, this, &MainWindow::readyForCaptureChanged);
    }
    for(QCameraInfo cam_info : QCameraInfo::availableCameras()){
        if(cam_info.deviceName().contains(text)){
            camera = new QCamera(cam_info);
            cam_name = cam_info.deviceName();
            connect(camera, &QCamera::stateChanged, this, &MainWindow::stateChanged);
            connect(camera, &QCamera::statusChanged, this, &MainWindow::statusChanged);
            connect(camera, &QCamera::captureModeChanged, this, &MainWindow::captureModeChanged);
            camera->load();
            break;
        }
    }
}

void MainWindow::settings_box_text_changed(const QString &text)
{
    qDebug() << "SettingsBox changed to " << text;
    QCameraViewfinderSettings settings = build_settings_from_string(text);
    camera->setViewfinderSettings(settings);
    if(camera->viewfinderSettings() != settings){
        qDebug() << "Couldn't set settings?";
    }
}

void MainWindow::mode_box_text_changed(const QString &text)
{
    qDebug() << "ModeBox changed to " << text;
    camera->stop();
    if(text.contains("CaptureStillImage")){
        camera->setCaptureMode(QCamera::CaptureStillImage);
    }else if(text.contains("CaptureVideo")){
        camera->setCaptureMode(QCamera::CaptureVideo);
    }else if(text.contains("CaptureViewfinder")){
        camera->setCaptureMode(QCamera::CaptureViewfinder);
    }
}

void MainWindow::bufferFormatChanged(QVideoFrame::PixelFormat format)
{
    qDebug() << "Image Capture Buffer Format Changed " << format;
}

void MainWindow::captureDestinationChanged(QCameraImageCapture::CaptureDestinations destination)
{
    qDebug() << "Image Capture Destination Changed " << destination;
}

void MainWindow::imageAvailable(int id, const QVideoFrame &frame)
{
    qDebug() << "Got Frame " << timer.elapsed();
    buffer = new QVideoFrame(frame);
    //qDebug() << "Frame to buffer " << timer.nsecsElapsed();
    //qDebug() << "Buffer valid?? " << buffer->isValid();
    //qDebug() << "Buffer readable? " << buffer->isReadable();
    //qDebug() << "Buffer writable? " << buffer->isWritable();
    //qDebug() << "Buffer already mapped? " << buffer->isMapped();
    bool buffer_mapped = buffer->map(QAbstractVideoBuffer::MapMode::ReadOnly);
    //qDebug() << "Mapping " << timer.nsecsElapsed();
    //qDebug() << "Mapping Buffer: " << buffer_mapped;
    //qDebug() << "Image Capture Image Available " << id << " Frame " << buffer;
    //qDebug() << "Camera Mode " << camera->captureMode();
    //qDebug() << "Frame Format " << pixelformat_to_string(buffer->pixelFormat());
    if(buffer_mapped){
        QPixmap pix;
        bool load_pixmap = pix.loadFromData(buffer->bits(), buffer->height() * buffer->width() * buffer->planeCount(), "JPG");
        //qDebug() << "Pixmap " << timer.nsecsElapsed();
        //qDebug() << "Assuming Size of " << (buffer->height() * buffer->width() * buffer->planeCount());
        //qDebug() << "Load Pix " << load_pixmap;
        if(!pix.isNull()){
            image_label->setPixmap(pix);
            //qDebug() << "Set Pixmap " << timer.nsecsElapsed();
            image_label->update();
            //qDebug() << "Upade "<< timer.nsecsElapsed();
        }
        buffer->unmap();
        //qDebug() << "Unmap "<< timer.nsecsElapsed();
    }
    image_capture->capture();
}

void MainWindow::imageCaptured(int id, const QImage &preview)
{
    qDebug() << "Image Capture Image Captured " << id << " preview " << preview;
    /*
    qDebug() << "Camera Mode " << camera->captureMode();
    image_label->setPixmap(QPixmap::fromImage(preview, Qt::AutoColor));
    image_label->update();
    image_capture->capture();
    */
}

void MainWindow::imageExposed(int id)
{
    //qDebug() << "Image Capture Image Exposed " << id;
}

void MainWindow::readyForCaptureChanged(bool ready)
{
    //qDebug() << "Image Capture is Ready? " << ready;
    if(ready){
        image_capture->capture();
    }
}






