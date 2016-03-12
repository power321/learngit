#include <CThreadPool.h>
#include <cassert>
using namespace std;

CThreadPool::CThreadPool()
{
	m_MaxNum = 50;
	m_AvailLow = 5;
	m_InitNum = m_AvailNum = 10;
	m_AvailHigh = 20;

	m_BusyList.clear();
	m_IdleList.clear();
	for(int i=0; i<m_InitNum; ++i)
	{
		CWorkerThread *thr = new CWorkerThread();
		thr->setThreadPool(this);
		appendToIdleList(thr);
		thr->start();
	}
}

CThreadPool::CThreadPool(int num)
{
	assert(num>0 && num<=30);
	m_MaxNum = 30;
	m_AvailLow = num-10 > 0 ? num-10 : 3;
	m_InitNum = m_AvailNum = num;
	m_AvailHigh = num + 10;

	m_BusyList.clear();
	m_IdleList.clear();
	for(int i=0; i<m_InitNum; ++i)
	{
		CWorkerThread *thr = new CWorkerThread();
		appendToIdleList(thr);
		thr->setThreadPool(this);
		thr->start();//Wait for Job
	}
}

CThreadPool::~CThreadPool()
{
	terminateAll();
}

void CThreadPool::terminateAll()
{
	for(int i=0; i<m_ThreadList.size(); ++i)
	{
		CWorkerThread *thr = m_ThreadList[i];
		thr->join();
	}
	return ;
}

CWorkerThread *CThreadPool::getIdleThread(void)
{
	while(m_IdleList.size() == 0)
		m_IdleCond.Wait();

	m_IdleMutex.Lock();
	if(m_IdleList.size() > 0)
	{
		CWorkerThread *thr = (CWorkerThread*)m_IdleList.front();
		cout << "Get Idle Thread " << thr->getThreadID() << endl;
		m_IdleMutex.Unlock();
		return thr;
	}
	m_IdleMutex.Unlock();
	return NULL;
}

//Add An Idle Thread to Idle List
void CThreadPool::appendToIdleList(CWorkerThread *jobthread)
{
	m_IdleMutex.Lock();
	m_IdleList.push_back(jobthread);
	m_ThreadList.push_back(jobthread);
	m_IdleMutex.Unlock();
}

//Move Idle Thread to Busy Thread
void CThreadPool::moveToBusyList(CWorkerThread *idlethread)
{
	m_BusyMutex.Lock();
	m_BusyList.push_back(idlethread);
	m_AvailNum--;
	m_BusyMutex.Unlock();

	m_IdleMutex.Lock();
	vector<CWorkerThread*>::iterator pos;
	pos = find(m_IdleList.begin(), m_IdleList.end(), idlethread);
	if(pos != m_IdleList.end())
		m_IdleList.erase(pos);
	m_IdleMutex.Unlock();
}

void CThreadPool::moveToIdleList(CWorkerThread *busythread)
{
	m_IdleMutex.Lock();
	m_IdleList.push_back(busythread);
	m_AvailNum++;
	m_IdleMutex.Unlock();

	m_BusyMutext.Lock();
	vector<CWorkerThread*>::iterator pos;
	pos = find(m_BusyList.begin(), m_BusyList.end(), busythread);
	if(pos != m_BusyList.end())
		m_BusyList.erase(pos);
	m_BusyMutex.Unlock();

	m_IdleCond.Signal();
	m_MaxNumCond.Signal();
}


