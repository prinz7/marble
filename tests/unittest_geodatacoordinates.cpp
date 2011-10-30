//
// This file is part of the Marble Desktop Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.

// Copyright 2010 Matias Kallio <matias.kallio@gmail.com>
// Copyright 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

#include <QtTest/QtTest>
#include <QtGui/QApplication>
#include "global.h"
#include "MarbleWidget.h"
#include "AbstractFloatItem.h"
#include "GeoDataCoordinates.h"


using namespace Marble;


class TestGeoDataCoordinates : public QObject
{
    Q_OBJECT

private slots:
    void testConstruction_data();
    void testConstruction();
    void testSet_data();
    void testSet();
    void testLongitude_data();
    void testLongitude();
    void testLatitude_data();
    void testLatitude();
    void testAltitude();
    void testDetail();
    void testIsPole_data();
    void testIsPole();
    void testNotation();
    void testNormalize_data();
    void testNormalize();
    void testFromStringDMS_data();
    void testFromStringDMS();
    void testFromStringDM_data();
    void testFromStringDM();
    void testFromStringD_data();
    void testFromStringD();
    void testString_data();
    void testString();
    void testPack_data();
    void testPack();
};

/*
 * test data for testConstruction()
 */
void TestGeoDataCoordinates::testConstruction_data()
{
    QTest::addColumn<qreal>("lon");
    QTest::addColumn<qreal>("lat");
    QTest::addColumn<qreal>("alt");

    QTest::newRow("coords1") << 180.0 << 90.0 << 400.0;
}

/*
 * test constructions
 */
void TestGeoDataCoordinates::testConstruction()
{
    QFETCH(qreal, lon);
    QFETCH(qreal, lat);
    QFETCH(qreal, alt);

    GeoDataCoordinates coordinates1;
    GeoDataCoordinates coordinates2(coordinates1);
    QCOMPARE(coordinates1, coordinates2);

    GeoDataCoordinates* coordinates3 = new GeoDataCoordinates(lon, lat, alt, GeoDataCoordinates::Degree);
    GeoDataCoordinates* coordinates4(coordinates3);
    QCOMPARE(coordinates3->longitude(GeoDataCoordinates::Degree), coordinates4->longitude(GeoDataCoordinates::Degree));
    QCOMPARE(coordinates3->latitude(GeoDataCoordinates::Degree), coordinates4->latitude(GeoDataCoordinates::Degree));
    QCOMPARE(coordinates3->altitude(), coordinates4->altitude());

    delete coordinates3;
}

/*
 * test data for testSet()
 */
void TestGeoDataCoordinates::testSet_data()
{
    QTest::addColumn<qreal>("lon");
    QTest::addColumn<qreal>("lat");
    QTest::addColumn<qreal>("alt");
    QTest::addColumn<QString>("unit");

    QTest::newRow("deg") << 180.0 << 90.0 << 400.0 << "degree";
    QTest::newRow("rad") << 3.1 << 1.5 << 250.0 << "radian";
}

/*
 * test setting coordinates
 */
void TestGeoDataCoordinates::testSet()
{
    QFETCH(qreal, lon);
    QFETCH(qreal, lat);
    QFETCH(qreal, alt);
    QFETCH(QString, unit);

    GeoDataCoordinates coordinates1;
    if(unit == "degree") {
        coordinates1.set(lon, lat, alt, GeoDataCoordinates::Degree );
        QCOMPARE(coordinates1.longitude(GeoDataCoordinates::Degree), lon);
        QCOMPARE(coordinates1.latitude(GeoDataCoordinates::Degree), lat);
        QCOMPARE(coordinates1.altitude(), alt);
    } else if(unit == "radian") {
        coordinates1.set(lon, lat, alt, GeoDataCoordinates::Radian);
        QCOMPARE(coordinates1.longitude(GeoDataCoordinates::Radian), lon);
        QCOMPARE(coordinates1.latitude(GeoDataCoordinates::Radian), lat);
        QCOMPARE(coordinates1.altitude(), alt);
    }

    qreal myLongitude = 0;
    qreal myLatitude = 0;

    if(unit == "degree") {
        coordinates1.set(lon, lat, alt, GeoDataCoordinates::Degree);
        coordinates1.geoCoordinates(myLongitude, myLatitude, GeoDataCoordinates::Degree);
        QCOMPARE(myLongitude, lon);
        QCOMPARE(myLatitude, lat);
    }
}

