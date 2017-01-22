// tls_session_reuse.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <WinSock2.h>
#include <stdio.h>
#include <string.h>
//#include <unistd.h>
//#include <arpa/inet.h>
//#include <sys/socket.h>

#include <openssl/ssl.h>
#include <openssl/err.h>

int main(int argc, char **argv)
{
   WSADATA wsadata;
   memset(&wsadata, 0, sizeof(wsadata));
   WSAStartup(0x101, &wsadata);
   SSL                *ssl = NULL;
   SSL_CTX            *ctx = NULL;
   SSL_SESSION        *session = NULL;
   int                 clntfd = -1;
   struct sockaddr_in  clnt;
   size_t              i;

   /* Initialize OpenSSL. */
   //SSL_library_init();
   //SSL_load_error_strings();
   //OpenSSL_add_all_algorithms();

   /* Generate an SSL client context. */
   ctx = SSL_CTX_new(TLS_client_method());
   if (ctx == NULL) {
      printf("Failed to create SSL client context\n");
      goto cleanup;
   }

   /* Set some options and the session id.
   * SSL_OP_NO_SSLv2: SSLv2 is insecure, disable it.
   */
   //SSL_CTX_set_options(ctx, SSL_OP_NO_SSLv2);

   /* We're going to connect and disconnect multiple times. */
   for (i = 0; i<5; i++) {
      /* Setup the socket to connect to localhost. */
      memset(&clnt, 0, sizeof(clnt));
      clnt.sin_family = AF_INET;
      //clnt.sin_addr.s_addr = htonl(0xa7721101);
      //clnt.sin_addr.s_addr = htonl(0xa7721101);
      clnt.sin_addr.S_un.S_un_b.s_b1 = 167;
      clnt.sin_addr.S_un.S_un_b.s_b2 = 114;
      clnt.sin_addr.S_un.S_un_b.s_b3 = 17;
      clnt.sin_addr.S_un.S_un_b.s_b4 = 1;

      clnt.sin_port = htons(443);
      clntfd = socket(AF_INET, SOCK_STREAM, 0);
      if (clntfd == -1) {
         printf("Failed to create socket\n");
         return 0;
      }
      if (connect(clntfd, (struct sockaddr *)&clnt, sizeof(struct sockaddr)) != 0) {
         printf("Failed to connect\n");
         goto cleanup;
      }

      /* Create an ssl object and bind it to our socket. */
      ssl = SSL_new(ctx);
      SSL_set_fd(ssl, clntfd);

      /* If we have a stored session set it as the ssl's session so it will be reused. */
      if (session != NULL) {
         SSL_set_session(ssl, session);
      }

      /* Connect via SSL. */
      if (SSL_connect(ssl) < 1) {
         printf("Failed to connect (SSL)\n");
         goto cleanup;
      }

      /* Was the stored session reused? */
      if (SSL_session_reused(ssl)) {
         printf("REUSED SESSION\n");
      }
      else {
         printf("NEW SESSION\n");
      }

      /* Save client session. Sever session is saves as part of shutdown.
      * The saved session is destroyed and stored from the ssl object
      * before shutdown in case a renegotation took place and a new
      * session was created. */
      if (session != NULL)
         SSL_SESSION_free(session);
      session = SSL_get1_session(ssl);

      /* Disconnect. */
      SSL_shutdown(ssl);
      SSL_free(ssl);
      ssl = NULL;
      closesocket(clntfd);
      clntfd = -1;
   }

cleanup:
   if (session != NULL)
      SSL_SESSION_free(session);
   if (ssl != NULL)
      SSL_free(ssl);
   if (clntfd != -1)
      closesocket(clntfd);
   if (ctx != NULL)
      SSL_CTX_free(ctx);
   return 0;
}