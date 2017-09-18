include_directories(/usr/im/workspace/include)
include_directories(/usr/im/Downloads/build/release-install/include)
include_directories(/usr/im/Downloads/mysql-connector-c-6.1.10-linux-glibc2.12-x86_64/include)
include_directories($(PROJECT_SOURCE_DIR))
link_directories(/usr/im/Downloads/build/release-install/lib)
link_directories(/usr/im/Downloads/mysql-connector-c-6.1.10-linux-glibc2.12-x86_64/lib)

set(MY_LIB_FILES
	JsonUtil.cpp
	Microhttp.cpp	
)
ADD_LIBRARY(myhttp ${MY_LIB_FILES})
target_link_libraries(myhttp microhttpd muduo_net)
install(TARGETS myhttp DESTINATION lib)
