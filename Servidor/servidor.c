/*
Versão Inicial do servidor.
Com base no código: http://www.linuxhowtos.org/C_C++/socket.html

Desenvolvido por:
	- Carlos Magno Geraldo Barbosa
	- Lucas Geraldo Silva Cruz

Licença: MIT
Disciplina: Redes de Computadores
Universidade Federal de São João del Rei - UFSJ
*/
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "header.h"

void embaralhar(carta baralho[40]){
/* Esta função atribui os nomes e valores de cada carta
   do baralho de truco a uma poisição do array baralho */
	// As cartas estão ordenadas por ordem decrescente de valor
	strcpy(baralho[0].nome ,"4p"); baralho[0].valor = 14;
	strcpy(baralho[1].nome ,"7c"); baralho[1].valor = 13;
	strcpy(baralho[2].nome ,"Ae"); baralho[2].valor = 12;
	strcpy(baralho[3].nome ,"7o"); baralho[3].valor = 11;

	strcpy(baralho[4].nome ,"3p"); baralho[4].valor = 10; 
	strcpy(baralho[5].nome ,"3c"); baralho[5].valor = 10; 
	strcpy(baralho[6].nome ,"3e"); baralho[6].valor = 10; 
	strcpy(baralho[7].nome ,"3o"); baralho[7].valor = 10; 

	strcpy(baralho[8].nome ,"2p"); baralho[8].valor = 9;
	strcpy(baralho[9].nome ,"2c"); baralho[9].valor = 9;
	strcpy(baralho[10].nome, "2e"); baralho[10].valor = 9;
	strcpy(baralho[11].nome, "2o"); baralho[11].valor = 9;

	strcpy(baralho[12].nome, "Ap"); baralho[12].valor = 8;
	strcpy(baralho[13].nome, "Ac"); baralho[13].valor = 8;
	strcpy(baralho[14].nome, "Ao"); baralho[14].valor = 8;

	strcpy(baralho[15].nome, "Kp"); baralho[15].valor = 7;
	strcpy(baralho[16].nome, "Kc"); baralho[16].valor = 7;
	strcpy(baralho[17].nome, "Ke"); baralho[17].valor = 7;
	strcpy(baralho[18].nome, "Ko"); baralho[18].valor = 7;  

	strcpy(baralho[19].nome, "Jp"); baralho[19].valor = 6;
	strcpy(baralho[20].nome, "Jc"); baralho[20].valor = 6;
	strcpy(baralho[21].nome, "Je"); baralho[21].valor = 6;
	strcpy(baralho[22].nome, "Jo"); baralho[22].valor = 6;

	strcpy(baralho[23].nome, "Qp"); baralho[23].valor = 5;
	strcpy(baralho[24].nome, "Qc"); baralho[24].valor = 5;
	strcpy(baralho[25].nome, "Qe"); baralho[25].valor = 5;
	strcpy(baralho[26].nome, "Qo"); baralho[26].valor = 5;

	strcpy(baralho[27].nome, "7p"); baralho[27].valor = 4;
	strcpy(baralho[28].nome, "7e"); baralho[28].valor = 4;

	strcpy(baralho[29].nome, "6p"); baralho[29].valor = 3;
	strcpy(baralho[30].nome, "6c"); baralho[30].valor = 3;
	strcpy(baralho[31].nome, "6e"); baralho[31].valor = 3;
	strcpy(baralho[32].nome, "6o"); baralho[32].valor = 3;

	strcpy(baralho[33].nome, "5p"); baralho[33].valor = 2;
	strcpy(baralho[34].nome, "5c"); baralho[34].valor = 2;
	strcpy(baralho[35].nome, "5e"); baralho[35].valor = 2;
	strcpy(baralho[36].nome, "5o"); baralho[36].valor = 2;

	strcpy(baralho[37].nome, "4c"); baralho[37].valor = 1;
	strcpy(baralho[38].nome, "4e"); baralho[38].valor = 1;
	strcpy(baralho[39].nome, "4o"); baralho[39].valor = 1;
}

