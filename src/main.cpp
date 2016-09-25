#include <iostream>
using namespace std;
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <set>
#include <service/ServerFrame.h>
#include <tr1/unordered_map>
#include <map>

#include <work/Game.h>
#include <work/GameConfig.h>
#include <work/TimeQueue.h>
#include <common/MySQLManager.h>
//#include <work/>

std::tr1::unordered_map<std::string, iTCPHandlePtr> ID2Sock;
std::map<iTCPHandlePtr, std::string> Sock2ID;

struct CMyFrame: public CFrame
{
// 框架必须显示构造其基类，  传递 服务名称， 监听的端口
	CMyFrame(): CFrame("Game", GameConfig::listen_port){}
	/////////////////////////////////////////////////////////////////////////
	//初始化
	virtual bool Initialize(void)
	{ 
		static char buf[]= "hello signal!";
		static char buf1[] = "hello Timer 1";
		static char buf3[] = "hello Timer 3";

		m_pServer->AddSignal(20, (void*)buf); // ctrl-z 信号
		m_pServer->SetDeadCheck(300);
			
		struct	timeval  ti = {1, 0};  //1秒一次的定时器
		struct	timeval  ti2 = {30, 0};  //30秒一次的定时器
		m_pServer->SetTimer(100, ti, (void*)buf1);
		m_pServer->SetTimer(101, ti2, (void*)buf1);

		return true;
	
	}
//////////////////////////////////////////////////////////////////////////////////////
//
//

//////////////////////////////////////////////////////////////////////
///反初始化
	virtual void Uninitalize(void)
	{
		m_handles.clear(); //释放所有的客户端连接

		m_pServer->KillTimer(100);//释放定时器
		m_pServer->KillTimer(101);//释放定时器
	}

////////////////////////////////////////////////////////////////////////////////////////////////////
//登录、下线
	virtual void OnLogin(iTCPHandlePtr& pTCPHandle, bool bLogin, const SLoginInfo& loginInfo)
	{
		if(bLogin)
		{
			//std::string str("well come to heiye chat!");
			
			cout << "CMyFrame::OnLogin 有客户端登录服务器 ip = " << loginInfo.strLoginIP.c_str() << "port = " << loginInfo.nRemotePort << endl;
			//pTCPHandle->SendMsg(str.c_str(), str.size());
			m_handles.insert(pTCPHandle); //保存这个连接

		}
		else
		{
			cout << "CMyFrame::OnLogin 有客户端断开连接 ip = " << loginInfo.strLoginIP.c_str() << "port = " << loginInfo.nRemotePort << endl;

			std::string uid;
			if(Sock2ID.find(pTCPHandle) != Sock2ID.end()) {
				uid = Sock2ID[pTCPHandle];
			}
			//单方面存在的直接删除
			if(uid != "" && ID2Sock.find(uid) == ID2Sock.end()) {
				cerr << "单方面存在" << std::endl;
				std::map<iTCPHandlePtr, std::string>::iterator iit = Sock2ID.find(pTCPHandle);
				Sock2ID.erase(iit);
				uid = "";
			}
			//单方面存在的直接删除
			if(uid != "" && ID2Sock.find(uid) != ID2Sock.end()) {
				if(pTCPHandle != ID2Sock[uid]) {
					cerr << "不对等存在" << std::endl;
					std::map<iTCPHandlePtr, std::string>::iterator iit = Sock2ID.find(pTCPHandle);
					Sock2ID.erase(iit);
					uid = "";
				}
			}

			if(uid != "") {
				std::vector<Resp> resp;
				Game::doGame(uid, "logout:", resp);
				if(uid != "") {
					if(Sock2ID.find(pTCPHandle) == Sock2ID.end()) {
						Sock2ID[pTCPHandle] = uid;
					}
					if(ID2Sock.find(uid) == ID2Sock.end()) {
						ID2Sock[uid] = pTCPHandle;
					}

					for(int i = 0; i < resp.size(); i ++) {
						std::string &id = resp[i].m_uid;
						std::string res = resp[i].m_resp+"#";
						cerr << "to id:" << id << "   resp:" << res << endl;
						if(ID2Sock.find(id) != ID2Sock.end()) {
							iTCPHandlePtr &sender = ID2Sock[id];
							sender->SendMsg(res.c_str(), res.size());
						}
						else {
							cerr << id << "  error id" << endl;
						}
					}
				}
				else {
					for(int i = 0; i < resp.size(); i ++) {
						std::string &id = resp[i].m_uid;
						std::string res = resp[i].m_resp+"#";
						cerr << "to id:" << id << "   resp:" << res << endl;
						pTCPHandle->SendMsg(res.c_str(), res.size());
					}
				}

				std::tr1::unordered_map<std::string, iTCPHandlePtr>::iterator sit = ID2Sock.find(uid);
				std::map<iTCPHandlePtr, std::string>::iterator iit = Sock2ID.find(pTCPHandle);
				ID2Sock.erase(sit);
				Sock2ID.erase(iit);
			}

			m_handles.erase(pTCPHandle); //释放这个链接,如果你不释放这个链接， 框架不会再次通知你， 这个链接将一直持续到你释放他的一段时间后
		}
	}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//

////////////////////////////////////////////////////////////////////////////////////////////////////
//接受消息
	virtual void OnRecvMsg(iTCPHandlePtr& pTCPHandle, const std::string& msg)
	{

		cerr << "recv = " << msg << endl;
		std::string uid = "";
		if(Sock2ID.find(pTCPHandle) != Sock2ID.end()) {
			uid = Sock2ID[pTCPHandle];
		}
		//单方面存在的直接删除
		if(uid != "" && ID2Sock.find(uid) == ID2Sock.end()) {
			cerr << "单方面存在" << std::endl;
			std::map<iTCPHandlePtr, std::string>::iterator iit = Sock2ID.find(pTCPHandle);
			Sock2ID.erase(iit);
			uid = "";
		}
		//单方面存在的直接删除
		if(uid != "" && ID2Sock.find(uid) != ID2Sock.end()) {
			if(pTCPHandle != ID2Sock[uid]) {
				std::map<iTCPHandlePtr, std::string>::iterator iit = Sock2ID.find(pTCPHandle);
				Sock2ID.erase(iit);
				cerr << "不对等存在" << std::endl;
				uid = "";
			}
		}

		std::vector<Resp> resp;
		Game::doGame(uid, msg, resp);

		if(uid != "") {
			if(Sock2ID.find(pTCPHandle) == Sock2ID.end()) {
				Sock2ID[pTCPHandle] = uid;
			}
			//if(ID2Sock.find(uid) == ID2Sock.end()) {
				ID2Sock[uid] = pTCPHandle;
			//}

			for(int i = 0; i < resp.size(); i ++) {
				std::string &id = resp[i].m_uid;
				std::string res = resp[i].m_resp+"#";
				cerr << "to id:" << id << "   resp:" << res << endl;
				if(ID2Sock.find(id) != ID2Sock.end()) {
					iTCPHandlePtr &sender = ID2Sock[id];
					sender->SendMsg(res.c_str(), res.size());
				}
				else {
					cerr << id << "  error id" << endl;
				}
			}
		}
		else {
			for(int i = 0; i < resp.size(); i ++) {
				std::string &id = resp[i].m_uid;
				std::string res = resp[i].m_resp+"#";
				cerr << "to id:" << id << "   resp:" << res << endl;
				pTCPHandle->SendMsg(res.c_str(), res.size());
			}
		}
		/*std::string stringWho = pTCPHandle->GetPeerAddress().strLoginIP;
		
		LOG_DEBUG("recv = %s", msg.c_str());

		for (std::set<iTCPHandlePtr>::iterator it = m_handles.begin(); it != m_handles.end(); ++it)
		{
			
			//std::string temp = stringWho + " say: " + msg.c_str();
			//遍历所有链接
			(*it)->SendMsg(msg.c_str(), msg.size()); //将消息发送给所有客户端
		}*/

	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//连接、套接字、事件、服务等错误处理
	virtual void OnError(iTCPHandlePtr& pTCPHandle, const EnumError& err_code, const char* pszError)
	{
		std::cout << "错误" << std::endl;

		std::string uid;
		if(Sock2ID.find(pTCPHandle) != Sock2ID.end()) {
			uid = Sock2ID[pTCPHandle];
		}
		//单方面存在的直接删除
		if(uid != "" && ID2Sock.find(uid) == ID2Sock.end()) {
			cerr << "单方面存在" << std::endl;
			std::map<iTCPHandlePtr, std::string>::iterator iit = Sock2ID.find(pTCPHandle);
			Sock2ID.erase(iit);
			uid = "";
		}
		//单方面存在的直接删除
		if(uid != "" && ID2Sock.find(uid) != ID2Sock.end()) {
			if(pTCPHandle != ID2Sock[uid]) {
				cerr << "不对等存在" << std::endl;
				std::map<iTCPHandlePtr, std::string>::iterator iit = Sock2ID.find(pTCPHandle);
				Sock2ID.erase(iit);
				uid = "";
			}
		}
		if(uid != "") {
			std::vector<Resp> resp;
			Game::doGame(uid, "logout:", resp);
			if(uid != "") {
				if(Sock2ID.find(pTCPHandle) == Sock2ID.end()) {
					Sock2ID[pTCPHandle] = uid;
				}
				if(ID2Sock.find(uid) == ID2Sock.end()) {
					ID2Sock[uid] = pTCPHandle;
				}

				for(int i = 0; i < resp.size(); i ++) {
					std::string &id = resp[i].m_uid;
					std::string res = resp[i].m_resp+"#";
					cerr << "to id:" << id << "   resp:" << res << endl;
					if(ID2Sock.find(id) != ID2Sock.end()) {
						iTCPHandlePtr &sender = ID2Sock[id];
						sender->SendMsg(res.c_str(), res.size());
					}
					else {
						cerr << id << "  error id" << endl;
					}
				}
			}
			else {
				for(int i = 0; i < resp.size(); i ++) {
					std::string &id = resp[i].m_uid;
					std::string res = resp[i].m_resp+"#";
					cerr << "to id:" << id << "   resp:" << res << endl;
					pTCPHandle->SendMsg(res.c_str(), res.size());
				}
			}

			std::tr1::unordered_map<std::string, iTCPHandlePtr>::iterator sit = ID2Sock.find(uid);
			std::map<iTCPHandlePtr, std::string>::iterator iit = Sock2ID.find(pTCPHandle);
			ID2Sock.erase(sit);
			Sock2ID.erase(iit);
		}
		m_handles.erase(pTCPHandle); //在大多数的套接字错误情况下，你都应该释放这个连接
	}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//信号处
	virtual void OnSignal(iEventPtr& pEventHandle, void* params)
	{
		cout << "recv Signal" << pEventHandle->GetEventId() << ", params = " << (char*)params << endl;
		
		if (std::string((const char*)params) ==  "hello signal!")
		{
			static char buf[] = "hello two signal!";
			struct timeval t;
			pEventHandle->SetEvent(20,t, (void*)buf);  //重新设定信号的参数二，你不能改变这个信号编码（即第一个参数）
		}
		else
		{
			m_pServer->FreeSignal(20); //手动释放信号， 你无法释放框架默认接收的四个信号
		}
	}

	virtual void OnTimer(iEventPtr& pEventHandle, void* params)
	{
		//cout << "定时器到: " << pEventHandle->GetEventId() << ", params = " << (char*)params << endl;
		if(pEventHandle->GetEventId() == 100) {
			//cerr <<  "in Timer" << endl;
			PlayerQueue *playerQueue = PlayerQueue::getPlayerQueue();
			playerQueue->addTime();
			playerQueue->show();
			std::vector<std::string> gjUid;
			playerQueue->getGuaJI(gjUid);
			for(int i = 0; i < gjUid.size(); i ++) {
				std::vector<Resp> resp;
				Game::doGame(gjUid[i], "game:guaji", resp);
				LOG_DEBUG("GUAJI %s", gjUid[i].c_str());
				for(int i = 0; i < resp.size(); i ++) {
					std::string &id = resp[i].m_uid;
					std::string res = resp[i].m_resp+"#";
					cerr << "to id:" << id << "   resp:" << res << endl;
					if(ID2Sock.find(id) != ID2Sock.end()) {
						iTCPHandlePtr &sender = ID2Sock[id];
						sender->SendMsg(res.c_str(), res.size());
					}
					else {
						cerr << id << "  error id" << endl;
					}
				}
			}
		}
		if(pEventHandle->GetEventId() == 101) {
			PlayerQueue *playerQueue = PlayerQueue::getPlayerQueue();
			playerQueue->ClearTable();
		}
	}
	std::set<iTCPHandlePtr> m_handles;
};


int main(int argc, char* argv[])
{
	MyConfig::init("conf/server.cfg");
	GameConfig::Init();
	cerr << GameConfig::db_addr << " - " << GameConfig::db_name << endl;
	CMyFrame f;
	f.main();
//	LOG_DEBUG("测试日志文件");
	return 0;
	
}



