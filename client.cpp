#include <iostream>
#include <string>
#include <fstream>
using namespace std;

#define UPLOAD "upload"
#define DOWNLOAD "download"
#define IP "192.168.0.110"
#define PORT 5000
#define BUFFER_SIZE 1024

int main(int argc, char const *argv[])
{
	string tipoTransferencia;
	string nomeArquivo;
	char buffer[BUFFER_SIZE];

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
				cout << UPLOAD << endl;
			}else{
				cout << DOWNLOAD << endl;
			}
		}
	}

	return 0;
}
