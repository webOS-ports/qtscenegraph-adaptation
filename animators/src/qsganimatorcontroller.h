/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the plugins of %MODULE%.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/


#ifndef QSGANIMATORCONTROLLER_H
#define QSGANIMATORCONTROLLER_H

#include "qsgtoplevelanimator.h"
#include "qsganimatedtransform.h"
#include "qsganimatedproperty.h"

class QQuickItem;

class QSGAnimatorController : public QObject
{
    Q_OBJECT
public:
    QSGAnimatorController(QQuickItem *);
    ~QSGAnimatorController();

    void sync();

    void registerProperty(QSGAnimatedProperty *);
    void unregisterProperty(QSGAnimatedProperty *);
    QSGAnimatedProperty *registeredProperty(QString, QObject *o = 0);

    bool isInitialized();
    bool isUpdating();
    QMatrix4x4 transformMatrix();

    void registerAnimation(QSGAbstractAnimation *);
    QQuickItem* item();

public Q_SLOTS:
    void advance();

private:
    void createProperties();
    qreal createAnimators(QSGAbstractAnimator *, QObject *, bool, qreal);

private:
    QQuickItem *m_item;
    QList<QSGAnimatedTransform*> m_transform;
    bool m_initialized;
    QSGTopLevelAnimator m_topLevelAnimator;
    QElapsedTimer m_timer;
    int m_frameCounter;
    qreal m_stableVsync;
    qreal m_currentAnimationTime;
    qreal m_currentAnimationDelay;
    qreal m_currentAnimationCatchup;
    qreal m_thresholdForCatchup;
    qreal m_catchupRatio;
    QHash<QString, QSGAnimatedProperty*> m_registeredProperties;
};

#endif