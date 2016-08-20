#include "Game.h"
#include "OnlinePlayers.h"
#include "common/CommonFuc.h"
#include "GameConfig.h"

/*int Game::doGame(std::string &Account, const std::string &Commands, std::vector<Resp> Resp) {
	if(Opt == "login") {
		LogIn(Account, Command, Resp);
	}
	else if(Opt == "logout") {
		LogOut(Account, Command, Resp);
	}
	else if(Opt == "createroom") {
		CreateTable(Account, Command, Resp);
	}
	else if(Opt == "inroom") {
		InTable(Account, Command, Resp);
	}
	else if(Opt == "outroom") {
		OutTable(Account, Command, Resp);
	}
	else if(Opt == "game") {
		Play(Account, Command, Resp);
	}
	return 0;
}*/

//游戏模块
int Game::PlayGame(const std::string &uid, const std::string &commands, std::vector<Resp> &resp) {
	int ret = -1;
	do {
		int type = 0;
		std::vector<std::string> Info;
		COMMON::sepString(commands, "|", Info);
		int card = -1, card2 = -1, card3 = -1;
		if(Info.size() >= 4) {
			card3 = COMMON::convert<std::string, int>(Info[3]);
		}
		if(Info.size() >= 3) {
			card2 = COMMON::convert<std::string, int>(Info[2]);
		}
		if(Info.size() >= 2) {
			card = COMMON::convert<std::string, int>(Info[1]);
		}
		if(Info[0] == "th") {
			type |= 1;
		}
		else if(Info[0] == "chi") {
			type |= 2;
		}
		else if(Info[0] == "peng") {
			type |= 4;
		}
		else if(Info[0] == "gang") {
			type |= 8;
		}
		else if(Info[0] == "hu") {
			type |= 16;
		}
		else if(Info[0] == "dapai") {
			type |= 32;
		}
		else if(Info[0] == "guo") {
			type |= 64;
		}
		else if(Info[0] == "guaji") {
			type |= 128;
		}
		
		OnlinePlayers *onlinePlayer = OnlinePlayers::getOnlinePlayers();
		Table *table = onlinePlayer->GetTable(uid);
		table->PlayerOperator(uid, type, card, card2, card3, resp);
		if(table->m_tableStatus == 0 && table->m_remain == 0) {
			onlinePlayer->DestroyTable(table->m_id);
		}
		ret = 0;
	} while(0);
	
	/*std::tr1::unordered_map<std::string, std::string> playersOp;
	
	do {
		OnlinePlayers *onlinePlayer = OnlinePlayers::getOnlinePlayers();
		Table *table = onlinePlayer->GetTable(uid);
		table->toClient(playersOp);
		std::tr1::unordered_map<std::string, std::string>::iterator it = playersOp.begin();
		Resp res;
		for(; it != playersOp.end(); it ++) {
			res.m_uid = it->first;
			res.m_resp = it->second;
			resp.push_back(res);
		}
	}while(0);*/
	
	return 0;
}

int Game::Begin(const std::string &uid, const std::string &commands, std::vector<Resp> &resp) {
	OnlinePlayers *onlinePlayer = OnlinePlayers::getOnlinePlayers();
	Table *table = onlinePlayer->GetTable(uid);
	if(table == NULL) {
		LOG_ERROR("table id error:%s", uid.c_str());
		return -1;
	}
	if(table->BeginGame() == -1) {
		return -1;
	}
	Resp res;
	for(int i = 0; i < 4; i ++) {
		table->GetAllCards(i, res.m_uid, res.m_resp);
		resp.push_back(res);
	}

	int zhuang = table->m_zhuang;
	std::string zid = table->m_tablePlayers[zhuang].m_player->m_account;

	table->PlayerOperator(zid, MOPAI, 0, 0, 0, resp);

	return 0;
}

//房间模块
int Game::OutRoom(const std::string &uid, const std::string &commands, std::vector<Resp> &resp) {
	int ret = -1;
	int id;
	do {
		OnlinePlayers *onlinePlayer = OnlinePlayers::getOnlinePlayers();
		if(onlinePlayer->OutTable(uid, id) == -1) {
			break;
		}
		ret = 0;
	} while(0);

	Resp res;
	do {
		if(ret == -1) {
			break;
		}
		OnlinePlayers *onlinePlayer = OnlinePlayers::getOnlinePlayers();
		Table *table = onlinePlayer->GetTable(id);
		if(table == NULL) {
			ret = -1;
			break;
		}
		for(int i = 0; i < 4; i ++) {
			if(table->toResp(i, res.m_uid, res.m_resp) != -1) {
				resp.push_back(res);
			}
		}
		if(table->m_roomMan == 0) {
			if(GameConfig::debug > 1) {
				std::cerr << "DestroyTable:" << table->m_id << std::endl;
			}
			onlinePlayer->DestroyTable(id);
			id = -1;
		}
	} while(0);
	if(ret == -1) {
		res.m_resp = "outroom:no";
		res.m_uid = uid;
		resp.push_back(res);
	}
	return ret;
}

