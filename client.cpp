#include <iostream>
#include <string.h>
#include <fstream>
using namespace std;

#define UPLOAD "upload"
#define DOWNLOAD "download"
#define IP "192.168.0.110"
#define PORT 5000
#define BUFFER_SIZE 5

void upload(string nomeArquivo);

int main(int argc, char const *argv[])
{
	string tipoTransferencia;
	string nomeArquivo;

	if(argc!=3){
		cout << ">> Erro! Número de argumentos inválido." << endl;
	}else{
		tipoTransferencia = argv[1];
		nomeArquivo = argv[2];
		(tipoTransferencia);
		if(tipoTransferencia!=UPLOAD && tipoTransferencia!=DOWNLOAD){
			cout << ">> Erro! Tipo de transferência inválida!." << endl;
		}else{
			if(tipoTransferencia==UPLOAD){
				
				upload(nomeArquivo);
			}else{
				cout << DOWNLOAD << endl;
			}
		}
	}

	return 0;
}

void upload(string nomeArquivo){
	cout << UPLOAD << endl;
	ifstream arquivo;
	char buffer[BUFFER_SIZE];
	char caractere;
	int i;
	i=0;
	arquivo.open(nomeArquivo);
	if(!arquivo.is_open()){
		cout << "Erro! Não foi possivel abrir o arquivo: " << nomeArquivo << endl;
	}else{
		while(arquivo.get(caractere)){
			if(i>=BUFFER_SIZE){
				cout << buffer;
				i=0;
				bzero(buffer, BUFFER_SIZE);
			}	
			buffer[i] = caractere;
			i++;			
		}
		cout << buffer << endl;
		i=0;
		bzero(buffer, BUFFER_SIZE);
	}
	arquivo.close();
}