/*
 * test data for testLongitude()
 */
void TestGeoDataCoordinates::testLongitude_data()
{
    QTest::addColumn<qreal>("lon");
    QTest::addColumn<QString>("unit");

    QTest::newRow("deg") << 150.0 << "degree";
    QTest::newRow("rad") << 2.5 << "radian";
}

/*
 * test setLongitude() and longitude()
 */
void TestGeoDataCoordinates::testLongitude()
{
    QFETCH(qreal, lon);
    QFETCH(QString, unit);

    GeoDataCoordinates coordinates1;

    if(unit == "radian") {
        coordinates1.setLongitude(lon);
        QCOMPARE(coordinates1.longitude(), lon);
    } else if(unit == "degree") {
        coordinates1.setLongitude(lon, GeoDataCoordinates::Degree);
        QCOMPARE(coordinates1.longitude(GeoDataCoordinates::Degree), lon);
    }

    if(unit == "radian"){
        coordinates1.setLongitude(lon);
        QEXPECT_FAIL("", "This should fail", Continue);
        QCOMPARE(coordinates1.longitude(GeoDataCoordinates::Degree), lon);
        QCOMPARE(coordinates1.longitude(GeoDataCoordinates::Radian), lon);
    }
}

/*
 * test data for testLatitude()
 */
void TestGeoDataCoordinates::testLatitude_data()
{
    QTest::addColumn<qreal>("lat");
    QTest::addColumn<QString>("unit");

    QTest::newRow("deg") << 75.0 << "degree";
    QTest::newRow("rad") << 1.2 << "radian";
}

/*
 * test setLatitude() and latitude()
 */
void TestGeoDataCoordinates::testLatitude()
{
    QFETCH(qreal, lat);
    QFETCH(QString, unit);

    GeoDataCoordinates coordinates1;
    if(unit == "radian") {
        coordinates1.setLatitude(lat);
        QCOMPARE(coordinates1.latitude(), lat);
    } else if(unit == "degree") {
        coordinates1.setLatitude(lat, GeoDataCoordinates::Degree);
        QCOMPARE(coordinates1.latitude(GeoDataCoordinates::Degree), lat);
    }

    if(unit == "radian"){
        coordinates1.setLatitude(lat);
        QEXPECT_FAIL("", "This should fail", Continue);
        QCOMPARE(coordinates1.latitude(GeoDataCoordinates::Degree), lat);
        QCOMPARE(coordinates1.latitude(GeoDataCoordinates::Radian), lat);
    }
}

/*
 * test setAltitude() and altitude()
 */
void TestGeoDataCoordinates::testAltitude()
{
    qreal alt = 400;
    GeoDataCoordinates coordinates1;
    coordinates1.setAltitude(alt);
    QCOMPARE(coordinates1.altitude(), alt);
}

/*
 * test setDetail() and detail()
 */
void TestGeoDataCoordinates::testDetail()
{
    int detailnumber = 15;
    GeoDataCoordinates coordinates1;
    coordinates1.setDetail(detailnumber);
    QCOMPARE(coordinates1.detail(), detailnumber);
}

/*
 * test setDefaultNotation() and defaultNotation
 */
void TestGeoDataCoordinates::testNotation()
{
    GeoDataCoordinates::setDefaultNotation(GeoDataCoordinates::Decimal);
    QVERIFY(GeoDataCoordinates::defaultNotation() == GeoDataCoordinates::Decimal);

    GeoDataCoordinates::setDefaultNotation(GeoDataCoordinates::DMS);
    QVERIFY(GeoDataCoordinates::defaultNotation() == GeoDataCoordinates::DMS);
}

/*
 * test data for testIsPole()
 */