int Game::InRoom(const std::string &uid, const std::string &commands, std::vector<Resp> &resp) {
	//inroom:xxxxxx    xxxx是房间号码
	LOG_DEBUG("in InRoom");
	int ret = -1;
	int id;
	do {
		id = COMMON::convert<std::string, int>(commands);
		OnlinePlayers *onlinePlayer = OnlinePlayers::getOnlinePlayers();
		if(onlinePlayer->InTable(uid, id) == -1) {
			break;
		}
		ret = 0;
	} while(0);

	Resp res;
	do {
		if(ret == -1) {
			break;
		}
		OnlinePlayers *onlinePlayer = OnlinePlayers::getOnlinePlayers();
		Table *table = onlinePlayer->GetTable(id);
		if(table == NULL) {
			ret = -1;
			break;
		}
		res.m_resp = "inroom:yes";
		res.m_uid = uid;
		resp.push_back(res);
		for(int i = 0; i < 4; i ++) {
			if(table->toResp(i, res.m_uid, res.m_resp) != -1) {
				resp.push_back(res);
			}
		}
		//table->GetPlayersID(players);
		//std::vector<std::string> playerID;
		//table->GetPlayersAccount(playerID);
		//for(int i = 0; i < playerID.size(); i ++) {
		//	res.m_uid = playerID[i];
		//	res.m_resp = "inroom:"+COMMON::convert<int, std::string>(i)+"|"+COMMON::convert<int, std::string>(playerID.size())+"#+"+COMMON::convert<int, std::string>(id)+"#"+players;
		//	resp.push_back(res);
		//}
	} while(0);
	if(ret == -1) {
		res.m_resp = "inroom:no";
		res.m_uid = uid;
		resp.push_back(res);
	}
	return 0;
}

int Game::CreateTable(const std::string &uid, const std::string &commands, std::vector<Resp> &resp) {
	int ret = -1;
	std::cerr << "in CreateTable Game" << std::endl;
	int id;
	do {
		std::vector<std::string> Info;
		COMMON::sepString(commands, "|", Info);
		OnlinePlayers *onlinePlayer = OnlinePlayers::getOnlinePlayers();
		const Player *player = onlinePlayer->GetPlayer(uid);
		if(onlinePlayer->CreateTable(uid, id, COMMON::convert<std::string, int>(Info[0]), COMMON::convert<std::string, int>(Info[1])) == -1) {
			break;
		}
		ret = 0;
	} while(0);

	Resp res;
	do {
		if(ret == -1) {
			break;
		}

		OnlinePlayers *onlinePlayer = OnlinePlayers::getOnlinePlayers();
		Table *table = onlinePlayer->GetTable(id);
		if(table == NULL) {
			ret = -1;
			break;
		}
		res.m_resp = "createroom:yes|"+COMMON::convert<int, std::string>(id);
		res.m_uid = uid;
		resp.push_back(res);
		for(int i = 0; i < 4; i ++) {
			if(table->toResp(i, res.m_uid, res.m_resp) != -1) {
				resp.push_back(res);
			}
		}
	} while(0);
	if(ret == -1) {
		res.m_resp = "inroom:no";
		res.m_uid = uid;
		resp.push_back(res);
	}
	return ret;
}

int Game::doGame(std::string &uid, const std::string &commands, std::vector<Resp> &resp) {

	if(GameConfig::debug > 1) {
		std::cerr << "in DoGame uid = " << uid << " commands = " << commands << std::endl;

	}
	int pos = commands.find(":");
	if(pos == std::string::npos) {
		return -1;
	}
	std::string command = commands.substr(pos+1);
	std::string Opt = commands.substr(0, pos);
	if(Opt == "reg") {
		Game::Register(uid, command, resp);
	}
	else if(Opt == "login") {
		Game::Login(uid, command, resp);
	}
	else if(Opt == "logout") {
		Game::Logout(uid, command, resp);
	}
	else if(Opt == "inroom") {
		Game::InRoom(uid, command, resp);
	}
	else if(Opt == "createroom") {
		Game::CreateTable(uid, command, resp);
	}
	else if(Opt == "outroom") {
		Game::OutRoom(uid, command, resp);
	}
	else if(Opt == "startgame") {
		Game::Begin(uid, command, resp);	//开始游戏
	}
	else if(Opt == "game") {
		Game::PlayGame(uid, command, resp);	//玩游戏
	}
	else if(Opt == "again") {
		Game::Again(uid, command, resp);	//再来一局
	}
	/*else if(commands.find("end") != std::string::npos) {
		//Game::End();	//结束游戏
	}
	
	
	else if(commands.find("speek") != std::string::npos) {

	}*/
}

