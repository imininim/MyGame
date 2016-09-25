#include "Player.h"
#include "define.h"
#include "Table.h"
#include "TimeQueue.h"

int Table::toRespAgain(const int &id, std::string &uid ,std::string &resp) {
	std::cerr << "Again toResp" << std::endl;
	if(m_pStatus[id] == 0) {
		std::cerr << "No palyer" << std::endl;
		return -1;
	}
	uid = m_tablePlayers[id].m_player->m_account;
	resp = "waitchange:";
	resp += COMMON::convert<int, std::string>(m_planPlay)+"|"+
			COMMON::convert<int, std::string>(m_remain)+"|"+
			COMMON::convert<int, std::string>(m_pPeople)+"|"+
			COMMON::convert<int, std::string>(id+1)+"|"+
			COMMON::convert<int, std::string>(m_roomMan+1)+"|"+
			COMMON::convert<int, std::string>(m_zhuang+1);
	for(int i = 0; i < 4; i ++) {
		resp += "|";
		if(m_pStatus[i]) {
			resp += COMMON::convert<int,std::string>(m_tablePlayers[i].m_player->m_id);
		}
	}
	for(int i = 0; i < 4; i ++) {
		resp += "|";
		if(m_pStatus[i]) {
			resp += m_tablePlayers[i].m_player->m_name;
		}
	}
	for(int i = 0; i < 4; i ++) {
		resp += "|";
		if(m_pStatus[i]) {
			resp += m_tablePlayers[i].m_player->m_sex;
		}
	}
	for(int i = 0; i < 4; i ++) {
		resp += "|";
		if(m_pStatus[i]) {
			resp += m_tablePlayers[i].m_player->m_url;
		}
	}
	resp += "|"+COMMON::convert<int,std::string>(m_cardSender->getZhaMa());
	std::cerr << "Again toResp" << std::endl;
	return 0;
}

int Table::toResp(const int &id, std::string &uid, std::string &resp) {
	if(m_status[id] == 0) {
		return -1;
	}
	uid = m_tablePlayers[id].m_player->m_account;
	resp = "waitchange:";
	resp += COMMON::convert<int, std::string>(m_planPlay)+"|"+
			COMMON::convert<int, std::string>(m_remain)+"|"+
			COMMON::convert<int, std::string>(m_people)+"|"+
			COMMON::convert<int, std::string>(id+1)+"|"+
			COMMON::convert<int, std::string>(m_roomMan+1)+"|"+
			COMMON::convert<int, std::string>(m_zhuang+1);
	for(int i = 0; i < 4; i ++) {
		resp += "|";
		if(m_status[i]) {
			resp += m_tablePlayers[i].m_player->m_account;
		}
	}
	for(int i = 0; i < 4; i ++) {
		resp += "|";
		if(m_status[i]) {
			resp += m_tablePlayers[i].m_player->m_name;
		}
	}
	for(int i = 0; i < 4; i ++) {
		resp += "|";
		if(m_status[i]) {
			resp += m_tablePlayers[i].m_player->m_sex;
		}
	}
	for(int i = 0; i < 4; i ++) {
		resp += "|";
		if(m_status[i]) {
			resp += m_tablePlayers[i].m_player->m_url;
		}
	}
	resp += "|"+COMMON::convert<int,std::string>(m_cardSender->getZhaMa());
	return 0;
}

int Table::JieSuan(std::vector<Resp> &resp) {
	//如果不是结算，直接结束
	if(!m_end) {
		return 0;
	}
	//设置这句游戏结束
	m_tableStatus = 0;
	//给出全部人的牌
	for(int i = 0; i < 4; i ++) {
		Resp res;
		res.m_uid = m_tablePlayers[i].m_player->m_account;
		for(int j = 0; j < 4; j ++) {
			std::string Cards;
			std::string Uid;
			m_tablePlayers[j].GetCard(Uid, Cards);
			res.m_resp = "game:allpai|"+COMMON::convert<int,std::string>(j+1)+"|"+Cards;
			resp.push_back(res);
		}
	}

	if(m_endStatus == 0) {
		//臭庄
		Resp res;
		std::string strScore;
		for(int i = 0; i < 4; i ++) {
			strScore += "|"+COMMON::convert<int,std::string>(m_ScoreArray[i]);
		}
		for(int i = 0; i < 4; i ++) {
			res.m_uid = m_tablePlayers[i].m_player->m_account;
			res.m_resp = "game:couz"+strScore;
			resp.push_back(res);
		}
	}
	else if(m_endStatus == 1) {
		//有人胡牌
		int ZhaMa = m_cardSender->getZhaMa();
		std::string strCard = "";
		std::vector<Pai> vZhaMa;
		for(int i = 0; i < ZhaMa; i ++) {
			Pai newCard = m_cardSender->getCard();
			//记录扎马
			vZhaMa.push_back(newCard);
			newCard = PlayerCards::TransCard(newCard);
			strCard += "|"+COMMON::convert<int,std::string>(newCard);
		}

		int HuNum = 0;
		int NewZhuang = m_zhuang;
		std::cerr << "zhuang = " << m_zhuang << std::endl;
		for(int i = 0; i < 4; i ++) {
			//如果某个玩家胡牌
			if(m_tablePlayers[i].m_status == 2) {
				if(m_cardSender->getRemain() == 0) {
					m_tablePlayers[i].m_dahu ++;
				}
				if(m_tablePlayers[i].m_kaiGang) {
					m_tablePlayers[i].m_dahu ++;
				}
				HuNum ++;
				NewZhuang = i;
				int type = 1;
				if(m_tablePlayers[i].m_huStats) {
					type = 0;
				}
				//int type = (m_tablePlayers[i].m_playerCards.m_size%3 == 1);
				Resp res;
				res.m_uid = m_tablePlayers[i].m_player->m_account;
				res.m_resp = "game:hu|"+COMMON::convert<int, std::string>(i+1)+"|";
				m_calScore.init();
				if(type == 0) {
					//告诉其他人胡什么
					res.m_resp += COMMON::convert<int,std::string>(m_tablePlayers[i].m_pai);

					//根据扎马计算得分
					for(int j = 0; j < vZhaMa.size(); j ++) {
						m_calScore.addZhaMa(vZhaMa[j], i, m_zhuang);
					}
					int GetScore = 0;
					for(int j = 0; j < 4; j ++) {
						if(j != i) {
							m_ScoreArray[j] += m_calScore.GetScore(type, i, j, m_zhuang, m_tablePlayers[i].m_dahu);
							GetScore += m_ScoreArray[j];
						}
					}
					m_ScoreArray[i] = -GetScore;
				}
				else if(type == 1) {
					//告诉其他人胡什么
					res.m_resp += COMMON::convert<int,std::string>(m_prePai);
					//根据扎马计算得分
					for(int j = 0; j < vZhaMa.size(); j ++) {
						m_calScore.addZhaMa(vZhaMa[j], i, m_zhuang);
						m_calScore.addZhaMa(vZhaMa[j], m_prePlayer, m_zhuang);
					}
					int GetScore = m_calScore.GetScore(type, i, m_prePlayer, m_zhuang, m_tablePlayers[i].m_dahu);
					m_ScoreArray[m_prePlayer] += GetScore;
					m_ScoreArray[i] = -GetScore;
				}
				//告诉每个人胡什么
				for(int j = 0; j < 4; j ++) {
					res.m_uid = m_tablePlayers[j].m_player->m_account;
					resp.push_back(res);
				}
			}
		}

		if(HuNum > 1) {
			NewZhuang = m_prePlayer;
		}
		m_zhuang = NewZhuang;
		
		for(int i = 0; i < 4; i ++) {
			Resp res;
			res.m_uid = m_tablePlayers[i].m_player->m_account;
			res.m_resp = "game:zama"+strCard;
			resp.push_back(res);
		}

		//记录全部人的大胡次数，得分
		for(int j = 0; j < 4; j ++) {
			if(m_tablePlayers[j].m_status) {
				int xiaohu = 1;
				int dahu = 0;
				if(m_tablePlayers[j].m_dahu) {
					xiaohu = 0;
					dahu = 1;
				}
				m_playScore.add(j, m_ScoreArray[j], dahu, xiaohu, m_calScore.getZhaMa());
			}
			else {
				m_playScore.add(j, m_ScoreArray[j], 0, 0, 0);
			}
		}

		//分数变化
		for(int j = 0; j < 4; j ++) {
			Player *tplayer = (Player *)m_tablePlayers[j].m_player;
			tplayer->changeScore(m_ScoreArray[j]);
		}

		//计算
		std::string strScore;
		for(int i = 0; i < 4; i ++) {
			strScore += "|"+COMMON::convert<int,std::string>(m_ScoreArray[i]);
		}
		for(int i = 0; i < 4; i ++) {
			Resp res;
			res.m_uid = m_tablePlayers[i].m_player->m_account;
			res.m_resp = "game:end"+strScore;
			resp.push_back(res);
		}
	}

	//把所有人清除队列
	PlayerQueue *playerQueue = PlayerQueue::getPlayerQueue();
	for(int i = 0; i < 4; i ++) {
		playerQueue->sub(m_tablePlayers[m_doingPlayer].m_player);
	}

	m_tableStatus = 0;
	//所有人都进入等待房间
	for(int i = 0; i < 4; i ++) {
		m_pStatus[i] = 0;
	}
	m_pPeople = 0;
	if(m_remain == 0) {
		std::string str;
		m_playScore.toResp(str);
		for(int i = 0; i < 4; i ++) {
			Resp res;
			res.m_uid = m_tablePlayers[i].m_player->m_account;
			res.m_resp = str;
			resp.push_back(res);
		}
	}
	return 0;
}

