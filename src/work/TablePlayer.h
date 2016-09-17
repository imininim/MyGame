#ifndef __TABLEPLAYER_H__
#define __TABLEPLAYER_H__

#include "Player.h"
#include "define.h"

class TablePlayer {
public:
	const Player *m_player;		//玩家
	PlayerCards m_playerCards;	//玩家手牌
	int m_status;				//玩家状态 0.正常  1.挂机  2.胡牌
	int m_huStats;				//胡牌状态 0.放炮  1.自摸
	int m_dahu;					//大胡状态 

	std::vector<Pai> m_zhama;
	Pai m_pai;					//上一张摸的牌
public:
	TablePlayer(const Player *player) {
		m_player = player;
	}
	TablePlayer() {
		m_player = NULL;
	}
	int init();
	int toHu() {
		m_playerCards.toHu();
		return 0;
	}
	int GetRandomCard() {
		for(int i = 0; i < 27; i ++) {
			if(m_playerCards.m_playCards[i] > 0) {
				return i;
			}
		}
		return -1;
	}
	int GetCard(std::string &uid, std::string &resp);
	//出牌
	int outCard(Pai idx);
	//摸牌
	int addCard(Pai idx);
	//碰
	int pengCard(Pai idx);
	//杠
	int gangCard(Pai idx);
	//暗杠
	int anGangCard(Pai idx);
	//吃
	int chiCard(Pai idx, Pai card1, Pai card2);
	//胡
	int huCard();
	int guaJi();
	//显示给客户端
	int showLong(std::string &resp);
	int showShort(std::string &resp);
};

#endif
