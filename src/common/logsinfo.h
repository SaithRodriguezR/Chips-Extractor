#ifndef LOGSINFO_H
#define LOGSINFO_H

class SLog
{
public:
    QString logFilename;
    int frame;
};

namespace logsInfo
{
    static const int nFramesAfter		= 50; 
    static const int nFramesBefore              = 50; 
    
    static const SLog logs[8] 	= { 	{QString("TestLog"), 4164}, 
					{QString("TestLog"), 9938},
					{QString("TestLog"), 15240},
					{QString("TestLog"), 18572},
					{QString("TestLog2"), 4164}, 
					{QString("TestLog2"), 9938},
					{QString("TestLog2"), 15240},
					{QString("TestLog2"), 18572}};
    
}

#endif // GEOMETRYCONSTANTS_H