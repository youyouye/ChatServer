include_directories(/usr/im/workspace/include)
include_directories(/usr/im/Downloads/build/release-install/include)
include_directories(/usr/im/Downloads/mysql-connector-c-6.1.10-linux-glibc2.12-x86_64/include)
include_directories(/usr/local/include)
include_directories($(PROJECT_SOURCE_DIR))
link_directories(/usr/im/Downloads/build/release-install/lib)
link_directories(/usr/im/Downloads/mysql-connector-c-6.1.10-linux-glibc2.12-x86_64/lib)
link_directories(/usr/local/lib)

set(MY_LIB_FILES
	mysql_conn.cpp
	mysql_conn_poll.cpp
	redis_conn.cpp
	offline_msg.cpp
	online_user.cpp		
)
ADD_LIBRARY(database ${MY_LIB_FILES})
target_link_libraries(database muduo_net)
target_link_libraries(database mysqlclient hiredis)
install(TARGETS database DESTINATION lib)
