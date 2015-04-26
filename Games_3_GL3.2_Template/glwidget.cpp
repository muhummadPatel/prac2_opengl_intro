#include "glwidget.h"
#include "stlModel.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>

#include <QCoreApplication>
#include <QKeyEvent>
#include <stdexcept>

#define VERT_SHADER ":/simple.vert"
#define FRAG_SHADER ":/simple.frag"

GLWidget::GLWidget( const QGLFormat& format, QWidget* parent )
    : QGLWidget( format, parent ),
      m_vertexBuffer( QOpenGLBuffer::VertexBuffer ),
      red(1.0f),
      green(0.0f),
      blue(0.0f)
{
}

void GLWidget::initializeGL()
{
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
//    float points[] = { -0.5f, -0.5f, 0.0f, 1.0f,
//                        0.5f, -0.5f, 0.0f, 1.0f,
//                        0.0f,  0.5f, 0.0f, 1.0f };
    model.read("bunny.stl");
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
    glUniform4f(glGetUniformLocation(m_shader.programId(),"fcolor"), red, green, blue, 1.0f);

    glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);

    glm::vec3 eye(0, 0, 2);
    glm::vec3 center(0, 0, 0);
    glm::vec3 up(0, 1, 0);
    glm::mat4 view = glm::lookAt(eye, center, up);

    glm::mat4 model = glm::mat4(1.0f);

    glm::mat4 MVP = projection * view * model;
    for(int r = 0; r < 4; r++){
        for(int c = 0; c < 4; c++){
            std::cout << MVP[r][c] << " ";
        }
        std::cout << std::endl;
    }
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

        default:
            QGLWidget::keyPressEvent( e );
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