int Table::Laoyue(const std::string &uid, const int &card, const int &card2, const int &card3, std::vector<Resp> &resp) {
	m_tablePlayers[m_doingPlayer].m_kaiGang = 1;
	m_tablePlayers[m_doingPlayer].m_status = 1;
	Resp res;
	res.m_uid = m_tablePlayers[m_doingPlayer].m_player->m_account;
	res.m_resp = "game:tuoguan";
	resp.push_back(res);
	int newCard = m_cardSender->getCard();
	if(newCard < 0) {
		m_end = 1;
		m_endStatus = 0;
		return 0;
	}
	//起手胡牌
	//{
	//	m_tablePlayers[m_doingPlayer].toHu();
	//	m_prePai = 9;
	//	newCard = 7;
	//}
	newCard = PlayerCards::TransCard(newCard);
	LOG_DEBUG("MOPAI Card %d", newCard);
	m_tablePlayers[m_doingPlayer].addCard(newCard);

	PlayerQueue *playerQueue = PlayerQueue::getPlayerQueue();
	playerQueue->add(m_tablePlayers[m_doingPlayer].m_player);

	for(int i = 0; i < 4; i ++) {
		Resp res;
		res.m_uid = m_tablePlayers[i].m_player->m_account;
		res.m_resp = "game:remainpai|"+COMMON::convert<int,std::string>(m_cardSender->getRemain());
		resp.push_back(res);
	}

	for(int i = 0; i < 4; i ++) {
		Resp res;
		if(i == m_doingPlayer) {
			res.m_uid = m_tablePlayers[i].m_player->m_account;
			res.m_resp = "game:mopai|"+COMMON::convert<int, std::string>(newCard);
			resp.push_back(res);
		}
		else {
			res.m_uid = m_tablePlayers[i].m_player->m_account;
			res.m_resp = "game:mopai|mopai|"+COMMON::convert<int, std::string>(m_doingPlayer+1);
			resp.push_back(res);
		}
	}

	for(int i = 0; i < 4; i ++) {
		Resp res;
		res.m_uid = m_tablePlayers[i].m_player->m_account;
		for(int j = 0; j < 4; j ++) {
			if(i == j) {
				std::string Cards;
				std::string Uid;
				m_tablePlayers[i].GetCard(Uid, Cards);
				res.m_resp = "game:pai|"+COMMON::convert<int,std::string>(j+1)+"|"+
						COMMON::convert<int,std::string>(m_tablePlayers[j].m_playerCards.m_size)+"|"+
						Cards;
			}
			else {
				res.m_resp = "game:pai|"+COMMON::convert<int,std::string>(j+1)+"|"+
						COMMON::convert<int,std::string>(m_tablePlayers[j].m_playerCards.m_size);
			}
			resp.push_back(res);
		}
	}
	
	int type = (m_tablePlayers[m_doingPlayer].m_playerCards.m_size%3 == 1);
	//判断是否胡牌
	int Operator = 0;
	int dahu = 0;
	m_playGame.Operator(m_tablePlayers[m_doingPlayer].m_playerCards, Operator, type, newCard, dahu);
	res.m_uid = m_tablePlayers[m_doingPlayer].m_player->m_account;
	res.m_resp = "game:choose";
	if(Operator) {
		if(Operator&HU) {
			res.m_resp += "|hu";
		}
		//if(m_tablePlayers[m_doingPlayer].m_status == 0) {
			//if(Operator&GANG) {
			//	res.m_resp += "|gang";
			//}
			//if(Operator&ANGANG) {
			//	res.m_resp += "|gang";
			//}
			//if(Operator&TIANHU) {
			//	res.m_resp += "|tianhu|fanpai";
			//}
		//}
		if(Operator&CHUPAI) {
			res.m_resp += "|dapai";
			//m_cardSender->getCard();
		}
	}
	resp.push_back(res);
	return 0;
}