void TestGeoDataCoordinates::testIsPole_data()
{
    QTest::addColumn<qreal>("lon");
    QTest::addColumn<qreal>("lat");
    QTest::addColumn<qreal>("alt");
    QTest::addColumn<QString>("pole");

    QTest::newRow("false") << 50.0 << 50.0 << 0.0 << "false_pole";
    QTest::newRow("south") << 0.0 << -90.0 << 0.0 << "south_pole";
    QTest::newRow("north") << 0.0 << 90.0 << 0.0 << "north_pole";
}

/*
 * Test isPole-method
 */
void TestGeoDataCoordinates::testIsPole()
{
    QFETCH(qreal, lon);
    QFETCH(qreal, lat);
    QFETCH(qreal, alt);
    QFETCH(QString, pole);

    GeoDataCoordinates coordinates1;

    if(pole == "false_pole") {
        coordinates1.set(lon, lat, alt, GeoDataCoordinates::Degree);
        QVERIFY(coordinates1.isPole() == false);
    } else if(pole == "south_pole") {
        coordinates1.set(lon, lat, alt, GeoDataCoordinates::Degree);
        QVERIFY(coordinates1.isPole(SouthPole));
    } else if(pole == "north_pole") {
        coordinates1.set(lon, lat, alt, GeoDataCoordinates::Degree);
        QVERIFY(coordinates1.isPole(NorthPole));
    }
}

/*
 * test data for testNormalize()
 */
void TestGeoDataCoordinates::testNormalize_data()
{
    QTest::addColumn<qreal>("lon");
    QTest::addColumn<qreal>("lat");
    QTest::addColumn<QString>("unit");

    QTest::newRow("deg") << 200.0 << 130.0 << "degree";
    QTest::newRow("rad") << 3.6 << 2.7  << "radian";
}

/*
 * test normalizeLon(), normalizeLat() and normalizeLonLat()
 */
void TestGeoDataCoordinates::testNormalize()
{
    QFETCH(qreal, lon);
    QFETCH(qreal, lat);
    QFETCH(QString, unit);

    if(unit == "degree") {
        QCOMPARE(GeoDataCoordinates::normalizeLon(lon, GeoDataCoordinates::Degree), qreal(-160));
        QCOMPARE(GeoDataCoordinates::normalizeLat(lat, GeoDataCoordinates::Degree), qreal(50));

        qreal normalized_lon = lon;
        qreal normalized_lat = lat;

        GeoDataCoordinates::normalizeLonLat( normalized_lon, normalized_lat, GeoDataCoordinates::Degree);
        QCOMPARE(normalized_lon, qreal(20));
        QCOMPARE(normalized_lat, qreal(50));
    } else if (unit == "radian") {
        // Compare up to three decimals
        qreal value = GeoDataCoordinates::normalizeLon(lon, GeoDataCoordinates::Radian);
        QCOMPARE(ceil(value * 1000) / 1000, qreal(-2.683));

        value = GeoDataCoordinates::normalizeLat(lat, GeoDataCoordinates::Radian);
        QCOMPARE(ceil(value * 1000) / 1000, qreal(0.442));

        qreal normalized_lon = lon;
        qreal normalized_lat = lat;

        GeoDataCoordinates::normalizeLonLat( normalized_lon, normalized_lat, GeoDataCoordinates::Radian);
        QCOMPARE(ceil(normalized_lon * 1000) / 1000, qreal(0.459));
        QCOMPARE(ceil(normalized_lat * 1000) / 1000, qreal(0.442));
     }
}

enum SignType {NoSign, PositiveSign, NegativeSign};
enum SphereType {PosSphere, NegSphere};
enum UnitsType {NoUnits, WithUnits};
enum SpacesType {NoSpaces, WithSpaces};

