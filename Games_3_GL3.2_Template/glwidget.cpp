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

bool shadersLoaded = false;

float transStep = 0.2f;
float rotStep = 12.0f;
float scaleStep = 0.5f;

glm::mat4 translationMat, rotationMat, scaleMat;
glm::mat4 modelMat, view, projection;

GLuint activeAxis = 0; //x=0, y=1, z=2
GLuint activeTransformation = 0; //translate=0, rotate=1, scale=2

//constructor
GLWidget::GLWidget( const QGLFormat& format, QWidget* parent )
    : QGLWidget( format, parent ),
      m_vertexBuffer( QOpenGLBuffer::VertexBuffer ),
      red(1.0f),
      green(0.0f),
      blue(0.0f),
      model_filename("")
{
    //set up the menu bar
    mainMenu = new QMenuBar(this);
    fileMenu = new QMenu("File");
    openAction = fileMenu->addAction("Open");
    QObject::connect(openAction, SIGNAL(triggered()), this, SLOT(handle_open_clicked()));
    resetAction = fileMenu->addAction("New/Reset");
    QObject::connect(resetAction, SIGNAL(triggered()), this, SLOT(handle_reset_clicked()));
    mainMenu->addMenu(fileMenu);
}

//slots --------------------------------------------------------

//slot for Open action in the File menu
bool GLWidget::handle_open_clicked(){
    std::cout << "open" << std::endl;

    //show an open file dialog and get the selected file name
    model_filename = QFileDialog::getOpenFileName(this, tr("Open stl model"), "~", "Stl files (*.stl)").toStdString();

    //run intialiseGL to reset the scene and load up the new model
    initializeGL();
    return true;
}

//slot for reset action in the File menu
bool GLWidget::handle_reset_clicked(){
    std::cout << "reset" << std::endl;

    //run intialiseGL to reload the current  and reset the scene
    initializeGL();
    return true;
}

//slot for wheelEvent. Performs current ActiveTransformation in ActiveAxis on the model
void GLWidget::wheelEvent(QWheelEvent * evt){
    int delta = evt->delta();
    int dir = glm::abs(delta) / delta; //gets +1 or -1 based on direction scrolling

    //do the correct transformation
    switch(activeTransformation){
        //translate
        case 0:
            if(activeAxis == 0){
                translateModel(glm::vec3(dir * transStep, 0.0f, 0.0f));
                std::cout << "translate x" << std::endl;
            }else if(activeAxis == 1){
                translateModel(glm::vec3(0.0f, dir * transStep, 0.0f));
                std::cout << "translate y" << std::endl;
            }else{
                translateModel(glm::vec3(0.0f, 0.0f, dir * transStep));
                std::cout << "translate z" << std::endl;
            }
            break;

        //rotate
        case 1:
            if(activeAxis == 0){
                rotateModel(glm::vec3(1.0f, 0.0f, 0.0f), dir * rotStep);
                std::cout << "rotate x" << std::endl;
            }else if(activeAxis == 1){
                rotateModel(glm::vec3(0.0f, 1.0f, 0.0f), dir * rotStep);
                std::cout << "rotate y" << std::endl;
            }else{
                rotateModel(glm::vec3(0.0f, 0.0f, 1.0f), dir * rotStep);
                std::cout << "rotate z" << std::endl;
            }
            break;

        //scale
        case 2:
            if(activeAxis == 0){
                scaleModel(glm::vec3((dir * scaleStep) + 1.0f, 1.0f, 1.0f));
                std::cout << "scale x" << std::endl;
            }else if (activeAxis == 1){
                scaleModel(glm::vec3(1.0f, (dir * scaleStep) + 1.0f, 1.0f));
                std::cout << "scale y" << std::endl;
            }else{
                scaleModel(glm::vec3(1.0f, 1.0f, (dir * scaleStep) + 1.0f));
                std::cout << "scale z" << std::endl;
            }

    }

    //update the scene
    updateGL();
}

//handles keyPress input
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
            break;

        case Qt::Key_R:
            incrementActiveAxis();
            activeTransformation = 1;
            break;

        case Qt::Key_S:
            incrementActiveAxis();
            activeTransformation = 2;
            break;

        default:
            QGLWidget::keyPressEvent( e );
    }
}

//helpers-------------------------------------------------------------------------------

//loads in the model at location model_filename
void GLWidget::loadModel(){
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


    //shaders can only be loaded once. Don't want to run the code if we have already loaded
    //the shaders.
    if(!shadersLoaded){
        qDebug() << "Attempting vertex shader load from " << VERT_SHADER;
        qDebug() << "Attempting fragment shader load from " << FRAG_SHADER;

        // Prepare a complete shader program...
        if ( !prepareShaderProgram( VERT_SHADER, FRAG_SHADER) )
            std::runtime_error("Failed to load shader");

        shadersLoaded = true;
    }

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
}

//translates the model
void GLWidget::translateModel(glm::vec3 translation){
    //set the translation matrix to the new translation
    translationMat = glm::translate(translationMat, translation);

    //push changed MVP to vertexShader
    updateMVP();
}

//rotate the model
void GLWidget::rotateModel(glm::vec3 rotationAxis, float degrees){
    //set the rotation matrix to the new rotation
    rotationMat = glm::rotate(rotationMat, degrees, rotationAxis);

    //push changed MVP matrix to vertexShader
    updateMVP();
}

//scales the model
void GLWidget::scaleModel(glm::vec3 scaleFactor){
    //set the scale matrix to the newly generated scale matrix
    scaleMat = glm::scale(scaleMat, scaleFactor);

    //push the changed MVP matrix to the vertex shader
    updateMVP();
}

//updates the MVP and pushes it to the vertex shader
void GLWidget::updateMVP(){
    //build up the modelMatrix from all the transformation matrices
    modelMat = translationMat * rotationMat * scaleMat;

    //build up the MVP matrix from the moedl, view and projection matrices
    glm::mat4 MVP = projection * view * modelMat;
    //push MVP matrix to the vertexShader
    glUniformMatrix4fv(glGetUniformLocation(m_shader.programId(),"MVP"), 1, GL_FALSE, &MVP[0][0]);

    //Redraw/update the scene
    updateGL();
}

//increments the ActiveAxis for transformations (cycles in range from 0 to 2)
void GLWidget::incrementActiveAxis(){
    activeAxis++;
    if(activeAxis > 2){
        activeAxis = 0;
    }
}

//increments the transformation being performed by mousewheel (cycles in range 0 to 2)
void GLWidget::incrementActiveTransformation(){
    activeTransformation++;
    if(activeTransformation > 2){
        activeTransformation = 0;
    }
}

//Changes the render colour to one of 5 preset options
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

    //push new colour to fcolour uniform variable in the shader program
    glUniform4f(glGetUniformLocation(m_shader.programId(),"fcolor"), red, green, blue,1.0f);
    updateGL();
}

//GL-Functions----------------------------------------------------------------------------------------------------

//Loads neccessary components and sets up the OpenGl context
//Also loads and draws the model
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


    //If we have a model to load, then load it
    if(model_filename != ""){
        loadModel();
    }


    //set up the projection matrix (perspective projection for our 3d models)
    projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);

    //set up the view matrix (the "camera")
    glm::vec3 eye(0, 0, 3);
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

void GLWidget::resizeGL( int w, int h )
{
    // Set the viewport to window dimensions
    glViewport( 0, 0, w, qMax( h, 1 ) );
}

void GLWidget::paintGL()
{
    // Clear the buffer with the current clearing color
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // Draw stuff
    glDrawArrays( GL_TRIANGLES, 0,  model.numTriangles * 3);
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