int Table::MoPai(const std::string &uid, const int &card, const int &card2, const int &card3, std::vector<Resp> &resp) {
	LOG_DEBUG("in MoPai");
	if(m_cardSender->getRemain() == 1) {
		int next = m_doingPlayer;
		
		{
			int Operator = 0;
			m_playGame.isTingPai(m_tablePlayers[m_doingPlayer].m_playerCards, Operator, 1, -1);
			if(Operator & HU) {
				Resp res;
				res.m_uid = m_tablePlayers[m_doingPlayer].m_player->m_account;
				res.m_resp = "game:choose|laoyue|guo";
				m_laoyue = 1;
				resp.push_back(res);
				m_prePlayer = m_doingPlayer;
				return 0;
			}
		}

		next = (m_doingPlayer+1)%4;
		while(next != m_doingPlayer) {
			int Operator = 0;
			m_playGame.isTingPai(m_tablePlayers[next].m_playerCards, Operator, 1, -1);
			if(Operator & HU) {
				Resp res;
				res.m_uid = m_tablePlayers[next].m_player->m_account;
				res.m_resp = "game:choose|laoyue|guo";
				m_laoyue = 1;
				resp.push_back(res);
				m_prePlayer = m_doingPlayer;
				m_doingPlayer = next;
				return 0;
			}
			next = (next+1)%4;
		}
	}
	int newCard = m_cardSender->getCard();
	if(newCard < 0) {
		m_end = 1;
		m_endStatus = 0;
		return 0;
	}
	//起手胡牌
	//{
	//	m_tablePlayers[m_doingPlayer].toHu();
	//	m_prePai = 9;
	//	newCard = 7;
	//}
	newCard = PlayerCards::TransCard(newCard);
	LOG_DEBUG("MOPAI Card %d", newCard);
	m_tablePlayers[m_doingPlayer].addCard(newCard);

	PlayerQueue *playerQueue = PlayerQueue::getPlayerQueue();
	playerQueue->add(m_tablePlayers[m_doingPlayer].m_player);

	for(int i = 0; i < 4; i ++) {
		Resp res;
		res.m_uid = m_tablePlayers[i].m_player->m_account;
		res.m_resp = "game:remainpai|"+COMMON::convert<int,std::string>(m_cardSender->getRemain());
		resp.push_back(res);
	}

	for(int i = 0; i < 4; i ++) {
		Resp res;
		if(i == m_doingPlayer) {
			res.m_uid = m_tablePlayers[i].m_player->m_account;
			res.m_resp = "game:mopai|"+COMMON::convert<int, std::string>(newCard);
			resp.push_back(res);
		}
		else {
			res.m_uid = m_tablePlayers[i].m_player->m_account;
			res.m_resp = "game:mopai|mopai|"+COMMON::convert<int, std::string>(m_doingPlayer+1);
			resp.push_back(res);
		}
	}

	for(int i = 0; i < 4; i ++) {
		Resp res;
		res.m_uid = m_tablePlayers[i].m_player->m_account;
		for(int j = 0; j < 4; j ++) {
			if(i == j) {
				std::string Cards;
				std::string Uid;
				m_tablePlayers[i].GetCard(Uid, Cards);
				res.m_resp = "game:pai|"+COMMON::convert<int,std::string>(j+1)+"|"+
						COMMON::convert<int,std::string>(m_tablePlayers[j].m_playerCards.m_size)+"|"+
						Cards;
			}
			else {
				res.m_resp = "game:pai|"+COMMON::convert<int,std::string>(j+1)+"|"+
						COMMON::convert<int,std::string>(m_tablePlayers[j].m_playerCards.m_size);
			}
			resp.push_back(res);
		}
	}
	
	int type = (m_tablePlayers[m_doingPlayer].m_playerCards.m_size%3 == 1);
	//判断是否胡牌
	int Operator = 0;
	int dahu = 0;
	m_playGame.Operator(m_tablePlayers[m_doingPlayer].m_playerCards, Operator, type, newCard, dahu);
	Resp res;
	res.m_uid = m_tablePlayers[m_doingPlayer].m_player->m_account;
	res.m_resp = "game:choose";
	if(Operator) {
		if(Operator&HU) {
			res.m_resp += "|hu";
		}
		if(m_tablePlayers[m_doingPlayer].m_status == 0) {
			if(Operator&GANG) {
				res.m_resp += "|gang";
			}
			if(Operator&ANGANG) {
				res.m_resp += "|gang";
			}
			//if(Operator&TIANHU) {
			//	res.m_resp += "|tianhu|fanpai";
			//}
		}
		if(Operator&CHUPAI) {
			res.m_resp += "|dapai";
		}
	}
	resp.push_back(res);
	return 0;
}

int Table::ChuPai(const std::string &uid, const int &card, const int &card2, const int &card3, std::vector<Resp> &resp) {
	LOG_DEBUG("DAPAI Card %d", card);
	m_tablePlayers[m_doingPlayer].outCard(card);
	PlayerQueue *playerQueue = PlayerQueue::getPlayerQueue();
	playerQueue->sub(m_tablePlayers[m_doingPlayer].m_player);
	Resp res;
	for(int i = 0; i < 4; i ++) {
		res.m_uid = m_tablePlayers[i].m_player->m_account;;
		res.m_resp = "game:dapai|"+COMMON::convert<int, std::string>(m_doingPlayer+1)+"|"+
							COMMON::convert<int, std::string>(card);
		resp.push_back(res);
	}

	for(int i = 0; i < 4; i ++) {
		Resp res;
		res.m_uid = m_tablePlayers[i].m_player->m_account;
		for(int j = 0; j < 4; j ++) {
			if(i == j) {
				std::string Cards;
				std::string Uid;
				m_tablePlayers[i].GetCard(Uid, Cards);
				res.m_resp = "game:pai|"+COMMON::convert<int,std::string>(j+1)+"|"+
						COMMON::convert<int,std::string>(m_tablePlayers[j].m_playerCards.m_size)+"|"+
						Cards;
			}
			else {
				res.m_resp = "game:pai|"+COMMON::convert<int,std::string>(j+1)+"|"+
						COMMON::convert<int,std::string>(m_tablePlayers[j].m_playerCards.m_size);
			}
			resp.push_back(res);
		}
	}

	m_prePai = card;
	m_prePlayer = m_doingPlayer;
	m_doingPlayer = (m_doingPlayer+1)%4;

	int next = m_doingPlayer;
	int flag = 0;
	int Operator = 0;
	m_level = 0;
	int OptPlayer = -1;
	int pOperator = 0;
	while(next != m_prePlayer) {
		int type = (m_tablePlayers[next].m_playerCards.m_size%3 == 1);
		Operator = 0;
		int Operator = 0;
		int dahu = 0;
		m_playGame.Operator(m_tablePlayers[next].m_playerCards, Operator, type, m_prePai, dahu);
		if(next!=(m_prePlayer+1)%4 && Operator&CHI) {
			Operator -= CHI;
		}
		if(m_tablePlayers[next].m_status == 0) {
			if(Operator&(HU|CHI|GANG|PENG)) {
				if(Operator&HU && m_level<9) {
					m_level = 9;
					OptPlayer = next;
					pOperator = Operator;
				}
				if(Operator&GANG && m_level<8) {
					m_level = 8;
					OptPlayer = next;
					pOperator = Operator;
				}
				if(Operator&PENG && m_level<7) {
					m_level = 7;
					OptPlayer = next;
					pOperator = Operator;
				}
				if(Operator&CHI && m_level<6 && next==(m_prePlayer+1)%4) {
					m_level = 6;
					OptPlayer = next;
					pOperator = Operator;
				}
				flag = 1;
			}
		}
		else if(m_tablePlayers[next].m_status == 1) {
			if(Operator&(HU)) {
				if(Operator&HU && m_level<9) {
					m_level = 9;
					OptPlayer = next;
					pOperator = Operator;
				}
				flag = 1;
			}
		}
		next = (next+1)%4;
	}

	if(flag) {
		m_doingPlayer = OptPlayer;
		PlayerQueue *playerQueue = PlayerQueue::getPlayerQueue();
		playerQueue->add(m_tablePlayers[m_doingPlayer].m_player);
		res.m_uid = m_tablePlayers[m_doingPlayer].m_player->m_account;
		res.m_resp = "game:choose";
		int flag = 0;
		if(pOperator&HU) {
			res.m_resp += "|hu";
			flag ++;
		}
		if(m_tablePlayers[next].m_status == 0) {
			if(pOperator&CHI) {
				res.m_resp += "|chi";
				flag ++;
			}
			if(pOperator&GANG) {
				res.m_resp += "|gang";
				flag ++;
			}
			if(Operator&ANGANG) {
				res.m_resp += "|gang";
			}
			if(pOperator&PENG) {
				res.m_resp += "|peng";
				flag ++;
			}
		}
		if(pOperator&GUO) {
			res.m_resp += "|guo";
			flag ++;
		}
		if(pOperator&CHUPAI) {
			res.m_resp += "|dapai";
			flag ++;
		}
		if(!(flag==1&&pOperator&GUO)) {
			resp.push_back(res);
			return 0;
		}
	}

	MoPai(m_tablePlayers[m_doingPlayer].m_player->m_account, 0, 0, 0, resp);

	/*int newCard = m_cardSender->getCard();
	if(newCard < 0) {
		m_end = 1;
		m_endStatus = 0;
		return 0;
	}

	for(int i = 0; i < 4; i ++) {
		Resp res;
		res.m_uid = m_tablePlayers[i].m_player->m_account;
		res.m_resp = "game:remainpai|"+COMMON::convert<int,std::string>(m_cardSender->getRemain());
		resp.push_back(res);
	}

	newCard = PlayerCards::TransCard(newCard);
	m_tablePlayers[m_doingPlayer].addCard(newCard);
	playerQueue->add(m_tablePlayers[m_doingPlayer].m_player);
	for(int i = 0; i < 4; i ++) {
		Resp res;
		if(i == m_doingPlayer) {
			res.m_uid = m_tablePlayers[i].m_player->m_account;
			res.m_resp = "game:mopai|"+COMMON::convert<int, std::string>(newCard);
			resp.push_back(res);
		}
		else {
			res.m_uid = m_tablePlayers[i].m_player->m_account;
			res.m_resp = "game:mopai|mopai|"+COMMON::convert<int, std::string>(m_doingPlayer+1);
			resp.push_back(res);
		}
	}

	for(int i = 0; i < 4; i ++) {
		Resp res;
		res.m_uid = m_tablePlayers[i].m_player->m_account;
		for(int j = 0; j < 4; j ++) {
			if(i == j) {
				std::string Cards;
				std::string Uid;
				m_tablePlayers[i].GetCard(Uid, Cards);
				res.m_resp = "game:pai|"+COMMON::convert<int,std::string>(j+1)+"|"+
						COMMON::convert<int,std::string>(m_tablePlayers[j].m_playerCards.m_size)+"|"+
						Cards;
			}
			else {
				res.m_resp = "game:pai|"+COMMON::convert<int,std::string>(j+1)+"|"+
						COMMON::convert<int,std::string>(m_tablePlayers[j].m_playerCards.m_size);
			}
			resp.push_back(res);
		}
	}

	//判断是否胡牌
	int type = (m_tablePlayers[m_doingPlayer].m_playerCards.m_size%3 == 1);
	Operator = 0;
	int dahu = 0;
	m_playGame.Operator(m_tablePlayers[m_doingPlayer].m_playerCards, Operator, type, -1, dahu);
	if(Operator) {
		res.m_uid = m_tablePlayers[m_doingPlayer].m_player->m_account;
		res.m_resp = "game:choose";
		if(Operator&HU) {
			res.m_resp += "|hu";
		}
		if(Operator&GANG) {
			res.m_resp += "|gang";
		}
		if(Operator&ANGANG) {
			res.m_resp += "|gang";
		}
		if(Operator&TIANHU) {
			res.m_resp += "|tianhu";
		}
		if(Operator&CHUPAI) {
			res.m_resp += "|dapai";
		}
		resp.push_back(res);
		return 0;
	}*/
}

