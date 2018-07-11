/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Copyright (C) 2014 Jolla Ltd, author: <gunnar.sletta@jollamobile.com>
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Scenegraph Playground module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef CONTEXT_H
#define CONTEXT_H

#include <private/qsgdefaultcontext_p.h>
#include <private/qsgdefaultrendercontext_p.h>
#include <QtCore/QElapsedTimer>
#include <QtGui/QOpenGLShaderProgram>

#ifdef CUSTOMCONTEXT_DITHER
#include "renderhooks/ordereddither2x2.h"
#endif

QT_BEGIN_NAMESPACE
struct QSGMaterialType;
class QSGMaterialShader;
QT_END_NAMESPACE

namespace CustomContext
{

class RenderContext : public QSGDefaultRenderContext
{
public:
    RenderContext(QSGContext *ctx);
    void initialize(void *context) override;
    void invalidate() override;
    void renderNextFrame(QSGRenderer *renderer, GLuint fbo) override;

    QSGTexture *createTexture(const QImage &image, uint flags) const override;

    QSGRenderer *createRenderer() override;

#ifdef PROGRAM_BINARY
    void compileShader(QSGMaterialShader *shader, QSGMaterial *material, const char *vertex = 0, const char *fragment = 0) override;
#endif

#ifdef CUSTOMCONTEXT_DITHER
    bool m_dither;
    OrderedDither2x2 *m_ditherProgram;
#endif

};

class Context : public QSGDefaultContext
{
    Q_OBJECT
public:
    explicit Context(QObject *parent = 0);

    QSGRenderContext *createRenderContext() override { return new RenderContext(this); }
    void renderContextInitialized(QSGRenderContext *renderContext) override;

    QAnimationDriver *createAnimationDriver(QObject *parent) override;
#ifdef CUSTOMCONTEXT_SURFACEFORMAT
    QSurfaceFormat defaultSurfaceFormat() const;
#endif
    QQuickTextureFactory *createTextureFactory(const QImage &image);

#ifdef CUSTOMCONTEXT_EGLGRALLOCTEXTURE
    bool hasEglGrallocTextures() const { return m_eglGrallocTexture; }
#endif

#ifdef CUSTOMCONTEXT_HYBRISTEXTURE
    bool hasHybrisTextures() const { return m_hybrisTexture; }
#endif

private:

    int m_sampleCount;
    uint m_useMultisampling : 1;
    uint m_depthBuffer : 1;

#ifdef CUSTOMCONTEXT_NONPRESERVEDTEXTURE
    bool m_nonPreservedTexture;
    friend class NonPreservedTextureFactory;
#endif

#ifdef CUSTOMCONTEXT_ANIMATIONDRIVER
    bool m_animationDriver;
#endif

#ifdef CUSTOMCONTEXT_SWAPLISTENINGANIMATIONDRIVER
    bool m_swapListeningAnimationDriver;
#endif

#ifdef CUSTOMCONTEXT_EGLGRALLOCTEXTURE
    bool m_eglGrallocTexture;
#endif

#ifdef CUSTOMCONTEXT_HYBRISTEXTURE
    bool m_hybrisTexture;
#endif

};

} // namespace

#endif // CONTEXT_H
