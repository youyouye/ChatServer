include_directories(/usr/im/workspace/include)
include_directories(/usr/im/Downloads/build/release-install/include)
include_directories(/usr/im/Downloads/mysql-connector-c-6.1.10-linux-glibc2.12-x86_64/include)
include_directories($(PROJECT_SOURCE_DIR))
link_directories(/usr/im/Downloads/build/release-install/lib)
link_directories(/usr/im/Downloads/mysql-connector-c-6.1.10-linux-glibc2.12-x86_64/lib)

add_executable(chat_server
	chat.pb.cc
	group.pb.cc	
	ChatServer.cpp
	GroupServer.cpp
	codec.cpp
	Start.cpp	
)
target_link_libraries( chat_server
inspector myhttp database protobuf jsoncpp microhttpd mysqlclient muduo_http muduo_inspect muduo_net muduo_base pthread dl z
)
install(TARGETS chat_server DESTINATION bin)
