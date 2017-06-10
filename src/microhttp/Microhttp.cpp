#include "Microhttpd.h"
#include <muduo/base/Logging.h>
#include <muduo/net/Channel.h>
#include <muduo/net/EventLoop.h>
#include <microhttpd.h>
#include <boost/bind.hpp>
#include <string>
#include <stdio.h>
#include "../database/mysql_conn_pool.h"

using namespace micro;
using namespace muduo;
using namespace muduo::net;

const char *loginpage = "<html><body> How fuck!!! </body></html>";
const char *errorpage = "<html><body>This is a error!</body></html>";
const char *okpage = "<html><body>OK!</body></html>";

MicroHttp::Session* MicroHttp::sessions = NULL;

MicroHttp::MicroHttp(const int port)
{
	daemon =MHD_start_daemon(MHD_USE_DEBUG|MHD_USE_SELECT_INTERNALLY,port,
			NULL,NULL,
			&MicroHttp::create_response,NULL,
			MHD_OPTION_END
			);
}
MicroHttp::~MicroHttp()
{
}


int MicroHttp::create_response(void *cls,
		 struct MHD_Connection *connection,
		 const char *url,
		 const char *method,
		 const char *version,
		 const char *upload_data,
		 size_t *upload_data_size,
		 void **con_cls)
{
	LOG_INFO<<"create_response!";
	struct MHD_Response* response;
	struct Request* request;
	struct Session* session;
	int ret;
	request = *con_cls;
	if (NULL == request){
		request = calloc(1,sizeof(struct Request));
		if ( NULL == request){
			LOG_ERROR<<"calloc error!";
			return MHD_NO;
		}
		*con_cls = request;
		if (0 == strcmp(method,MHD_HTTP_METHOD_POST)){
			request->pp = MHD_create_post_processor(connection,512,&MicroHttp::post_iterator,request);
			if (NULL == request->pp){
				LOG_ERROR<<"setup post processer failed!";
				return MHD_NO;
			}
		}
		return MHD_YES;
	}

	if (NULL == request->session){
		request->session = get_session(connection);
		if (NULL == request->session){
			LOG_ERROR<<"Failed to set session!";
			return MHD_NO;
		}
	}
	session = request->session;
	session->start = time(NULL);
	if (0 == strcmp(method,MHD_HTTP_METHOD_POST)){
		MHD_post_process(request->pp,upload_data,*upload_data_size);
		if (0 != *upload_data_size){
			*upload_data_size = 0;
			return MHD_YES;
		}else{
			return deal_post(connection,request);
		}
	}
	if (0 == strcmp(method,MHD_HTTP_METHOD_GET)){
		struct MHD_Response* response;
		response = MHD_create_response_from_buffer(strlen(loginpage),(void*)loginpage,MHD_RESPMEM_PERSISTENT);
		if (!response){
			return MHD_NO;
		}
		ret = MHD_queue_response(connection,MHD_HTTP_OK,response);
		MHD_destroy_response(response);
		return ret;
	}
	response = MHD_create_response_from_buffer(strlen(errorpage),(void*)errorpage,MHD_RESPMEM_PERSISTENT);
	ret = MHD_queue_response(connection,MHD_HTTP_OK,response);
	MHD_destroy_response(response);
	return ret;
}
int MicroHttp::post_iterator(void *cls,
	       enum MHD_ValueKind kind,
	       const char *key,
	       const char *filename,
	       const char *content_type,
	       const char *transfer_encoding,
	       const char *data, uint64_t off, size_t size)
{
	LOG_INFO<<"post_iterator";
	struct Request* request = cls;
	struct Session* session = request->session;
	if (0 == strcmp("account",key)){
		memcpy(request->post.regis.account,data,size);
		request->type = 1;
		return MHD_YES;
	}
	if (0 == strcmp("password",key)){
		memcpy(request->post.regis.password,data,size);
		request->type = 1;
		return MHD_YES;
	}
	if (0 == strcmp("loginaccount",key)){
		memcpy(request->post.login.account,data,size);
		request->type = 2;
		return MHD_YES;
	}
	if (0 == strcmp("loginpassword",key)){
		memcpy(request->post.login.password,data,size);
		request->type = 2;
		return MHD_YES;
	}
	return MHD_NO;
}
void MicroHttp::request_completed_callback(void *cls,
	    struct MHD_Connection *connection,
	    void **con_cls,
	    enum MHD_RequestTerminationCode toe)
{
	LOG_INFO<<"request_completed!";
	struct Request *request = *con_cls;
	if(NULL == request)
		return;
	if (NULL != request->session)
		request->session->rc--;
	if (NULL != request->pp)
		MHD_destroy_post_processor(request->pp);
	free(request);
}

MicroHttp::Session* MicroHttp::get_session(struct MHD_Connection* connection)
{
	LOG_INFO<<"get_session!";
	Session* ret;
	const char *cookie;
	cookie = MHD_lookup_connection_value(connection,MHD_COOKIE_KIND,"session");
	if (cookie != NULL){
		ret = sessions;
		while (NULL != ret){
			if (0 == strcmp(cookie,ret->sid))
				break;
			ret = ret->next;
		}
		if (NULL != ret){
			ret->rc++;
			return ret;
		}
	}
	ret = calloc(1,sizeof(struct Session));
	if (NULL == ret){
		LOG_ERROR<<"calloc error";
		return NULL;
	}
	snprintf(ret->sid,sizeof(ret->sid),"%X%X%X%X",
			(unsigned int)rand(),
			(unsigned int)rand(),
			(unsigned int)rand(),
			(unsigned int)rand());
	ret->rc++;
	ret->start = time(NULL);
	ret->next = sessions;
	sessions = ret;
	return ret;
}
int MicroHttp::deal_post(MHD_Connection* conn,Request* req)
{
	LOG_INFO<<"deal_post!";
	struct MHD_Response* response;
	int type = req->type;
	if (type == 0){
		return MHD_NO;
	}else if (type == 1){
		Regis regis = req->post.regis;
		std::string account = regis.account;
		std::string password = regis.password;
		int res = MysqlConnPool::instance()->getNextConn()->regis(account,password);
		if (res > 0){
			response = MHD_create_response_from_buffer(strlen(okpage),(void*)okpage,MHD_RESPMEM_PERSISTENT);
			return MHD_queue_response(conn,MHD_HTTP_OK,response);
		}else{
			response = MHD_create_response_from_buffer(strlen(errorpage),(void*)errorpage,MHD_RESPMEM_PERSISTENT);
			return MHD_queue_response(conn,MHD_HTTP_OK,response);
		}
	}else if (type == 2){
		std::string loginaccount = req->post.login.account;
		std::string loginpassword = req->post.login.password;
		int res = MysqlConnPool::instance()->getNextConn()->login(loginaccount,loginpassword);
		if (res > 0){
			response = MHD_create_response_from_buffer(strlen(okpage),(void*)okpage,MHD_RESPMEM_PERSISTENT);
			return MHD_queue_response(conn,MHD_HTTP_OK,response);
		}else{
			response = MHD_create_response_from_buffer(strlen(errorpage),(void*)errorpage,MHD_RESPMEM_PERSISTENT);
			return MHD_queue_response(conn,MHD_HTTP_OK,response);
		}
	}
	return MHD_NO;
}