int Game::Again(const std::string &uid, const std::string &commands, std::vector<Resp> &resp) {
	int Ret = -1;
	do {
		OnlinePlayers *onlinePlayer = OnlinePlayers::getOnlinePlayers();
		Table *table = onlinePlayer->GetTable(uid);
		if(table == NULL) {
			break;
		}
		std::cerr << "IN Again" << std::endl;
		table->Again(uid);
		Resp res;
		for(int i = 0; i < 4; i ++) {
			if(table->toRespAgain(i, res.m_uid, res.m_resp) != -1) {
				resp.push_back(res);
			}
		}
		Ret = 0;
	} while(0);

	return Ret;
}

int Game::Register(const std::string &uid, const std::string &commands, std::vector<Resp> &resp) {
	//reg:account|xxxxx#passwd|yyyyyy    xxx是帐号 yyyy是密码
	if(GameConfig::debug > 1) {
		std::cerr << "in Register" << std::endl;
	}
	int ret = -1;
	do {
		std::vector<std::string> Info;
		COMMON::sepString(commands, "|", Info);
		if(Info.size() != 2) {
			break;
		}
		std::string account = Info[0];
		std::string passwd = Info[1];
		if(GameConfig::debug > 1) {
			std::cerr << "account = " << account << " passwd = " << passwd << std::endl;
		}
		OnlinePlayers *onlinePlayer = OnlinePlayers::getOnlinePlayers();
		if(onlinePlayer->Register(account, passwd) == -1) {
			break;
		}
		ret = 0;
	}while(0);
	Resp res;
	if(ret == 0) {
		res.m_resp = "reg:ok";
		res.m_uid = uid;
	}
	else {
		res.m_resp = "reg:no";
		res.m_uid = uid;
	}
	resp.push_back(res);
	return ret;
}

int Game::Login(std::string &uid, const std::string &commands, std::vector<Resp> &resp) {
	LOG_DEBUG("in Login");
	int ret = -1;
	Resp res;
	do {
		LOG_ALERT("将消息打印出来: %s,  %u", commands.c_str(), commands.size());
		std::vector<std::string> Info;
		COMMON::sepString(commands, "|", Info);
		if(Info.size() != 4) {
			LOG_ALERT("%s commands error", commands.c_str());
			break;
		}
		std::string account = Info[0];
		std::string name = Info[1];
		std::string sex = Info[2];
		std::string url = Info[3];
		std::string passwd = "1";
		OnlinePlayers *onlinePlayer = OnlinePlayers::getOnlinePlayers();
		if(GameConfig::debug > 1) {
			std::cerr << "account = " << account << " passwd = " << passwd << " name = " << name << std::endl;
		}
		if(onlinePlayer->Login(account, passwd, name, sex, url) == -1) {
			break;
		}
		const Player *player = onlinePlayer->GetPlayer(account);
		ret = 0;
		uid = account;
		res.m_resp = "login:yes|"+COMMON::convert<int, std::string>(player->GetRoomCard());
	} while(0);
	
	if(ret == -1) {
		res.m_resp = "login:no";
	}
	res.m_uid = uid;
	resp.push_back(res);
	return ret;
}

int Game::Logout(const std::string &uid, const std::string &commands, std::vector<Resp> &resp) {
	int ret = -1;
	int id = -1;
	do {
		OnlinePlayers *onlinePlayer = OnlinePlayers::getOnlinePlayers();
		if(onlinePlayer->Logout(uid, id) == -1) {
			break;
		}
		ret = 0;
	} while(0);

	Resp res;
	do {
		if(ret == -1) {
			break;
		}
		OnlinePlayers *onlinePlayer = OnlinePlayers::getOnlinePlayers();
		Table *table = onlinePlayer->GetTable(id);
		if(table == NULL) {
			ret = -1;
			break;
		}
		for(int i = 0; i < 4; i ++) {
			if(table->toResp(i, res.m_uid, res.m_resp) != -1) {
				resp.push_back(res);
			}
		}
		if(table->m_roomMan == 0) {
			if(GameConfig::debug > 1) {
				std::cerr << "DestroyTable:" << table->m_id << std::endl;
			}
			onlinePlayer->DestroyTable(id);
			id = -1;
		}
	} while(0);
	if(ret == -1) {
		res.m_resp = "outroom:no";
		res.m_uid = uid;
		resp.push_back(res);
	}

	return ret;
}
