#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<pthread.h> //for threading , link with lpthread

pthread_mutex_t mutex;
FILE *climadata;
char direccion[];

pthread_mutex_lock(mutex);
    char direccion[] = "DatosMDia.txt"; 
pthread_mutex_unlock(mutex); 


void *connection_handler(void *);
void lectorMetereologico();
void escribeMeterologico(char escritura);
int main(int argc , char *argv[]){
    

    pthread_mutex_init(&mutex,NULL);
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
        puts("Fallo en el bind");
        return 1;
    }
    puts("Arranque exitoso");
     
    //Listen
    listen(socket_desc , 3);
     
    //Accept and incoming connection
    puts("Esperando por las conexiones entrantes...");
    c = sizeof(struct sockaddr_in);
    while( (new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) ){
        puts("¡Conexion aceptada!");
         
        //Reply to the client
        message = "Hola, la conexion fue exitosa, a partir de ahora puedes utilizar los servicios disponibles. Seras asignado con un operador.\n";
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
	int selecccion=0;
       
	pthread_t datosm;
    pthread_t comenm;
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
    int read_size;
    char *message , client_message[2000];
    int *count =0;
     
    //Send some messages to the client
    message = "Hola, soy el operador, te dire el menu\n";
    write(sock , message , strlen(message));
     
    message = "Ingresa: \n 1-Ver datos metereologicos y comentarios\n 2-Comentar datos metereologicos\n 3-Mensaje broadcast\n 4-Salir\n";
    write(sock , message , strlen(message));
     
    //Receive a message from client
    while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 ){
        //Send the message back to client
        if(recv(socket_desc, client_message , 2000 , 0) < 0){
        	if(count>0){
        		puts("Fallo en el recv o no se envio ninguna orden");
        	} 	
    	}count++;
    	printf("Elgida la opcion: %s\n", &client_message);
    	if(atoi(client_message)==1){
	       puts("Cargando info de metereologia...\n");	                
	    		if( pthread_create( &datosm , NULL , (void*)lectorMetereologico , NULL) < 0){
	           		perror("No fue posible crear el hilo, por favor intenta más tarde");
	            	return -1;
	        	}
    		}		
    	if(atoi(client_message)==2){
    		puts("Preparando los comentarios \n");
            if( pthread_create( &comenm , NULL , (void*)escribeMeterologico , &client_message) < 0){
                perror("No fue posible crear el hilo, por favor intenta más tarde");
                return -1;
            }
    	}
        else if(atoi(client_message)==3){
            puts("Preparando el broadcast \n");
        }
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
void lectorMetereologico(){
	int c;
	climadata = fopen(direccion,"r"); //rt = read text -- leer texto
	if(climadata == NULL){
		printf("Error al tratar de leer el archivo, por favor intenta de nuevo más tarde\n");
		return 1;
	}
	while((c=fgetc(climadata))!=EOF){
		if(c=='\n'){
			printf("\n");
		}
		else{
			putchar(c);
		}		
	}
	printf("\n");
	fclose(climadata);
    return 0;	
}
void escribeMeterologico(char escritura){
    int c;

    climadata = fopen(direccion,"w");
    
    if(climadata == NULL){
        printf("Ha ocurrido un error, por favor intenta de nuevo más tarde");
        return 1;
    }
    
    while(c=escritura!= EOF){
        fputs(c,climadata);
    }
    
    fclose(climadata);
    return 0;
}