static QString
createDegreeString(SignType signType,
                   int degreeValue, int minutesValue, qreal secondsValue,
                   UnitsType unitsType, SpacesType spacesType)
{
    QString string;

    // add degree
    if (signType != NoSign) string.append(QLatin1Char(signType==PositiveSign?'+':'-'));
    string.append(QString::number(degreeValue));
    if (unitsType == WithUnits) string.append(QChar(0xb0));

    // add minutes
    string.append(QLatin1Char(' ') + QString::number(minutesValue));
    if (unitsType == WithUnits) string.append(QLatin1Char('\''));

    // add seconds
    string.append(QString::fromLatin1(" %L1").arg(secondsValue, 0, 'f', 10));
    if (unitsType == WithUnits) string.append(QLatin1Char('"'));

    if (spacesType == WithSpaces) string.append(QLatin1Char(' '));

    return string;
}

static QString
createDegreeString(SignType signType,
                   int degreeValue, qreal minutesValue,
                   UnitsType unitsType, SpacesType spacesType)
{
    QString string;

    // add degree
    if (signType != NoSign) string.append(QLatin1Char(signType==PositiveSign?'+':'-'));
    string.append(QString::number(degreeValue));
    if (unitsType == WithUnits) string.append(QChar(0xb0));

    // add minutes
    string.append(QString::fromLatin1(" %L1").arg(minutesValue, 0, 'f', 10));
    if (unitsType == WithUnits) string.append(QLatin1Char('\''));

    if (spacesType == WithSpaces) string.append(QLatin1Char(' '));

    return string;
}

static QString
createDegreeString(SignType signType,
                   qreal degreeValue,
                   UnitsType unitsType, SpacesType spacesType)
{
    QString string;

    // add degree
    if (signType != NoSign) string.append(QLatin1Char(signType==PositiveSign?'+':'-'));
    string.append(QString::fromLatin1("%L1").arg(degreeValue, 0, 'f', 10));
    if (unitsType == WithUnits) string.append(QChar(0xb0));

    if (spacesType == WithSpaces) string.append(QLatin1Char(' '));

    return string;
}

/*
 * test data for testStringDMS()
 */
