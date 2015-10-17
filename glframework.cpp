/****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include <iostream>

#include "glframework.h"

#include <GLUT/glut.h>

#include <QMouseEvent>

#include <QtMath>

using namespace std;

YGL_USE_NAMESPACE

static GLuint inner_mode = GL_SMOOTH;
static int action=0;

float anklea=0,		anklex=0,		ankley=0,		anklez=1;		//手肘
float plama,		plamx=0,		plamy=0,	plamz=0;			//手掌 (靜止)
float anklef1a[2],	anklef1x[2],	anklef1y[2],	anklef1z[2];	//拇指
float anklef2a[3],	anklef2x[3],	anklef2y[3],	anklef2z[3];	//食指
float anklef3a[3],	anklef3x[3],	anklef3y[3],	anklef3z[3];	//中指
float anklef4a[3],	anklef4x[3],	anklef4y[3],	anklef4z[3];	//無名指
float anklef5a[3],	anklef5x[3],	anklef5y[3],	anklef5z[3];	//小指
//	  ^^^轉動量		^^^轉動軸x		^^^轉動軸y		^^^轉動軸z

int state=1;

enum {STANDBY=0, ROCK, ONE};

GLFramework::GLFramework(QWidget *parent)
    : QOpenGLWidget(parent)
    , enabledGrid(false)
    , enabledLocator(false)
{
}

GLFramework::~GLFramework()
{
}

void GLFramework::mousePressEvent(QMouseEvent *e)
{
    // Save mouse press position
    cachePostion = QVector2D(e->localPos());
}

void GLFramework::mouseMoveEvent(QMouseEvent *e)
{
    QVector2D diff = QVector2D(e->localPos()) - cachePostion;
    cachePostion = QVector2D(e->localPos());

    trackBallCamera.rotate( diff / qMax(width(),height()) * 180.0f );

    this->update();
}

void GLFramework::wheelEvent(QWheelEvent *event)
{
    qreal delta = event->angleDelta().x() + event->angleDelta().y();

    trackBallCamera.zoom( - delta / 360.0f );

    this->update();
}

void GLFramework::timerEvent(QTimerEvent *)
{

    switch(action)
    {
    case STANDBY:
        //cout << "standby" << endl;
        standby();
        break;
    case ONE:
        //cout << "one" << endl;
        one();
        break;
    case ROCK:
        //cout << "rock" << endl;
        rock_pose();
        break;
    }

     this->update();
}

void GLFramework::setPose(int pose)
{
    standby();

    switch(pose)
    {
        case 0:
            action = STANDBY;
        break;
    case 2:
        action = ONE;
        break;
    case 1:
        action = ROCK;
        break;
    default:
        cout << "Undefined Pose:" << pose << endl;
    }
    cout << "Anklea " << anklea << endl;
}


void GLFramework::initializeGL()
{
    /* 相當重要的初始化函式 */
    initializeOpenGLFunctions();

    glClearColor(0, 0, 0, 1);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);


    init();
    //standby();

    /* 將自己的 timerEvent 設定為每 16 ms 呼叫一次 */
    timer.start(16, this);
}

void GLFramework::resizeGL(int w, int h)
{
    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 3.0, far plane to 300.0, field of view 45 degrees
    const qreal zNear = 0.1, zFar = 300.0, fov = 45.0;

    trackBallCamera.setPerspective(fov, aspect, zNear, zFar);
    // trackBallCamera.setOrthogonal(0, w, 0, h, zNear, zFar);
}


void GLFramework::standby(){
    state=1;
    anklea=120,anklex=0,ankley=0,anklez=1;  //手掌
    plama=0,plamx=0,plamy=0,plamz=0;  //手掌

    //-----------------------------------------
    anklef2a[0]=15;		//食指接掌
    anklef2x[0]=0;
    anklef2y[0]=0;
    anklef2z[0]=1;

    anklef2a[1]=-10;	//食指1
    anklef2x[1]=0;
    anklef2y[1]=0;
    anklef2z[1]=1;

    anklef2a[2]=-10;	//食指2
    anklef2x[2]=0;
    anklef2y[2]=0;
    anklef2z[2]=1;


    cout << "Calling StandBy" << endl;
}

