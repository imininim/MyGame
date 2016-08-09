#include "define.h"

Pai PlayerCards::TransCard(const Pai &card) {
	if(card > 26) {
		return card+14;
	}
	if(card > 17) {
		return card+13;
	}
	if(card > 8) {
		return card+12;
	}
	return card+11;
}

Pai PlayerCards::reTransCard(const Pai &card) {
	if(card > 40) {
		return card-14;
	}
	if(card > 30) {
		return card-13;
	}
	if(card > 20) {
		return card-12;
	}
	if(card > 10) {
		return card-11;
	}
	return card;
}

int PlayerCards::getCard(std::string &resp) {
	resp = "";
	int flag = 0;
	for(int i = 0; i < 27; i ++) {
		for(int j = 0; j < m_playCards[i]; j ++) {
			if(flag) {
				resp += "|";
			}
			flag = 1;
			resp += COMMON::convert<int, std::string>(TransCard(i));
		}
	}
	return 0;
}

int PlayerCards::addCard(Pai idx) {
	m_size ++;
	m_playCards[idx] ++;
	m_prePai = idx;
	return 0;
}

int PlayerCards::outCard(Pai idx) {
	m_size --;
	m_playCards[idx] --;
	return 0;
}

int PlayerCards::chiCard(Pai idx, Pai card1, Pai card2) {
	m_size -= 2;
	m_playCards[card1] --;
	m_playCards[card2] --;
	std::vector<Pai> chi;
	Pai Min = std::min(idx, std::min(card1, card2));
	chi.push_back(Min);
	chi.push_back(Min+1);
	chi.push_back(Min+2);
	m_playCardsChi.push_back(chi);
	return 0;
}

int PlayerCards::pengCard(Pai idx) {
	m_size -= 2;
	m_playCards[idx] -= 2;
	std::vector<Pai> peng;
	for(int i = 0; i < 3; i ++) {
		peng.push_back(idx);
	}
	m_playCardsPeng.push_back(peng);
	return 0;
}

int PlayerCards::gangCard(Pai idx) {
	if(m_playCards[idx] == 3) {
		m_size -= 3;
		m_playCards[idx] -= 3;
		std::vector<Pai> gang;
		for(int i = 0; i < 4; i ++) {
			gang.push_back(idx);
		}
		m_playCardsGang.push_back(gang);
	}
	else {
		m_size -= 1;
		m_playCards[idx] -= 1;
		std::vector<Pai> gang;
		for(int i = 0; i < 4; i ++) {
			gang.push_back(idx);
		}
		m_playCardsGang.push_back(gang);
		std::vector<std::vector<Pai> >::iterator it = m_playCardsPeng.begin();
		for(; it != m_playCardsPeng.end(); it ++) {
			if((*it)[0] == idx) {
				m_playCardsPeng.erase(it);
				break;
			}
		}
	}
	return 0;
}

int PlayerCards::anGangCard(Pai idx) {
	m_size -= 4;
	m_playCards[idx] -= 4;
	std::vector<Pai> gang;
	for(int i = 0; i < 4; i ++) {
		gang.push_back(idx);
	}
	m_playCardsGang.push_back(gang);
	return 0;
}

int PlayerCards::showPublic(std::string &resp) {
	resp = "";
	bool flag = 0;
	for(int i = 0; i < m_playCardsPeng.size(); i ++) {
		if(flag) {
			resp += "|";
		}
		flag = true;
		for(int j = 0; j < m_playCardsPeng[i].size(); j ++) {
			resp += char(m_playCardsPeng[i][j]);
		}
	}
	for(int i = 0; i < m_playCardsGang.size(); i ++) {
		if(flag) {
			resp += "|";
		}
		flag = true;
		for(int j = 0; j < m_playCardsGang[i].size(); j ++) {
			resp += char(m_playCardsGang[i][j]);
		}
	}
	for(int i = 0; i < m_playCardsAnGang.size(); i ++) {
		if(flag) {
			resp += "|";
		}
		flag = true;
		for(int j = 0; j < m_playCardsAnGang[i].size(); j ++) {
			resp += char(m_playCardsAnGang[i][j]);
		}
	}
	return 0;
}

int PlayerCards::showPrivate(std::string &resp) {
	for(int i = 0; i < 27; i ++) {
		if(m_playCards[i]) {
			for(int j = 0; j < m_playCards[i]; j ++) {
				resp += char(i+(int('@')));
			}
		}
	}
	return 0;
}
