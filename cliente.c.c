                    /*
                       CLIENTE
                   */

                   #include <stdio.h>
                   #include <stdlib.h>
                   #include <string.h>
                   #include <winsock.h>

               #define BUFFER_SIZE 128
               #define EXIT_CALL_STRING "#quit"

               int remote_socket = 0;
               int message_length = 0;

               unsigned short remote_port = 0;

               char remote_ip[32];
               char message[BUFFER_SIZE];

               struct sockaddr_in remote_address;

               WSADATA wsa_data;

               /* Exibe uma mensagem de erro e termina o programa */
               void msg_err_exit(char *msg)
               {
                   fprintf(stderr, msg);
                   system("PAUSE");
                   exit(EXIT_FAILURE);
               }

               int main(int argc, char **argv)
               {
                   if (WSAStartup(MAKEWORD(2, 0), &wsa_data) != 0)
                       msg_err_exit("WSAStartup() failed\n");

                   printf("IP do servidor: ");
                   scanf("%s", remote_ip);
                   fflush(stdin);

                   printf("Porta do servidor: ");
                   scanf("%d", &remote_port);
                   fflush(stdin);

                   remote_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
                   if (remote_socket == INVALID_SOCKET)
                   {
                       WSACleanup();
                       msg_err_exit("socket() failed\n");
                   }

                   // preenchendo o remote_address (servidor)
                   memset(&remote_address, 0, sizeof(remote_address));
                   remote_address.sin_family = AF_INET;
                   remote_address.sin_addr.s_addr = inet_addr(remote_ip);
                   remote_address.sin_port = htons(remote_port);

                   printf("conectando ao servidor %s...\n", remote_ip);
                   if (connect(remote_socket, (struct sockaddr *) &remote_address, sizeof(remote_address)) == SOCKET_ERROR)
                   {
                       WSACleanup();
                       msg_err_exit("connect() failed\n");
                   }

                   printf("digite as mensagens\n");
                   do
                   {
                       // limpa o buffer
                       memset(&message, 0, BUFFER_SIZE);

                       printf("msg: ");
                       gets(message);
                       fflush(stdin);

                       message_length = strlen(message);

                       // envia a mensagem para o servidor
                       if (send(remote_socket, message, message_length, 0) == SOCKET_ERROR)
                       {
                           WSACleanup();
                           closesocket(remote_socket);
                           msg_err_exit("send() failed\n");
                       }
                   }
                   while(strcmp(message, EXIT_CALL_STRING)); // sai quando enviar um "#quit" para o servidor

                   printf("encerrando\n");
                   WSACleanup();
                  closesocket(remote_socket);

                   system("PAUSE");
                   return 0;
               }
