#include "chipslogsextractor.h"

#include <QTimer>
#include <iostream>

ChipsLogsExtractor::ChipsLogsExtractor()
{
    _logChip.Clear();
    _logNoChip.Clear();
    _currentFilename = QString("");
    outputChip.setFileName(QString("outputchip.txt"));
    outputChip.open(QIODevice::WriteOnly);
    outputNoChip.setFileName(QString("outputnochip.txt"));
    outputNoChip.open(QIODevice::WriteOnly);
    process();
    outputChip.close();
    outputNoChip.close();
    saveLog();
    exit(0);
}

ChipsLogsExtractor::~ChipsLogsExtractor()
{
    
}

void ChipsLogsExtractor::process()
{
    STOxs_Logfile log;
    int frameList[logsInfo::logSize];
    memset(frameList, -1, sizeof(int)*logsInfo::logSize);
    int k = 0;
    for(int i=0; i<logsInfo::logSize; i++)
    {
	if(_currentFilename != logsInfo::logs[i].logFilename)
	{
	    log = loadLog(QString("../../Logs/RoboCup2015") + logsInfo::logs[i].logFilename + QString(".log"));
	    _currentFilename = logsInfo::logs[i].logFilename;
	   memset(frameList, -1, sizeof(int)*logsInfo::logSize);
	    k=0;
	}
	frameList[k] = logsInfo::logs[i].frame; 
	extractInfo(i, log, logsInfo::logs[i].logFilename, logsInfo::logs[i].frame, frameList, &k);
	k++;
    }
}

STOxs_Logfile ChipsLogsExtractor::loadLog(QString filename)
{
    STOxs_Logfile log;
    QString logHeader;
    QByteArray data;
    QFile file(filename);
    if(file.open(QIODevice::ReadOnly))
    {
	QDataStream in(&file);
	in >> logHeader;
	
	if(logHeader == "STOxs_Logfile2")
	{
	    unsigned long nTotalBytes = file.size();
	    unsigned long nBytes = 0;
	    
	    log.Clear();
	    in >> data;
	    while(!data.isEmpty())
	    {
		nBytes += data.size();
		STOxs_LogfileFrame *frame = log.add_frame();
		frame->ParseFromArray(data.data(), data.size());
		in >> data;
		printf("[%3d%c] Loading File %s\n", (int)(((double)(nBytes)/(double)(nTotalBytes))*100), 37, filename.toStdString().c_str());
	    }
	    printf("[%3d%c] Loading File %s\n", 100, 37, filename.toStdString().c_str());
	    printf("File Loaded %d frames\n", log.frame_size());
	    
	}
	else
	{
	    printf("File %s: Format not supported!\n", filename.toStdString().c_str());
	}
	file.close();
	data.clear();
    }
    else
    {
	printf("File %s can't open\n", filename.toStdString().c_str());
    }
    return log;
}

void ChipsLogsExtractor::extractInfo(int n, STOxs_Logfile log, QString filename, int frame, int frameList[], int *k)
{
    int initFrame = frame - logsInfo::nFramesAfter;
    int endFrame = frame + logsInfo::nFramesBefore;
    printf("Extracting info to %s: frames: %d to %d\n", filename.toStdString().c_str(), initFrame, endFrame);
    for(int i=initFrame; i<=endFrame; i++)
    {
	_logChip.add_frame()->CopyFrom(log.frame(i));
	QString str;
	str.sprintf("%d %d %f %f %f\n", n, log.frame(i).balltracking().ball().enable(), log.frame(i).balltracking().ball().pos().x(), log.frame(i).balltracking().ball().pos().y(), log.frame(i).world().ball().speed());
	outputChip.write(str.toStdString().c_str());
    }
    bool valid = false;
    unsigned int randomFrame;
    int m = 0;
    while(!valid)
    {
	randomFrame = rand()%log.frame_size();
	if((randomFrame > logsInfo::nFramesAfter) && (randomFrame < log.frame_size()-logsInfo::nFramesBefore))
	{
	    if((log.frame(randomFrame).referee().command() != SSL_Referee_Command_HALT) && (log.frame(randomFrame).referee().command() != SSL_Referee_Command_STOP) && (log.frame(randomFrame).referee().command() != SSL_Referee_Command_TIMEOUT_BLUE)&& (log.frame(randomFrame).referee().command() != SSL_Referee_Command_TIMEOUT_YELLOW))
	    {
		bool _valid = true;
		for(int i=0; i<logsInfo::logSize; i++)
		{
		    if(frameList[i] != -1)
		    {
			if((randomFrame < (frameList[i]-(logsInfo::nFramesAfter+logsInfo::nFramesBefore))) && (randomFrame > (frameList[i]+(logsInfo::nFramesAfter+logsInfo::nFramesBefore))))
			{
			    _valid &= false;
			}
		    }
		}
		valid = _valid;
	    }
	}
	m++;
	if(m > 1000)
	    valid = true;
    }
    
    if(m < 1000)
    {
	frameList[*k] = randomFrame;
	*k++;
	initFrame = randomFrame - logsInfo::nFramesAfter;
	endFrame = randomFrame + logsInfo::nFramesBefore;
	for(int i=initFrame; i<=endFrame; i++)
	{
	    _logNoChip.add_frame()->CopyFrom(log.frame(i));
	    QString str;
	    str.sprintf("%d %d %f %f %f\n", n, log.frame(i).balltracking().ball().enable(), log.frame(i).balltracking().ball().pos().x(), log.frame(i).balltracking().ball().pos().y(), log.frame(i).world().ball().speed());
	    outputNoChip.write(str.toStdString().c_str());
	}
    }
    
}

void ChipsLogsExtractor::saveLog()
{
    QFile outputLogFile;
    QByteArray data;
    outputLogFile.setFileName(QString("OutputLogFileChip.log"));
    outputLogFile.open(QIODevice::WriteOnly);
    QDataStream out(&outputLogFile);
    out << QString("STOxs_Logfile2");
    for(int i=0; i<_logChip.frame_size(); i++)
    {
	data.resize(_logChip.frame(i).ByteSize());
	_logChip.frame(i).SerializeToArray(data.data(), data.size());
	out << data;
    }
    outputLogFile.close();
    
    QFile outputLogFileNoChip;
    QByteArray dataNoChip;
    outputLogFileNoChip.setFileName(QString("OutputLogFileNoChip.log"));
    outputLogFileNoChip.open(QIODevice::WriteOnly);
    QDataStream outNoChip(&outputLogFileNoChip);
    outNoChip << QString("STOxs_Logfile2");
    for(int i=0; i<_logNoChip.frame_size(); i++)
    {
	dataNoChip.resize(_logNoChip.frame(i).ByteSize());
	_logNoChip.frame(i).SerializeToArray(dataNoChip.data(), dataNoChip.size());
	outNoChip << dataNoChip;
    }
    outputLogFileNoChip.close();	
}




