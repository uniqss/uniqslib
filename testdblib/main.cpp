#include "dbtransactionmanager.h"
#include "dbtransaction.h"
#include "mysql_database.h"
using namespace UniqsDB;

#include <string>
#include <iostream>
#include <thread>
using namespace std;

#include "idus.h"
#include "idus_simple.h"

atomic<bool> bInsert;
atomic<bool> bDelete;
atomic<bool> bUpdate;
atomic<bool> bSelect;

atomic<bool> bInsertSimple;
atomic<bool> bDeleteSimple;
atomic<bool> bUpdateSimple;
atomic<bool> bSelectSimple;

bool bLogicThreadWorking = true;
bool bLogicThreadTerminated = false;

CDBTransactionManager* pManager = nullptr;

class CGameLogic
{
public:
	void LogicThread()
	{
		std::string strError;

		unsigned BulletinIDInsert = 0;
		unsigned BulletinIDDelete = 0;
		unsigned BulletinIDUpdate = 0;

		WEB_NOTICE_DBMEM oFakeNotice;
		WEB_NOTICE_DBMEMS oFakeNotices;

		unsigned InsertSimple = 0;
		unsigned DeleteSimple = 0;
		unsigned UpdateSimple = 100;

		while (bLogicThreadWorking)
		{
			this_thread::sleep_for(chrono::milliseconds(1));
			if (pManager)
			{
				pManager->Update();

				if (bInsert)
				{
					WEB_NOTICE_DBMEM oNotice;
					oNotice.BulletinContent = "hello world";
					oNotice.BulletinID = ++BulletinIDInsert;
					oNotice.CreateTime = (unsigned)time(0);
					oNotice.GMTOrderId = "GMTID:";
					oNotice.GMTOrderId += std::to_string(oNotice.BulletinID);
					oNotice.ValidDateStart = oNotice.CreateTime;
					oNotice.ValidDateEnd = oNotice.CreateTime + 86400;
					EDBOperationType opType = EDBOperationType_Insert;
					CDBTransactionTest * pTestTransaction = new CDBTransactionTest(oNotice, oFakeNotices, opType);
					if (!pManager->AddTransaction(pTestTransaction, strError))
					{
						cout << (char)opType << " !pManager->AddTransaction error:" << strError << endl;
						delete pTestTransaction;
						pTestTransaction = nullptr;
					}
					bInsert = false;
				}
				if (bDelete)
				{
					WEB_NOTICE_DBMEM oNotice;
					oNotice.BulletinID = ++BulletinIDDelete;
					oNotice.GMTOrderId = "GMTID:";
					oNotice.GMTOrderId += std::to_string(oNotice.BulletinID);
					EDBOperationType opType = EDBOperationType_Delete;
					CDBTransactionTest * pTestTransaction = new CDBTransactionTest(oNotice, oFakeNotices, opType);
					if (!pManager->AddTransaction(pTestTransaction, strError))
					{
						cout << (char)opType << " !pManager->AddTransaction error:" << strError << endl;
						delete pTestTransaction;
						pTestTransaction = nullptr;
					}
					bDelete = false;
				}
				if (bUpdate)
				{
					WEB_NOTICE_DBMEM oNotice;
					oNotice.BulletinContent = "world hello";
					oNotice.BulletinID = ++BulletinIDUpdate;
					oNotice.CreateTime = (unsigned)time(0);
					oNotice.GMTOrderId = "GMTID:";
					oNotice.GMTOrderId += std::to_string(oNotice.BulletinID);
					oNotice.ValidDateStart = oNotice.CreateTime;
					oNotice.ValidDateEnd = oNotice.CreateTime + 86400;
					EDBOperationType opType = EDBOperationType_Update;
					CDBTransactionTest * pTestTransaction = new CDBTransactionTest(oNotice, oFakeNotices, opType);
					if (!pManager->AddTransaction(pTestTransaction, strError))
					{
						cout << (char)opType << " !pManager->AddTransaction error:" << strError << endl;
						delete pTestTransaction;
						pTestTransaction = nullptr;
					}
					bUpdate = false;
				}
				if (bSelect)
				{
					EDBOperationType opType = EDBOperationType_Select;
					CDBTransactionTest * pTestTransaction = new CDBTransactionTest(oFakeNotice, oFakeNotices, opType);
					if (!pManager->AddTransaction(pTestTransaction, strError))
					{
						cout << (char)opType << " !pManager->AddTransaction error:" << strError << endl;
						delete pTestTransaction;
						pTestTransaction = nullptr;
					}
					bSelect = false;
				}

				// simple
				if (bInsertSimple)
				{
					EDBOperationType opType = EDBOperationType_Insert;
					CDBTransactionTestSimple * pTestTransaction = new CDBTransactionTestSimple(++InsertSimple, opType);
					if (!pManager->AddTransaction(pTestTransaction, strError))
					{
						cout << (char)opType << " !pManager->AddTransaction error:" << strError << endl;
						delete pTestTransaction;
						pTestTransaction = nullptr;
					}
					bInsertSimple = false;
				}
				if (bDeleteSimple)
				{
					EDBOperationType opType = EDBOperationType_Delete;
					CDBTransactionTestSimple * pTestTransaction = new CDBTransactionTestSimple(++DeleteSimple, opType);
					if (!pManager->AddTransaction(pTestTransaction, strError))
					{
						cout << (char)opType << " !pManager->AddTransaction error:" << strError << endl;
						delete pTestTransaction;
						pTestTransaction = nullptr;
					}
					bDeleteSimple = false;
				}
				if (bUpdateSimple)
				{
					EDBOperationType opType = EDBOperationType_Update;
					CDBTransactionTestSimple * pTestTransaction = new CDBTransactionTestSimple(++UpdateSimple, opType, InsertSimple - 1);
					if (!pManager->AddTransaction(pTestTransaction, strError))
					{
						cout << (char)opType << " !pManager->AddTransaction error:" << strError << endl;
						delete pTestTransaction;
						pTestTransaction = nullptr;
					}
					bUpdateSimple = false;
				}
				if (bSelectSimple)
				{
					EDBOperationType opType = EDBOperationType_Select;
					CDBTransactionTestSimple * pTestTransaction = new CDBTransactionTestSimple(0, opType);
					if (!pManager->AddTransaction(pTestTransaction, strError))
					{
						cout << (char)opType << " !pManager->AddTransaction error:" << strError << endl;
						delete pTestTransaction;
						pTestTransaction = nullptr;
					}
					bSelectSimple = false;
				}
			}
		}


		pManager->SafeTerminate();
		while (!pManager->SafeTerminated())
		{
			pManager->Update();
			this_thread::sleep_for(chrono::milliseconds(1));
		}

		bLogicThreadTerminated = true;
	}
};


