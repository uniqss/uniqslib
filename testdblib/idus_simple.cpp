#include "idus_simple.h"
#include "db_parameterset.h"

#include "../common_header/safecstringop.h"

#include <time.h>

#include <memory>
#include <iostream>
using namespace std;

atomic<int> CDBTransactionTestSimple::m_DBTransactionTestSimpleIdx;


CDBTransactionTestSimple::CDBTransactionTestSimple(int id, EDBOperationType opType, int idFrom)
{
	m_idFrom = idFrom;
	m_id = id;
	m_DBOperationType = opType;
	++m_DBTransactionTestSimpleIdx;
	cout << "CDBTransactionTestSimple::ctor:" << m_DBTransactionTestSimpleIdx << endl;
}

CDBTransactionTestSimple::~CDBTransactionTestSimple(void)
{
	m_DBOperationType = EDBOperationType_Invalid;
	cout << "CDBTransactionTestSimple::dtor:" << m_DBTransactionTestSimpleIdx << endl;
}

bool CDBTransactionTestSimple::OnExecute(IDBConnection * pConnection)
{
	const int nSQLBufferSize = 1024;
	char szSQL[nSQLBufferSize];

	int DBResult;

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

		sprintf_s(szSQL, nSQLBufferSize, "insert into simple(`id`) values(%d)", m_id);

		cout << "CDBTransBulletinInGame::OnExecute:执行SQL:" << szSQL << endl;;
		DBResult = pConnection->ExecuteSQL(szSQL);

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
			m_id = pRecordSet->GetField(0);
		}
		else
		{
			cout << "CDBTransBulletinInGame::OnExecute:无法获得最近插入的ID" << endl;
			return false;
		}
#endif
	}
	else if (m_DBOperationType == EDBOperationType_Delete)
	{
#if 1
		sprintf_s(szSQL, nSQLBufferSize, "delete from simple where `id`=%d ", m_id);

		cout << "CDBTransBulletinInGame::OnExecute:执行SQL:" << szSQL << m_id << endl;
		DBResult = pConnection->ExecuteSQL(szSQL);
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
		sprintf_s(szSQL, 1024, "update simple set `id`=%d"
			" where `id`=%d ",
			m_idFrom, m_idFrom);

		cout << "CDBTransBulletinInGame::OnExecute:执行SQL:" << szSQL << endl;
		DBResult = pConnection->ExecuteSQL(szSQL);

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
		sprintf_s(szSQL, 1024, "select `id` from simple ");

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
			int nIdx = 0;
			m_vecSelectResult.push_back(pRecordSet->GetField(nIdx++));

			pRecordSet->MoveNext();
		}

		pRecordSet->Destory();
#endif
	}

	return true;
}
void CDBTransactionTestSimple::OnFinish()
{
	printf("CDBTransBulletinInGame::OnFinish. %d\n", m_DBOperationType);
	if (m_DBOperationType == EDBOperationType_Select)
	{
		for (auto it = m_vecSelectResult.begin(); it != m_vecSelectResult.end(); ++it)
		{
			cout << "ID:" << *it << endl;
		}
	}
}
