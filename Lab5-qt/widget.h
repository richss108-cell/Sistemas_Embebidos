#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QVector>
#include <QSerialPort>
#include <QElapsedTimer>
#include <QFile>
#include <QTextStream>



namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:

    void setupPlot();
    void makeplot(double rpm);
    void on_ABRIR_clicked();
    void leerDato();
    void on_ARRANCAR_clicked();
    void on_DETENERSE_clicked();
    void on_ASIGNAR_clicked();

    void on_rpmdat_customContextMenuRequested(const QPoint &pos);

    void on_segdat_customContextMenuRequested(const QPoint &pos);

private:
    Ui::Widget *ui;

    //double x[100],y[100];
    QVector<double> x;
    QVector<double> y;
    QVector<double> z;
    QVector<double> w;
    uint RPM;
    int contador=0;
    uint TIEMPO=0;
    uint TIEMPOMAX=0;
    int sendrpm;
    int flag1=0;
    uint RPManter=0;
    QSerialPort*ttl=nullptr;
    static const quint16 ttl_vendor_id = 9476;
    static const quint16 ttl_product_id = 768;
    QByteArray serialData;
    QString serialBuffer;
    QString parsed_data;
    QSerialPort *serial;
    QElapsedTimer timer;
    QFile logFile;
    QTextStream logStream;

};

#endif // WIDGET_H
