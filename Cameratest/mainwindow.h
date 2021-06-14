#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QComboBox>
#include <QCamera>
#include <QString>
#include <QCameraViewfinderSettings>
#include <QCameraImageCapture>
#include <QElapsedTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
const QString ARGB32 = "ARGB32";
const QString ARGB32_Premultiplied = "ARGB32_Premultiplied";
const QString ARGB8565_Premultiplied = "ARGB565_Premultiplied";
const QString AYUV444 = "AYUV444";
const QString AYUV444_Premultiplied = "AYUV444_Premultiplied";
const QString AdobeDng = "AdobeDng";
const QString BGR24 = "BGR24";
const QString BGR32 = "BGR32";
const QString BGR555 = "BGR555";
const QString BGR565 = "BGR565";
const QString BGRA32 =  "BGRA32";
const QString BGRA32_Premultiplied = "BGRA32_Premultiplied";
const QString BGRA5658_Premultiplied  = "BGRA5658_Premultiplied";
const QString CameraRaw  = "CameraRaw";
const QString IMC1  = "IMC1";
const QString IMC2  = "IMC2";
const QString IMC3  = "IMC3";
const QString IMC4  = "IMC4";
const QString Invalid  = "Invalid";
const QString Jpeg  = "Jpeg";
const QString NV12  = "NV12";
const QString NV21  = "NV21";
const QString RGB24  = "RGB24";
const QString RGB32  = "RGB32";
const QString RGB555  = "RGB555";
const QString RGB565  = "RGB565";
const QString UYVY  = "UYVY";
const QString User  = "User";
const QString Y16  = "Y16";
const QString Y8  = "Y8";
const QString YUV420P  = "YUV420P";
const QString YUV444  = "YUV444";
const QString YUYV  = "YUYV";
const QString YV12  = "YV12";

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QLabel *image_label;
    QComboBox *cam_box;
    QComboBox *mode_box;
    QComboBox *settings_box;
    QCamera *camera;
    QCameraImageCapture *image_capture;
    QString cam_name;
    QString build_settings_string(QCameraViewfinderSettings settings);
    QCameraViewfinderSettings build_settings_from_string(QString text);
    QString pixelformat_to_string(QVideoFrame::PixelFormat format);
    QVideoFrame::PixelFormat string_to_pixelformat(QString text);
    QVideoFrame *buffer;
    QElapsedTimer timer;

public slots:
    void stateChanged(QCamera::State state);
    void statusChanged(QCamera::Status status);
    void captureModeChanged(QCamera::CaptureModes mode);
    void cam_box_text_changed(const QString &text);
    void settings_box_text_changed(const QString &text);
    void mode_box_text_changed(const QString &text);
    void bufferFormatChanged(QVideoFrame::PixelFormat format);
    void captureDestinationChanged(QCameraImageCapture::CaptureDestinations destination);
    void imageAvailable(int id, const QVideoFrame &frame);
    void imageCaptured(int id, const QImage &preview);
    void imageExposed(int id);
    void readyForCaptureChanged(bool ready);


private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
