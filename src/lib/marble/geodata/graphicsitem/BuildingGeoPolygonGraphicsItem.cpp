//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2011      Konstantin Oblaukhov <oblaukhov.konstantin@gmail.com>
//

#include "BuildingGeoPolygonGraphicsItem.h"

#include "MarbleDebug.h"
#include "ViewportParams.h"
#include "GeoDataTypes.h"
#include "GeoDataPlacemark.h"
#include "GeoDataLatLonAltBox.h"
#include "GeoDataLinearRing.h"
#include "GeoDataPolygon.h"
#include "GeoDataIconStyle.h"
#include "GeoDataPolyStyle.h"
#include "OsmPlacemarkData.h"
#include "GeoPainter.h"

namespace Marble
{

BuildingGeoPolygonGraphicsItem::BuildingGeoPolygonGraphicsItem(const GeoDataPlacemark *placemark,
                                                               const GeoDataPolygon *polygon)
    : AbstractGeoPolygonGraphicsItem(placemark, polygon)
    , m_buildingHeight(polygon->latLonAltBox().maxAltitude() - polygon->latLonAltBox().minAltitude())
{
    setZValue(this->zValue() + m_buildingHeight);
    Q_ASSERT(m_buildingHeight > 0.0);

    QStringList paintLayers;
    paintLayers << QStringLiteral("Polygon/Building/frame")
                << QStringLiteral("Polygon/Building/roof");
    setPaintLayers(paintLayers);
}

BuildingGeoPolygonGraphicsItem::BuildingGeoPolygonGraphicsItem(const GeoDataPlacemark *placemark,
                                                               const GeoDataLinearRing* ring)
    : AbstractGeoPolygonGraphicsItem(placemark, ring)
    , m_buildingHeight(ring->latLonAltBox().maxAltitude() - ring->latLonAltBox().minAltitude())
{
    setZValue(this->zValue() + m_buildingHeight);
    Q_ASSERT(m_buildingHeight > 0.0);

    QStringList paintLayers;
    paintLayers << QStringLiteral("Polygon/Building/frame")
                << QStringLiteral("Polygon/Building/roof");
    setPaintLayers(paintLayers);
}

void BuildingGeoPolygonGraphicsItem::initializeBuildingPainting(const GeoPainter* painter, const ViewportParams *viewport,
                                                                bool &drawAccurate3D, bool &isCameraAboveBuilding, bool &hasInnerBoundaries,
                                                                QVector<QPolygonF*>& outlinePolygons,
                                                                QVector<QPolygonF*>& innerPolygons) const
{
    drawAccurate3D = false;
    isCameraAboveBuilding = false;

    QPointF offsetAtCorner = buildingOffset(QPointF(0, 0), viewport, &isCameraAboveBuilding);
    qreal maxOffset = qMax( qAbs( offsetAtCorner.x() ), qAbs( offsetAtCorner.y() ) );
    drawAccurate3D = painter->mapQuality() == HighQuality ? maxOffset > 5.0 : maxOffset > 8.0;

    // Since subtracting one fully contained polygon from another results in a single
    // polygon with a "connecting line" between the inner and outer part we need
    // to first paint the inner area with no pen and then the outlines with the correct pen.
    hasInnerBoundaries = polygon() ? !polygon()->innerBoundaries().isEmpty() : false;
    if (polygon()) {
        if (hasInnerBoundaries) {
            screenPolygons(viewport, polygon(), innerPolygons, outlinePolygons);
        }
        viewport->screenCoordinates(polygon()->outerBoundary(), outlinePolygons);
    } else if (ring()) {
        viewport->screenCoordinates(*ring(), outlinePolygons);
    }
}

QPointF BuildingGeoPolygonGraphicsItem::centroid(const QPolygonF &polygon, double &area)
{
    auto centroid = QPointF(0.0, 0.0);
    area = 0.0;
    for (auto i=0, n=polygon.size(); i<n; ++i) {
        auto const x0 = polygon[i].x();
        auto const y0 = polygon[i].y();
        auto const j = i == n-1 ? 0 : i+1;
        auto const x1 = polygon[j].x();
        auto const y1 = polygon[j].y();
        auto const a = x0*y1 - x1*y0;
        area += a;
        centroid.rx() += (x0 + x1)*a;
        centroid.ry() += (y0 + y1)*a;
    }

    area *= 0.5;
    return centroid / (6.0*area);
}

QPointF BuildingGeoPolygonGraphicsItem::buildingOffset(const QPointF &point, const ViewportParams *viewport, bool* isCameraAboveBuilding) const
{
    qreal const cameraFactor = 0.5 * tan(0.5 * 110 * DEG2RAD);
    Q_ASSERT(m_buildingHeight > 0.0);
    qreal const buildingFactor = m_buildingHeight / EARTH_RADIUS;

    qreal const cameraHeightPixel = viewport->width() * cameraFactor;
    qreal buildingHeightPixel = viewport->radius() * buildingFactor;
    qreal const cameraDistance = cameraHeightPixel-buildingHeightPixel;

    if (isCameraAboveBuilding) {
        *isCameraAboveBuilding = cameraDistance > 0;
    }

    qreal const cc = cameraDistance * cameraHeightPixel;
    qreal const cb = cameraDistance * buildingHeightPixel;

    // The following lines calculate the same result, but are potentially slower due
    // to using more trigonometric method calls
    // qreal const alpha1 = atan2(offsetX, cameraHeightPixel);
    // qreal const alpha2 = atan2(offsetX, cameraHeightPixel-buildingHeightPixel);
    // qreal const shiftX = 2 * (cameraHeightPixel-buildingHeightPixel) * sin(0.5*(alpha2-alpha1));

    qreal const offsetX = point.x() - viewport->width() / 2.0;
    qreal const offsetY = point.y() - viewport->height() / 2.0;

    qreal const shiftX = offsetX * cb / (cc + offsetX);
    qreal const shiftY = offsetY * cb / (cc + offsetY);

    return QPointF(shiftX, shiftY);
}

void BuildingGeoPolygonGraphicsItem::paint(GeoPainter* painter, const ViewportParams* viewport, const QString &layer)
{
    if (layer.endsWith(QLatin1String("/frame"))) {
        paintFrame(painter, viewport);
    } else if (layer.endsWith(QLatin1String("/roof"))) {
        paintRoof(painter, viewport);
    } else {
        mDebug() << "Didn't expect to have to paint layer " << layer << ", ignoring it.";
    }
}

void BuildingGeoPolygonGraphicsItem::paintRoof(GeoPainter* painter, const ViewportParams* viewport)
{
    bool drawAccurate3D;
    bool isCameraAboveBuilding;
    bool hasInnerBoundaries;
    QVector<QPolygonF*> outlinePolygons;
    QVector<QPolygonF*> innerPolygons;
    initializeBuildingPainting(painter, viewport, drawAccurate3D, isCameraAboveBuilding, hasInnerBoundaries, outlinePolygons, innerPolygons);
    if (!isCameraAboveBuilding) {
        return; // do not render roof if we look inside the building
    }

    painter->save();
    QPen const currentPen = configurePainter(painter, viewport);

    if (hasInnerBoundaries) {
        painter->setPen(Qt::NoPen);
    }

    // first paint the area and icon (and the outline if there are no inner boundaries)

    foreach(QPolygonF* outlinePolygon, outlinePolygons) {
        QRectF const boundingRect = outlinePolygon->boundingRect();
        QPolygonF buildingRoof;
        if ( drawAccurate3D) {
            buildingRoof.reserve(outlinePolygon->size());
            foreach(const QPointF &point, *outlinePolygon) {
                buildingRoof << point + buildingOffset(point, viewport);
            }
            if (hasInnerBoundaries) {
                QRegion clip(buildingRoof.toPolygon());

                foreach(QPolygonF* innerPolygon, innerPolygons) {
                    QPolygonF buildingInner;
                    buildingInner.reserve(innerPolygon->size());
                    foreach(const QPointF &point, *innerPolygon) {
                        buildingInner << point + buildingOffset(point, viewport);
                    }
                    clip-=QRegion(buildingInner.toPolygon());
                }
                painter->setClipRegion(clip);
            }
            painter->drawPolygon(buildingRoof);
        } else {
            QPointF const offset = buildingOffset(boundingRect.center(), viewport);
            painter->translate(offset);
            if (hasInnerBoundaries) {
                QRegion clip(outlinePolygon->toPolygon());

                foreach(QPolygonF* clipPolygon, innerPolygons) {
                    clip-=QRegion(clipPolygon->toPolygon());
                }
                painter->setClipRegion(clip);
            }
            painter->drawPolygon(*outlinePolygon);
            painter->translate(-offset);
        }
    }

    // then paint the outlines if there are inner boundaries
    if (hasInnerBoundaries) {
        painter->setPen(currentPen);
        foreach(QPolygonF * polygon, outlinePolygons) {
            QRectF const boundingRect = polygon->boundingRect();
            if ( drawAccurate3D) {
                QPolygonF buildingRoof;
                buildingRoof.reserve(polygon->size());
                foreach(const QPointF &point, *polygon) {
                    buildingRoof << point + buildingOffset(point, viewport);
                }
                painter->drawPolyline(buildingRoof);
            } else {
                QPointF const offset = buildingOffset(boundingRect.center(), viewport);
                painter->translate(offset);
                painter->drawPolyline(*polygon);
                painter->translate(-offset);
            }
        }
    }

    qDeleteAll(outlinePolygons);
    painter->restore();
}

void BuildingGeoPolygonGraphicsItem::paintFrame(GeoPainter *painter, const ViewportParams *viewport)
{
    // TODO: how does this match the Q_ASSERT in the constructor?
    if (m_buildingHeight == 0.0) {
        return;
    }

    if ((polygon() && !viewport->resolves(polygon()->outerBoundary().latLonAltBox(), 4))
            || (ring() && !viewport->resolves(ring()->latLonAltBox(), 4))) {
        return;
    }

    painter->save();

    bool drawAccurate3D;
    bool isCameraAboveBuilding;
    bool hasInnerBoundaries;
    QVector<QPolygonF*> outlinePolygons;
    QVector<QPolygonF*> innerPolygons;
    initializeBuildingPainting(painter, viewport, drawAccurate3D, isCameraAboveBuilding, hasInnerBoundaries, outlinePolygons, innerPolygons);

    configureFramePainter(painter);
    foreach(QPolygonF* outlinePolygon, outlinePolygons) {
        if (outlinePolygon->isEmpty()) {
            continue;
        }
        if ( drawAccurate3D && isCameraAboveBuilding ) {
            // draw the building sides
            int const size = outlinePolygon->size();
            QPointF & a = (*outlinePolygon)[0];
            QPointF shiftA = a + buildingOffset(a, viewport);
            for (int i=1; i<size; ++i) {
                QPointF const & b = (*outlinePolygon)[i];
                QPointF const shiftB = b + buildingOffset(b, viewport);
                QPolygonF buildingSide = QPolygonF() << a << shiftA << shiftB << b;
                if (hasInnerBoundaries) {
                    //smoothen away our loss of antialiasing due to the QRegion Qt-bug workaround
                    painter->setPen(QPen(painter->brush().color(), 1.5));
                }
                painter->drawPolygon(buildingSide);
                a = b;
                shiftA = shiftB;
            }
        } else {
            // don't draw the building sides - just draw the base frame instead
            if (hasInnerBoundaries) {
                QRegion clip(outlinePolygon->toPolygon());

                foreach(QPolygonF* clipPolygon, innerPolygons) {
                    clip-=QRegion(clipPolygon->toPolygon());
                }
                painter->setClipRegion(clip);
            }
            painter->drawPolygon(*outlinePolygon);
        }
    }
    qDeleteAll(outlinePolygons);

    painter->restore();
}

void BuildingGeoPolygonGraphicsItem::screenPolygons(const ViewportParams *viewport, const GeoDataPolygon *polygon,
                                                    QVector<QPolygonF *> &innerPolygons, QVector<QPolygonF *> &outlines)
{
    Q_ASSERT(polygon);

    QVector<QPolygonF*> outerPolygons;
    viewport->screenCoordinates( polygon->outerBoundary(), outerPolygons );

    outlines << outerPolygons;

    QVector<GeoDataLinearRing> innerBoundaries = polygon->innerBoundaries();
    foreach (const GeoDataLinearRing &innerBoundary, innerBoundaries) {
        QVector<QPolygonF*> innerPolygonsPerBoundary;
        viewport->screenCoordinates(innerBoundary, innerPolygonsPerBoundary);

        outlines << innerPolygonsPerBoundary;
        innerPolygons.reserve(innerPolygons.size() + innerPolygonsPerBoundary.size());
        foreach( QPolygonF* innerPolygonPerBoundary, innerPolygonsPerBoundary ) {
            innerPolygons << innerPolygonPerBoundary;
        }
    }
}

void BuildingGeoPolygonGraphicsItem::configureFramePainter(GeoPainter *painter) const
{
    GeoDataStyle::ConstPtr style = this->style();
    if (!style) {
        painter->setPen( QPen() );
    }
    else {
        const GeoDataPolyStyle& polyStyle = style->polyStyle();
        const QColor transparentColor(Qt::transparent);

        QPen currentPen = painter->pen();
        currentPen.setColor(transparentColor);
        painter->setPen(currentPen);

        if (!polyStyle.fill()) {
            painter->setBrush(transparentColor);
        }
        else {
            const QColor paintedColor = polyStyle.paintedColor();
            painter->setBrush(paintedColor.darker(150));
        }
    }
}

}
