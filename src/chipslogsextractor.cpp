#include "chipslogsextractor.h"

#include <QTimer>
#include <iostream>

ChipsLogsExtractor::ChipsLogsExtractor()
{
    _log = new STOxs_Logfile();
    _log->Clear();
    _currentFilename = QString("");
    output.setFileName(QString("output.txt"));
    output.open(QIODevice::WriteOnly);
    process();
    output.close();
    saveLog();
    exit(0);
}

ChipsLogsExtractor::~ChipsLogsExtractor()
{
    
}

void ChipsLogsExtractor::process()
{
    STOxs_Logfile *log = new STOxs_Logfile();
    for(int i=0; i<8; i++)
    {
	if(_currentFilename != logsInfo::logs[i].logFilename)
	{
	    log = loadLog(QString("../../Logs/") + logsInfo::logs[i].logFilename + QString(".log"));
	    _currentFilename = logsInfo::logs[i].logFilename;
	}
	extractInfo(i, log, logsInfo::logs[i].logFilename, logsInfo::logs[i].frame);
    }
}

STOxs_Logfile* ChipsLogsExtractor::loadLog(QString filename)
{
    STOxs_Logfile *log = new STOxs_Logfile();
    QString logHeader;
    QByteArray data;
    QFile file(filename);
    if(file.open(QIODevice::ReadOnly))
    {
	QDataStream in(&file);
	in >> logHeader;
	
	if(logHeader == "STOxs_Logfile2")
	{
	    int nTotalBytes = file.size();
	    int nBytes = 0;
	    
	    log->Clear();
	    in >> data;
	    while(!data.isEmpty())
	    {
		nBytes += data.size();
		STOxs_LogfileFrame *frame = log->add_frame();
		frame->ParseFromArray(data.data(), data.size());
		in >> data;
		printf("[%3d%c] Loading File %s\n", (int)(((double)(nBytes)/(double)(nTotalBytes))*100), 37, filename.toStdString().c_str());
	    }
	    printf("[%3d%c] Loading File %s\n", 100, 37, filename.toStdString().c_str());
	    
	}
	else
	{
	    printf("File %s: Format not supported!\n", filename.toStdString().c_str());
	}
	file.close();
    }
    else
    {
	printf("File %s can't open\n", filename.toStdString().c_str());
    }
    return log;
}

void ChipsLogsExtractor::extractInfo(int n, STOxs_Logfile *log, QString filename, int frame)
{
    int initFrame = frame - logsInfo::nFramesAfter;
    int endFrame = frame + logsInfo::nFramesBefore;
    printf("Extracting info to %s: frames: %d to %d\n", filename.toStdString().c_str(), initFrame, endFrame);
    for(int i=initFrame; i<=endFrame; i++)
    {
	_log->add_frame()->CopyFrom(log->frame(i));
	QString str;
	str.sprintf("%d %d %f %f %f\n", n, log->frame(i).balltracking().ball().enable(), log->frame(i).balltracking().ball().pos().x(), log->frame(i).balltracking().ball().pos().y(), log->frame(i).world().ball().speed());
	output.write(str.toStdString().c_str());
    }
}

void ChipsLogsExtractor::saveLog()
{
    QFile outputLogFile;
    QByteArray data;
    outputLogFile.setFileName(QString("OutputLogFile.log"));
    outputLogFile.open(QIODevice::WriteOnly);
    QDataStream out(&outputLogFile);
    out << QString("STOxs_Logfile2");
    for(int i=0; i<_log->frame_size(); i++)
    {
	data.resize(_log->frame(i).ByteSize());
	_log->frame(i).SerializeToArray(data.data(), data.size());
	out << data;
    }
    outputLogFile.close();	    
}




