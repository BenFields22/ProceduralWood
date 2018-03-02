#include "glwidget.h"
#include "window.h"
#include "mainwindow.h"
#include <QSlider>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QKeyEvent>
#include <QPushButton>
#include <QDesktopWidget>
#include <QApplication>
#include <QMessageBox>
#include <QStackedLayout>
#include <QFormLayout>
#include <QGridLayout>


Window::Window(MainWindow *mw)
    : mainWindow(mw)
{
    glWidget = new GLWidget;

    QVBoxLayout *mainLayout = new QVBoxLayout;
    QVBoxLayout *interface = new QVBoxLayout;
    QHBoxLayout *container = new QHBoxLayout;
    QGroupBox *box2 = new QGroupBox;
    QFormLayout *vbox2 = new QFormLayout;


    box2->setTitle(tr("Wood Parameters"));
    QSlider *slider = new QSlider();
    QSlider *slider2 = new QSlider();
    QSlider *slider3 = new QSlider();
    QSlider *slider4 = new QSlider();
    QSlider *slider5 = new QSlider();
    QLabel *label = new QLabel();
    label->setText("uNoiseAmp");
    slider->setMinimum(0);
    slider->setMaximum(100);
    slider->setValue(10);
    slider->setOrientation(Qt::Horizontal);
    connect(slider, &QSlider::sliderMoved, this->glWidget, [=](){ glWidget->setColorSlide(slider->value()); });


    QLabel *label2 = new QLabel();
    label2->setText("uKa");
    slider2->setMinimum(0);
    slider2->setMaximum(100);
    slider2->setValue(20);
    slider2->setOrientation(Qt::Horizontal);
    connect(slider2, &QSlider::sliderMoved, this->glWidget, [=](){ glWidget->setLightSlide(slider2->value(),slider3->value(),slider4->value(),slider5->value()); });


    QLabel *label3 = new QLabel();
    label3->setText("uKd");
    slider3->setMinimum(0);
    slider3->setMaximum(100);
    slider3->setValue(85);
    slider3->setOrientation(Qt::Horizontal);
    connect(slider3, &QSlider::sliderMoved, this->glWidget, [=](){ glWidget->setLightSlide(slider2->value(),slider3->value(),slider4->value(),slider5->value()); });


    QLabel *label4 = new QLabel();
    label4->setText("uKs");
    slider4->setMinimum(0);
    slider4->setMaximum(100);
    slider4->setValue(20);
    slider4->setOrientation(Qt::Horizontal);
    connect(slider4, &QSlider::sliderMoved, this->glWidget, [=](){ glWidget->setLightSlide(slider2->value(),slider3->value(),slider4->value(),slider5->value()); });

    QLabel *label5 = new QLabel();
    label5->setText("Shineness");
    slider5->setMinimum(0);
    slider5->setMaximum(500);
    slider5->setValue(20);
    slider5->setOrientation(Qt::Horizontal);
    connect(slider5, &QSlider::sliderMoved, this->glWidget, [=](){ glWidget->setLightSlide(slider2->value(),slider3->value(),slider4->value(),slider5->value()); });

    vbox2->addRow(label);
    vbox2->addRow(slider);
    vbox2->addRow(label2);
    vbox2->addRow(slider2);
    vbox2->addRow(label3);
    vbox2->addRow(slider3);
    vbox2->addRow(label4);
    vbox2->addRow(slider4);
    vbox2->addRow(label5);
    vbox2->addRow(slider5);
    box2->setLayout(vbox2);
    dockBtn = new QPushButton(tr("Quit"), this);
    dockBtn->setMinimumSize(100,50);
    dockBtn->setMaximumSize(100,50);
    connect(dockBtn, &QPushButton::clicked, this->mainWindow, &MainWindow::closeApp);
    interface->setAlignment(Qt::AlignLeft);
    interface->addWidget(box2);
    interface->addWidget(dockBtn);


    QWidget *w2 = new QWidget;
    w2->setLayout(interface);
    w2->setMaximumWidth(200);
    w2->setMinimumWidth(200);

    container->addWidget(w2,0,Qt::AlignTop);
    container->addWidget(glWidget);


    QWidget *w = new QWidget;
    w->setLayout(container);
    mainLayout->addWidget(w);


    setLayout(mainLayout);

    setWindowTitle(tr("Wood Generator"));
}



void Window::closeApp(){
    close();
}

void Window::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape)
        close();
    else
        QWidget::keyPressEvent(e);
}

