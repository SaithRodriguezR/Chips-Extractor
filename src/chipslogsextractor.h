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
    void extractInfo(int n, STOxs_Logfile log, QString filename, int frame);
    void saveLog();
    
private:
    QFile output;
    QString _currentFilename;
    STOxs_Logfile _log;
private slots:

};

#endif // CHIPSLOGSEXTRACTOR_H
