#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QVector>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QFile>
#include <QTextStream>
#include <QDateTime>



Widget::Widget(QWidget *parent):
    QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    ttl = new QSerialPort(this);
    foreach (const QSerialPortInfo &serialport, QSerialPortInfo::availablePorts()) {
        ui->Puertos->addItem(serialport.portName());
    }

    timer.start();   // 🟢 Iniciamos el temporizador aquí
    setupPlot();     // ⚙️ También corrige el error: quita "Widget:"
    // 🟢 Crear y abrir archivo de log
    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");
    QString filename = QString("rpm_log_%1.log").arg(timestamp);

    logFile.setFileName(filename);
    if (logFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        logStream.setDevice(&logFile);
        logStream << "Tiempo(s),RPM_suavizado\n";  // encabezado del archivo
    } else {
        qDebug() << "⚠️ No se pudo crear el archivo de log.";
    }

}



Widget::~Widget()
{
    if (logFile.isOpen()) {
        logFile.close();
    }

    delete ui;
}

void Widget::setupPlot(){
/*
    x.resize(101);
    y.resize(101);
    z.resize(101);
    w.resize(101);
    for (int i=0; i<101; ++i)
    {
      x[i] = (double)i;
      y[i] = (double)2;
      z[i] = (double)i;
      w[i] = (double)4;
    }

    ui->customPlot->addGraph(ui->customPlot->xAxis, ui->customPlot->yAxis);
    ui->customPlot->addGraph(ui->customPlot->xAxis, ui->customPlot->yAxis2);
    ui->customPlot->graph(0)->setData(x, y);
    ui->customPlot->graph(1)->setData(z, w);
    ui->customPlot->graph(1)->setPen(QPen(Qt::red));
    ui->customPlot->graph(0)->setName("RPM");
    //ui->customPlot->graph(1)->setName("Acceleracion");
    ui->customPlot->plotLayout()->insertRow(0);
    ui->customPlot->plotLayout()->addElement(0, 0, new QCPTextElement(ui->customPlot, "Grafica RPM VS Tiempo", QFont("sans", 12, QFont::Bold)));


    ui->customPlot->legend->setVisible(true);
    QFont legendFont = font();  // start out with MainWindow's font..
    legendFont.setPointSize(9); // and make a bit smaller for legend
    ui->customPlot->legend->setFont(legendFont);
    ui->customPlot->legend->setBrush(QBrush(QColor(255,255,255,230)));
    // by default, the legend is in the inset layout of the main axis rect. So this is how we access it to change legend placement:
    //ui->customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignBottom|Qt::AlignRight);

    ui->customPlot->xAxis->setLabel("Tiempo transcurrido");
    ui->customPlot->yAxis->setLabel("RPM");
    ui->customPlot->xAxis->setRange(0, 100);
    ui->customPlot->yAxis->setRange(0, 5000);
    ui->customPlot->yAxis2->setLabel("mA");
    ui->customPlot->yAxis2->setRange(0, 1000);
    ui->customPlot->yAxis2->setVisible(true);
    ui->customPlot->replot();
    */

    QVector<double> x(101), y(101);
    for (int i = 0; i < 101; ++i)
    {
        x[i] = static_cast<double>(i);        // Tiempo transcurrido
        y[i] = static_cast<double>(i * 50);   // Ejemplo: RPM creciendo lineal
    }

    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setData(x, y);
    ui->customPlot->graph(0)->setPen(QPen(Qt::blue));
    ui->customPlot->graph(0)->setName("RPM");

    // Título
    ui->customPlot->plotLayout()->insertRow(0);
    ui->customPlot->plotLayout()->addElement(
        0, 0,
        new QCPTextElement(ui->customPlot, "Gráfica RPM VS Tiempo transcurrido", QFont("sans", 12, QFont::Bold))
    );

    // Leyenda
    ui->customPlot->legend->setVisible(true);
    QFont legendFont = font();
    legendFont.setPointSize(9);
    ui->customPlot->legend->setFont(legendFont);
    ui->customPlot->legend->setBrush(QBrush(QColor(255, 255, 255, 230)));

    // Ejes
    ui->customPlot->xAxis->setLabel("Tiempo transcurrido (s)");
    ui->customPlot->yAxis->setLabel("RPM");
    ui->customPlot->xAxis->setRange(0, 100);
    ui->customPlot->yAxis->setRange(0, 5000);

    ui->customPlot->replot();

}

void Widget::makeplot(double rpm)
{
    static double lastRPM = -1;
    static double smoothRPM = 0;
    static double lastTime = 0.0;

    double currentTime = timer.elapsed() / 1000.0;  // tiempo en segundos
    const int windowSize = 10;                      // segundos visibles
    const int maxPoints = 200;                      // máximo de puntos guardados

    // 🟢 Evita descartar el primer dato
    if (lastRPM < 0) {
        lastRPM = rpm;
        smoothRPM = rpm;
    }

    // 🧩 1. Filtro antirruido adaptativo
    double delta = fabs(rpm - lastRPM);
    if (delta > 500 && delta > (0.5 * lastRPM)) {
        // ignora saltos absurdos (por tramas erróneas)
        rpm = lastRPM;
    }

    // 🧩 2. Suavizado exponencial (reacciona pero sin picos)
    smoothRPM = 0.7 * smoothRPM + 0.3 * rpm;

    // 🧩 3. Guardamos y actualizamos
    x.append(currentTime);
    y.append(smoothRPM);
    if (x.size() > maxPoints) {
        x.removeFirst();
        y.removeFirst();
    }

    // 🧩 4. Dibujar
    ui->customPlot->graph(0)->setData(x, y);
    ui->customPlot->xAxis->setRange(currentTime - windowSize, currentTime);

    // Escala vertical estable
    double minY = *std::min_element(y.begin(), y.end());
    double maxY = *std::max_element(y.begin(), y.end());
    if (maxY - minY < 100) maxY = minY + 100;
    ui->customPlot->yAxis->setRange(minY - 50, maxY + 50);

    ui->customPlot->replot(QCustomPlot::rpQueuedReplot);

    lastRPM = rpm;  // guarda para próxima comparación
    if (logFile.isOpen()) {
        logStream << QString::number(currentTime, 'f', 3) << ","
                  << QString::number(smoothRPM, 'f', 2) << "\n";
        logStream.flush(); // asegura que se escriba inmediatamente
    }

}


