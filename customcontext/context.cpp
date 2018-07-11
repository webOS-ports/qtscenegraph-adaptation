/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
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

#include "context.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QElapsedTimer>

#include <QtGui/QWindow>
#include <QtGui/QOpenGLContext>

#include <QtQuick/QSGFlatColorMaterial>
#include <QtQuick/QSGVertexColorMaterial>
#include <QtQuick/QSGOpaqueTextureMaterial>
#include <QtQuick/QSGTextureMaterial>
#include <QtQuick/QQuickWindow>
#include <private/qsgdefaultimagenode_p.h>
#include <private/qsgdefaultrectanglenode_p.h>
#include <private/qsgdistancefieldglyphnode_p_p.h>
#include <private/qsgdefaultglyphnode_p.h>

#ifdef CUSTOMCONTEXT_ANIMATIONDRIVER
#include "animation/animationdriver.h"
#endif

#ifdef CUSTOMCONTEXT_SWAPLISTENINGANIMATIONDRIVER
#include "animation/swaplisteninganimationdriver.h"
#endif

#ifdef CUSTOMCONTEXT_SIMPLERENDERER
#include "renderer/simplerenderer.h"
#endif

#ifdef CUSTOMCONTEXT_NONPRESERVEDTEXTURE
#include "nonpreservedtexture.h"
#endif

#ifdef CUSTOMCONTEXT_EGLGRALLOCTEXTURE
#include "eglgralloctexture.h"
#endif

#ifdef CUSTOMCONTEXT_HYBRISTEXTURE
#include "hybristexture.h"
#endif

#ifndef QSG_NO_RENDERER_TIMING
static bool qsg_render_timing = !qgetenv("QSG_RENDER_TIMING").isEmpty();
#endif

namespace CustomContext
{

RenderContext::RenderContext(QSGContext *ctx)
    : QSGDefaultRenderContext(ctx)
{
#ifdef CUSTOMCONTEXT_DITHER
    m_dither = qgetenv("CUSTOMCONTEXT_NO_DITHER").isEmpty();
    m_ditherProgram = 0;
    qDebug(" - ordered 2x2 dither: %s", m_dither ? "yes" : "no");
#endif
}

Context::Context(QObject *parent)
    : QSGDefaultContext(parent)
    , m_sampleCount(0)
    , m_useMultisampling(false)
#ifdef CUSTOMCONTEXT_HYBRISTEXTURE
    , m_hybrisTexture(false)
#endif
{
    m_useMultisampling = !qgetenv("CUSTOMCONTEXT_MULTISAMPLE").isEmpty();
    if (m_useMultisampling) {
        m_sampleCount= 16;
        QByteArray overrideSamples = qgetenv("CUSTOMCONTEXT_MULTISAMPLE_COUNT");
        bool ok;
        int override = overrideSamples.toInt(&ok);
        if (ok)
            m_sampleCount = override;
    }

    m_depthBuffer = qgetenv("CUSTOMCONTEXT_NO_DEPTH_BUFFER").isEmpty();

#ifdef CUSTOMCONTEXT_ANIMATIONDRIVER
    m_animationDriver = qgetenv("CUSTOMCONTEXT_NO_ANIMATIONDRIVER").isEmpty();
#endif

#ifdef CUSTOMCONTEXT_SWAPLISTENINGANIMATIONDRIVER
    m_swapListeningAnimationDriver = qgetenv("CUSTOMCONTEXT_NO_SWAPLISTENINGANIMATIONDRIVER").isEmpty();
#endif

#ifdef CUSTOMCONTEXT_EGLGRALLOCTEXTURE
    m_eglGrallocTexture = qEnvironmentVariableIsEmpty("CUSTOMCONTEXT_NO_EGLGRALLOCTEXTURE");
#endif

#ifdef CUSTOMCONTEXT_NONPRESERVEDTEXTURE
    m_nonPreservedTexture = qgetenv("CUSTOMCONTEXT_NO_NONPRESERVEDTEXTURE").isEmpty();
#endif

#ifdef CUSTOMCONTEXT_DEBUG
    qDebug("CustomContext created:");
    qDebug(" - multisampling: %s, samples=%d", m_useMultisampling ? "yes" : "no", m_sampleCount);
    qDebug(" - depth buffer: %s", m_depthBuffer ? "yes" : "no");

#ifdef PROGRAM_BINARY
    qDebug(" - program binary: yes");
#endif

#ifdef CUSTOMCONTEXT_SWAPLISTENINGANIMATIONDRIVER
    qDebug(" - swap listening animation driver: %s", m_swapListeningAnimationDriver ? "yes" : "no");
#endif
#ifdef CUSTOMCONTEXT_ANIMATIONDRIVER
    qDebug(" - custom animation driver: %s", m_animationDriver ? "yes" : "no");
#endif
#ifdef CUSTOMCONTEXT_EGLGRALLOCTEXTURE
    qDebug(" - EGLImage/Gralloc based texture: %s", m_eglGrallocTexture ? "yes" : "no");
#endif
#ifdef CUSTOMCONTEXT_NONPRESERVEDTEXTURE
    qDebug(" - non preserved textures: %s", m_nonPreservedTexture ? "yes" : "no");
#endif

#endif

}

void Context::renderContextInitialized(QSGRenderContext *ctx)
{
#if defined(CUSTOMCONTEXT_HYBRISTEXTURE)
    // This check is delayed until there is an EGL display present.
    m_hybrisTexture = qEnvironmentVariableIsEmpty("CUSTOMCONTEXT_NO_HYBRISTEXTURE");
    if (m_hybrisTexture && strstr(eglQueryString(eglGetCurrentDisplay(), EGL_EXTENSIONS), "EGL_HYBRIS_native_buffer") == 0) {
        qDebug() << "EGL_HYBRIS_native_buffer is not available...";
        m_hybrisTexture = false;
    }

#if defined(CUSTOMCONTEXT_DEBUG)
    qDebug(" - EGL/Hybris based texture: %s", m_hybrisTexture ? "yes" : "no");
#endif

#endif // CUSTOMCONTEXT_HYBRISTEXTURE

    QSGDefaultContext::renderContextInitialized(ctx);
}

void RenderContext::initialize(void *abstractContext)
{
    QOpenGLContext *context = static_cast<QOpenGLContext *>(abstractContext);

    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &m_maxTextureSize);

#ifdef CUSTOMCONTEXT_DITHER
    if (m_dither)
        m_ditherProgram = new OrderedDither2x2(context);
#endif

#ifdef CUSTOMCONTEXT_DEBUG
    qDebug("CustomContext: initialized..");
#endif