int Table::Guo(const std::string &uid, const int &card, const int &card2, const int &card3, std::vector<Resp> &resp) {
	LOG_DEBUG("GUO");
	//捞月
	LOG_DEBUG("No player OPT");
	if(m_laoyue) {
		int next = (m_doingPlayer+1)%4;
		std::cerr << "doing = " << m_doingPlayer << "  next = " << next << std::endl;
		while(next != m_prePlayer) {
			int Operator = 0;
			m_playGame.isTingPai(m_tablePlayers[next].m_playerCards, Operator, 1, -1);
			if(Operator & HU) {
				Resp res;
				res.m_uid = m_tablePlayers[next].m_player->m_account;
				res.m_resp = "game:choose|laoyue|guo";
				m_laoyue = 1;
				resp.push_back(res);
				m_doingPlayer = next;
				return 0;
			}
			next = (next+1)%4;
		}
		m_end = 1;
		m_endStatus = 0;
		return 0;
	}

	int OptArray[4] = {0,0,0,0};
	int LevelArray[4] = {0,0,0,0};
	int next = (m_prePlayer+1)%4;
	while(next != m_prePlayer) {
		int type = (m_tablePlayers[next].m_playerCards.m_size%3 == 1);
		int Operator = 0;
		int dahu = 0;
		m_playGame.Operator(m_tablePlayers[next].m_playerCards, Operator, type, m_prePai, dahu);
		if(next!=(m_prePlayer+1)%4 && Operator&CHI) {
			Operator -= CHI; 
		}
		OptArray[next] = Operator;
		if(m_tablePlayers[next].m_status == 0) {
			if(Operator&(HU|CHI|GANG|PENG)) {
				int level = 0;
				if(Operator&CHI) {
					level = 6;
				}
				if(Operator&PENG) {
					level = 7;
				}
				if(Operator&GANG) {
					level = 8;
				}
				if(Operator&HU) {
					level = 9;
				}
				if(Operator&TIANHU) {
					level = 10;
				}
				LevelArray[next] = level;
			}
		}
		else if(m_tablePlayers[next].m_status == 1) {
			if(Operator&(HU)) {
				int level = 0;
				if(Operator&HU) {
					level = 9;
				}
				LevelArray[next] = level;
			}
		}
		next = (next+1)%4;
	}
	
	int doingPlayer = -1;
	next = (m_doingPlayer+1)%4;
	while(next != m_prePlayer) {
		if(LevelArray[next] == m_level) {
			doingPlayer = next;
			break;
		}
		next = (next+1)%4;
	}
	if(doingPlayer == -1) {
		next = (m_prePlayer+1)%4;
		int level = 0;
		while(next != m_prePlayer) {
			if(LevelArray[next] > level && LevelArray[next] < m_level) {
				level = LevelArray[next];
				doingPlayer = next;
			}
			next = (next+1)%4;
		}
		m_level = level;
	}

	if(m_endStatus) {
		m_end = 1;
		if(doingPlayer != -1) {
			int Operator = OptArray[m_doingPlayer];
			if(Operator&HU) {
				m_end = 0;
			}
			else if(Operator&TIANHU) {
				m_end = 0;
			}
		}
	}

	if(m_end) {
		return 0;
	}

	if(doingPlayer != -1) {
		LOG_DEBUG("has Player OPT");
		m_doingPlayer = doingPlayer;
		PlayerQueue *playerQueue = PlayerQueue::getPlayerQueue();
		playerQueue->add(m_tablePlayers[m_doingPlayer].m_player);
		int Operator = OptArray[m_doingPlayer];
		Resp res;
		res.m_uid = m_tablePlayers[m_doingPlayer].m_player->m_account;
		res.m_resp = "game:choose";
		if(Operator&HU) {
			res.m_resp += "|hu";
		}
		if(Operator&CHI) {
			res.m_resp += "|chi";
		}
		if(Operator&GANG) {
			res.m_resp += "|gang";
		}
		if(Operator&ANGANG) {
			res.m_resp += "|gang";
		}
		if(Operator&PENG) {
			res.m_resp += "|peng";
		}
		if(Operator&GUO) {
			res.m_resp += "|guo";
		}
		if(Operator&TIANHU) {
			res.m_resp += "|tianhu|fanpai";
		}
		resp.push_back(res);
		return 0;
	}
	m_doingPlayer = (m_prePlayer+1)%4;
	MoPai(uid, card, card2, card3, resp);
	return 0;
}

