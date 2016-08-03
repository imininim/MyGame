#include "mythreadpool.h"

using namespace std;
/*
Detach 状态 脱离主线程，线程结束之后自动关闭
Join 状态 必须在主线程里面被 join()

detach() 进入Detach状态
joinable()  线程是否可以join()
join()  join线程

std::mutex 最基本的锁
lock() 上锁
unlock() 解锁
try_lock() 尝试锁

std::recursive_mutex 递归锁
std::time_mutex 定时锁
std::recursive_timed_mutex 定时递归锁

Lock
std::lock_guard 
使用 std::lock_guard<std::mutex> lck(mtx) 自动解锁

std::unique_lock

std::call_once 如过多个线程需要同时调用某个函数,call_once可以保证该函数只被调用一次

condition_variable 条件变量
notify_one 唤醒一个线程
notify_all 唤醒所有线程
*/

namespace COMMON {

MyThreadpool::MyThreadpool(const int &thread_num, const int &stack_size):m_thread_num(thread_num), m_running(false) {}

MyThreadpool::~MyThreadpool(){}

bool MyThreadpool::push(Worker *work) {
	if(m_running) {
		m_taskqueue.put(work);
		return true;
	}
	return false;
}

bool MyThreadpool::start() {
	if(!m_threads.empty() || (m_thread_num == 0)) {
		return false;
	}
	m_threads.resize(m_thread_num);
	m_running = true;
	pthread_t thd;
	for(int i = 0; i < m_thread_num; i ++) {
		//++++++
		pthread_create(&thd, NULL, MyThreadpool::run, this);
		//std::thread *pthread = new thread(std::bind(&MyThreadpool::run, this));
		m_threads[i] = thd;
	}
	cout << "thread start!" << endl;
	return true;
}

void* MyThreadpool::run(void *args) {
	Worker *work;
	MyThreadpool *pool = (MyThreadpool *)args;
	while(pool->m_running && pool->m_taskqueue.get(work)) {
		work->doWork();
	}
}

void MyThreadpool::waitforstop() {
	for(int i = 0; i < m_threads.size(); i ++) {
		//pthread_kill(m_thread[i], SIGTERM);
		pthread_join(m_threads[i], NULL);
		//thread *pthread = *it;
		//pthread->join();
		//delete pthread;
	}
	m_threads.clear();
}

void MyThreadpool::exit() {
	m_running = false;
	m_taskqueue.stop();

	waitforstop();
	cout << "threadpool exit!" << endl;
}

}
