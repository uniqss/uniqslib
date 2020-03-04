#include "idus.h"
#include "db_parameterset.h"
#include "idbconnection.h"

#include "../common_header/safecstringop.h"

#include <time.h>

#include <memory>
#include <iostream>
using namespace std;

atomic<int> CDBTransactionTest::m_DBTransactionTestIdx;


CDBTransactionTest::CDBTransactionTest(const WEB_NOTICE_DBMEM& rNotice, const WEB_NOTICE_DBMEMS& rNotices, EDBOperationType opType)
{
	m_WebNotice = rNotice;
	m_WebNotices = rNotices;
	m_DBOperationType = opType;
	++m_DBTransactionTestIdx;
	cout << "CDBTransactionTest::ctor:" << m_DBTransactionTestIdx << endl;
}

CDBTransactionTest::~CDBTransactionTest(void)
{
	m_DBOperationType = EDBOperationType_Invalid;
	cout << "CDBTransactionTest::dtor:" << m_DBTransactionTestIdx << endl;
}

bool CDBTransactionTest::OnExecute(IDBConnection * pConnection)
{
	const int nSQLBufferSize = 1024;
	char szSQL[nSQLBufferSize];

	int DBResult;
	CDBParameterSet ParameterSet;
	DB_PARAM_INFO ParamInfo;


	IDBRecordSet* __pTmpRecordSet = pConnection->CreateRecordSet(DB_RS_TYPE_DYNAMIC);
	if (__pTmpRecordSet == nullptr)
	{
		cout << "CDBTransBulletinInGame::OnExecute:无法创建结果集" << endl;
		return false;
	}

	std::auto_ptr<IDBRecordSet> pRecordSet(__pTmpRecordSet);

	if (m_DBOperationType == EDBOperationType_Insert)
	{
#if 1
		ParamInfo.Type = DB_TYPE_STRING;
		ParamInfo.ParamType = DB_PARAM_TYPE_INPUT;
		ParamInfo.IsNull = false;
		ParamInfo.Size = m_WebNotice.GMTOrderId.size();
		ParamInfo.DigitSize = 0;
		ParameterSet.AddParam(ParamInfo, (const char*)m_WebNotice.GMTOrderId.c_str());

		{ // Content
			ParamInfo.Type = DB_TYPE_STRING;
			ParamInfo.ParamType = DB_PARAM_TYPE_INPUT;
			ParamInfo.IsNull = false;
			ParamInfo.Size = m_WebNotice.BulletinContent.size();
			ParamInfo.DigitSize = 0;
			ParameterSet.AddParam(ParamInfo, m_WebNotice.BulletinContent.c_str());
		}

		sprintf_s(szSQL, nSQLBufferSize, "insert into t_bulletin_info_ingame(`GMTOrderId`, `Content`, "
			" `ValidDateStart`,`ValidDateEnd`) "
			" values(?,?,"
			" from_unixtime(%d),"
			" from_unixtime(%d)"
			" ) ",
			m_WebNotice.ValidDateStart,
			m_WebNotice.ValidDateEnd);

		cout << "CDBTransBulletinInGame::OnExecute:执行SQL:" << szSQL << endl;;
		DBResult = pConnection->ExecuteSQL(szSQL, 0, &ParameterSet);

		if (DBResult != DBERR_SUCCEED)
		{
			cout<< "CDBTransBulletinInGame::OnExecute:执行SQL出错:" <<
				pConnection->GetLastDatabaseErrorCode() << 
				pConnection->GetLastDatabaseErrorString() << endl;
			cout<<"CDBTransBulletinInGame::OnExecute:出错的SQL:" << szSQL;
			return false;
		}

		sprintf_s(szSQL, nSQLBufferSize, "select LAST_INSERT_ID() ");
		cout<< "CDBTransBulletinInGame::OnExecute:执行SQL:" << szSQL;
		DBResult = pConnection->ExecuteSQL(szSQL, 0);

		if (DBResult != DBERR_SUCCEED)
		{
			cout<< "CDBTransBulletinInGame::OnExecute:执行SQL出错:" <<
				pConnection->GetLastDatabaseErrorCode() <<
				pConnection->GetLastDatabaseErrorString() << endl;
			cout<< "CDBTransBulletinInGame::OnExecute:出错的SQL:" << szSQL;
			return false;
		}

		DBResult = pConnection->GetResults(pRecordSet.get());
		if (DBResult != DBERR_SUCCEED&&DBResult != DBERR_NO_RECORDS)
		{
			cout << "CDBTransBulletinInGame::OnExecute:获取结果集出错:" <<
				pConnection->GetLastDatabaseErrorCode() <<
				pConnection->GetLastDatabaseErrorString() << endl;
			cout << "CDBTransBulletinInGame::OnExecute:出错的SQL:" << szSQL << endl;
			return false;
		}
		if (!pRecordSet->IsEOF())
		{
			m_WebNotice.BulletinID = pRecordSet->GetField(0);
		}
		else
		{
			cout << "CDBTransBulletinInGame::OnExecute:无法获得最近插入的ID" << endl;
			return false;
		}

		m_WebNotice.CreateTime = (unsigned)time(0);
#endif
	}
	else if (m_DBOperationType == EDBOperationType_Delete)
	{
#if 1
		sprintf_s(szSQL, nSQLBufferSize, "delete from t_bulletin_info_ingame where GMTOrderId=? ");
		ParameterSet.Destory();

		ParamInfo.Type = DB_TYPE_STRING;
		ParamInfo.ParamType = DB_PARAM_TYPE_INPUT;
		ParamInfo.IsNull = false;
		ParamInfo.Size = m_WebNotice.GMTOrderId.size();
		ParamInfo.DigitSize = 0;
		ParameterSet.AddParam(ParamInfo, m_WebNotice.GMTOrderId.c_str());

		cout << "CDBTransBulletinInGame::OnExecute:执行SQL:" << szSQL << m_WebNotice.GMTOrderId << endl;
		DBResult = pConnection->ExecuteSQL(szSQL, 0, &ParameterSet);
		if (DBResult != DBERR_SUCCEED)
		{
			cout << "CDBTransBulletinInGame::OnExecute:执行SQL出错:" <<
				pConnection->GetLastDatabaseErrorCode() <<
				pConnection->GetLastDatabaseErrorString() << endl;
			cout << "CDBTransBulletinInGame::OnExecute:出错的SQL:" << szSQL << endl;
			return false;
		}
#endif
	}
	else if (m_DBOperationType == EDBOperationType_Update)
	{
#if 1
		ParamInfo.Type = DB_TYPE_STRING;
		ParamInfo.ParamType = DB_PARAM_TYPE_INPUT;
		ParamInfo.IsNull = false;
		ParamInfo.Size = m_WebNotice.BulletinContent.size();
		ParamInfo.DigitSize = 0;
		ParameterSet.AddParam(ParamInfo, m_WebNotice.BulletinContent.c_str());

		sprintf_s(szSQL, 1024, "update t_bulletin_info_ingame set Content=?, "
			" ValidDateStart=case when %d=0 then null else from_unixtime(%d) end, "
			" ValidDateEnd=case when %d=0 then null else from_unixtime(%d) end "
			" where GMTOrderId=? ",
			m_WebNotice.ValidDateStart,
			m_WebNotice.ValidDateStart,
			m_WebNotice.ValidDateEnd,
			m_WebNotice.ValidDateEnd);

		ParamInfo.Type = DB_TYPE_STRING;
		ParamInfo.ParamType = DB_PARAM_TYPE_INPUT;
		ParamInfo.IsNull = false;
		ParamInfo.Size = m_WebNotice.GMTOrderId.size();
		ParamInfo.DigitSize = 0;
		ParameterSet.AddParam(ParamInfo, m_WebNotice.GMTOrderId.c_str());

		cout << "CDBTransBulletinInGame::OnExecute:执行SQL:" << szSQL << endl;
		DBResult = pConnection->ExecuteSQL(szSQL, 0, &ParameterSet);

		if (DBResult != DBERR_SUCCEED)
		{
			cout << "CDBTransBulletinInGame::OnExecute:执行SQL出错:" <<
				pConnection->GetLastDatabaseErrorCode() <<
				pConnection->GetLastDatabaseErrorString() << endl;
			cout << "CDBTransBulletinInGame::OnExecute:出错的SQL:" << szSQL << endl;
			return false;
		}

		if (pConnection->GetAffectedRowCount() <= 0)
		{
			cout << "CDBTransBulletinInGame::OnExecute:更新操作没有影响到任何记录" << endl;
			return false;
		}
#endif
	}
	else if (m_DBOperationType == EDBOperationType_Select)
	{
#if 1
		sprintf_s(szSQL, 1024, "select BulletinID, `GMTOrderId`,Content, "
			"		unix_timestamp(ValidDateStart),unix_timestamp(ValidDateEnd), "
			"		unix_timestamp(CreateTime)"
			"	from t_bulletin_info_ingame  where ValidDateStart < now() and ValidDateEnd > now() "
			);

		cout << "CDBTransBulletinInGame::OnExecute:执行SQL:" << szSQL << endl;
		DBResult = pConnection->ExecuteSQL(szSQL, 0);

		if (DBResult != DBERR_SUCCEED)
		{
			cout << "CDBTransBulletinInGame::OnExecute:执行SQL出错:" <<
				pConnection->GetLastDatabaseErrorCode() <<
				pConnection->GetLastDatabaseErrorString() << endl;
			cout << "CDBTransBulletinInGame::OnExecute:出错的SQL:" << szSQL << endl;
			return false;
		}

		DBResult = pConnection->GetResults(pRecordSet.get());
		if (DBResult != DBERR_SUCCEED&&DBResult != DBERR_NO_RECORDS)
		{
			cout << "CDBTransBulletinInGame::OnExecute:获取结果集出错:" <<
				pConnection->GetLastDatabaseErrorCode() <<
				pConnection->GetLastDatabaseErrorString() << endl;
			cout << "CDBTransBulletinInGame::OnExecute:出错的SQL:" << szSQL << endl;
			return false;
		}

		while (!pRecordSet->IsEOF())
		{
			m_WebNotices.INFOS.resize(m_WebNotices.INFOS.size() + 1);
			auto pNotice = &m_WebNotices.INFOS[m_WebNotices.INFOS.size() - 1];
			int nIdx = 0;
			pNotice->BulletinID = pRecordSet->GetField(nIdx++);
			pNotice->GMTOrderId = (const char*)pRecordSet->GetField(nIdx++);
			pNotice->BulletinContent = (const char*)pRecordSet->GetField(nIdx++);
			pNotice->ValidDateStart = pRecordSet->GetField(nIdx++);
			pNotice->ValidDateEnd = pRecordSet->GetField(nIdx++);
			pNotice->CreateTime = pRecordSet->GetField(nIdx++);

			pRecordSet->MoveNext();
		}

		pRecordSet->Destory();
#endif
	}

	return true;
}
void CDBTransactionTest::OnFinish()
{
	printf("CDBTransBulletinInGame::OnFinish. %d\n", m_DBOperationType);
	if (m_DBOperationType == EDBOperationType_Select)
	{
		for (auto it = m_WebNotices.INFOS.begin(); it != m_WebNotices.INFOS.end(); ++it)
		{
			cout << "ID:" << it->BulletinID
				<< " GID:"<< it->GMTOrderId
				<< " C:"<< it->BulletinContent
				<< " CT:"<< it->CreateTime
				<< " VS:"<< it->ValidDateStart
				<< " VE:"<< it->ValidDateEnd << endl;
		}
	}
}