void GLFramework::drawFinger2()
{
    //------------------------------------------------------------食指
    glTranslatef(-7, -5, 0.0);
    glRotatef(anklef2a[0],anklef2x[0],anklef2y[0], anklef2z[0]);

    glPushMatrix();
    glColor3ub(0,0,0);
    glutSolidSphere(1.5, 30.0, 30.0);
    glPopMatrix();

    //-----------------------------
    glTranslatef(-4,0,0);//食指節3

    glPushMatrix();
    glScalef(1.2, 0.6, 1);
    glColor3ub(255,177,98);
    glutSolidSphere(2.5, 30.0, 30.0);
    glPopMatrix();

    //-----------------------------
    glTranslatef(-4,0,0);//食指中關節
    glRotatef(anklef2a[1],anklef2x[1],anklef2y[1], anklef2z[1]);

    glPushMatrix();
    glColor3ub(0,0,0);
    glutSolidSphere(1.0,30.0,30.0);
    glPopMatrix();

    //-----------------------------
    glTranslatef(-3.0,0,0.0);//食指節2

    glPushMatrix();
    glScalef(1,0.6,1);
    glColor3ub(89,197,255);
    glutSolidSphere(2.0,30.0,30.0);
    glPopMatrix();

    //-----------------------------
    glTranslatef(-3, 0, 0.0);//食指上關節
    glRotatef(anklef2a[2],anklef2x[2],anklef2y[2], anklef2z[2]);

    glPushMatrix();
    glColor3ub(0,0,0);
    glutSolidSphere(1.0,30.0,30.0);
    glPopMatrix();

    //-----------------------------
    glTranslatef(-2.5,0.0,0.0); //食指節3

    glPushMatrix();
    glRotatef(20.0,0,1,1.0);
    glScalef(1.0,0.6,1.0);
    glColor3ub(0,177,98);
    glutSolidSphere(1.7, 30.0, 30.0);
    glPopMatrix();
}

void GLFramework::draw_hand()
{
    glPushMatrix();						//上臂
    glScalef(4, 1, 1);
    glColor3ub(255,177,98);
    glutSolidSphere(7.0, 30.0, 30.0);
    glPopMatrix();

    glTranslatef(anklex-25, 0.0, 0.0);	//肘關節
    glRotatef(anklea, anklex, ankley, anklez);

    glPushMatrix();						//肘關節
    glColor3ub(219,156,53);
    glutSolidSphere(5.0, 30.0, 30.0);
    glPopMatrix();

    glTranslatef(-22.0, 0.0, 0.0);		//下臂

    glPushMatrix();						//下臂
    glScalef(4, 1, 1);
    glColor3ub(255,177,98);
    glutSolidSphere(6.0, 30.0, 30.0);
    glPopMatrix();

    glTranslatef(-22.0, 0.0, 0.0);		//掌關節
    glRotatef(0, 0, 0, 0);

    glPushMatrix();						//掌關節
    glColor3ub(219,156,53);
    glutSolidSphere(3.0, 30.0, 30.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-10.0, 0.0, 3);		//掌

    glPushMatrix();
    glColor3ub(255,102,125);
    glScalef(1, 1, 0.5);
    glutSolidSphere(8, 30.0, 30.0);
    glPopMatrix();
    glPopMatrix();

    glTranslatef(-10.0, 0.0, -3);//原點位置：食指與掌接點

    glPushMatrix();//食指接掌關節

    drawFinger2();

    glPopMatrix();

    //-------------------------------make Finger3
    glPushMatrix();

    glTranslatef(2.0, 4.0, -3);
    glRotatef(-20, 0, 0, 1);
    drawFinger2();

    glPopMatrix();

    //-------------------------------make Finger4
    glPushMatrix();

    glTranslatef(4.0, 6.0, -3);
    glRotatef(-40, 0, 0, 1);
    drawFinger2();

    glPopMatrix();

    //-------------------------------make Finger5
    glPushMatrix();

    glTranslatef(10.0, 8.0, -3);
    glRotatef(-50, 0, 0, 1);
    drawFinger2();

    glPopMatrix();
    //-------------------------------make Finger1
    glPushMatrix();

    glTranslatef(4.0, 0.0, -3);
    glRotatef(30, 0, 0, 1);
    drawFinger2();

    glPopMatrix();
}


void GLFramework::draw()
{
    glTranslatef(55.0, 0.0, -200.0);
    glRotatef(180, 1, 0, 0);            //角度,X,Y,Z
    draw_hand();
}


void GLFramework::display()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glShadeModel(inner_mode);

    //Viewport 1
    //glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
    //glViewport(0,0,WinWidth,WinHeight);
    //gluPerspective(45.0, 1.0, 1.0, 400);// fov, aspect ratio ,near,far
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    draw();

    //Viewport 2
    //glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
    //glViewport(600,0,600,600);
    //gluPerspective(45.0, 1.0, 1.0, 400);
    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();
    //gluLookAt(100, 0, -10, 0, 0, -200, 0.0, 1.0, 0.0);
    //draw();


}

