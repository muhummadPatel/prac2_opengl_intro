#ifndef GLWIDGET_H
#define GLWIDGET_H

#include "glheaders.h" // Must be included before QT opengl headers
#include <QGLWidget>

#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

#include <string>

#include "glm/glm.hpp"
#include "stlModel.h"

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    GLWidget( const QGLFormat& format, QWidget* parent = 0 );

public slots:
    //slots to handle QT events
    bool handle_open_clicked();
    bool handle_reset_clicked();
    void wheelEvent(QWheelEvent * evt);

protected:
    virtual void initializeGL();
    virtual void resizeGL( int w, int h );
    virtual void paintGL();

    virtual void keyPressEvent( QKeyEvent* e );

private:
    bool prepareShaderProgram( const QString& vertexShaderPath,
                               const QString& fragmentShaderPath );
    QOpenGLShaderProgram m_shader;
    QOpenGLBuffer m_vertexBuffer;

    //model related members
    stlModel model;
    std::string model_filename;
    void loadModel();

    //members used to allow for colour changing
    float red, green, blue;
    void setRenderColor(int opt);

    //used to keep track of which transformation and on which axis
    void incrementActiveAxis();
    void incrementActiveTransformation();

    //used to generate model matrix and upload MVP matrix to shader
    void translateModel(glm::vec3 translation);
    void rotateModel(glm::vec3 rotationAxis, float degrees);
    void scaleModel(glm::vec3 scaleFactor);
    void updateMVP();
};

#endif // GLWIDGET_H
