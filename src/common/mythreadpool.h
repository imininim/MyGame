#ifndef __MYTHREADPOOL__
#define __MYTHREADPOOL__

#include "myqueue.hpp"
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <string>
#include <stdio.h>
#include <stdlib.h>  
#include <syslog.h>  
#include <errno.h>
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>
#include <unistd.h>
#include <fstream>
#include <string.h>

namespace COMMON {

/*
template<typename T>
class Worker {
public:
private:
	COMMON::MyTaskQueue<T> taskque;
};*/

//加锁队列
template<typename T>
class MyTaskQueue {
public:
	MyTaskQueue():m_size(0), m_alive(true) {
		m_head._next = &m_head;
		m_head._pre = &m_head;
	}
	void put(const T &v) {
		//printf("PUT\n");
		link_node<T> *tmp = new link_node<T>();
		tmp->_value = v;
		std::unique_lock<std::mutex> lck(m_mtx);
		if(m_alive) {
			tmp->_next = m_head._next;
			(m_head._next)->_pre = tmp;
			tmp->_pre = &m_head;
			m_head._next = tmp;

			m_size ++;
		}
		m_cond.notify_one();
		lck.unlock();
		//printf("PUT SUCCESS\n");
	}
	/*bool get(T *v) {
		link_node<T> *t;
		std::unique_lock<std::mutex> lck(m_mtx);
		while(m_alive && isempty()) {
			m_cond.wait(lck);
		}
		
		if(m_alive) {
			t = m_head._pre;

			t->_next->_next = &m_head;
			m_head._pre = t->_pre;
			v = &t->_value;

			m_size --;
		}
		else {
			t = nullptr;
			return false;
		}
		lck.unlock();
		if(t != nullptr) {
			delete t;
		}
		return true;
	}*/
	bool get(T &v) {
		//printf("GET\n");
		link_node<T> *t;
		std::unique_lock<std::mutex> lck(m_mtx);
		while(m_alive && isempty()) {
			m_cond.wait(lck);
		}
		if(m_alive) {
			t = m_head._pre;

			t->_next->_next = &m_head;
			m_head._pre = t->_pre;
			v = t->_value;

			m_size --;
		}
		else {
			t = nullptr;
			return false;
		}
		lck.unlock();
		if(t != nullptr) {
			delete t;
		}
		//printf("GET SUCCESS\n");
		return true;
	}
	int size() {
		return m_size;
	}
	bool isempty() {
		return m_head._pre == &m_head;
	}

	void stop() {
		std::unique_lock<std::mutex> lck(m_mtx);
		m_alive = false;
		m_cond.notify_all();
	}
private:
	link_node<T> m_head;
	int m_size;
	bool m_alive;
	std::mutex m_mtx;
	std::condition_variable m_cond;
};


class Worker {
public:
	Worker(){};
	virtual ~Worker(){};
public:
	virtual int doWork()=0;
};

class Work:public Worker {
public:
	Work(){}
	Work(const std::string &com, const int &fd, std::string path = "a.out"):m_com(com), m_fd(fd), m_path(path) {}
	virtual int doWork() {
		std::cout << "AAA" << std::endl;
		std::cout << m_com << std::endl;
		if(m_com == "file") {
			std::cerr << "start sendfile()" << std::endl;
			sendfile();
		}
		return 0;
	}
	virtual ~Work() {};
private:
	enum {MAXLENGTH = 1024*100};
	std::string m_path;
	std::string m_com;
	int m_fd;
public:
	void sendfile() {
		//write();
		std::ifstream fin;
		fin.open(m_path, std::ios::binary);

		char buf[MAXLENGTH];
		bzero(buf, MAXLENGTH);

		fin.read(buf, MAXLENGTH);

		int len = send(m_fd, buf, MAXLENGTH, 0);

		if((len == -1)) {
			std::cerr << "error" << std::endl;
			exit(1);
		}
		std::cerr << "fread ok" << std::endl;
		//传输
		std::cerr << "recv ok" << std::endl;
		::close(m_fd);
	}
};



class MyThreadpool {
//typedef std::function<void(void)> Work;
public:
	MyThreadpool(const int &thread_num, const int &stack_size);
	~MyThreadpool();

	bool start();
	bool push(Worker *work);
	void exit();

private:
	void run();
	void waitforstop();

private:
	std::vector<std::thread *> m_threads;
	int m_thread_num;
	volatile bool m_running;
public:
	COMMON::MyTaskQueue<Worker *> m_taskqueue;
};

}

#endif
