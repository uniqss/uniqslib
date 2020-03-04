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
	// 增加、删除、更新的一条信息使用
	WEB_NOTICE_DBMEM							m_WebNotice;
	// 这个是服务器起来的时候查询用的
	WEB_NOTICE_DBMEMS							m_WebNotices;
	EDBOperationType							m_DBOperationType;


	static atomic<int> m_DBTransactionTestIdx;

public:
	CDBTransactionTest(const WEB_NOTICE_DBMEM& rNotice, const WEB_NOTICE_DBMEMS& rNotices, EDBOperationType opType);
	virtual ~CDBTransactionTest(void);

	virtual bool OnExecute(IDBConnection * pConnection);
	virtual void OnFinish();
};
