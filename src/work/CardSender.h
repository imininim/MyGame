#ifndef __CARDSENDER_H__
#define __CARDSENDER_H__

#include "define.h"
#include "common/CommonFuc.h"

class CardSender {
public:
	virtual ~CardSender() {}
	virtual Pai getCard()=0;
	virtual int init()=0;
	virtual bool isLastOne()=0;
	virtual bool isNull()=0;
	virtual int getZhaMa()=0;
};

//发牌器
class CardSenderA:public CardSender {
public:
	int m_p;	//当前在什么位置
	int m_retCard;	//扎马数
	Pai m_card[30];	//麻将
public:
	CardSenderA(int retCard) {
		m_retCard = retCard;
		//m_card = new Pai[30];
	}
	~CardSenderA() {
		//delete[] m_card;
		//m_card = NULL;
	}
	virtual int getZhaMa() {
		return m_retCard;
	}
	virtual int init() {
		for(int i = 0; i < 30; i ++) {
			m_card[i] = 4;
		}
		m_p = 108;
		m_p -= m_retCard;
	}
	virtual Pai getCard() {
		if(m_p == 0) {
			return -1;
		}
		int pai = COMMON::random(27);
		while(m_card[pai] == 0) {
			pai ++;
			pai %= 27;
		}
		std::cerr << "pai size = " << m_card[pai] << std::endl;
		m_card[pai] --;
		m_p --;
		return pai;
	}

	virtual bool isNull() {
		return (m_p==0);
	}
	virtual bool isLastOne() {
		if(m_p == 1) {
			return true;
		}
		return false;
	}
};

#endif
