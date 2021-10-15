/**
 * File              : http_connector.c
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 15.10.2021
 * Last Modified Date: 15.10.2021
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */
int egisz_ssl_connector_socket(const char *hostname, int port, const char *apikey, const char *http_method, int argc, char *argv[], void *data, int (*callback)(char*,int,int*,void*)){

	//init SSL
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    const SSL_METHOD *method = SSLv23_client_method();
    SSL_CTX *ctx = SSL_CTX_new(method);
    if ( ctx == NULL ){
		fprintf(stderr, "Error. Can't init SSL_CTX\n");	
		return -1;
	} 
 
    struct hostent *host;
    struct sockaddr_in addr;
 
    if ( (host = gethostbyname(hostname)) == NULL ){
		fprintf(stderr, "Error. Can't get host ip address with hostname: %s\n", hostname);	
		return -1;
	} 
    int sd = socket(PF_INET, SOCK_STREAM, 0); //init socket
    //bzero(&addr, sizeof(addr));
	memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = *(long*)(host->h_addr);
    if ( connect(sd, (struct sockaddr*)&addr, sizeof addr)){ //connect socket
		fprintf(stderr, "Error. Can't connect to socket with address: %s:%d\n", host->h_addr, port);	
		return -1;
	} 
    
    SSL *ssl = SSL_new(ctx); //create ssl structure
    SSL_set_fd(ssl, sd); //connect SSL to socket 
    if ( SSL_connect(ssl) == -1 ){
		fprintf(stderr, "Error. Can't connect SSL to socket with address: %s:%d\n", host->h_addr, port);	
		return -1;
	}  

	//generage HTTP REQUEST MESSAGE
	char *write_buf = http_msg_with_args(hostname, http_method, argc, argv);
	if (write_buf == NULL){
		char argv_string[BUFSIZ];
		int i;
		for (i = 0; i < argc; ++i) {
			strcat(argv_string, argv[i]);	
			strcat(argv_string, " ");	
		}
		fprintf(stderr, "Error. Can't merge http request for host: %s with http method: %s, arguments: %s \n", hostname, http_method, argv_string);	
		return -1;		
	}

	//SSL WRITE
	int retval;
	retval = SSL_write(ssl, write_buf, strlen(write_buf));

	free(write_buf); //no need any more

	if (retval <= 0 ){ //handle with error
		handle_with_ssl_error(ssl, retval);
		fprintf(stderr, "Error while SSL_write\n");
		return retval;			
	}

    //SSL READ
	int count = 0;
    char buf[1024];
	long bytes;
    while ((bytes = SSL_read(ssl, buf, sizeof buf)) >0 ) {
		buf[bytes] = 0;
		
		//printf("%s", buf); //print for debug
		
		if (callback) {
			int c = callback(buf, bytes, &count, data); //run callback
			if (c != 0) { //stop function if callback returned non zero
				fprintf(stderr, "Stop SSL_read - callback returned: %d\n", c);
				break;
			}
			count++; //we need count to know how many times callback was called
		}
    }
	if (bytes < 0 ){ //hendle with error
		handle_with_ssl_error(ssl, bytes);
		fprintf(stderr, "Error while SSL_read\n");
		return retval;			
	}	

	//Close SSL
    SSL_free(ssl);   
    close(sd);      
    SSL_CTX_free(ctx);   
	

	return 0;
}

