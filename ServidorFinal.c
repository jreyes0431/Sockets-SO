#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<pthread.h> //for threading , link with lpthread
 
pthread_mutex_t mutex;
char *c="voy a escribir esto mucho\n";
int a=0;
FILE *ficheroM, *ficheroMM;

void *connection_handler(void *);
int main(int argc , char *argv[]){
    system("clear");
    pthread_mutex_init(&mutex,NULL);
    int socket_desc , new_socket , c , *new_sock;
    struct sockaddr_in server , client;
    char *message;
     
    //Creación del socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1){
        printf("Error al crear el socket");
    }
     
    //Llenado de la sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );
     
    //Enlazando el puerto
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0){
        puts("Fallo en el bind");
        return 1;
    }
    puts("Arranque exitoso");
     
    //Listen
    listen(socket_desc , 3);
     
    //Aceptar conexíones
    puts("Esperando por conexiones entrantes...");
    c = sizeof(struct sockaddr_in);
    while( (new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) ){
        puts("Nueva conexion exitosa");
         
        //Respondiendo al cliente
        message = "Hola, cliente, la conexion es exitosa y procederemos a asignarte un operador de menu\n";
        write(new_socket , message , strlen(message));
         
        pthread_t sniffer_thread;
        new_sock = malloc(1);
        *new_sock = new_socket;
         
        if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0){
            perror("Error al crear el hilo operador");
            return 1;
        }
         
        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( sniffer_thread , NULL);
        puts("Operador de menu asignado exitosamente");
    }
     
    if (new_socket<0){
        perror("Fallo en el accept");
        return 1;
    }
     
    return 0;
}
 
/*
 * Operador de menu
 * */
void *connection_handler(void *socket_desc){
    
    int sock = *(int*)socket_desc;
    int read_size;
    char *message , client_message[2000], *otroM, message1[600];
     
    //Enviando mensaje al cliente
    message = "\nSoy el operador de menu, a continuacion procedere a mostrarlo en pantalla\n 1-Ver datos metereologicos y comentarios\n 2-Comentar datos metereologicos\n 3-Mensaje lectura\n 4-Salir\n";
    write(sock , message , strlen(message));
     fflush(stdout);
     fflush(stdin);
     int aux=0;
    //Receive a message from client
    while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 ){
        if(atoi(client_message)==1){
            pthread_mutex_lock(&mutex);
            ficheroM = fopen("DatosMDia.txt","r");
            if(ficheroM==NULL){
                otroM=("No se pudo abrir el fichero o este esta vacio");
                aux=0;
                write(sock , otroM , strlen(otroM));
            }
            while(!feof(ficheroM)){
                fgets(message1,500,ficheroM);
            }
            fclose(ficheroM);
            aux=0;
            write(sock , &message1 , strlen(message1));
            pthread_mutex_unlock(&mutex);   
        }
        else if(atoi(client_message)==2){
            otroM = "Seleccionaste 2\n";
            aux=0;
            write(sock , otroM , strlen(otroM));            
        }
        else if(aux==1){
            pthread_mutex_lock(&mutex);
            ficheroMM= fopen("OrdenLectura.txt","w");
            if(ficheroMM==NULL){
                otroM=("No se pudo escribir, intente de nuevo más tarde");
            }
            fprintf(ficheroMM,"El mensaje es: %s\n",client_message);
            otroM=("El mensaje fue guardado de forma exitosa\n");
            fclose(ficheroMM);
            aux=0;
            write(sock , otroM , strlen(otroM));
            pthread_mutex_unlock(&mutex);           
        }
        else if(atoi(client_message)==3){
            otroM= "Prepara tu mensaje!\n";
            aux=1;
            write(sock , otroM , strlen(otroM));
        }
        
        else if ((atoi(client_message) != 3) && (atoi(client_message) != 2 ) && (atoi(client_message) != 1)  && (atoi(client_message) != 4)){
            otroM=("Eleccion invalida, ingresa un valor del menu \n");
            aux=0;
            write(sock , otroM , strlen(otroM)); 
        }
    }
     
    if(read_size == 0){
        puts("El cliente se ha desconectado");
        fflush(stdout);
    }
    else if(read_size == -1){
        perror("fallo en el recv");
    }        
    free(socket_desc);  
    return 0;
}