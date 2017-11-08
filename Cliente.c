#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<stdlib.h>
 
int main(int argc , char *argv[]){
    int socket_desc;
    struct sockaddr_in server;
    char *message1 , server_reply[2000];
    char *message2;
    char *message3;
     
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
     
    puts("¡Conexion exitosa!\n");
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
    message1="1";
    message2="2";
    message3="3";
    while(1){
    	puts("Escribe tu eleccion");
    	scanf("%s", elcMenu);
    	if(atoi(elcMenu)==1){
    		printf("Eleccion 1, los datos vienen en camino\n");
    		if( send(socket_desc , message1 , strlen(message1) , 0) < 0){
        		puts("Fallo al envíar el mensaje");
       			//return 0;
    		}
            /*while(recv(socket_desc, server_reply , 2000 , 0)<0){
                puts(server_reply);
            }*/
    		//close(socket_desc);
    	}
        else if(atoi(elcMenu)==3){
            puts("Eleccion 3, ¡Prepara tu mensaje!");
            if( send(socket_desc , message3 , strlen(message3) , 0) < 0){
                puts("Fallo al enviar el mensaje");
                //return 0;
            }
        }
    	else if(atoi(elcMenu)==2){
    		puts("Eleccion 2, ¡Prepara tu comentario!");
    		if( send(socket_desc , message2 , strlen(message2) , 0) < 0){
        		puts("Fallo al enviar el mensaje");
       			//return 0;
    		}
    	}
        
    	else if(atoi(elcMenu)==4){
    		puts("La conexion finalizar en breve, te esperamos de vuelta pronto");
    		close(socket_desc);
    		return 0;
    	}
    	else if ((atoi(elcMenu) != 3) && (atoi(elcMenu) != 2 ) && (atoi(elcMenu) != 1)  && (atoi(elcMenu) != 4)){
    		puts("Eleccion invalida, ingresa un valor del menu");
    	}

    	/*if( recv(socket_desc, server_reply , 2000 , 0) < 0){
        	puts("Fallo en el recv o no se recibió ningún mensaje del server");
    	}*/
    	//Receive a reply from the server
    	//puts("Mensaje del server: ");
    	//puts(server_reply);
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