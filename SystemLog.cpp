/******************************************************************************
  Copyright (C), 2008-2008, Shanghai Deewe Communication Tech. Co., Ltd.
  FileName: NetLesson.cpp
  Author:        Version :          Date: 
  Liu Shengju    1.1.1.0            08/10/23     
  Description:      ϵͳ��־      
  Version:          ��ʼ��
  Function List:   // ��Ҫ�������书�� (����Ӧ��ͷ�ļ�һ��)
    1. bool Init(QString fileName)     // ��ʼ��  ����־�ļ���������Ӧ�Ĵ��� 
	2. void Uninit()                   // �ر���־�ļ� 
	3. bool WriteLog(int nType, QString pMessage) // ��־д���ļ�
  History:         // �޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸�
                   // �߼��޸����ݼ���  
    1. Date:
       Author:
       Modification:
    2. ...
      
********************************************************************************/

#include "SystemLog.h"

QFile     CSystemLog::m_file; 
QMutex    CSystemLog::m_mutex;
bool      CSystemLog::m_bStart;

CSystemLog::CSystemLog()
{
	this->m_bStart = false;
}

CSystemLog::~CSystemLog()
{
	this->m_bStart = false;
}

bool CSystemLog::Init(QString fileName)
{	
	QString directory = "Log";	
	return Init(fileName, directory);    
}


bool CSystemLog::Init(QString fileName, QString directory)
{
	bool bSuccess = true;
	this->m_bStart  = true;
	
	do 
	{
		if (fileName.isEmpty() || directory.isEmpty())
		{
			bSuccess = false;
			break;
		}

		// 1 ���Ŀ¼
		/*QDir dir(directory);
		bool bResult = dir.exists();
		if (false == bResult)
			if (!dir.mkdir(directory))
			{
                bSuccess = false;
				break ;
			}*/

		// 2 ����ļ�
		QString psName = /* "./" + directory + "/" + */fileName;
		m_file.setFileName(psName);
		if (m_file.exists())
			if (m_file.size() >= LOG_MAX_BUFFER)
				if (!m_file.remove())
				{
					bSuccess = false;
				    break;
				}

		//  ���ļ�
		if (!m_file.open(QFile::WriteOnly | QFile::Text | QFile::Append))
		{
			bSuccess = false;
			break;
		}			
	}while(0);

	return bSuccess;
}

void CSystemLog::Uninit()
{
	this->m_bStart = false;
	m_file.close();
}


bool CSystemLog::WriteLog(int nType, QString pMessage)
{
	bool bSuccess = true;	
	QString pLogType;
	QMutexLocker locker(&m_mutex);

	do {	
		// ��ȡʱ����Ϣ
        QString     msgInfo("Log Time: ");
		QDateTime   LocTime = QDateTime::currentDateTime().toLocalTime();
		msgInfo.append(LocTime.toString("yy-MM-dd hh:mm:ss")); 
		msgInfo.append("\n");

		// ��־����
		switch(nType){
		case LOG_TYPE_INFORMATION:
			msgInfo.append("Log Class:Information\n" );
			break;
		case LOG_TYPE_WARNING:
			msgInfo.append("Log Class:Warning\n" );
			break;
		case LOG_TYPE_ERROR:
			msgInfo.append("Log Class:Error\n" );
			break;
		default:
			break;
		}

		msgInfo.append("Log information:");
		msgInfo.append(pMessage);
		msgInfo.append("\n\n");

        if (m_file.write(msgInfo.toUtf8()) < 0)
		{
			bSuccess = false;
		}
		else
			m_file.flush();

	}while(0);

	return bSuccess;
}
