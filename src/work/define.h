#ifndef __DEFINE_H__
#define __DEFINE_H__

#include <algorithm>
#include <string>
#include <vector>
#include <string.h>
#include <common/CommonFuc.h>

typedef int Pai;
typedef int PlayStatus;

#define GUO 64
#define DAPAI 32
#define HU 16
#define GANG 8
#define PENG 4
#define CHI 2
#define TIANHU 1
#define GUAJI 128
#define MOPAI 256
#define CHUPAI 512
#define ANGANG 1024
#define TIANHUMOPAI 2048
#define LAOYUE 4096
#define KAIGANG 8192

class PlayScore {
public:
	int m_Score[4];
	int m_ZhaMa[4];
	int m_Dahu[4];
	int m_Xiaohu[4];
public:
	int toResp(std::string &str) {
		str = "game:over";
		for(int i = 0; i < 4; i ++) {
			str += "|"+COMMON::convert<int, std::string>(m_Score[i]);
		}
		for(int i = 0; i < 4; i ++) {
			str += "|"+COMMON::convert<int, std::string>(m_Dahu[i]);
		}
		for(int i = 0; i < 4; i ++) {
			str += "|"+COMMON::convert<int, std::string>(m_Xiaohu[i]);
		}
		for(int i = 0; i < 4; i ++) {
			str += "|"+COMMON::convert<int, std::string>(m_ZhaMa[i]);
		}
		return 0;
	}
	int init() {
		for(int i = 0; i < 4; i ++) {
			m_Score[i] = 0;
			m_Dahu[i] = 0;
			m_Xiaohu[i] = 0;
			m_ZhaMa[i] = 0;
		}
		return 0;
	}
	int add(int idx, int score, int dahu, int xiaohu, int zhama) {
		if(idx >= 4 || idx < 0) {
			return -1;
		}
		m_Score[idx] += score;
		m_Dahu[idx] += dahu;
		m_Xiaohu[idx] += xiaohu;
		m_ZhaMa[idx] += zhama;
		std::cerr << "dahu" << dahu << "  xiaohu = " << xiaohu << " zhama = " << zhama << std::endl;
		return 0;
	}
};


//玩家手牌
class PlayerCards {
public:
	static Pai TransCard(const Pai &card);
	static Pai reTransCard(const Pai &card);
public:
	std::vector<std::vector<Pai> > m_playCardsPeng;	//碰的牌
	std::vector<std::vector<Pai> > m_playCardsGang;	//杠的牌
	std::vector<std::vector<Pai> > m_playCardsAnGang;	//暗杠的牌
	std::vector<std::vector<Pai> > m_playCardsChi;	//吃得牌
	Pai m_playCards[27];	//手牌
	Pai m_prePai;
	int m_size;				//手牌数目
public:
	PlayerCards() {
		memset(m_playCards, 0, sizeof(m_playCards));
		m_size = 0;
		m_prePai = -1;
	}
	int init() {
		m_playCardsPeng.clear();
		m_playCardsGang.clear();
		m_playCardsAnGang.clear();
		m_playCardsChi.clear();
		memset(m_playCards, 0, sizeof(m_playCards));
		m_size = 0;
		m_prePai = -1;
	}
	//直接胡牌
	int toHu() {
		//测试代码
		{
			memset(m_playCards, 0,sizeof(m_playCards));
			for(int xx = 0; xx < 4; xx ++) {
				m_playCards[xx] += 3;
			}
			m_playCards[7] = 1;
			//memset(m_playCards, 0,sizeof(m_playCards));
			//for(int xx = 0; xx < 5; xx ++) {
			//	m_playCards[xx] += 2;
			//}
			//m_playCards[9] = 4;
		}
		return 0;
	}
	int getCard(std::string &resp);
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
	//胡
	int huCard();
	//吃
	int chiCard(Pai idx, Pai card1, Pai card2);
	//显示私有部分
	int showPublic(std::string &resp);
	//显示公有部分
	int showPrivate(std::string &resp);
};

#endif
