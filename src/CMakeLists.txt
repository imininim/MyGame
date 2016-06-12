INCLUDE_DIRECTORIES(/home/yangshu/test_cmake/src)
link_directories(/usr/lib64/mysql)
add_definitions("-std=c++11")
#include_directories(/usr/lib64/mysql)
SET(SRC_LIST main.cpp
	work/add.cpp
	work/sub.hpp
	work/MySQLManager.cpp
	json/json_value.cpp
	json/json_writer.cpp
	json/json_reader.cpp
	common/myalgorithm.cpp
	common/mycommonfuc.cpp
	common/mymatrix.cpp
	common/myqueue.hpp
	common/mythreadpool.cpp
	common/socketServer.cpp
	common/sockLinker.cpp
	common/define.h
	common/myMempool.cpp
	common/sockClient.cpp
	)

ADD_EXECUTABLE(test ${SRC_LIST})

#include_directories(/usr/lib64/mysql)
TARGET_LINK_LIBRARIES(test pthread)
TARGET_LINK_LIBRARIES(test mysqlclient)

