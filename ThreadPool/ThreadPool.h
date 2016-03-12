#include <CWorkThread.h>
#include <CThreadMutex.h>
#include <CCondition.h>
#include <vector>
using namespace std;

class CThreadPool
{
	friend class CWorkerThread;
private:
	unsigned int m_MaxNum; //The Max Thread Num that can Create at Same Time
	unsigned int m_AvailLow; //The Min Num of Idle Thread that Should Kept
	unsigned int m_AvailHigh; //The Max Num of Idle Thead that Kept
	unsigned int m_AvailNum; //The Normal Thread Num of Idle Num;
	unsigned int m_InitNum; //Normal Thread Num;

protected:
	CWorkerThread *getIdleThread(void);
	
	void appendToIdleList(WorkerThread *jobThread);
	void moveToBusyList(WorkerThread *idleThread);
	void moveToIdleList(WorkerThread *busyThread);

	void deleteIdleThread(int num);
	void createIdleThread(int num);

public:
	CThreadMutex m_BusyMutex; //Lock or Unlock when Visit Busy List
	CThreadMutex m_IdleMutex; //Lock or Unlock when Visit Idle List
	CThreadMutex m_JobMutex; //Lock or Unlock Job List;
	CThreadMutex m_VarMutex;

	CCondition m_BusyCond; //To Sync Busy List
	CCondition m_IdleCond; //To Sync Idle List
	CCondition m_JobCond; //To Sync Job List
	CCondition m_MaxNumCond;

	vector<CWorkThread*> m_ThreadList;
	vector<CWorkThread*> m_BusyList;
	vector<CWorkThread*> m_IdleList;

	CThreadPool();
	CThreadPool(int num);
	virtual ~CThreadPool();

	void setMaxNum(int num) { m_MaxNum = num; }
	int getMaxNum(void) { return m_MaxNum; }
	void setAvailLowNum(int num) { m_AvailLow = num; }
	int getAvailLowNum(void) { return m_AvailLow; }
	void setAvailHighNum(int num) { m_AvailHigh = num; }
	int getAvailHighNum(void) { return m_AvailHigh; }
	int getActualAvailNum(void) { return m_AvailNum; }
	int getAllNum(void) { return m_ThreadList.size(); }
	int getBusyNum(void) { return m_BusyList.size(); }
	void setInitNum(int num) { m_InitNum = num; }
	int getInitNum(void) { return m_InitNum; }

	void terminateAll(void);
	void run(CJob *job, void *jobdata);
};