    QSGDefaultRenderContext::initialize(context);
}

void RenderContext::invalidate()
{
    QSGDefaultRenderContext::invalidate();

#ifdef CUSTOMCONTEXT_DITHER
    delete m_ditherProgram;
    m_ditherProgram = 0;
#endif
}


#ifdef CUSTOMCONTEXT_SURFACEFORMAT
QSurfaceFormat Context::defaultSurfaceFormat() const
{
    QSurfaceFormat format;
    format.setStencilBufferSize(8);
    if (m_depthBuffer)
        format.setDepthBufferSize(24);
    if (m_useMultisampling)
        format.setSamples(m_sampleCount);
    format.setAlphaBufferSize(8);
    return format;
}
#endif

QSGTexture *RenderContext::createTexture(const QImage &image, uint flags) const
{
    if (!(flags & QQuickWindow::TextureCanUseAtlas)) {
#ifdef CUSTOMCONTEXT_EGLGRALLOCTEXTURE
    if (static_cast<Context *>(sceneGraphContext())->hasEglGrallocTextures()) {

        // Only use gralloc textures for textures created outside the render thread.
        // They can still block for as long as normal texture, so better to not waste
        // the precious resource.
        if (openglContext() != 0 && openglContext()->thread() != QThread::currentThread()) {
            EglGrallocTexture *t = EglGrallocTexture::create(image);
            if (t)
                return t;
        }
    }
#endif
#ifdef CUSTOMCONTEXT_HYBRISTEXTURE
    if (static_cast<Context *>(sceneGraphContext())->hasHybrisTextures()) {

        // Only use hybris textures for textures created outside the render thread.
        // They can still block for as long as normal texture, so better to not waste
        // the precious resource.
        if (openglContext() != 0 && openglContext()->thread() != QThread::currentThread()) {
            HybrisTexture *t = HybrisTexture::create(image);
            if (t)
                return t;
        }
    }
#endif
    }

    return QSGDefaultRenderContext::createTexture(image, flags);
}

QSGRenderer *RenderContext::createRenderer()
{
#ifdef CUSTOMCONTEXT_SIMPLERENDERER
    return new QSGSimpleRenderer::Renderer(this);
#endif
    return QSGDefaultRenderContext::createRenderer();
}

QAnimationDriver *Context::createAnimationDriver(QObject *parent)
{
#ifdef CUSTOMCONTEXT_ANIMATIONDRIVER
    if (m_animationDriver)
        return new AnimationDriver();
#endif
#ifdef CUSTOMCONTEXT_SWAPLISTENINGANIMATIONDRIVER
    if (m_swapListeningAnimationDriver)
        return new SwapListeningAnimationDriver();
#endif

   return QSGDefaultContext::createAnimationDriver(parent);
}



QQuickTextureFactory *Context::createTextureFactory(const QImage &image)
{
    Q_UNUSED(image);

#ifdef CUSTOMCONTEXT_HYBRISTEXTURE
    if (m_hybrisTexture) {
        HybrisTextureFactory *tf = HybrisTextureFactory::create(image);
        if (tf)
            return tf;
    }
#endif

#ifdef CUSTOMCONTEXT_EGLGRALLOCTEXTURE
    if (m_eglGrallocTexture) {
        EglGrallocTextureFactory *tf = EglGrallocTextureFactory::create(image);
        if (tf)
            return tf;
    }
#endif

#ifdef CUSTOMCONTEXT_NONPRESERVEDTEXTURE
    if (m_nonPreservedTexture)
        return new NonPreservedTextureFactory(image, this);
#endif

    return 0;
}



void RenderContext::renderNextFrame(QSGRenderer *renderer, GLuint fbo)
{
    QSGDefaultRenderContext::renderNextFrame(renderer, fbo);

#ifdef CUSTOMCONTEXT_DITHER
    if (m_dither) {
        QSurface *s = QOpenGLContext::currentContext()->surface();
        QSize size = static_cast<QWindow *>(s)->size();
        m_ditherProgram->prepare();
        m_ditherProgram->draw(size.width(), size.height());
    }
#endif
}


} // namespace
