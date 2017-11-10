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
    int read_size;

    //Recibe el segundo mensaje, necesario antes del while
    if(recv(socket_desc, server_reply , 2000 , 0) < 0){
        puts("Error en el recv");
    }
    puts("Mensaje del servidor: ");
    puts(server_reply);
 

    while( read_size=recv(socket_desc, server_reply , 2000 , 0) > 0){
        puts("Mensaje del servidor: \n");
        puts(server_reply);
        fflush(stdout);
        fflush(stdin);
        scanf("%s",elec);
        if(atoi(elec)==4){
            puts("La conexion finalizara en breve, te esperamos de vuelta pronto");
            close(socket_desc);
            return 0;
        }
        if(atoi(elec)!=3){
            if((send(socket_desc , elec , strlen(elec) , 0) < 0)){
                puts("Envio fallido");
                return 1;
            }   
        }
        if(atoi(elec)==3){
            printf("Ingresa el mensaje a guardar: \n");
            if((send(socket_desc , elec , strlen(elec) , 0) < 0)){
                puts("Envio fallido");
                return 1;
            } 
        }
        puts("Data send");
        fflush(stdin);
    }
    if(read_size == 0){
        puts("El servidor se ha desconectado, intente de nuevo mas tarde");
        fflush(stdout);
    }
    return 0;
}