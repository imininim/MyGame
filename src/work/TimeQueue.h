#include "Player.h"
#include "OnlinePlayers.h"

class PlayerQueue {
private:
	PlayerQueue() {}
public:
	static PlayerQueue* getPlayerQueue() {
		static PlayerQueue playerQueue;
		return &playerQueue;
	}
public:
	std::tr1::unordered_map<const Player*, int> m_playerTimeMap;
	//map<const Player*, int> m_playerTimeMap;
public:
	int ClearTable() {
		OnlinePlayers* onlinePlayer = OnlinePlayers::getOnlinePlayers();
		std::tr1::unordered_map<int, Table*>::iterator it;	//全部牌桌
		for(it = onlinePlayer->m_onlineTableMap.begin(); it != onlinePlayer->m_onlineTableMap.end(); it ++) {
			int flag = 0;
			for(int i = 0; i < 4; i ++) {
				if(it->second->m_online[i]) {
					flag = 1;
				}
			}
			if(flag == 0) {
				it->second->m_tableStatus = 0;
				for(int i = 0; i < 4; i ++) {
					sub(it->second->m_tablePlayers[i].m_player);
				}
				onlinePlayer->DestroyTable(it->first);
			}
		}
		return 0;
	}
	int add(const Player *player) {
		if(m_playerTimeMap.find(player) == m_playerTimeMap.end()) {
			m_playerTimeMap[player] = 0;
		}
		return 0;
	}
	int set(const Player *player) {
		if(m_playerTimeMap.find(player) == m_playerTimeMap.end()) {
			m_playerTimeMap[player] = 0;
		}
		m_playerTimeMap[player] = 0;
		return 0;
	}
	int sub(const Player *player) {
		std::tr1::unordered_map<const Player*, int>::iterator it;
		it = m_playerTimeMap.find(player);
		if(it != m_playerTimeMap.end()) {
			m_playerTimeMap.erase(it);
		}
		return 0;
	}
	int show() {
		std::tr1::unordered_map<const Player*, int>::iterator it;
		//map<const Player *, int>::iterator it;
		for(it = m_playerTimeMap.begin(); it != m_playerTimeMap.end(); it ++) {
			std::cerr << it->first << " - " << it->second << std::endl;
		}
		return 0;
	}

	int addTime() {
		std::tr1::unordered_map<const Player*, int>::iterator it;
		//map<const Player *, int>::iterator it;
		for(it = m_playerTimeMap.begin(); it != m_playerTimeMap.end(); it ++) {
			it->second += 1;
		}
		return 0;
	}
	int getGuaJI(std::vector<std::string> &list) {
		std::tr1::unordered_map<const Player*, int>::iterator it;
		//map<const Player*, int>::iterator it;
		for(it = m_playerTimeMap.begin(); it != m_playerTimeMap.end(); it ++) {
			if(it->second >= 60) {
				list.push_back((it->first)->m_account);
			}
		}
		return 0;
	}
};