int Table::Gang(const std::string &uid, const int &card, const int &card2, const int &card3, std::vector<Resp> &resp) {
	//判断可行性
	int type = (m_tablePlayers[m_doingPlayer].m_playerCards.m_size%3 == 1);
	int Operator = 0;
	int dahu = 0;
	//有问题
	m_playGame.Operator(m_tablePlayers[m_doingPlayer].m_playerCards, Operator, type, card, dahu);
	if(Operator & GANG) {
		m_tablePlayers[m_doingPlayer].gangCard(card);
	}
	else if(Operator & ANGANG) {
		m_tablePlayers[m_doingPlayer].anGangCard(card);
	}
	else {
		return -1;
	}

	{
		Resp res;
		for(int i = 0; i < 4; i ++) {
			res.m_uid = m_tablePlayers[i].m_player->m_account;
			res.m_resp = "game:gang|"+COMMON::convert<int,std::string>(m_doingPlayer+1)+"|"+
						COMMON::convert<int,std::string>(card);
			resp.push_back(res);
		}
	}

	if(1) {
		std::cerr << "in choose" << std::endl;
		for(int i = 0; i < 4; i ++) {
			Resp res;
			res.m_uid = m_tablePlayers[i].m_player->m_account;
			for(int j = 0; j < 4; j ++) {
				if(i == j) {
					std::string Cards;
					std::string Uid;
					m_tablePlayers[i].GetCard(Uid, Cards);
					res.m_resp = "game:pai|"+COMMON::convert<int,std::string>(j+1)+"|"+
							COMMON::convert<int,std::string>(m_tablePlayers[j].m_playerCards.m_size)+"|"+
							Cards;
				}
				else {
					res.m_resp = "game:pai|"+COMMON::convert<int,std::string>(j+1)+"|"+
							COMMON::convert<int,std::string>(m_tablePlayers[j].m_playerCards.m_size);
				}
				resp.push_back(res);
			}
		}

		//听牌m_playGame.Operator(m_tablePlayers[m_doingPlayer].m_playerCards, Operator, type, -1, dahu);
		int Operator = 0;
		m_playGame.isTingPai(m_tablePlayers[m_doingPlayer].m_playerCards, Operator, 1, -1);
		if(Operator & HU) {
			Resp res;
			res.m_uid = m_tablePlayers[m_doingPlayer].m_player->m_account;
			res.m_resp = "game:choose|buzhang|kaigang";
			resp.push_back(res);
			return 0;
		}
	}
	MoPai(uid, card, card2, card3, resp);
	return 0;
}

int Table::Peng(const std::string &uid, const int &card, const int &card2, const int &card3, std::vector<Resp> &resp) {
	m_tablePlayers[m_doingPlayer].pengCard(card);
	Resp res;
	for(int i = 0; i < 4; i ++) {
		res.m_uid = m_tablePlayers[i].m_player->m_account;
		res.m_resp = "game:peng|"+COMMON::convert<int,std::string>(m_doingPlayer+1)+"|"+
						COMMON::convert<int,std::string>(card);
		resp.push_back(res);
	}

	for(int i = 0; i < 4; i ++) {
		Resp res;
		res.m_uid = m_tablePlayers[i].m_player->m_account;
		for(int j = 0; j < 4; j ++) {
			if(i == j) {
				std::string Cards;
				std::string Uid;
				m_tablePlayers[i].GetCard(Uid, Cards);
				res.m_resp = "game:pai|"+COMMON::convert<int,std::string>(j+1)+"|"+
						COMMON::convert<int,std::string>(m_tablePlayers[j].m_playerCards.m_size)+"|"+
						Cards;
			}
			else {
				res.m_resp = "game:pai|"+COMMON::convert<int,std::string>(j+1)+"|"+
						COMMON::convert<int,std::string>(m_tablePlayers[j].m_playerCards.m_size);
			}
			resp.push_back(res);
		}
	}
	int type = (m_tablePlayers[m_doingPlayer].m_playerCards.m_size%3 == 1);
	int Operator = 0;
	int dahu = 0;
	m_playGame.Operator(m_tablePlayers[m_doingPlayer].m_playerCards, Operator, type, -1, dahu);
	if(Operator) {
		res.m_uid = m_tablePlayers[m_doingPlayer].m_player->m_account;
		res.m_resp = "game:choose";
		/*if(Operator&HU) {
			res.m_resp += "|hu";
		}
		if(Operator&TIANHU) {
			res.m_resp += "|tianhu";
		}*/
		if(Operator&CHUPAI) {
			res.m_resp += "|dapai";
		}
		resp.push_back(res);
	}
	return 0;
}

int Table::Chi(const std::string &uid, const int &card, const int &card2, const int &card3, std::vector<Resp> &resp) {
	m_tablePlayers[m_doingPlayer].chiCard(card, card2, card3);
	Resp res;
	for(int i = 0; i < 4; i ++) {
		res.m_uid = m_tablePlayers[i].m_player->m_account;
		res.m_resp = "game:chi|"+COMMON::convert<int,std::string>(m_doingPlayer+1)+"|"+
						COMMON::convert<int,std::string>(card)+"|"+
						COMMON::convert<int,std::string>(card2)+"|"+
						COMMON::convert<int,std::string>(card3);
		resp.push_back(res);
	}
	int type = (m_tablePlayers[m_doingPlayer].m_playerCards.m_size%3 == 1);
	int Operator = 0;
	int dahu = 0;
	m_playGame.Operator(m_tablePlayers[m_doingPlayer].m_playerCards, Operator, type, m_prePai, dahu);
	if(Operator) {
		res.m_uid = m_tablePlayers[m_doingPlayer].m_player->m_account;
		res.m_resp = "game:choose";
		/*if(Operator&HU) {
			res.m_resp += "|hu";
		}*/
		if(Operator&GANG) {
			res.m_resp += "|gang";
		}
		if(Operator&ANGANG) {
			res.m_resp += "|gang";
		}
		if(Operator&CHUPAI) {
			res.m_resp += "|dapai";
		}
		resp.push_back(res);
	}

	for(int i = 0; i < 4; i ++) {
		Resp res;
		res.m_uid = m_tablePlayers[i].m_player->m_account;
		for(int j = 0; j < 4; j ++) {
			if(i == j) {
				std::string Cards;
				std::string Uid;
				m_tablePlayers[i].GetCard(Uid, Cards);
				res.m_resp = "game:pai|"+COMMON::convert<int,std::string>(j+1)+"|"+
						COMMON::convert<int,std::string>(m_tablePlayers[j].m_playerCards.m_size)+"|"+
						Cards;
			}
			else {
				res.m_resp = "game:pai|"+COMMON::convert<int,std::string>(j+1)+"|"+
						COMMON::convert<int,std::string>(m_tablePlayers[j].m_playerCards.m_size);
			}
			resp.push_back(res);
		}
	}
	return 0;
}

