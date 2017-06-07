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
	struct MHD_Daemon *daemon ;
};


}

#endif /* MICROHTTPD_H_ */
