#include "utils.h"
#include <iostream>
#include <string>
#include <thread>
#include <list>
#include "gestorClientes.h"


int main(int argc, char** argv)
{
	
	//iniciar server, puerto 5000
	int serverSocketID=initServer(5000);
	std::vector<unsigned char> buffer;
	//bucle de gestión de conexiones
	while(1){
		//esperar conexion
		while(!checkClient()) usleep(100);
		std::cout<<"Cliente conectado\n";
		//recuperar identificador de conexión
		int clientId=getLastClientID();
		//crear hilo de gestión de clientes
		std::thread *th=new std::thread( GestorClientes::atiendeCliente,clientId);
	}
	//cerrar
	close(serverSocketID);
    return 0; 
}