int Table::Hu(const std::string &uid, const int &card, const int &card2, const int &card3, std::vector<Resp> &resp) {
	int type = (m_tablePlayers[m_doingPlayer].m_playerCards.m_size%3 == 1);
	int Operator = 0;
	int dahu = 0;
	m_playGame.Operator(m_tablePlayers[m_doingPlayer].m_playerCards, Operator, type, m_prePai, dahu);
	if(Operator&HU) {
		
	}
	else {
		return -1;
	}

	m_level = 9;
	m_tablePlayers[m_doingPlayer].m_status = 2;
	m_tablePlayers[m_doingPlayer].m_dahu = dahu;
	if(type == 0) {
		m_tablePlayers[m_doingPlayer].m_huStats = 1;
	}
	else if(type == 1) {
		m_tablePlayers[m_doingPlayer].m_huStats = 0;
	}

	//有人胡牌
	m_endStatus = 1;
	if(type == 0) {
		//检查海底捞月
		/*if(m_cardSender->getRemain() == 1) {
			int next = (m_doingPlayer+1)%4;
			while(next != m_doingPlayer) {
				int Operator = 0;
				m_playGame.isTingPai(m_tablePlayers[next].m_playerCards, Operator, 1, -1);
				if(Operator & HU) {
					Resp res;
					res.m_uid = m_tablePlayers[next].m_player->m_account;
					res.m_resp = "game:choose|laoyue|guo";
					resp.push_back(res);
					m_prePlayer = m_doingPlayer;
					m_doingPlayer = next;
					return 0;
				}
				next = (next+1)%4;
			}
			m_end = 1;
		}
		else {
			m_end = 1;
		}*/
		m_end = 1;
		return 0;
	}

	//轮询，查看是否有通炮
	int OptArray[4] = {0,0,0,0};
	int LevelArray[4] = {0,0,0,0};
	int next = (m_doingPlayer+1)%4;
	while(next != m_prePlayer) {
		int type = (m_tablePlayers[next].m_playerCards.m_size%3 == 1);
		int Operator = 0;
		int dahu = 0;
		m_playGame.Operator(m_tablePlayers[next].m_playerCards, Operator, type, m_prePai, dahu);
		OptArray[next] = Operator;
		if(Operator&(HU|TIANHU)) {
			int level = 0;
			if(Operator&HU) {
				level = 9;
			}
			//if(Operator&TIANHU) {
			//	level = 10;
			//}
			LevelArray[next] = level;
		}
		next = (next+1)%4;
	}
	
	int doingPlayer = -1;
	next = (m_doingPlayer+1)%4;
	while(next != m_prePlayer) {
		if(LevelArray[next] == m_level) {
			doingPlayer = next;
			break;
		}
		next = (next+1)%4;
	}
	//如果同时有人胡牌
	if(doingPlayer != -1) {
		m_doingPlayer = doingPlayer;
		PlayerQueue *playerQueue = PlayerQueue::getPlayerQueue();
		playerQueue->add(m_tablePlayers[m_doingPlayer].m_player);
		int Operator = OptArray[m_doingPlayer];
		Resp res;
		res.m_uid = m_tablePlayers[m_doingPlayer].m_player->m_account;
		res.m_resp = "game:choose";
		if(Operator&HU) {
			res.m_resp += "|hu";
		}
		if(Operator&GUO) {
			res.m_resp += "|guo";
		}
		//if(Operator&TIANHU) {
		//	res.m_resp += "|tianhu|fanpai";
		//}
		resp.push_back(res);
	}
	else {
		//否则游戏结束
		m_end = 1;
	}
	return 0;
}

int Table::TianHu(const std::string &uid, std::vector<Resp> &resp) {
	int type = (m_tablePlayers[m_doingPlayer].m_playerCards.m_size%3 == 1);
	int Operator = 0;
	int dahu = 0;
	m_playGame.Operator(m_tablePlayers[m_doingPlayer].m_playerCards, Operator, type, m_prePai, dahu);
	if(Operator&TIANHU) {
		
	}
	else {
		return -1;
	}

	m_level = 10;
	m_tablePlayers[m_doingPlayer].m_status = 2;
	m_tablePlayers[m_doingPlayer].m_dahu = 0;
	m_tablePlayers[m_doingPlayer].m_huStats = 1;

	//有人胡牌
	m_endStatus = 1;

	//轮询，查看是否有通炮
	int OptArray[4] = {0,0,0,0};
	int LevelArray[4] = {0,0,0,0};
	int next = (m_doingPlayer+1)%4;
	while(next != m_prePlayer) {
		int type = (m_tablePlayers[next].m_playerCards.m_size%3 == 1);
		int Operator = 0;
		int dahu = 0;
		m_playGame.Operator(m_tablePlayers[next].m_playerCards, Operator, type, m_prePai, dahu);
		OptArray[next] = Operator;
		if(Operator&(TIANHU)) {
			int level = 0;
			if(Operator&HU) {
				level = 9;
			}
			if(Operator&TIANHU) {
				level = 10;
			}
			LevelArray[next] = level;
		}
		next = (next+1)%4;
	}
	
	int doingPlayer = -1;
	next = (m_doingPlayer+1)%4;
	while(next != m_prePlayer) {
		if(LevelArray[next] == m_level) {
			doingPlayer = next;
			break;
		}
		next = (next+1)%4;
	}
	//如果同时有人胡牌
	if(doingPlayer != -1) {
		m_doingPlayer = doingPlayer;
		PlayerQueue *playerQueue = PlayerQueue::getPlayerQueue();
		playerQueue->add(m_tablePlayers[m_doingPlayer].m_player);
		int Operator = OptArray[m_doingPlayer];
		Resp res;
		res.m_uid = m_tablePlayers[m_doingPlayer].m_player->m_account;
		res.m_resp = "game:choose";
		if(Operator&TIANHU) {
			res.m_resp += "|tianhu|fanpai";
		}
		resp.push_back(res);
	}
	else {
		//否则游戏结束
		m_end = 1;
	}
	return 0;
}

int Table::TianHuMoPai(const std::string &uid, std::vector<Resp> &resp) {
	{
		int type = (m_tablePlayers[m_doingPlayer].m_playerCards.m_size%3 == 1);
		int Operator = 0;
		int dahu = 0;
		m_playGame.Operator(m_tablePlayers[m_doingPlayer].m_playerCards, Operator, type, m_prePai, dahu);
		if(Operator&TIANHU) {

		}
		else {
			return -1;
		}
	}
	Pai c1 = PlayerCards::TransCard(m_cardSender->getCard());
	Pai c2 = PlayerCards::TransCard(m_cardSender->getCard());
	TablePlayer tmp;
	tmp.addCard(c1);
	tmp.addCard(c2);
	//m_tablePlayers[m_doingPlayer].addCard(c1);
	//m_tablePlayers[m_doingPlayer].addCard(c2);
	{
		Resp res;
		res.m_uid = m_tablePlayers[m_doingPlayer].m_player->m_account;
		res.m_resp = "game:fanpai|"+COMMON::convert<int, std::string>(c1)+"|"+COMMON::convert<int, std::string>(c2);
		resp.push_back(res);
	}
	int type = (m_tablePlayers[m_doingPlayer].m_playerCards.m_size%3 == 1);
	int Operator = 0;
	int dahu = 0;
	m_playGame.Operator(tmp.m_playerCards, Operator, type, m_prePai, dahu);
	if(Operator&TIANHU) {
		dahu = 1;
		//有人胡牌
		m_endStatus = 1;
		m_level = 10;
		m_tablePlayers[m_doingPlayer].m_status = 2;
		m_tablePlayers[m_doingPlayer].m_dahu = 1;
		m_tablePlayers[m_doingPlayer].m_huStats = 1;
	}
	else {
		m_tablePlayers[m_doingPlayer].m_status = 1;
		Resp res;
		res.m_uid = m_tablePlayers[m_doingPlayer].m_player->m_account;
		res.m_resp = "game:tuoguan";
		resp.push_back(res);
	}

	//轮询，查看是否有通炮
	int OptArray[4] = {0,0,0,0};
	int LevelArray[4] = {0,0,0,0};
	int next = (m_doingPlayer+1)%4;
	while(next != m_prePlayer) {
		int type = (m_tablePlayers[next].m_playerCards.m_size%3 == 1);
		int Operator = 0;
		int dahu = 0;
		m_playGame.Operator(m_tablePlayers[next].m_playerCards, Operator, type, m_prePai, dahu);
		OptArray[next] = Operator;
		if(Operator&(TIANHU)) {
			int level = 0;
			if(Operator&HU) {
				level = 9;
			}
			if(Operator&TIANHU) {
				level = 10;
			}
			LevelArray[next] = level;
		}
		next = (next+1)%4;
	}
	
	int doingPlayer = -1;
	next = (m_doingPlayer+1)%4;
	//std::cerr << "doing = " << m_doingPlayer << "  prePlayer = " << m_prePlayer << std::endl;
	while(next != m_prePlayer) {
		if(LevelArray[next] == m_level) {
			doingPlayer = next;
			break;
		}
		next = (next+1)%4;
	}
	//for(int i = 0; i < 4; i ++) {
	//	std::cerr << "Opt = " << OptArray[i] << std::endl;
	//}
	//如果同时有人胡牌
	if(doingPlayer != -1) {
		m_doingPlayer = doingPlayer;
		PlayerQueue *playerQueue = PlayerQueue::getPlayerQueue();
		playerQueue->add(m_tablePlayers[m_doingPlayer].m_player);
		int Operator = OptArray[m_doingPlayer];
		Resp res;
		res.m_uid = m_tablePlayers[m_doingPlayer].m_player->m_account;
		res.m_resp = "game:choose";
		if(Operator&TIANHU) {
			res.m_resp += "|tianhu|fanpai";
		}
		resp.push_back(res);
	}
	else {
		if(m_endStatus) {
			//有人胡牌，游戏结束
			m_end = 1;
		}
		else {
			//没人胡牌
			std::cerr << "uid = " << m_tablePlayers[m_prePlayer].m_player->m_account << std::endl;
			m_doingPlayer = m_prePlayer;
			MoPai(m_tablePlayers[m_prePlayer].m_player->m_account, 0, 0, 0, resp);
		}
	}
	return 0;
}