void TestGeoDataCoordinates::testFromStringDMS_data()
{
    QTest::addColumn<QString>("string");
    QTest::addColumn<qreal>("lon");
    QTest::addColumn<qreal>("lat");

    const QVector<SignType> signTypes = QVector<SignType>()
        << NoSign << PositiveSign << NegativeSign;
    const QVector<SphereType> sphereTypes = QVector<SphereType>()
        << PosSphere << NegSphere;
    const QVector<UnitsType> unitsTypes = QVector<UnitsType>()
        << NoUnits << WithUnits;
    const QVector<SpacesType> spacesTypes = QVector<SpacesType>()
        << NoSpaces << WithSpaces;

    const QVector<uint> degreeSamples = QVector<uint>()
        << 0 << 140 << 180;
    const QVector<uint> minutesSamples = QVector<uint>()
        << 0 << 23 << 59;
    const QVector<qreal> secondsSamples = QVector<qreal>()
        << 0.0 << 3.14159 << 59.9999999;

    foreach(const UnitsType unitsType, unitsTypes) {
    foreach(const SpacesType spacesType, spacesTypes) {
    // lon
    foreach(const SphereType lonSphere, sphereTypes) {
    foreach(const SignType lonSignType, signTypes) {
        const bool lonIsPositive =
            (lonSphere==PosSphere && lonSignType!=NegativeSign) ||
            (lonSphere==NegSphere && lonSignType==NegativeSign);
    foreach(const uint lonDegree, degreeSamples) {
    foreach(const uint lonMinutes, minutesSamples) {
        if(lonDegree == 180 && lonMinutes != 0) continue;
    foreach(const qreal lonSeconds, secondsSamples) {
        if(lonDegree == 180 && lonSeconds != 0.0) continue;
    // lat
    foreach(const SphereType latSphere, sphereTypes) {
    foreach(const SignType latSignType, signTypes) {
        const bool latIsPositive =
            (latSphere==PosSphere && latSignType!=NegativeSign) ||
            (latSphere==NegSphere && latSignType==NegativeSign);
    foreach(const uint latDegree, degreeSamples) {
    foreach(const uint latMinutes, minutesSamples) {
        if(latDegree == 180 && latMinutes != 0) continue;
    foreach(const qreal latSeconds, secondsSamples) {
        if(latDegree == 180 && latSeconds != 0.0) continue;

    // actual construction
        // Create lon & lat values
        qreal lon = (qreal)lonDegree + lonMinutes*MIN2HOUR + lonSeconds*SEC2HOUR;
        if( ! lonIsPositive )
            lon *= -1;
        qreal lat = (qreal)latDegree + latMinutes*MIN2HOUR + latSeconds*SEC2HOUR;
        if( ! latIsPositive )
            lat *= -1;

        // Create string
        QString string;
        string.append(createDegreeString(latSignType,
                                         latDegree, latMinutes, latSeconds,
                                         unitsType, spacesType));
        string.append(QLatin1Char(latSphere==PosSphere?'N':'S'));
        string.append(QLatin1Char(' '));
        string.append(createDegreeString(lonSignType,
                                         lonDegree, lonMinutes, lonSeconds,
                                         unitsType, spacesType));
        string.append(QLatin1Char(lonSphere==PosSphere?'E':'W'));

        // Create row title
        QString rowTitle;
        rowTitle.append(QLatin1String(spacesType==WithSpaces?"spaced dir":"unspaced dir"))
                .append(QLatin1String(unitsType==WithUnits?"|units":"|no units"))
                .append(QLatin1String("|lon:"))
                .append(QLatin1Char(lonIsPositive?'+':'-'))
                .append(QString::number(lonDegree)+QChar(0xb0))
                .append(QString::number(lonMinutes)+QLatin1Char('\''))
                .append(QString::fromLatin1("%L1").arg(lonSeconds, 0, 'f', 10)+QLatin1Char('"'))
                .append(QLatin1Char(lonSphere==PosSphere?'P':'N'))
                .append(QLatin1String("|lat:"))
                .append(QLatin1Char(latIsPositive?'+':'-'))
                .append(QString::number(latDegree)+QChar(0xb0))
                .append(QString::number(latMinutes)+QLatin1Char('\''))
                .append(QString::fromLatin1("%L1").arg(latSeconds, 0, 'f', 10)+QLatin1Char('"'))
                .append(QLatin1Char(latSphere==PosSphere?'P':'N'))
                .append(QLatin1Char('|')).append(string).append(QLatin1Char('|'));
        QTest::newRow(rowTitle.toLatin1())
            << string
            << lon
            << lat;
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
}

/*
 * test fromString() with DMS notation
 */
void TestGeoDataCoordinates::testFromStringDMS()
{
    QFETCH(QString, string);
    QFETCH(qreal, lon);
    QFETCH(qreal, lat);

    bool succeeded = false;
    const GeoDataCoordinates coords = GeoDataCoordinates::fromString(string, succeeded);

    if(! succeeded)
        qWarning() << "Could not parse"<<string <<"for"<<lon<<lat;

    QVERIFY(succeeded);
    QCOMPARE(coords.longitude(GeoDataCoordinates::Degree), lon);
    QCOMPARE(coords.latitude(GeoDataCoordinates::Degree),  lat);
}

/*
 * test data for testStringDM()
 */
void TestGeoDataCoordinates::testFromStringDM_data()
{
    QTest::addColumn<QString>("string");
    QTest::addColumn<qreal>("lon");
    QTest::addColumn<qreal>("lat");

    const QVector<SignType> signTypes = QVector<SignType>()
        << NoSign << PositiveSign << NegativeSign;
    const QVector<SphereType> sphereTypes = QVector<SphereType>()
        << PosSphere << NegSphere;
    const QVector<UnitsType> unitsTypes = QVector<UnitsType>()
        << NoUnits << WithUnits;
    const QVector<SpacesType> spacesTypes = QVector<SpacesType>()
        << NoSpaces << WithSpaces;

    const QVector<uint> degreeSamples = QVector<uint>()
        << 0 << 140 << 180;
    const QVector<qreal> minutesSamples = QVector<qreal>()
        << 0.0 << 3.14159 << 59.9999999;

    foreach(const UnitsType unitsType, unitsTypes) {
    foreach(const SpacesType spacesType, spacesTypes) {
    // lon
    foreach(const SphereType lonSphere, sphereTypes) {
    foreach(const SignType lonSignType, signTypes) {
        const bool lonIsPositive =
            (lonSphere==PosSphere && lonSignType!=NegativeSign) ||
            (lonSphere==NegSphere && lonSignType==NegativeSign);
    foreach(const uint lonDegree, degreeSamples) {
    foreach(const qreal lonMinutes, minutesSamples) {
        if(lonDegree == 180 && lonMinutes != 0.0) continue;
    // lat
    foreach(const SphereType latSphere, sphereTypes) {
    foreach(const SignType latSignType, signTypes) {
        const bool latIsPositive =
            (latSphere==PosSphere && latSignType!=NegativeSign) ||
            (latSphere==NegSphere && latSignType==NegativeSign);
    foreach(const uint latDegree, degreeSamples) {
    foreach(const qreal latMinutes, minutesSamples) {
        if(latDegree == 180 && latMinutes != 0.0) continue;

    // actual construction
        // Create lon & lat values
        qreal lon = (qreal)lonDegree + lonMinutes*MIN2HOUR;
        if( ! lonIsPositive )
            lon *= -1;
        qreal lat = (qreal)latDegree + latMinutes*MIN2HOUR;
        if( ! latIsPositive )
            lat *= -1;

        // Create string
        QString string;
        string.append(createDegreeString(latSignType,
                                         latDegree, latMinutes,
                                         unitsType, spacesType));
        string.append(QLatin1Char(latSphere==PosSphere?'N':'S'));
        string.append(QLatin1Char(' '));
        string.append(createDegreeString(lonSignType,
                                         lonDegree, lonMinutes,
                                         unitsType, spacesType));
        string.append(QLatin1Char(lonSphere==PosSphere?'E':'W'));

        // Create row title
        QString rowTitle;
        rowTitle.append(QLatin1String(spacesType==WithSpaces?"spaced dir":"unspaced dir"))
                .append(QLatin1String(unitsType==WithUnits?"|units":"|no units"))
                .append(QLatin1String("|lon:"))
                .append(QLatin1Char(lonIsPositive?'+':'-'))
                .append(QString::number(lonDegree)+QChar(0xb0))
                .append(QString::fromLatin1("%L1").arg(lonMinutes, 0, 'f', 10)+QLatin1Char('\''))
                .append(QLatin1Char(lonSphere==PosSphere?'P':'N'))
                .append(QLatin1String("|lat:"))
                .append(QLatin1Char(latIsPositive?'+':'-'))
                .append(QString::number(latDegree)+QChar(0xb0))
                .append(QString::fromLatin1("%L1").arg(latMinutes, 0, 'f', 10)+QLatin1Char('\''))
                .append(QLatin1Char(latSphere==PosSphere?'P':'N'))
                .append(QLatin1Char('|')).append(string).append(QLatin1Char('|'));
        QTest::newRow(rowTitle.toLatin1())
            << string
            << lon
            << lat;
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
}

/*
 * test fromString() with DM notation
 */
void TestGeoDataCoordinates::testFromStringDM()
{
    QFETCH(QString, string);
    QFETCH(qreal, lon);
    QFETCH(qreal, lat);

    bool succeeded = false;
    const GeoDataCoordinates coords = GeoDataCoordinates::fromString(string, succeeded);

    if(! succeeded)
        qWarning() << "Could not parse"<<string <<"for"<<lon<<lat;

    QVERIFY(succeeded);
    QCOMPARE(coords.longitude(GeoDataCoordinates::Degree), lon);
    QCOMPARE(coords.latitude(GeoDataCoordinates::Degree),  lat);
}

/*
 * test data for testStringDM()
 */
void TestGeoDataCoordinates::testFromStringD_data()
{
    QTest::addColumn<QString>("string");
    QTest::addColumn<qreal>("lon");
    QTest::addColumn<qreal>("lat");

    const QVector<SignType> signTypes = QVector<SignType>()
        << NoSign << PositiveSign << NegativeSign;
    const QVector<SphereType> sphereTypes = QVector<SphereType>()
        << PosSphere << NegSphere;
    const QVector<UnitsType> unitsTypes = QVector<UnitsType>()
        << NoUnits << WithUnits;
    const QVector<SpacesType> spacesTypes = QVector<SpacesType>()
        << NoSpaces << WithSpaces;

    const QVector<qreal> degreeSamples = QVector<qreal>()
        << 0.0 << 3.14159 << 180.0;

    foreach(const UnitsType unitsType, unitsTypes) {
    foreach(const SpacesType spacesType, spacesTypes) {
    // lon
    foreach(const SphereType lonSphere, sphereTypes) {
    foreach(const SignType lonSignType, signTypes) {
        const bool lonIsPositive =
            (lonSphere==PosSphere && lonSignType!=NegativeSign) ||
            (lonSphere==NegSphere && lonSignType==NegativeSign);
    foreach(const qreal lonDegree, degreeSamples) {
    // lat
    foreach(const SphereType latSphere, sphereTypes) {
    foreach(const SignType latSignType, signTypes) {
        const bool latIsPositive =
            (latSphere==PosSphere && latSignType!=NegativeSign) ||
            (latSphere==NegSphere && latSignType==NegativeSign);
    foreach(const qreal latDegree, degreeSamples) {

    // actual construction
        // Create lon & lat values
        qreal lon = lonDegree;
        if (! lonIsPositive)
            lon *= -1;
        qreal lat = latDegree;
        if (! latIsPositive)
            lat *= -1;

        // Create string
        QString string;
        string.append(createDegreeString(latSignType,
                                         latDegree,
                                         unitsType, spacesType));
        string.append(QLatin1Char(latSphere==PosSphere?'N':'S'));
        string.append(QLatin1Char(' '));
        string.append(createDegreeString(lonSignType,
                                         lonDegree,
                                         unitsType, spacesType));
        string.append(QLatin1Char(lonSphere==PosSphere?'E':'W'));

        // Create row title
        QString rowTitle;
        rowTitle.append(QLatin1String(spacesType==WithSpaces?"spaced dir":"unspaced dir"))
                .append(QLatin1String(unitsType==WithUnits?"|units":"|no units"))
                .append(QLatin1String("|lon:"))
                .append(QLatin1Char(lonIsPositive?'+':'-'))
                .append(QString::fromLatin1("%L1").arg(lonDegree, 0, 'f', 10)+QChar(0xb0))
                .append(QLatin1Char(lonSphere==PosSphere?'P':'N'))
                .append(QLatin1String("|lat:"))
                .append(QLatin1Char(latIsPositive?'+':'-'))
                .append(QString::fromLatin1("%L1").arg(latDegree, 0, 'f', 10)+QChar(0xb0))
                .append(QLatin1Char(latSphere==PosSphere?'P':'N'))
                .append(QLatin1Char('|')).append(string).append(QLatin1Char('|'));
        QTest::newRow(rowTitle.toLatin1())
            << string
            << lon
            << lat;
    }
    }
    }
    }
    }
    }
    }
    }
}

/*
 * test fromString() with DM notation
 */
void TestGeoDataCoordinates::testFromStringD()
{
    QFETCH(QString, string);
    QFETCH(qreal, lon);
    QFETCH(qreal, lat);

    bool succeeded = false;
    const GeoDataCoordinates coords = GeoDataCoordinates::fromString(string, succeeded);

    if(! succeeded)
        qWarning() << "Could not parse"<<string <<"for"<<lon<<lat;

    QVERIFY(succeeded);
    QCOMPARE(coords.longitude(GeoDataCoordinates::Degree), lon);
    QCOMPARE(coords.latitude(GeoDataCoordinates::Degree),  lat);
}

/*
 * test data for testString()
 */
void TestGeoDataCoordinates::testString_data()
{
    QTest::addColumn<qreal>("lon");
    QTest::addColumn<qreal>("lat");
    QTest::addColumn<qreal>("alt");

    QTest::newRow("deg") << 140.0 << 65.0 << 400.0;
}

/*
 * test lonToString(), latToString(), fromString() and toString()
 */
void TestGeoDataCoordinates::testString()
{
    QString lon_str = "150 E";
    QString lat_str = "75 N";
    bool succeeded = false;

    //fromString
    GeoDataCoordinates coordinates1 = GeoDataCoordinates::fromString(lon_str + lat_str , succeeded);
    QCOMPARE(succeeded, true);
    QCOMPARE(lon_str, coordinates1.lonToString().replace(QChar(0x00b0), QChar(' ')));

    QString tmpString = coordinates1.latToString();
    QCOMPARE(lat_str, tmpString.replace(QChar(0x00b0), QChar(' ')).trimmed() );

    QCOMPARE(coordinates1.longitude(GeoDataCoordinates::Degree), qreal(150.0));
    QCOMPARE(coordinates1.latitude(GeoDataCoordinates::Degree), qreal(75.0));

    //latToString and lonToString
    QFETCH(qreal, lon);
    QFETCH(qreal, lat);
    QFETCH(qreal, alt);

    coordinates1.set(lon, lat, alt, GeoDataCoordinates::Degree);
    tmpString = coordinates1.toString(GeoDataCoordinates::Decimal, GeoDataCoordinates::Degree);

    QCOMPARE(tmpString.mid(tmpString.indexOf(", ") + 1).trimmed(),
             GeoDataCoordinates::latToString(lat, GeoDataCoordinates::Decimal, GeoDataCoordinates::Degree, 1).trimmed()); //one decimal precision

    tmpString = coordinates1.toString(GeoDataCoordinates::DMS);
    QCOMPARE(tmpString.mid(tmpString.indexOf(", ") + 1).trimmed(),
             GeoDataCoordinates::latToString(lat, GeoDataCoordinates::DMS,GeoDataCoordinates::Degree).trimmed() );

    tmpString = coordinates1.toString(GeoDataCoordinates::Decimal);
    QCOMPARE(tmpString.remove(tmpString.indexOf(", "), tmpString.length()),
             GeoDataCoordinates::lonToString(lon, GeoDataCoordinates::Decimal, GeoDataCoordinates::Degree).trimmed() );

    tmpString = coordinates1.toString(GeoDataCoordinates::DMS);
    QCOMPARE(tmpString.remove(tmpString.indexOf(", "), tmpString.length()),
             GeoDataCoordinates::lonToString(lon, GeoDataCoordinates::DMS, GeoDataCoordinates::Degree).trimmed() );
}

/*
 * test data for testPack()
 */
void TestGeoDataCoordinates::testPack_data()
{
    QTest::addColumn<qreal>("lon");
    QTest::addColumn<qreal>("lat");
    QTest::addColumn<qreal>("alt");

    QTest::newRow("deg") << 180.0 << 90.0 << 400.0;
}

/*
 * test pack() and unPack()
 */
void TestGeoDataCoordinates::testPack()
{
    QFETCH(qreal, lon);
    QFETCH(qreal, lat);
    QFETCH(qreal, alt);

    GeoDataCoordinates coordinates1,coordinates2;
    coordinates1.set(lon, lat, alt, GeoDataCoordinates::Degree);

    QTemporaryFile file;
    if(file.open()) {
        QDataStream out(&file);
        coordinates1.pack(out);
    }
    file.close();

    if(file.open()) {
        QDataStream in(&file);
        coordinates2.unpack(in);
    }
    file.close();

    QCOMPARE(coordinates1.longitude(GeoDataCoordinates::Degree), coordinates2.longitude(GeoDataCoordinates::Degree));
    QCOMPARE(coordinates1.latitude(GeoDataCoordinates::Degree), coordinates2.latitude(GeoDataCoordinates::Degree));
    QCOMPARE(coordinates1.altitude(), coordinates2.altitude());
}

QTEST_MAIN(TestGeoDataCoordinates)
#include "unittest_geodatacoordinates.moc"



