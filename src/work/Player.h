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
	int subRoomCard(int idx);
	int addPlayNum();
	int changeScore(int score);
	int toSQL(std::string &sql) const {
		sql = "update Player set roomCard = "+COMMON::convert<int, std::string>(m_roomCard)+
		", playNum = "+COMMON::convert<int, std::string>(m_PlayNum)+
		", score = "+COMMON::convert<int, std::string>(m_score)+
		", name = '"+m_name+
		"' where id = '"+COMMON::convert<int, std::string>(m_id)+"';";
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
	int m_id;
	std::string m_account;
	std::string m_name;
	std::string m_url;
	std::string m_sex;
private:
	std::string m_passwd;
	int m_roomCard;
	int m_PlayNum;
	int m_score;
};

#endif