int Table::KaiGang(const std::string &uid, const int &card, const int &card2, const int &card3, std::vector<Resp> &resp) {
	m_tablePlayers[m_doingPlayer].m_kaiGang = 1;
	m_tablePlayers[m_doingPlayer].m_status = 1;
	Resp res;
	res.m_uid = m_tablePlayers[m_doingPlayer].m_player->m_account;
	res.m_resp = "game:tuoguan";
	resp.push_back(res);

	MoPai(uid, card, card2, card3, resp);
}

int Table::PlayerOperator(const std::string &uid, const int &type, const int &card, const int &card2, const int &card3, std::vector<Resp> &resp) {
	LOG_DEBUG("in PlayerOperator");
	if(m_tableStatus == 0) {
		LOG_DEBUG("Operator Error %s", uid.c_str());
		return -1;
	}
	if(m_tablePlayers[m_doingPlayer].m_player->m_account != uid) {
		LOG_DEBUG("m_doingPlayer != %s", uid.c_str());
		return -1;
	}

	PlayerQueue *playerQueue = PlayerQueue::getPlayerQueue();
	playerQueue->sub(m_tablePlayers[m_doingPlayer].m_player);
	m_online[m_doingPlayer] = 1;

	if(type & KAIGANG) {
		KaiGang(uid, card, card2, card3, resp);
	}
	else if(type & BUZHANG) {
		MoPai(uid, card, card2, card3, resp);
	}
	else if(type & LAOYUE) {
		Laoyue(uid, card, card2, card3, resp);
	}
	if(type & MOPAI) {
		MoPai(uid, card, card2, card3, resp);
	}
	if(type & DAPAI) {
		ChuPai(uid, card, card2, card3, resp);
	}
	else if(type & GUO) {
		Guo(uid, card, card2, card3, resp);
	}
	else if(type & GANG) {
		Gang(uid, card, card2, card3, resp);
	}
	else if(type & PENG) {
		Peng(uid, card, card2, card3, resp);
	}
	else if(type & HU) {
		Hu(uid, card, card2, card3, resp);
	}
	else if(type & TIANHUMOPAI) {
		TianHuMoPai(uid, resp);
	}
	else if(type & TIANHU) {
		TianHu(uid, resp);
	}
	else if(type & CHI) {
		Chi(uid, card, card2, card3, resp);
	}
	else if(type & GUAJI) {
		m_online[m_doingPlayer] = 0;
		if(m_tablePlayers[m_doingPlayer].m_playerCards.m_size%3 == 1) {
			//过
			Guo(uid, -1, -1, -1, resp);
		}
		else {
			//出牌
			Pai pai = m_tablePlayers[m_doingPlayer].GetRandomCard();
			pai = PlayerCards::TransCard(pai);
			ChuPai(uid, pai, -1, -1, resp);
		}
	}
	playerQueue->add(m_tablePlayers[m_doingPlayer].m_player);
	JieSuan(resp);
	return 0;
}

int Table::GetPlayersAccount(std::vector<std::string> &playersID) {
	playersID.clear();
	for(int i = 0; i < m_tablePlayer.size(); i ++) {
		playersID.push_back(m_tablePlayer[i].m_player->m_account);
	}
	return 0;
}

int Table::InRoom(const Player *player) {
	if(m_people >= 4) {
		return -1;
	}
	for(int i = 0; i < 4; i ++) {
		if(m_status[i] == 0) {
			m_status[i] = 1;
			m_pStatus[i] = 1;
			m_tablePlayers[i] = TablePlayer(player);
			m_people ++;
			m_pPeople ++;
			break;
		}
	}
	return 0;
}

int Table::OutRoom(const Player *player) {
	int ret = -1;
	if(m_tableStatus) {
		return ret;
	}
	if(m_people <= 0) {
		return ret;
	}
	for(int i = 0; i < 4; i ++) {
		if(m_status[i]) {
			if(m_tablePlayers[i].m_player == player) {
				m_status[i] = 0;
				m_pStatus[i] = 0;
				m_people --;
				m_pPeople --;
				ret = 0;
				break;
			}
		}
	}
	if(m_status[m_roomMan] == 0) {
		m_people = 0;
		m_pPeople = 0;
	}
	return ret;
}

int Table::GetPlayersID(std::string &players) {
	/*players = "";
	int num = 0;
	for(int i = 0; i < m_tablePlayer.size(); i ++) {
		if(i != 0) {
			players += "#";
		}
		num ++;
		players += m_tablePlayer[i].m_player->m_account;
	}
	while(num < 4) {
		players += "#";
		num ++;
	}*/
	return 0;
}

Table::~Table() {
	delete m_cardSender;
	//m_tablePlayer.clear();
}

/*
	int m_id;			//牌桌id
	int m_planPlay;		//计划局数
	int m_tableType;			//属于什么游戏
	int m_retCard;		//扎马数

	std::vector<TablePlayer> m_tablePlayer;	//牌桌上的玩家
	int m_roomMan;		//房主

	TablePlayer m_tablePlayers[4];	//玩家
	int m_status[4];	//位置状态
	
	int m_remain;		//剩余游戏局数
	int m_isPay;		//是否付费
	int m_tableStatus;	//牌桌状态 0.等人 1.游戏中
	int m_zhuang;		//庄家
	int m_people;		//人数
	
	int m_doingPlayer;	//正在执行操作的玩家
	int prePlayer;		//上一个玩家
	Pai prePai;			//上一张打出的牌
	CardSender *m_cardSender;	//发牌器
	GamePlayWay m_playGame;	//游戏玩法*/

