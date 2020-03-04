#pragma once

#include "dbtransaction.h"
#include "idbconnection.h"
using namespace UniqsDB;

#include <string>
#include <vector>

#include <atomic>
using namespace std;

class CDBTransactionTestSimple : public CDBTransaction
{
protected:
	int					m_idFrom;
	int					m_id;
	vector<int>			m_vecSelectResult;
	EDBOperationType	m_DBOperationType;


	static atomic<int> m_DBTransactionTestSimpleIdx;

public:
	CDBTransactionTestSimple(int id, EDBOperationType opType, int idFrom = 0);
	virtual ~CDBTransactionTestSimple(void);

	virtual bool OnExecute(IDBConnection * pConnection);
	virtual void OnFinish();
};
