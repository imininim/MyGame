#ifndef __TABLE_H__
#define __TABLE_H__

#include <string>
#include <vector>
#include <tr1/unordered_map>
#include <service/ServerFrame.h>

#include "Resp.h"
#include "Player.h"
#include "define.h"
#include "TablePlayer.h"
#include "CardSender.h"
#include "MJAlgorithm.h"
#include "CalScore.h"

//一桌游戏
class Table {
public:
	int m_id;			//牌桌id

	int m_tableType;			//属于什么游戏
	std::vector<TablePlayer> m_tablePlayer;	//牌桌上的玩家
	int m_roomMan;		//房主

	TablePlayer m_tablePlayers[4];	//玩家
	int m_status[4];	//位置状态
	int m_retCard;		//扎马数
	int m_planPlay;		//计划局数
	int m_remain;		//剩余游戏局数
	int m_isPay;		//是否付费
	int m_tableStatus;	//牌桌状态 0.等人 1.游戏中
	int m_zhuang;		//庄家
	int m_people;		//人数

	int m_online[4];	//在线玩家

	int m_laoyue;		//是不是在捞月
	
	int m_pStatus[4];	//游玩状态
	int m_pPeople;		//准备人数

	int m_doingPlayer;	//正在执行操作的玩家
	int m_prePlayer;		//上一个打牌玩家
	int m_level;			//当前优先级 10.天胡  9.胡牌  8.杠 7.碰  6.吃
	int m_end;			//结算
	int m_endStatus;	//结算状态  0.臭庄 1.胡牌
	int m_ScoreArray[4];	//得分

	Pai m_prePai;			//上一张打出的牌
	CardSender *m_cardSender;	//发牌器
	GamePlayWay m_playGame;	//游戏玩法
	CalScore m_calScore;	//得分计算方法
	PlayScore m_playScore;
public:
	Table(const Player *player, const int type, const int &id, const int &planPlay, const int &retCard);
	~Table();
public:
	//玩家uid进行了 type操作 出了 card牌
	int inRoomPai(const std::string &uid, std::vector<Resp> &resp);
	int Again(const std::string &uid);
	int PlayerOperator(const std::string &uid, const int &type, const int &card, const int &card2, const int &card3, std::vector<Resp> &resp);
	int BeginGame(const std::string &uid, std::vector<Resp> &resp);
	int GetPlayersID(std::string &players);
	int GetPlayersAccount(std::vector<std::string> &playersID);
	int InRoom(const Player *player);
	int OutRoom(const Player *player);
	int toClient(std::tr1::unordered_map<std::string, std::string> &playersOp);

	int TianHu(const std::string &uid, std::vector<Resp> &resp);
	int TianHuMoPai(const std::string &uid, std::vector<Resp> &resp);
	int JieSuan(std::vector<Resp> &resp);		//ok
	int MoPai(const std::string &uid, const int &card, const int &card2, const int &card3, std::vector<Resp> &resp);	//ok
	int Laoyue(const std::string &uid, const int &card, const int &card2, const int &card3, std::vector<Resp> &resp);	//ok
	int Gang(const std::string &uid, const int &card, const int &card2, const int &card3, std::vector<Resp> &resp);
	int Peng(const std::string &uid, const int &card, const int &card2, const int &card3, std::vector<Resp> &resp);
	int Hu(const std::string &uid, const int &card, const int &card2, const int &card3, std::vector<Resp> &resp);
	int Chi(const std::string &uid, const int &card, const int &card2, const int &card3, std::vector<Resp> &resp);
	int Guaji(const std::string &uid, const int &card, const int &card2, const int &card3, std::vector<Resp> &resp);
	int ChuPai(const std::string &uid, const int &card, const int &card2, const int &card3, std::vector<Resp> &resp);
	int KaiGang(const std::string &uid, const int &card, const int &card2, const int &card3, std::vector<Resp> &resp);
	int Guo(const std::string &uid, const int &card, const int &card2, const int &card3, std::vector<Resp> &resp);

	int toResp(const int &id, std::string &uid, std::string &resp);
	int toRespAgain(const int &id, std::string &uid, std::string &resp);
	int GetAllCards(const int &id, std::string &uid, std::string &resp);
};

#endif
