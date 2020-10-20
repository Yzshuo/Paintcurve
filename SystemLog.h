/******************************************************************************
  Copyright (C), 1988-1999, Shanghai Deewe Communication Tech. Co., Ltd.
  File name:      SystemLog.h
  Author:       Version:        Date: 
  Liu Shengju   1.1.1.0         08/09/25
  Description:    дϵͳ��־
  Others:         // �������ݵ�˵��
  Function List:  // ��Ҫ�����б�ÿ����¼Ӧ���������������ܼ�Ҫ˵��
    1. ....
  History:        // �޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸�
                  // �߼��޸����ݼ���  
    1. Date:
       Author:
       Modification:
    2. ...
******************************************************************************/

#ifndef WRITE_SYSTOM_LOG
#define WRITE_SYSTOM_LOG

#include <QtCore/qglobal.h>
#include <QFile>
#include <QDir>
#include <QDateTime>
#include <QMutex>


class  CSystemLog 
{
public:
#define JingyuTRY	try
#define JingyuCATCH	catch(...)
#define JingyuEXCEPTION_NOTHROW throw()
#define JingyuEXCEPTION_AVOIDABLE throw(...)
#define JingyuEXCEPTION_NOTAVOIDABLE throw(...)
	const CSystemLog& operator = (const CSystemLog&) JingyuEXCEPTION_NOTHROW;
	CSystemLog(void) JingyuEXCEPTION_NOTHROW;
	~CSystemLog(void) JingyuEXCEPTION_NOTHROW;

public:
	bool Init(QString fileName);
	bool Init(QString fileName, QString directory);
	void Uninit();

#define  LOG_MAX_BUFFER         100*1024*1024
	enum {
		LOG_TYPE_NONE = 0,
		LOG_TYPE_INFORMATION,
		LOG_TYPE_WARNING,
		LOG_TYPE_ERROR,
	};
#define LOG_WRITE_SYSTEM_LOG_INFORMATION(message)	CSystemLog::WriteLog(CSystemLog::LOG_TYPE_INFORMATION,  message)
#define LOG_WRITE_SYSTEM_LOG_WARNING(message)	    CSystemLog::WriteLog(CSystemLog::LOG_TYPE_WARNING, message)
#define LOG_WRITE_SYSTEM_LOG_ERROR(message)	        CSystemLog::WriteLog(CSystemLog::LOG_TYPE_ERROR, message)
#define Jingyu_MSG_MAX_SIZE (511+1)
#define LOG_WRITE_SYSTEM_LOG_ERROR_MYCATCH(chMuduleName)                             \
	char chMsgInMyCatch[Jingyu_MSG_MAX_SIZE];                                           \
	sprintf(chMsgInMyCatch,                                                          \
	"Exception! During excusing %s. Reason is: ",                                        \
	chMuduleName);                                                           \
	LOG_WRITE_SYSTEM_LOG_ERROR(chMsgInMyCatch);                                     

	static bool WriteLog(int nType,  QString pMessage) JingyuEXCEPTION_AVOIDABLE;
	
private:	
	static QFile     m_file; 
	static QMutex    m_mutex;
	static bool      m_bStart;
};

#endif //WRITE_SYSTOM_LOG
