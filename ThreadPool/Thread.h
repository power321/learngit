#include <semaphore.h>
#include <cstring>

class Thread
{
private:
	int m_ErrCode;
	Semaphore m_ThreadSemaphore;
	unsigned long m_ThreadID;
	bool m_Detach; //Thread is detached or not
	bool m_CreateSuspended; //If suspend after create
	char *m_ThreadName;
	ThreadState m_ThreadState; //The State of Thread

protected:
	void setErrcode(int errcode) { m_Errcode = errcode; }
	static void *ThreadFunction(void*);

public:
	Thread();
	Thread(bool createsuspended, bool detach);
	virtual ~Thread();
	virtual void run(void)=0;
	void setThreadState(ThreadState state) { m_ThreadState = state; }

	bool terminate(void); //Teminate the Thread
	bool start(void); //Start to Excute the Thread
	void exit(void); //Exit the Thread
	bool wakeUp(void);

	ThreadState getThreadState(void) { return m_ThreadState; }
	int getLastError(void) { return m_Errcode; }
	void setThreadName(char *thrname) { strcpy(m_ThreadName, thrname); }
	char *getThreadName(void) { return m_ThreadName; }
	int getThreadID(void) { return m_ThreadID; }

	bool setPriority(int priority);
	int getPriority(void);
	int getConcurrency(void);
	void setConcurrency(int num);
	bool detach(void);
	bool join(void);
	bool yield(void);
	int	self(void);
};
