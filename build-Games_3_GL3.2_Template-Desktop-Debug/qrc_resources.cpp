/****************************************************************************
** Resource object code
**
** Created by: The Resource Compiler for Qt version 5.3.0
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <QtCore/qglobal.h>

static const unsigned char qt_resource_data[] = {
  // /home/p/ptlmuh006/Documents/prac2_opengl_intro/Games_3_GL3.2_Template/simple.vert
  0x0,0x0,0x0,0x67,
  0x23,
  0x76,0x65,0x72,0x73,0x69,0x6f,0x6e,0x20,0x33,0x33,0x30,0xa,0x69,0x6e,0x20,0x76,
  0x65,0x63,0x34,0x20,0x76,0x65,0x72,0x74,0x65,0x78,0x3b,0xa,0xa,0x75,0x6e,0x69,
  0x66,0x6f,0x72,0x6d,0x20,0x6d,0x61,0x74,0x34,0x20,0x4d,0x56,0x50,0x3b,0xa,0xa,
  0x76,0x6f,0x69,0x64,0x20,0x6d,0x61,0x69,0x6e,0x28,0x20,0x76,0x6f,0x69,0x64,0x20,
  0x29,0xa,0x7b,0xa,0x20,0x20,0x20,0x20,0x67,0x6c,0x5f,0x50,0x6f,0x73,0x69,0x74,
  0x69,0x6f,0x6e,0x20,0x3d,0x20,0x4d,0x56,0x50,0x20,0x2a,0x20,0x76,0x65,0x72,0x74,
  0x65,0x78,0x3b,0xa,0x7d,0xa,
    // /home/p/ptlmuh006/Documents/prac2_opengl_intro/Games_3_GL3.2_Template/simple.frag
  0x0,0x0,0x0,0x53,
  0x23,
  0x76,0x65,0x72,0x73,0x69,0x6f,0x6e,0x20,0x33,0x33,0x30,0xa,0x75,0x6e,0x69,0x66,
  0x6f,0x72,0x6d,0x20,0x76,0x65,0x63,0x34,0x20,0x66,0x63,0x6f,0x6c,0x6f,0x72,0x3b,
  0xa,0x76,0x6f,0x69,0x64,0x20,0x6d,0x61,0x69,0x6e,0x28,0x20,0x76,0x6f,0x69,0x64,
  0x20,0x29,0xa,0x7b,0xa,0x20,0x20,0x20,0x20,0x67,0x6c,0x5f,0x46,0x72,0x61,0x67,
  0x43,0x6f,0x6c,0x6f,0x72,0x20,0x3d,0x20,0x66,0x63,0x6f,0x6c,0x6f,0x72,0x3b,0xa,
  0x7d,0xa,
  
};

static const unsigned char qt_resource_name[] = {
  // simple.vert
  0x0,0xb,
  0x2,0xb1,0xc4,0x74,
  0x0,0x73,
  0x0,0x69,0x0,0x6d,0x0,0x70,0x0,0x6c,0x0,0x65,0x0,0x2e,0x0,0x76,0x0,0x65,0x0,0x72,0x0,0x74,
    // simple.frag
  0x0,0xb,
  0x2,0xb0,0xd0,0x97,
  0x0,0x73,
  0x0,0x69,0x0,0x6d,0x0,0x70,0x0,0x6c,0x0,0x65,0x0,0x2e,0x0,0x66,0x0,0x72,0x0,0x61,0x0,0x67,
  
};

static const unsigned char qt_resource_struct[] = {
  // :
  0x0,0x0,0x0,0x0,0x0,0x2,0x0,0x0,0x0,0x2,0x0,0x0,0x0,0x1,
  // :/simple.frag
  0x0,0x0,0x0,0x1c,0x0,0x0,0x0,0x0,0x0,0x1,0x0,0x0,0x0,0x6b,
  // :/simple.vert
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1,0x0,0x0,0x0,0x0,

};

QT_BEGIN_NAMESPACE

extern Q_CORE_EXPORT bool qRegisterResourceData
    (int, const unsigned char *, const unsigned char *, const unsigned char *);

extern Q_CORE_EXPORT bool qUnregisterResourceData
    (int, const unsigned char *, const unsigned char *, const unsigned char *);

QT_END_NAMESPACE


int QT_MANGLE_NAMESPACE(qInitResources_resources)()
{
    QT_PREPEND_NAMESPACE(qRegisterResourceData)
        (0x01, qt_resource_struct, qt_resource_name, qt_resource_data);
    return 1;
}

Q_CONSTRUCTOR_FUNCTION(QT_MANGLE_NAMESPACE(qInitResources_resources))

int QT_MANGLE_NAMESPACE(qCleanupResources_resources)()
{
    QT_PREPEND_NAMESPACE(qUnregisterResourceData)
       (0x01, qt_resource_struct, qt_resource_name, qt_resource_data);
    return 1;
}

Q_DESTRUCTOR_FUNCTION(QT_MANGLE_NAMESPACE(qCleanupResources_resources))

