#include "glwidget.h"
#include "stlModel.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

#include <iostream>

#include <QCoreApplication>
#include <QKeyEvent>
#include <stdexcept>
#include <QMenuBar>
#include <QFileDialog>
#include <QLabel>

#define VERT_SHADER ":/simple.vert"
#define FRAG_SHADER ":/simple.frag"

QMenuBar* mainMenu;
QMenu* fileMenu;
QAction* openAction;
QAction* resetAction;
QLabel* modeLabel;

float transStep = 0.2f;
float rotStep = 12.0f;
float scaleStep = 0.5f;

glm::mat4 translationMat, rotationMat, scaleMat;
glm::mat4 modelMat, view, projection;

GLuint activeAxis = 0; //x=0, y=1, z=2
GLuint activeTransformation = 0; //translate=0, rotate=1, scale=2

GLWidget::GLWidget( const QGLFormat& format, QWidget* parent )
    : QGLWidget( format, parent ),
      m_vertexBuffer( QOpenGLBuffer::VertexBuffer ),
      red(1.0f),
      green(0.0f),
      blue(0.0f),
      model_filename("bunny.stl")
{
    mainMenu = new QMenuBar(this);
    fileMenu = new QMenu("File");
    openAction = fileMenu->addAction("Open");
    QObject::connect(openAction, SIGNAL(triggered()), this, SLOT(handle_open_clicked()));
    resetAction = fileMenu->addAction("New/Reset");
    QObject::connect(resetAction, SIGNAL(triggered()), this, SLOT(handle_reset_clicked()));
    mainMenu->addMenu(fileMenu);
}

bool GLWidget::handle_open_clicked(){
    //TODO: IS THIS ACTUALLY WORKING???
    std::cout << "open clicked" << std::endl;

    return true;
}

bool GLWidget::handle_reset_clicked(){
    //TODO: Actually implement this
    std::cout << "reset clicked" << std::endl;

    return true;
}

void GLWidget::wheelEvent(QWheelEvent * evt){
    int delta = evt->delta();
    int dir = glm::abs(delta) / delta; //gets +1 or -1 based on direction scrolling
    //std::cout << dir << std::endl;

    switch(activeTransformation){
        //translation
        case 0:
            if(activeAxis == 0){
                translateModel(glm::vec3(dir * transStep, 0.0f, 0.0f));
            }else if(activeAxis == 1){
                translateModel(glm::vec3(0.0f, dir * transStep, 0.0f));
            }else{
                translateModel(glm::vec3(0.0f, 0.0f, dir * transStep));
            }
            break;

        //rotate
        case 1:
            if(activeAxis == 0){
                rotateModel(glm::vec3(1.0f, 0.0f, 0.0f), dir * rotStep);
            }else if(activeAxis == 1){
                rotateModel(glm::vec3(0.0f, 1.0f, 0.0f), dir * rotStep);
            }else{
                rotateModel(glm::vec3(0.0f, 0.0f, 1.0f), dir * rotStep);
            }
            break;

        //scale
        case 2:
            if(activeAxis == 0){
                scaleModel(glm::vec3((dir * scaleStep) + 1.0f, 1.0f, 1.0f));
            }else if (activeAxis == 1){
                scaleModel(glm::vec3(1.0f, (dir * scaleStep) + 1.0f, 1.0f));
            }else{
                scaleModel(glm::vec3(1.0f, 1.0f, (dir * scaleStep) + 1.0f));
            }

    }

    updateGL();
}

void GLWidget::initializeGL(){
    // Resolve OpenGL functions
    glewExperimental = true;
    GLenum GlewInitResult = glewInit();
    if (GlewInitResult != GLEW_OK) {
        const GLubyte* errorStr = glewGetErrorString(GlewInitResult);
        size_t size = strlen(reinterpret_cast<const char*>(errorStr));
        qDebug() << "Glew error "
                 << QString::fromUtf8(
                        reinterpret_cast<const char*>(errorStr), size);
    }

    // get context opengl-version
    qDebug() << "Widget OpenGl: " << format().majorVersion() << "." << format().minorVersion();
    qDebug() << "Context valid: " << context()->isValid();
    qDebug() << "Really used OpenGl: " << context()->format().majorVersion() << "." << context()->format().minorVersion();
    qDebug() << "OpenGl information: VENDOR:       " << (const char*)glGetString(GL_VENDOR);
    qDebug() << "                    RENDERDER:    " << (const char*)glGetString(GL_RENDERER);
    qDebug() << "                    VERSION:      " << (const char*)glGetString(GL_VERSION);
    qDebug() << "                    GLSL VERSION: " << (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);

    QGLFormat glFormat = QGLWidget::format();
    if ( !glFormat.sampleBuffers() )
        qWarning() << "Could not enable sample buffers";

    // Set the clear color to black
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

    // we need a VAO in core!
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // We need us some vertex data. Start simple with a triangle ;-)
    model.read(model_filename);
    float* points = model.points;

    m_vertexBuffer.create();
    m_vertexBuffer.setUsagePattern( QOpenGLBuffer::StaticDraw );
    if ( !m_vertexBuffer.bind() )
    {
        qWarning() << "Could not bind vertex buffer to the context";
        return;
    }
    m_vertexBuffer.allocate( points, model.numTriangles * 3 * 4 * sizeof( float ) );

    qDebug() << "Attempting vertex shader load from " << VERT_SHADER;
    qDebug() << "Attempting fragment shader load from " << FRAG_SHADER;

    // Prepare a complete shader program...
    if ( !prepareShaderProgram( VERT_SHADER, FRAG_SHADER) )
        std::runtime_error("Failed to load shader");
    // Bind the shader program so that we can associate variables from
    // our application to the shaders
    if ( !m_shader.bind() )
    {
        qWarning() << "Could not bind shader program to context";
        return;
    }
    // Enable the "vertex" attribute to bind it to our currently bound
    // vertex buffer.
    m_shader.setAttributeBuffer( "vertex", GL_FLOAT, 0, 4 );
    m_shader.enableAttributeArray( "vertex" );
    setRenderColor(1);

    //set up the projection matrix (perspective projection for our 3d models)
    projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);

    //set up the view matrix (the "camera")
    glm::vec3 eye(0, 0, 2);
    glm::vec3 center(0, 0, 0);
    glm::vec3 up(0, 1, 0);
    view = glm::lookAt(eye, center, up);

    //set up the model matrices (initialise all transformation matrices to id matrix)
    translationMat = glm::mat4(1.0f);
    rotationMat = glm::mat4(1.0f);
    scaleMat = glm::mat4(1.0f);

    //create MVP matrix ad push it to the vertex shader
    updateMVP();
}