void distribuir(carta baralho[40], jogador clientes[4]){
/* Esta função distribui as cartas aleatoriamenteentre os jogadores */
	int i,j,k = 0;
	int card[12];
    srand(time(NULL));

	for(i = 0; i < 12; i++){
        card[i] = rand()%40;        
        for(j = 0; j < i; j++){
            if(card[j] == card[i]){
                card[i] = rand()%40;
                j = 0;
            }
        }
    }

    for(i = 0; i < 4; i++){
    	bzero(clientes[i].mao, 6);
    	for(j = 0; j < 3; j++){
    		strcat(clientes[i].mao, baralho[card[k]].nome);
    		k++;
    	}
    }
}

void broadCast(jogador clientes[4]){
/* Esta funçaõ realiza o envio de uma mensagem padrao para todos os clientes */
	write(clientes[0].porta, clientes[0].mao, 6);
	sleep(3);
	write(clientes[1].porta, clientes[1].mao, 6);
	sleep(3);
	write(clientes[2].porta, clientes[2].mao, 6);
	sleep(3);
	write(clientes[3].porta, clientes[3].mao, 6);
}

// int getValor(char *nome, carta baralho[40]){
//  Retorna o valor de uma carta com base no nome da mesma 
// 	int i;
// 	for(i = 0; i < 40; i++){
// 		if(baralho[i].nome == nome)
// 	}

// }

int main(){
	//Variaveis para estabelecer a comunicacao
	int socket_con = 0, num_porta = 5003, temp = 0;
	jogador clientes[4];
	char mensagem[23];
	socklen_t cliente_len;
	struct sockaddr_in endereco_servidor, endereco_cliente;

	//Variaveis do jogo
	carta baralho[40];
	int i;

	// Abrindo o socket
	socket_con = socket(AF_INET, SOCK_STREAM,0); 
	if(socket_con < 0)
		printf("Erro ao criar o socket.\n");

	//Preenche com zero.
	bzero((char *)&endereco_servidor, sizeof(endereco_servidor));
	endereco_servidor.sin_family = AF_INET;
	endereco_servidor.sin_addr.s_addr = INADDR_ANY;
	endereco_servidor.sin_port = htons(num_porta);

	//Associa uma porta ao socket
	if (bind(socket_con, (struct sockaddr *) &endereco_servidor,sizeof(endereco_servidor)) < 0)
		printf("Erro ao Abrir a porta.\n");
	else
		printf("Aguardando conexão dos clientes...\n");	

	//Tamanho maximo da fila de clientes.
	listen(socket_con, 5);
	
	//Estabelece a conexão com os clientes.
	for(i = 0; i < 4; i++)
		clientes[i].porta = accept(socket_con,(struct sockaddr *) &endereco_cliente, &cliente_len);
	

	if(clientes[0].porta < 0 || clientes[1].porta < 0 || 
	   clientes[2].porta < 0 || clientes[3].porta < 0)
		printf("Erro ao conectar com o cliente.\n");
	else
		printf("Conectado aos clientes: %d, %d, %d, %d\n",
		clientes[0].porta, clientes[1].porta, clientes[2].porta, clientes[3].porta);

	cliente_len = sizeof(endereco_cliente);	//???


	//Loop para troca de mensagens com o(s) cliente(s).
	//for(;;){
		embaralhar(baralho);
		distribuir(baralho, clientes);
		broadCast(clientes);
		
		//Zera o buffer de mensagens		
		bzero(mensagem, 23);

		//Realiza a leitura do socket do cliente
		/*temp = read(clientes[0].porta, mensagem, 23);
		printf("Mensagem: %s   e o temp vale: %d\n", mensagem,temp); */

		/*Escreve uma reposta no socket para o cliente.
		  O último parametro é o numero de bytes */
		
	//}

	//Fecha o socket de conexão com o cliente.
	for(i = 0; i < 4; i++)
		close(clientes[i].porta);
   
	//Fecha o socket que está ouvindo a porta.
	close(socket_con);

	return 0;
}
