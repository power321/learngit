#include <ThreadManager.h>

ThreadManager::ThreadManager()
{
	m_NumOfThread = 10;
	m_Pool = new ThreadPool(m_NumOfThread);
}

ThreadManager::ThreadManager(int num)
{
	m_NumOfThread = num;
	m_Pool = new ThreadPool(m_NumOfThread);
}

ThreadManager::~ThreadManager()
{
	if(NULL != m_Pool)
		delete m_Pool;
}

void ThreadManager::setParalleNum(int num)
{
	m_NumOfThread = num;
}

void ThreadManager::run(Job *job, void *jobdata)
{
	m_Pool->run(job, jobdata);
}

void ThreadManager::terminateAll(void)
{
	m_Pool->terminateAll();
}
