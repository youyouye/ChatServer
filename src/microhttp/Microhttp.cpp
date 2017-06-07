#include "Microhttpd.h"
#include <muduo/base/Logging.h>
#include <muduo/net/Channel.h>
#include <muduo/net/EventLoop.h>
#include <microhttpd.h>
#include <boost/bind.hpp>

using namespace micro;
using namespace muduo;
using namespace muduo::net;
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
		 void **ptr)
{
	const char *page = "<html><body>Hello, browser!</body></html>";
	  struct MHD_Response *response;
	  int ret;

	  response =
	    MHD_create_response_from_buffer (strlen (page), (void *) page,
					     MHD_RESPMEM_PERSISTENT);
	  ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
	  MHD_destroy_response (response);

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

}
void MicroHttp::request_completed_callback(void *cls,
	    struct MHD_Connection *connection,
	    void **con_cls,
	    enum MHD_RequestTerminationCode toe)
{
	LOG_INFO<<"request_completed!";

}

