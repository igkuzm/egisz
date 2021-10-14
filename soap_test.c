/**
 * File              : soap_test.c
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 14.10.2021
 * Last Modified Date: 14.10.2021
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */
#include "soap_test.h"
#include <egisz_ssl_connector.h>

#define HOST " https://nsi.rosminzdrav.ru"
#define PORT 443

int soap_test(){

	egisz_ssl_connector_socket(HOST, PORT, "", "GET /wsdl/SOAP-server.v2.php?wsdl", int argc, char **argv, void *data, int (*callback)(char *, int, int *, void *))

	return 0;
}