void GLWidget::translateModel(glm::vec3 translation){
    translationMat = glm::translate(translationMat, translation);

    updateMVP();
}

void GLWidget::rotateModel(glm::vec3 rotationAxis, float degrees){
    rotationMat = glm::rotate(rotationMat, degrees, rotationAxis);

    updateMVP();
}

void GLWidget::scaleModel(glm::vec3 scaleFactor){
    scaleMat = glm::scale(scaleMat, scaleFactor);

    updateMVP();
}

void GLWidget::updateMVP(){
    modelMat = translationMat * rotationMat * scaleMat;

    glm::mat4 MVP = projection * view * modelMat;
    glUniformMatrix4fv(glGetUniformLocation(m_shader.programId(),"MVP"), 1, GL_FALSE, &MVP[0][0]);
}

void GLWidget::resizeGL( int w, int h )
{
    //std::cout << "RESIZE" << std::endl;
    // Set the viewport to window dimensions
    glViewport( 0, 0, w, qMax( h, 1 ) );
}

void GLWidget::paintGL()
{
    // Clear the buffer with the current clearing color
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    //glColor3f(red, green, blue);
    //std::cout << "PAINTING" << std::endl;

    // Draw stuff
    glDrawArrays( GL_TRIANGLES, 0,  model.numTriangles * 3);
}

void GLWidget::keyPressEvent( QKeyEvent* e )
{
    switch ( e->key() )
    {
        case Qt::Key_Escape:
            QCoreApplication::instance()->quit();
            break;

        case Qt::Key_1:
            std::cout << "change col 1" << std::endl;
            setRenderColor(1);
            break;

        case Qt::Key_2:
            std::cout << "change col 2" << std::endl;
            setRenderColor(2);
            break;

        case Qt::Key_3:
            std::cout << "change col 3" << std::endl;
            setRenderColor(3);
            break;

        case Qt::Key_4:
            std::cout << "change col 4" << std::endl;
            setRenderColor(4);
            break;

        case Qt::Key_5:
            std::cout << "change col 5" << std::endl;
            setRenderColor(5);
            break;

        case Qt::Key_T:
            incrementActiveAxis();
            activeTransformation = 0;
            std::cout << "translate " << activeTransformation << " " << activeAxis << std::endl;
            break;

        case Qt::Key_R:
            incrementActiveAxis();
            activeTransformation = 1;
            std::cout << "rotate " << activeTransformation << " " << activeAxis << std::endl;
            break;

        case Qt::Key_S:
            incrementActiveAxis();
            activeTransformation = 2;
            std::cout << "scale " << activeTransformation << " " << activeAxis << std::endl;
            break;

        default:
            QGLWidget::keyPressEvent( e );
    }
}

void GLWidget::incrementActiveAxis(){
    activeAxis++;
    if(activeAxis > 2){
        activeAxis = 0;
    }
}

void GLWidget::incrementActiveTransformation(){
    activeTransformation++;
    if(activeTransformation > 2){
        activeTransformation = 0;
    }
}

void GLWidget::setRenderColor(int opt){
    switch(opt){
        case 1:
            //red
            red = 1.0f;
            blue = green = 0.0f;
            break;

        case 2:
            //green
            green = 1.0f;
            blue = red = 0.0f;
            break;

        case 3:
            //blue
            blue = 1.0f;
            red = green = 0.0f;
            break;

        case 4:
            //yellow
            red = green = 1.0f;
            blue = 0.0f;
            break;
        case 5:
            //magenta
            red = blue = 1.0f;
            green = 0.0f;
            break;

        default:
            //red
            setRenderColor(1);
    }

    glUniform4f(glGetUniformLocation(m_shader.programId(),"fcolor"), red, green, blue,1.0f);
    updateGL();
}

bool GLWidget::prepareShaderProgram( const QString& vertexShaderPath,
                                     const QString& fragmentShaderPath )
{
    // First we load and compile the vertex shader...
    bool result = m_shader.addShaderFromSourceFile( QOpenGLShader::Vertex, vertexShaderPath );
    if ( !result )
        qWarning() << m_shader.log();

    // ...now the fragment shader...
    result = m_shader.addShaderFromSourceFile( QOpenGLShader::Fragment, fragmentShaderPath );
    if ( !result )
        qWarning() << m_shader.log();

    // ...and finally we link them to resolve any references.
    result = m_shader.link();
    if ( !result )
        qWarning() << "Could not link shader program:" << m_shader.log();

    return result;
}
