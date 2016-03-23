//======================================================================//
//  This software is free: you can redistribute it and/or modify        //
//  it under the terms of the GNU General Public License Version 3,     //
//  as published by the Free Software Foundation.                       //
//  This software is distributed in the hope that it will be useful,    //
//  but WITHOUT ANY WARRANTY; without even the implied warranty of      //
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE..  See the      //
//  GNU General Public License for more details.                        //
//  You should have received a copy of the GNU General Public License   //
//  Version 3 in the file COPYING that came with this distribution.     //
//  If not, see <http://www.gnu.org/licenses/>                          //
//======================================================================//
//                                                                      //
//      Copyright (c) 2016 STOx's Small Size League RoboCup Team        //             
//      Universidad Santo Tomas - Bogota, Colombia                      //
//      http://www.stoxs.org/                                           //
//      small-size@stoxs.org                                            //
//                                                                      //
//======================================================================//

#ifndef CHIPSLOGSEXTRACTOR_H
#define CHIPSLOGSEXTRACTOR_H

#include <QtCore/QObject>
#include <QFile>
#include <QDataStream>

#include <src/common/logsinfo.h>
#include "messages_stoxs_logfile.pb.h"

class ChipsLogsExtractor : public QObject
{
Q_OBJECT
public:
    ChipsLogsExtractor();
    virtual ~ChipsLogsExtractor();
private:
    void process();
    STOxs_Logfile loadLog(QString filename);
    void extractInfo(int n, STOxs_Logfile log, QString filename, int frame, int frameList[], int *k);
    void saveLog();
    
private:
    QFile outputChip;
    QFile outputNoChip;
    QString _currentFilename;
    STOxs_Logfile _logChip;
    STOxs_Logfile _logNoChip;
private slots:

};

#endif // CHIPSLOGSEXTRACTOR_H