Table::Table(const Player *player, const int type, const int &id, const int &planPlay, const int &retCard) {
	m_id = id;
	m_tableType = type;
	m_roomMan = 0;
	m_planPlay = planPlay;

	m_tablePlayers[0] = TablePlayer(player);
	m_status[0] = 1;
	m_pStatus[0] = 1;
	for(int i = 1; i < 4; i ++) {
		m_status[i] = 0;
		m_pStatus[i] = 0;
	}
	m_playScore.init();
	m_retCard = retCard;
	
	//m_remain = 1;
	m_remain = planPlay;
	m_isPay = 0;
	m_tableStatus = 0;
	m_zhuang = 0;
	m_people = 1;
	m_pPeople = 1;

	m_doingPlayer = -1;
	m_prePlayer = -1;
	m_prePai = -1;
	m_cardSender = new CardSenderA(retCard);
}

int Table::BeginGame(const std::string &uid, std::vector<Resp> &resp) {
	LOG_DEBUG("in BeginGame");
	if(m_people != 4) {
		return -1;
	}
	if(m_tableStatus == 1) {
		return -1;
	}
	if(m_remain <= 0) {
		return -1;
	}
	if(!m_isPay) {
		int room = m_tablePlayers[m_roomMan].m_player->GetRoomCard();
		if(room >= 1) {
			Player *tplayer = (Player *)m_tablePlayers[m_roomMan].m_player;
			tplayer->subRoomCard(1);
		}
		else {
			return -1;
		}
		m_isPay = 1;
	}
	if(m_remain == 0) {
		return -1;
	}
	for(int i = 0; i < 4; i ++) {
		m_online[i] = 1;
	}
	m_endStatus = 0;
	m_tableStatus = 1;
	m_laoyue = 0;
	m_cardSender->init();
	for(int i = 0; i < 4; i ++) {
		m_tablePlayers[i].init();
	}
	for(int i = 0; i < 4; i ++) {
		Player *tplayer = (Player *)m_tablePlayers[i].m_player;
		tplayer->addPlayNum();
	}
	for(int i = 0; i < 4; i ++) {
		if(m_pStatus[i]) {
			for(int j = 0; j < 12; j ++) {
				Pai p = m_cardSender->getCard();
				p = PlayerCards::TransCard(p);
				m_tablePlayers[i].addCard(p);
			}
			Pai p = m_cardSender->getJiang();
			p = PlayerCards::TransCard(p);
			m_tablePlayers[i].addCard(p);
		}
	}
	LOG_DEBUG("end get Card");

	m_tablePlayers[0].toHu();
	m_tablePlayers[1].toHu();
	m_tablePlayers[2].toHu();

	m_end = 0;
	m_remain --;
	m_doingPlayer = m_zhuang;
	m_prePlayer = m_zhuang;
	for(int i = 0; i < 4; i ++) {
		m_ScoreArray[i] = 0;
	}

	//给每个人发牌
	{
		Resp res;
		for(int i = 0; i < 4; i ++) {
			GetAllCards(i, res.m_uid, res.m_resp);
			resp.push_back(res);
		}
	}
	for(int i = 0; i < 4; i ++) {
		Resp res;
		res.m_uid = m_tablePlayers[i].m_player->m_account;
		for(int j = 0; j < 4; j ++) {
			if(i == j) {
				std::string Cards;
				std::string Uid;
				m_tablePlayers[i].GetCard(Uid, Cards);
				res.m_resp = "game:pai|"+COMMON::convert<int,std::string>(j+1)+"|"+
						COMMON::convert<int,std::string>(m_tablePlayers[j].m_playerCards.m_size)+"|"+
						Cards;
			}
			else {
				res.m_resp = "game:pai|"+COMMON::convert<int,std::string>(j+1)+"|"+
						COMMON::convert<int,std::string>(m_tablePlayers[j].m_playerCards.m_size);
			}
			resp.push_back(res);
		}
	}

	m_level = 10;

	//判断是否天胡
	int OptArray[4] = {0,0,0,0};
	int LevelArray[4] = {0,0,0,0};
	for(int i = 0; i < 4; i ++) {
		int next = i;
		int type = (m_tablePlayers[next].m_playerCards.m_size%3 == 1);
		int Operator = 0;
		int dahu = 0;
		m_playGame.Operator(m_tablePlayers[next].m_playerCards, Operator, type, m_prePai, dahu);
		OptArray[next] = Operator;
		if(Operator&(TIANHU)) {
			int level = 0;
			if(Operator&TIANHU) {
				level = 10;
				m_level = level;
			}
			LevelArray[next] = level;
		}
	}
	
	for(int i = 0; i < 4; i ++) {
		std::cerr << "level = " << LevelArray[i] << std::endl;
	}

	int doingPlayer = -1;
	for(int i = 0; i < 4; i ++) {
		int next = (i + m_zhuang)%4;
		if(LevelArray[next] == m_level) {
			doingPlayer = next;
			break;
		}
	}

	if(doingPlayer == -1) {
		//没人天胡
		int zhuang = m_zhuang;
		std::string zid = m_tablePlayers[zhuang].m_player->m_account;
		PlayerOperator(zid, MOPAI, 0, 0, 0, resp);
	}
	else {
		//有人天胡
		std::cerr << "Tianhu" << std::endl;
		m_doingPlayer = doingPlayer;
		m_level = 10;
		Resp res;
		res.m_uid = m_tablePlayers[m_doingPlayer].m_player->m_account;
		res.m_resp = "game:choose|tianhu|fanpai";
		resp.push_back(res);
	}
	return 0;
}

int Table::toClient(std::tr1::unordered_map<std::string, std::string> &playersOp) {
	/*for(int i = 0; i < m_tablePlayer.size(); i ++) {
		std::string Operator;
		if(i == m_doingPlayer) {
			m_tablePlayer[i].showLong(Operator);
		}
		else {
			m_tablePlayer[i].showShort(Operator);
		}

	}*/
	return 0;
}

int Table::GetAllCards(const int &id, std::string &uid, std::string &resp) {
	resp = "startgame:";
	std::string cards;
	m_tablePlayers[id].GetCard(uid, cards);
	resp += cards;
	return 0;
}

int Table::Again(const std::string &uid) {
	std::cerr << "in Again" << std::endl;
	int id = -1;
	for(int i = 0; i < 4; i ++) {
		if(m_status[i]) {
			if(m_tablePlayers[i].m_player->m_account == uid) {
				id = i;
				break;
			}
		}
	}
	if(id == -1) {
		return -1;
	}
	m_pPeople ++;
	m_pStatus[id] = 1;
	std::cerr << "in Again end" << std::endl;
	return 0;
}

int Table::inRoomPai(const std::string &uid, std::vector<Resp> &resp) {
	for(int i = 0; i < 4; i ++) {
		if(m_tablePlayers[i].m_player->m_account != uid) {
			continue;
		}
		Resp res;
		res.m_uid = m_tablePlayers[i].m_player->m_account;
		for(int j = 0; j < 4; j ++) {
			if(i == j) {
				std::string Cards;
				std::string Uid;
				m_tablePlayers[i].GetCard(Uid, Cards);
				res.m_resp = "game:pai|"+COMMON::convert<int,std::string>(j+1)+"|"+
						COMMON::convert<int,std::string>(m_tablePlayers[j].m_playerCards.m_size)+"|"+
						Cards;
			}
			else {
				res.m_resp = "game:pai|"+COMMON::convert<int,std::string>(j+1)+"|"+
						COMMON::convert<int,std::string>(m_tablePlayers[j].m_playerCards.m_size);
			}
			resp.push_back(res);
		}
	}
	return 0;
}