void Widget::on_ABRIR_clicked()
{
    QString portname=ui->Puertos->currentText();

    if(ui->ABRIR->text()=="Abrir"){
        ttl->close();
        ttl->setPortName(portname);
        ttl->setBaudRate(QSerialPort::Baud115200);
        ttl->open(QSerialPort::ReadWrite);
        ttl->setDataBits(QSerialPort::Data8);
        ttl->setFlowControl(QSerialPort::NoFlowControl);
        ttl->setParity(QSerialPort::NoParity);
        connect(ttl, SIGNAL(readyRead()), this, SLOT(leerDato()));

        ui->ABRIR->setText("Cerrar");
    }else{
        ttl->close();
        disconnect(ttl,SIGNAL(readyRead()),this,SLOT(leerread()));
        ui->ABRIR->setText("Abrir");
    }
}

void Widget::on_ARRANCAR_clicked()
{
    QByteArray datos;
    datos.append(0xA);
    ttl->write(datos);
    flag1=0;
    RPManter=0;
}

void Widget::on_DETENERSE_clicked()
{
    QByteArray datos;
    datos.append(0xD);
    ttl->write(datos);
    qDebug()<<datos;
    flag1=0;
    RPManter=0;

}

void Widget::on_ASIGNAR_clicked()

{
    int signo=0;
        QString rpm_txt = ui->RPM->text();
        sendrpm = rpm_txt.toInt();
        QByteArray datos;
        datos.append(0x0F);
        if(sendrpm>0){
            signo=0xff;
        }else if(sendrpm<0){
            sendrpm=-sendrpm;
            signo=0xef;
        }
        int enviar1=sendrpm & 0xff;
        int enviar2=sendrpm >> 8;

        datos.append(enviar1);
        datos.append(enviar2);
        datos.append(signo);
        ttl->write(datos);
        flag1=1;
        RPManter=0;

}

/*
{
    QString rpm_txt = ui->RPM->text();
    int sendrpm = rpm_txt.toInt();

    QByteArray datos;
    datos.append(0x0F);  // Comando

    // Determinar signo
    quint8 signo = 0x00;
    int valor = sendrpm;

    if (sendrpm > 0) {
        signo = 0xFF;
    } else if (sendrpm < 0) {


        valor = -sendrpm;
        signo = 0xEF;
    }

    // Empaquetar datos
    datos.append(valor & 0xFF);       // Byte bajo
    datos.append((valor >> 8) & 0xFF); // Byte alto
    datos.append(signo);               // Signo

    ttl->write(datos);

    flag1 = 1;
    RPManter = sendrpm;
}
*/

void Widget::leerDato()
{
    QByteArray data = ttl->readAll(); // lee datos crudos del puerto
    serialBuffer.append(QString::fromUtf8(data)); // convierte a texto y acumula

    if (serialBuffer.contains('\n')) // si hay un fin de línea
    {
        QStringList lines = serialBuffer.split('\n'); // separamos por líneas
        serialBuffer = lines.last(); // guardamos lo que quedó incompleto

        for (int i = 0; i < lines.size() - 1; ++i)
        {
            bool ok = false;
            double rpmValue = lines[i].trimmed().toDouble(&ok);
            if (ok)
            {
                makeplot(rpmValue); // graficamos el valor válido
            }
        }
    }
}



void Widget::on_rpmdat_customContextMenuRequested(const QPoint &pos)
{
    QByteArray buffer = ttl->readAll();

       if (buffer.isEmpty()) {
           QMessageBox::warning(this, "Error", "No se recibieron datos desde la STM32.");
           return;
       }

       // Convertir texto recibido a número
       QString texto = QString::fromUtf8(buffer).trimmed();

       bool ok = false;
       float rpm = texto.toFloat(&ok);

       if (ok) {
           ui->rpmdat->setText(QString::number(rpm, 'f', 2));
           qDebug() << "Lectura RPM:" << rpm;
       } else {
           QMessageBox::warning(this, "Error", "Formato de dato no válido recibido: " + texto);
       }
}

void Widget::on_segdat_customContextMenuRequested(const QPoint &pos)
{
    QByteArray buffer = ttl->readAll();

        if (buffer.isEmpty()) {
            QMessageBox::warning(this, "Error", "No se recibieron datos desde la STM32.");
            return;
        }

        QString texto = QString::fromUtf8(buffer).trimmed();

        // Separar por coma si vienen ambos datos
        QStringList partes = texto.split(',');

        if (partes.size() >= 2) {
            bool ok2 = false;
            float seg = partes[1].toFloat(&ok2);

            if (ok2) {
                ui->segdat->setText(QString::number(seg, 'f', 0));
                qDebug() << "Lectura SEG:" << seg;
            }
        } else {
            QMessageBox::information(this, "Aviso", "El valor de SEG no fue recibido correctamente: " + texto);
        }
}
