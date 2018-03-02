#include "glwidget.h"
#include <QMouseEvent>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QCoreApplication>
#include <QTimer>
#include <math.h>
#include <iostream>
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/noise.hpp"


typedef QVector4D vec4;
typedef QVector3D vec3;
typedef QMatrix4x4 mat4;

QVector4D GLWidget::color = vec4(0.0,0.0,0.0,1.0);

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      uKa(0.2),
      uKd(.85),
      uKs(.2),
      uShineness(20.0),
      uNoiseAmp(10.0),
      texture(0),
      scale(1.0),
      m_xRot(0),
      m_yRot(0),
      m_zRot(0)
{
    m_core = QSurfaceFormat::defaultFormat().profile() == QSurfaceFormat::CoreProfile;
}

GLWidget::~GLWidget()
{
    cleanup();
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
    return QSize(400, 400);
}


void GLWidget::cleanup()
{
  //std::cout<<"Exiting\n";
}

void GLWidget::updateColor(int n){
    //std::cout<<"Hit update color\n";
    std::cout.flush();
    if(n==1)
        color = vec4(1.0,0.0,0.0,1.0);
    if(n==2)
        color = vec4(0.0,0.0,1.0,1.0);
    if(n==3)
        color = vec4(1.0,0.0,1.0,1.0);
    update();
}

void GLWidget::setLightSlide(float ka,float kd,float ks,float sh)
{
    uKa = (float)ka/100.0;
    uKd = (float)kd/100.0;
    uKs = (float)ks/100.0;
    uShineness = (float)sh;
    update();
}

void GLWidget::setColorSlide(int n){
    float val = (float)n;
    uNoiseAmp = val;
    update();
}

void GLWidget::setScale(float s){
    scale = s;
    update();
}



void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    GLfloat vertices[] ={
     -.75, -0.75, 0.0, 1.0,
     0.75,  0.75, 0.0, 1.0,
     -0.75, 0.75, 0.0, 1.0,
     -.75, -0.75, 0.0, 1.0,
     0.75, -0.75, 0.0, 1.0,
     0.75, 0.750, .0, 1.0};

    GLfloat ST[] =
    {0.0,0.0,
     1.0,1.0,
     0.0,1.0,//second
     0.0,0.0,
     1.0,0.0,
     1.0,1.0
    };
    glClearColor(0, 0, 0, 1);
    initShaders();

    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glGenBuffers(1,&VBO2);

    glBindVertexArray(VAO);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,24*sizeof(float),vertices,GL_STATIC_DRAW);
    glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,4*sizeof(GLfloat),(GLvoid*)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER,12*sizeof(float),ST,GL_STATIC_DRAW);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,2*sizeof(GLfloat),(GLvoid*)0);

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    int width = 128;
    int height = 128;
    GLubyte *data = new GLubyte[width*height*4];
    float xFactor = 1.0f / (width-1);
    float yFactor = 1.0f / (height-1);
    float a = 3.0;
    float b = 3.0;
    for(int row = 0;row<height;row++)
    {
        for(int col = 0;col<width;col++)
        {
            float x = xFactor*col;
            float y = yFactor*row;
            float sum = 0.0f;
            float freq = a;
            float scale = b;

            for(int oct = 0;oct<4;oct++)
            {
                glm::vec2 p(x*freq,y*freq);
                float val = 0.0f;
                int periodic = 1;
                if(periodic)
                {
                    val = glm::perlin(p,glm::vec2(freq))/scale;
                }
                else
                {
                    val = glm::perlin(p)/scale;
                }
                sum+=val;
                float result = (sum+1.0f)/2.0f;

                data[((row*width+col)*4)+oct]=(GLubyte)(result*255.0f);
                freq *= 2.0f;
                scale*= b;
            }
        }
    }

    texture = new QOpenGLTexture(QImage(data,width,height,QImage::Format_RGBA8888));

    // Set nearest filtering mode for texture minification
    texture->setMinificationFilter(QOpenGLTexture::Nearest);

    // Set bilinear filtering mode for texture magnification
    texture->setMagnificationFilter(QOpenGLTexture::Linear);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    texture->setWrapMode(QOpenGLTexture::Repeat);

    delete [] data;

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
void GLWidget::setAnim(){
    anim = !anim;
    update();
}
void GLWidget::animate()
{
    //std::cout<<"Hit rot= "<<rot<<"\n";
    //std::cout.flush();
    update();
}

