#include "mainwindow.h"
#include "window.h"
#include <QMenuBar>
#include <QMenu>
#include <QMessageBox>
#include <iostream>

MainWindow::MainWindow()
{
    QMenuBar *menubar = new QMenuBar;
    QMenu *menu = menubar->addMenu(tr("&Window"));
    setMenuBar(menubar);
    setCentralWidget(new Window(this));
}
void MainWindow::closeApp(){
    close();
}

MainWindow::~MainWindow()
{

}
