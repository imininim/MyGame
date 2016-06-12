#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <iostream>
#include "work/add.h"
#include "work/sub.hpp"
#include "json/json.h"
#include <mysql/mysql.h>
#include "work/MySQLManager.h"
#include <thread>
#include "common/mythreadpool.h"
#include "common/socketServer.h"
#include "common/myMempool.h"
#include "common/mycommonfuc.h"
#include "common/sockClient.h"

using namespace std;

//MySQLManager mysqlManager = MySQLManager("127.0.0.1","root","yangshu","test");

void fun1() {
	int cnt = 100;
	MySQLManager mysqlManager = MySQLManager("127.0.0.1","root","yangshu","test");
	mysqlManager.initConnection();
	while(cnt --) {
		vector<vector<string> > result;
		mysqlManager.select("select * from user", result);
		for(int i = 0; i < result.size(); i ++) {
			for(int j = 0; j < result[i].size(); j ++) {
				cerr << result[i][j] << " ";
			}
			cerr << endl;
		}
	}
}

void fun2() {
	int cnt = 100;
	MySQLManager mysqlManager = MySQLManager("127.0.0.1","root","yangshu","test");
	mysqlManager.initConnection();
	while(cnt --) {
		vector<vector<string> > result;
		mysqlManager.select("select * from card", result);
		for(int i = 0; i < result.size(); i ++) {
			for(int j = 0; j < result[i].size(); j ++) {
				cout << result[i][j] << " ";
			}
			cout << endl;
		}
	}
}

struct AA {
	int x;
};

int main1() {
	MemPool pool(sizeof(AA));
	//cerr << "sizeof int " << sizeof(int) << endl;
	//cerr << "sizeof int*" << sizeof(int*) << endl;
	std::vector<AA *> v;
	COMMON::MYTimer t;
	for(int j = 0; j < 100; j ++)
	{
		for(int i = 0; i < 5000; i ++) {
			AA *a = (AA*)pool.get();
			a->x = i;
			//printf("%d\n", a);
			//cerr << a->x << endl;
			v.push_back(a);
		}
	
		for(int i = 0; i < v.size(); i ++) {
			pool.del(v[i]);
		}
		v.clear();
	}

	cerr << t.cost() << endl;

	cout << "***********************************" << endl;
	v.clear();
	COMMON::MYTimer t1;
	for(int j = 0; j < 100; j ++) {
		for(int i = 0; i < 5000; i ++) {
			AA *a = new(AA);
			a->x = i;
			v.push_back(a);
		}
		for(int i = 0; i < v.size(); i ++) {
			delete v[i];
		}
		v.clear();
	}
	
	cerr << t1.cost() << endl;
	/*cerr << "******************************************" << endl;
	
	for(int i = 0; i < 10; i ++) {
		AA *a = (AA*)pool.get();
		a->x = i;
		//cerr << "XX" << endl;
		//cerr << a << endl;
		v.push_back(a);
	}
	for(int i = 0; i < v.size(); i ++) {
		cerr << v[i]->x << endl;
		pool.del(v[i]);
	}*/

	return 0;
}

int main3() {
	std::string str = "{\"begin_time\":1463538812,\"ccy\":\"CNY\",\"checkin\":\"20160528\",\"checkout\":\"20160528\",\"data_type\":\"hotel\",\"en\":\"0\",\"err_code\":100,\"hid\":\"ht20072100\",\"need_info\":{\"2\":{\"num\":1,\"occ\":2,\"route_source\":\"hotels\",\"type\":\"双人房\"}},\"otype\":0,\"qid\":\"1463538814669\",\"redis_key\":\"hotel|ht20072100|1|20160528|1463538814669\",\"req_count\":0,\"request_md5\":\"83c7785f59931ae3df7e8e5bcb0dd2d8\",\"uid\":\"1270\"}";
	cout << str << endl;
	Json::Reader jr;
	Json::Value Hotel;
	jr.parse(str.c_str(), Hotel["vid"]);
	//cout << Hotel << endl;
	COMMON::MathConstData::init(1000000);
	for(double x = 0; x < PI*2; x += 0.5) {
		cout << "MY_Sin = " << COMMON::MathConstData::Sin(x) << endl;
		cout << "sin = " << sin(x) << endl;
		cout << "MY_Cos = " << COMMON::MathConstData::Cos(x) << endl;
		cout << "cos = " << cos(x) << endl;
		cout << "MY_ACos = " << COMMON::MathConstData::ACos(x) << " x = " << x << endl;
		cout << "acos = " << acos(x) << " x = " << x << endl;
	}
	return 0;
}