void GLWidget::initShaders()
{

    m_program = new QOpenGLShaderProgram;
    // Compile vertex shader
    if (!m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/wood.vert")){
        std::cout<<"Vertex Compile Error\n";
        close();
    }
    // Compile fragment shader
    if (!m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/wood.frag"))
    {
        std::cout<<"Fragment Compile Error\n";
        close();
    }
    // Link shader pipeline
    if (!m_program->link())
    {
        std::cout<<"Link Error\n";
        close();
    }
    m_program->bindAttributeLocation("uPos", 0);
    m_program->bindAttributeLocation("ST", 1);

    // Bind shader pipeline for use
    if (!m_program->bind())
    {
        std::cout<<"Bind Error\n";
        close();
    }
}

void GLWidget::paintGL()
{
    glClearColor(color.x(), color.y(), color.z(), 1);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    // Reset projection
    proj.setToIdentity();

    // Set perspective projection
    proj.perspective(90.0, 4.0/3.0, .1, 100.0);
    model.setToIdentity();
    model.scale(scale);
    model.rotate(m_xRot / 16.0f, 1, 0, 0);
    model.rotate(m_yRot / 16.0f, 0, 1, 0);
    model.rotate(-m_zRot / 16.0f, 0, 0, 1);

    view.setToIdentity();
    view.lookAt(vec3(0.0,0.0,1.0),vec3(0.0,0.0,0.0),vec3(0.0,1.0,0.0));

    QMatrix4x4 slice;
    slice.setToIdentity();
    slice.scale(vec3(50.0,50.0,50.0));
    slice.translate(vec3(0.0,1.0,2.0));
    slice.rotate(45.0,vec3(0.0,0.0,1.0));
    slice.rotate(45.0,vec3(1.0,0.0,0.0));

    m_program->bind();
    m_program->setUniformValue("Model",model);
    m_program->setUniformValue("View",view);
    m_program->setUniformValue("Proj",proj);
    m_program->setUniformValue("slice",slice);
    //lightPos = vec3(0.0,0.0,5.0);
    //float x = 0.0;
   // float y = 0.0;
   // float z = 5.0;
    //m_program->setUniformValue("uLightX",x);
    //m_program->setUniformValue("uLightY",y);
   // m_program->setUniformValue("uLightZ",z);
    normalMatrix = view*model;
    m_program->setUniformValue("normalMatrix",normalMatrix);
    glActiveTexture(GL_TEXTURE0);
    m_program->setUniformValue("Tex1",0);
    m_program->setUniformValue("uNoiseAmp",uNoiseAmp);
    m_program->setUniformValue("uKa",uKa);
    m_program->setUniformValue("uKd",uKd);
    m_program->setUniformValue("uKs",uKs);
    m_program->setUniformValue("shineness",uShineness);

    texture->bind();
    glBindVertexArray( VAO );

    glDrawArrays(GL_TRIANGLES,0,6);
    glBindVertexArray( 0 );
    if(anim)
        QTimer::singleShot(16, this, &GLWidget::animate);
}

static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}

void GLWidget::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_xRot) {
        m_xRot = angle;
        emit xRotationChanged(angle);
        update();
    }
}

void GLWidget::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_yRot) {
        m_yRot = angle;
        emit yRotationChanged(angle);
        update();
    }
}

void GLWidget::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_zRot) {
        m_zRot = angle;
        emit zRotationChanged(angle);
        update();
    }
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    m_lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - m_lastPos.x();
    int dy = event->y() - m_lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(m_xRot + 8 * dy);
        setYRotation(m_yRot + 8 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(m_xRot + 8 * dy);
        setZRotation(m_zRot + 8 * dx);
    } else if (event->buttons() & Qt::MiddleButton) {

        scale += 0.002*((float)dx-(float)dy);
    }
    m_lastPos = event->pos();
}



