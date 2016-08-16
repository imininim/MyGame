#include "TablePlayer.h"

int TablePlayer::init() {
	return m_playerCards.init();
}

int TablePlayer::chiCard(Pai idx, Pai card1, Pai card2) {
	return m_playerCards.chiCard(PlayerCards::reTransCard(idx), PlayerCards::reTransCard(card1), PlayerCards::reTransCard(card2));
}

int TablePlayer::GetCard(std::string &uid, std::string &resp) {
	uid = m_player->m_account;
	return m_playerCards.getCard(resp);
}

int TablePlayer::addCard(Pai idx) {
	return m_playerCards.addCard(PlayerCards::reTransCard(idx));
}

int TablePlayer::outCard(Pai idx) {
	return m_playerCards.outCard(PlayerCards::reTransCard(idx));
}


int TablePlayer::pengCard(Pai idx) {
	return m_playerCards.pengCard(PlayerCards::reTransCard(idx));
}

int TablePlayer::gangCard(Pai idx) {
	if(m_playerCards.m_size%3 == 1) {
		m_playerCards.gangCard(PlayerCards::reTransCard(idx));
	}
	else {
		m_playerCards.anGangCard(PlayerCards::reTransCard(idx));
	}
	return 0;
}

int TablePlayer::anGangCard(Pai idx) {
	return m_playerCards.anGangCard(PlayerCards::reTransCard(idx));
}

int TablePlayer::showLong(std::string &resp) {
	std::string privatePai;
	m_playerCards.showPrivate(privatePai);
	std::string publicPai;
	m_playerCards.showPublic(publicPai);
	resp = privatePai+"#"+publicPai;
	return 0;
}

int TablePlayer::showShort(std::string &resp) {
	std::string privatePai = COMMON::convert<int, std::string>(m_playerCards.m_size);
	std::string publicPai;
	m_playerCards.showPublic(privatePai);
	resp = privatePai+"#"+publicPai;
	return 0;
}
