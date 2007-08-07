//
// This file is part of the Marble Desktop Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2006-2007 Torsten Rahn <tackat@kde.org>"
// Copyright 2007      Inge Wallin  <ingwa@kde.org>"
//

#include <QtGui/QApplication>
#include <QtCore/QFile>

#include "QtMainWindow.h"

#include "MarbleTest.h"

#if STATIC_BUILD
 #include <QtCore/QtPlugin>
 Q_IMPORT_PLUGIN(qjpeg)
 Q_IMPORT_PLUGIN(qsvg)
#endif

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow *window = new MainWindow();
    window->setAttribute( Qt::WA_DeleteOnClose, true );

    MarbleTest *marbleTest = new MarbleTest( window->marbleWidget() );

    window->show();

    for ( int i = 1; i < argc; ++i ) {
        if ( strcmp( argv[ i ], "--timedemo" ) == 0 )
        {
            window->resize(900, 640);
            marbleTest->timeDemo();
            return 0;
        }
        else if( strcmp( argv[ i ], "--gpsdemo" ) == 0 ) {
            window->resize( 900, 640 );
            marbleTest->gpsDemo();
            return 0;
        }
        else if( strcmp( argv[ i ], "--enableCurrentLocation" ) ==0 )
        {
            window->marbleControl()->setCurrentLocationTabShown(true);
        }
        else if ( QFile::exists( app.arguments().at( i ) ) )
            ( window->marbleControl() )->addPlaceMarkFile( argv[i] );
    }

    delete marbleTest;

    return app.exec();
}
