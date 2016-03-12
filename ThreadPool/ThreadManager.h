
class ThreadManager
{
private:
	ThreadPool *m_Pool; //The Real Thread Pool
	int	m_NumOfThread; //Start Number of Thread
public:
	void setParalleNum(int num);
	ThreadManager();
	ThreadManager(int num);
	virtual ~ThreadManager();

	void run(Job *job, void *jobdata);
	void terminateAll(void);
};
