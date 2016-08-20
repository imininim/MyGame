#ifndef __TABLEPLAYER_H__
#define __TABLEPLAYER_H__

#include "Player.h"
#include "define.h"

class TablePlayer {
public:
	const Player *m_player;
	PlayerCards m_playerCards;
	int m_Operator;
	Pai m_pai;
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
