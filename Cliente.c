#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
 
int main(int argc , char *argv[]){
    int socket_desc;
    struct sockaddr_in server;
    char *message , server_reply[2000];
     
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1){
        printf("No fue posible crear el socket\n");
    }
         
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 8888 );
 
    //Connect to remote server
    if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0){
        puts("Error de conexión \n");
        return 1;
    }
     
    puts("¡Conexión exitosa!\n");
    if( recv(socket_desc, server_reply , 2000 , 0) < 0){
        puts("Fallo en el recv");
    }
    //Receive a reply from the server
    puts("Mensaje del server: ");
    puts(server_reply);
    if( recv(socket_desc, server_reply , 2000 , 0) < 0){
        puts("Fallo en el recv");
    }
    //Receive a reply from the server
    puts("Mensaje del server: ");
    puts(server_reply);

    char *elcMenu;
    *message="1";
    while(1){
    	puts("Escribe tu elección");
    	scanf("%s", elcMenu);
    	if(atoi(elcMenu)==1){
    		printf("Elección 1, los datos vienen en camino\n");
    		if( send(socket_desc , message , strlen(message) , 0) < 0){
        		puts("Fallo al envíar el mensaje");
       			//return 0;
    		}
    		//close(socket_desc);
    	}
    	if(atoi(elcMenu)==2){
    		puts("Elección 2, ¡Prepara tu mensaje!");
    	}
    	if(atoi(elcMenu)==3){
    		puts("La conexión finalizará en breve, te esperamos de vuelta pronto");
    		close(socket_desc);
    		return 0;
    	}
    	else if ((atoi(elcMenu) != 3) && (atoi(elcMenu) != 2 ) && (atoi(elcMenu) != 1)){
    		puts("Elección inválida, ingresa un valor del menú");
    	}
    }
    //Send some data
    /*message = "GET / HTTP/1.1\n";
    if( send(socket_desc , message , strlen(message) , 0) < 0){
        puts("Send failed");
        return 1;
    }
    puts("Data Send\n");*/
    close(socket_desc);
    return 0;
}