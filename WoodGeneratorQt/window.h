#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>



QT_BEGIN_NAMESPACE
class QPushButton;
QT_END_NAMESPACE

class GLWidget;
class MainWindow;

class Window : public QWidget
{
    Q_OBJECT

public:
    Window(MainWindow *mw);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void closeApp();

private:
    //QSlider *createSlider();

    GLWidget *glWidget;
    MainWindow *mainWindow;
    QPushButton *dockBtn;
};

#endif