int main()
{
	pManager = new CDBTransactionManager();
	IDatabase* pDatabase =  new CMySQLDatabase();

	DBConnectParam param;
	param.Host = "192.168.5.205";
	param.User = "sa";
	param.Password = "vbnm,.90";
	param.DB = "uniqs";
	param.Port = 3306;
	param.UnixSocket = "";
	param.ulFlags = 0;
	param.Charset = "utf8";
	param.dwConnectTimeout = 1;

	pManager->Init(pDatabase, param);

	CGameLogic oGameLogic;

	std::thread logicThread(&CGameLogic::LogicThread, oGameLogic);
	logicThread.detach();

	while (true)
	{
		string strInput;
		cin >> strInput;
		if (strInput == "I"||strInput == "i")
		{
			if (!bInsert) bInsert = true;
			continue;
		}
		if (strInput == "D"||strInput == "d")
		{
			if (!bDelete) bDelete = true;
			continue;
		}
		if (strInput == "U"||strInput == "u")
		{
			if (!bUpdate) bUpdate = true;
			continue;
		}
		if (strInput == "S"||strInput == "s")
		{
			if (!bSelect) bSelect = true;
			continue;
		}

		// simple
		if (strInput == "IS" || strInput == "is")
		{
			if (!bInsertSimple) bInsertSimple = true;
			continue;
		}
		if (strInput == "DS" || strInput == "ds")
		{
			if (!bDeleteSimple) bDeleteSimple = true;
			continue;
		}
		if (strInput == "US" || strInput == "us")
		{
			if (!bUpdateSimple) bUpdateSimple = true;
			continue;
		}
		if (strInput == "SS" || strInput == "ss")
		{
			if (!bSelectSimple) bSelectSimple = true;
			continue;
		}

		if (strInput == "E" || strInput == "e")
		{
			bLogicThreadWorking = false;
			break;
		}
	}

	while (!bLogicThreadTerminated)
	{
		this_thread::sleep_for(chrono::milliseconds(10));
	}

	delete pDatabase;
	delete pManager;

	return 0;
}

