#pragma once

#include "dbtransaction.h"
using namespace UniqsDB;

#include <string>
#include <vector>

#include <atomic>
using namespace std;


struct WEB_NOTICE_DBMEM
{
	unsigned		BulletinID;
	std::string		GMTOrderId;
	std::string		BulletinContent;
	unsigned		ValidDateStart;
	unsigned		ValidDateEnd;
	unsigned		CreateTime;
};

struct WEB_NOTICE_DBMEMS
{
	std::vector<WEB_NOTICE_DBMEM>	INFOS;
};

class CDBTransactionTest : public CDBTransaction
{
protected:
	// ���ӡ�ɾ�������µ�һ����Ϣʹ��
	WEB_NOTICE_DBMEM							m_WebNotice;
	// ����Ƿ�����������ʱ���ѯ�õ�
	WEB_NOTICE_DBMEMS							m_WebNotices;
	EDBOperationType							m_DBOperationType;


	static atomic<int> m_DBTransactionTestIdx;

public:
	CDBTransactionTest(const WEB_NOTICE_DBMEM& rNotice, const WEB_NOTICE_DBMEMS& rNotices, EDBOperationType opType);
	virtual ~CDBTransactionTest(void);

	virtual bool OnExecute(IDBConnection * pConnection);
	virtual void OnFinish();
};