int main2() {
	printf("hello world!\n");
	/*COMMON::MyThreadpool threadpool(2,0);
	threadpool.start();
	while(1) {
		COMMON::Work work;
		threadpool.push(&work);
		sleep(1);
	}*/
	SocketServer socketServer(2,10086);
	socketServer.start();
	socketServer.open();
	//int a = 1, b = 2;
	//cout << "1 add 2 = " << ADD::add(a, b) << endl;
	//cout << "1 sub 2 = " << SUB::sub(a, b) << endl;
	//Json::Value jv;
	//jv["json"] = "aaa";
	//cerr << jv << endl;
	//Json::Reader jr;
	//Json::FastWriter jw;
	//cerr << jw.write(jv) << endl;

	//MySQLManager mysqlManager = MySQLManager("127.0.0.1","root","yangshu","test");
	//mysqlManager.initConnection();
	//std::thread t1(fun1);
	//std::thread t2(fun2);
	//MySQLManager mysqlManager = MySQLManager("127.0.0.1","root","yangshu","test");
	//mysqlManager.initConnection();
	//vector<vector<string> > result;
	//mysqlManager.select("select * from user", result);
	//for(int i = 0; i < result.size(); i ++) {
	//	for(int j = 0; j < result[i].size(); j ++) {
	//		cerr << result[i][j] << " ";
	//	}
	//	cerr << endl;
	//}
	//t1.join();
	//t2.join();
	return 0;
}

int main_Server() {
	COMMON::MathConstData::init(1000000);
	COMMON::MYTimer t;
	for(int i = 0; i < 10000000; i ++) {
		sin(i*0.001);
		cos(i*0.001);
		acos(i*0.0000001);
	}
	cout << t.cost() << endl;

	COMMON::MYTimer t1;
	for(int i = 0; i < 10000000; i ++) {
		COMMON::MathConstData::Cos(i*0.001);
		COMMON::MathConstData::Sin(i*0.001);
		COMMON::MathConstData::ACos(i*0.0000001);
	}
	cout << t1.cost() << endl;

	return 0;
}


int main() {
	struct hostent *host;
	if((host=gethostbyname("www.baidu.com")) == NULL) {
		fprintf(stderr,"不能得到IP\n");
		exit(1);
	}
	printf("HostName :%s\n",host->h_name);
	printf("IP Address :%s\n",inet_ntoa(*((struct in_addr *)host->h_addr)));
	return 0;
	MYSOCK::SocketClient client("220.181.111.111:80", 0, 0);
	client.init();
	//client.send("data");
	client.send("<Head>GET /?token=95469678194374a4f25596e622edf1ed HTTP/1.1 A:A\r\n B:B\r\n\r\n");
	//client.recv();

	cout << "send end!" << endl;

	string file;
	client.recv(file);

	cout << file << endl;

	cout << "get end!" << endl;

	FILE *fp;
	fp = fopen("a.out", "w");
	if(fp == NULL) {
		cout << "Error" << endl;
		return 0;
	}

	for(int i = 0; i < file.length(); i ++) {
		fwrite(file.c_str(), sizeof( unsigned char ), i, fp);
	}

	cout << "write end!" << endl;
	fclose(fp);
}
