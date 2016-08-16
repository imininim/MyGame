#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <vector>
#include <string>
#include "common/CommonFuc.h"
#include <iostream>

class Player {
public:
	Player(const std::vector<std::string> &Info);
public:
	void show() const;
	int GetRoomCard() const {
		return m_roomCard;
	}
	int subRoomCard(int idx) {
		m_roomCard -= idx;
		return 0;
	}
	int changeScore(int score) {
		m_score += score;
		return 0;
	}
	int toSQL(std::string &sql) const {
		sql = "update Player set roomCard = "+COMMON::convert<int, std::string>(m_roomCard)+
		", playNum = "+COMMON::convert<int, std::string>(m_PlayNum)+
		" where id = "+COMMON::convert<int, std::string>(m_id)+";";
	}
	int setName(const std::string &Name) {
		m_name = Name;
		return 0;
	}
	int setSex(const std::string &Sex) {
		m_sex = Sex;
		return 0;
	}
	int setURL(const std::string &URL) {
		m_url = URL;
		return 0;
	}
public:
	std::string m_account;
	std::string m_name;
	std::string m_url;
	std::string m_sex;
private:
	std::string m_passwd;
	int m_roomCard;
	int m_PlayNum;
	int m_id;
	int m_score;
};

#endif
