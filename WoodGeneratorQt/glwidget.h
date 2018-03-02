#ifndef GLWIDGET_H
#define GLWIDGET_H
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <qopengl.h>
#include <QOpenGLTexture>

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:

    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;
    void updateColor(int n);
    void setColorSlide(int n);
    void setAnim();
    void setLightSlide(float uka,float ukd,float uks,float sh);


public slots:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);
    void setScale(float scale);
    void cleanup();
    void animate();

signals:
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void initializeGL() override;
    void paintGL() override;


private:
    void setupVertexAttribs();
    void initShaders();
    bool m_core;
    bool anim = 1;
    float uKa;
    float uKd;
    float uKs;
    float uShineness;
    float uNoiseAmp;
    static QVector4D color;
    static QVector3D lightPos;
    QOpenGLShaderProgram *m_program;
    GLuint VBO;
    GLuint VBO2;
    GLuint VAO;
    GLuint texID;
    QOpenGLTexture *texture;
    QMatrix4x4 model;
    QMatrix4x4 view;
    QMatrix4x4 proj;
    QMatrix4x4 normalMatrix;
    int m_xRot;
    int m_yRot;
    int m_zRot;
    float scale;
    QPoint m_lastPos;


};
#endif
