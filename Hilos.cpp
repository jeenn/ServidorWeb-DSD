#include <pthread.h>
#include <stdio.h>
#include <iostream>
#include <stdio.h>
#include <curl/curl.h>
#include <unistd.h>
#include <string.h>
#include "mongoose.h"


void *funcion1(void *);
void *funcion2(void *);


size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}





static const char *s_http_port = "8000";
static struct mg_serve_http_opts s_http_server_opts;

static void ev_handler(struct mg_connection *nc, int ev, void *p) {
    if (ev == MG_EV_HTTP_REQUEST) {
        mg_serve_http(nc, (struct http_message *) p, s_http_server_opts);
    }
}



using namespace std;

int i = 0;
void *funcion1(void *){
	struct mg_mgr mgr;
    struct mg_connection *nc;
    mg_mgr_init(&mgr, NULL);
    printf("Starting web server on port %s\n", s_http_port);
    nc = mg_bind(&mgr, s_http_port, ev_handler);
    if (nc == NULL) {
        printf("Failed to create listener\n");
        exit(0);
    }
    // Set up HTTP server parameters
    mg_set_protocol_http_websocket(nc);
    s_http_server_opts.document_root = "."; // Serve current directory
    s_http_server_opts.enable_directory_listing = "yes";
    for (;;) {
        mg_mgr_poll(&mgr, 1000);
    }
    mg_mgr_free(&mgr);
	//sleep(2);
	pthread_exit(0);
}

void *funcion2(void *)
{	
	while(1){
	    CURL *curl;
	    FILE *fp;
	    CURLcode res;
	    char *url = "http://localhost:8000/votos";
	    char outfilename[FILENAME_MAX] = "DESCARGADO";
	    curl = curl_easy_init();
	    if (curl) {
	        fp = fopen(outfilename,"wb");
	        curl_easy_setopt(curl, CURLOPT_URL, url);
	        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
	        res = curl_easy_perform(curl);
	        /* always cleanup */
	        curl_easy_cleanup(curl);
	        fclose(fp);
	    }

		sleep(60);
	}
	pthread_exit(0);
}


int main(void)
{
	int aux  = 20;
	pthread_t th1, th2;
	while(aux >  0)
	{	
	pthread_create(&th1, NULL,  &funcion1,  &i);
	pthread_create(&th2, NULL,  &funcion2,  &i);	
	pthread_join(th1, NULL);
	pthread_join(th2, NULL);
	aux --;
	}	

	return 0;	
}
