/*
    Copyright (C) 2008 Torsten Rahn <rahn@kde.org>

    This file is part of the KDE project

    This library is free software you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    aint with this library see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#ifndef GEOSCENEVECTOR_H
#define GEOSCENEVECTOR_H

#include <QtCore/QVector>

#include "GeoSceneLayer.h"

/**
 * @short Vector dataset stored in a layer.
 */

class GeoSceneVector : public GeoSceneAbstractDataset {
 public:
    GeoSceneVector( const QString& name );
    ~GeoSceneVector();

    QString sourceFile() const;
    void setSourceFile( const QString& sourceFile );

    QString feature() const;
    void setFeature( const QString& feature );

    QString color() const;
    void setColor(const QString&);

    virtual QString type();

 protected:
    QString m_sourceFile;
    QString m_feature;
    QString m_color;
};

#endif // GEOSCENEVECTOR_H