void GLFramework::init()
{
    GLfloat  ambientLight[] = { 1, 1, 1, 0.0f };
    GLfloat  diffuseLight[] = { 0, 0, 0, 1.0f };
    GLfloat  specular[] = { 0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat  ambientLight2[] = { 0.9f, 0.1f, 0.1f, 1.0f };
    GLfloat  specular2[] = { 0.3f, 0.3f, 0.3f, 1.3f};
    GLfloat  specref[] =  { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat  pos[] =  { 0.0f, 20.0f, -100.0f, 1.0f };
    GLfloat  dir[] =  { 0.0f, 0.0f, -1.0f};

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);

    // Setup and enable light 0
    glLightfv(GL_LIGHT0,GL_AMBIENT,ambientLight);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuseLight);
    glLightfv(GL_LIGHT0,GL_SPECULAR,specular);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT1,GL_AMBIENT,ambientLight2);
    glLightfv(GL_LIGHT1,GL_DIFFUSE,ambientLight2);
    glLightfv(GL_LIGHT1,GL_SPECULAR,specular2);
    glLightfv(GL_LIGHT1,GL_SPOT_DIRECTION,dir);
    glLightf(GL_LIGHT1,GL_SPOT_CUTOFF,60);
    glLightf(GL_LIGHT1,GL_SPOT_EXPONENT,100);
    glLightfv(GL_LIGHT1,GL_POSITION,pos);

    // Enable color tracking
    glEnable(GL_COLOR_MATERIAL);

    // Set Material properties to follow glColor values
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    // All materials hereafter have full specular reflectivity with a high shine
    glMaterialfv(GL_FRONT, GL_SPECULAR,specref);
    glMateriali(GL_FRONT,GL_SHININESS,128);

    //glGenTextures(2, &texture[0]);
    //glBindTexture(GL_TEXTURE_2D, texture[0]);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glClearColor(1.0f,1.0f,1.0f,0.0f);
}

void GLFramework::rock_pose()
{
    if(anklea>=0)
        anklea--;
    state=0;
    //-----------------------------------------
    if(anklef2a[0]>=-90)
        anklef2a[0]--;    //食指接掌
    anklef2x[0]=0;
    anklef2y[0]=1;
    anklef2z[0]=0;

    if(anklef2a[1]>=-90)
        anklef2a[1]--;    //食指1
    anklef2x[1]=0;
    anklef2y[1]=1;
    anklef2z[1]=0;

    if(anklef2a[2]>=-90)
        anklef2a[2]--;    //食指2
    anklef2x[2]=0;
    anklef2y[2]=1;
    anklef2z[2]=0;

    cout << "Calling Rock" << endl;
}

void GLFramework::one()                 //1
{
    if(anklea>=60)
        anklea--;
    state=0;

    //-----------------------------------------
    anklef2x[0]=0;    //食指接掌
    anklef2y[0]=0;
    anklef2z[0]=0;

    anklef2x[1]=0;    //食指1
    anklef2y[1]=0;
    anklef2z[1]=0;

    anklef2x[2]=0;    //食指2
    anklef2y[2]=0;
    anklef2z[2]=0;

    cout << "Calling One" << endl;
}
void GLFramework::paintGL()
{
    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    display();

    applyCameraSetting( trackBallCamera );

    if ( enabledLocator ) drawLocator(3);

    if ( enabledGrid ) drawGrid(20);
}

void GLFramework::applyCameraSetting(Camera &cam)
{
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(cam.getProjectMatrix().data());

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(cam.transform.getWorldToObjectMatrix().data());
}

void GLFramework::drawGrid(int size)
{
    glLineWidth(1.0f);
    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_LINES);
    for(int i = -size; i <= size; i++)
    {
        glVertex3f(    i, 0, size);
        glVertex3f(    i, 0,-size);
        glVertex3f( size, 0,    i);
        glVertex3f(-size, 0,    i);
    }
    glEnd();
}

void GLFramework::drawLocator(float size)
{
    glLineWidth(3.0f);

    glBegin(GL_LINES);
    {
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(0,0,0);
        glVertex3f(size,0,0);

        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0,0,0);
        glVertex3f(0,size,0);

        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0,0,0);
        glVertex3f(0,0,size);
    }
    glEnd();
}