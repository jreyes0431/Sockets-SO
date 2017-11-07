#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
 
#include<pthread.h> //for threading , link with lpthread
 
void *connection_handler(void *);
 
int main(int argc , char *argv[]){
    int socket_desc , new_socket , c , *new_sock;
    struct sockaddr_in server , client;
    char *message;
     
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1){
        printf("No fue posible crear el socket");
    }
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0){
        puts("bind fallido");
        return 1;
    }
    puts("bind exitoso");
     
    //Listen
    listen(socket_desc , 3);
     
    //Accept and incoming connection
    puts("Esperando por las conexiones entrantes...");
    c = sizeof(struct sockaddr_in);
    while( (new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) ){
        puts("¡Conexión aceptada!");
         
        //Reply to the client
        message = "Hola, la conexión fue exitosa, a partir de ahora puedes utilizar los servicios disponibles. Serás asignado con un operador.\n";
        write(new_socket , message , strlen(message));
         
        pthread_t sniffer_thread;
        new_sock = malloc(1);
        *new_sock = new_socket;
         
        if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0){
            perror("No fue posible crear el hilo");
            return 1;
        }     
        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( sniffer_thread , NULL);
        puts("Operador asignado");
    }
     
    if (new_socket<0){
        perror("Fallo en el accept");
        return 1;
    }   
    return 0;
}
/*
 * This will handle connection for each client
 * */
void *connection_handler(void *socket_desc){
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
    int read_size;
    char *message , client_message[2000];
     
    //Send some messages to the client
    message = "Hola, soy el operador, te diré el menú\n";
    write(sock , message , strlen(message));
     
    message = "Ingresa: \n 1-Datos metereológicos de hoy\n 2-Reproducir un mensaje en el altavoz\n 3-Terminar la conexión";
    write(sock , message , strlen(message));
     
    //Receive a message from client
    while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 ){
        //Send the message back to client
        write(sock , client_message , strlen(client_message));
    }
     
    if(read_size == 0){
        puts("El cliente se ha desconectado");
        fflush(stdout);
    }
    else if(read_size == -1){
        perror("Fallo en el recv");
    }     
    //Free the socket pointer
    free(socket_desc);
    return 0;
}