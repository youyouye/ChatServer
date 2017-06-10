/*
 * Microhttpd.h
 *
 *  Created on: Jun 6, 2017
 *      Author: moocos
 */

#ifndef MICROHTTPD_H_
#define MICROHTTPD_H_
#include <microhttpd.h>

namespace micro
{

class MicroHttp
{
public:
	MicroHttp(const int port);
	~MicroHttp();
private:
	struct Session{
		struct Session* next;
		char sid[33];
		unsigned int rc; //reference counter
		time_t start;
		char value_1[64];
		char value_2[64];
	};

	struct Regis{
		char account[20];
		char password[20];
	};

	union Post_type{
		Regis regis;
		Regis login;
	};

	struct Request{
		struct Session* session;
		struct MHD_PostProcessor* pp;
		Post_type post;
		int type = 0; //0为无.1为注册;2为登陆.
	};

	static int create_response(void *cls,
			 struct MHD_Connection *connection,
			 const char *url,
			 const char *method,
			 const char *version,
			 const char *upload_data,
			 size_t *upload_data_size,
			 void **ptr);
	static int post_iterator(void *cls,
		       enum MHD_ValueKind kind,
		       const char *key,
		       const char *filename,
		       const char *content_type,
		       const char *transfer_encoding,
		       const char *data, uint64_t off, size_t size);
	static void request_completed_callback(void *cls,
		    struct MHD_Connection *connection,
		    void **con_cls,
		    enum MHD_RequestTerminationCode toe);
	static Session* get_session(struct MHD_Connection* connection);
	static int deal_post(MHD_Connection* conn,Request* req);

	struct MHD_Daemon *daemon ;

	//Test
	static struct Session* sessions;
};


}

#endif /* MICROHTTPD_H_ */
