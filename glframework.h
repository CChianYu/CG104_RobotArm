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

#ifndef OPENGLTEMPLATE_H
#define OPENGLTEMPLATE_H

#include "glstdafx.h"

#include <vector>

#include <QVector2D>
#include <QVector3D>
#include <QMatrix4x4>
#include <QQuaternion>

#include <QOpenGLFunctions_1_1>
#include <QOpenGLWidget>
#include <QOpenGLTexture>

#include <QBasicTimer>

#include "trackballcamera.h"

YGL_BEGIN_NAMESPACE

// https://doc-snapshots.qt.io/qt5-5.4/qopenglwidget.html
class GLFramework : public QOpenGLWidget, protected QOpenGLFunctions_1_1
{
    Q_OBJECT

public:
    explicit GLFramework(QWidget *parent = 0);
    void setAction(int pose);
    ~GLFramework();

protected:
    /* 更多事件請參考 https://doc-snapshots.qt.io/qt5-5.4/qwidget.html */
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent * event);

    /* OpenGL 初始化實作在這 */
    void initializeGL();

    /* 當視窗大小被改變時此函式會被執行 */
    void resizeGL(int w, int h);

    /*** 將繪圖函式實作在這，當需要更新時此函式才會被執行 ***/
    /* 注意! 並不是每個 frame 都會繪製，可以用 this->update() 強制更新 */
    void paintGL();

    /* 預設每 16 ms 呼叫一次，方便控制動畫 */
    void timerEvent(QTimerEvent *e);

    void init();
    void standby();
    void draw_hand();
    void draw();
    void display();
    void stone_pose();
    void paper_pose();
    void scissor_pose();
    void one();
    void drawFinger1();
    void drawFinger2();
    void drawFinger3();
    void drawFinger4();
    void drawFinger5();

private:

    /* 將攝影機參數套用至 OpenGL */
    void applyCameraSetting(Camera &cam);

    void drawGrid(int size = 20);

    void drawLocator(float size = 3);

private:
    /* 用來設定 timerEvent 的頻率 */
    QBasicTimer timer;

    /* 用來記錄滑鼠座標 */
    QVector2D cachePostion;

public:
    bool enabledGrid;
    bool enabledLocator;

    TrackBallCamera trackBallCamera;

};

YGL_END_NAMESPACE

#endif // OPENGLTEMPLATE_H
