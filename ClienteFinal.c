#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<pthread.h> //for threading , link with lpthread
 
int main(int argc , char *argv[]){
    system("clear");
    int socket_desc;
    struct sockaddr_in server;
    char *message , server_reply[2000];
     
    //Crea el socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1){
        printf("Fallo al crear el socket");
    }
    
    //Rellenar la estructura     
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 8888 );
 
    //Conexión con el servidor
    if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0){
        puts("Error de Conexión");
        return 1;
    }
     
    puts("Conexion exitosa\n");
    char *elec[1];

    //Recibe el segundo mensaje, necesario antes del while
    if(recv(socket_desc, server_reply , 2000 , 0) < 0){
        puts("Error en el recv");
    }
    puts("Mensaje del servidor: ");
    puts(server_reply);


    while( recv(socket_desc, server_reply , 2000 , 0) > 0){
        puts("Mensaje del servidor: \n");
        puts(server_reply);
        scanf("%s",elec);
        if( send(socket_desc , elec , strlen(elec) , 0) < 0){
            puts("Envio fallido");
            return 1;
        }
        puts("Data Send\n");
        fflush(stdin);
    }
    return 0;
}

//Send some data
    /*message = "GET / HTTP/1.1\r\n\r\n";
    if( send(socket_desc , message , strlen(message) , 0) < 0){
        puts("Send failed");
        return 1;
    }
    puts("Data Send\n");
     */
    //Receive a reply from the server