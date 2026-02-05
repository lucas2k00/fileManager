#include "gestorClientes.h"
#include <sstream>   


void GestorClientes::atiendeCliente(int clientId){
	bool salir=false;
	vector<unsigned char> buffer;//Buffer de datos recibidos desde le cliente
	//Mantener la conexión hasta que el cliente invoque el destructor
	while(!salir) {
		//recibir mensaje
		recvMSG(clientId,buffer);
		//dependiendo de tipo de mensaje ejecutar funcion
		auto tipo=unpack<MSGTypes>(buffer);
		switch(tipo){ //crear un caso para cada tipo de mensaje
			case FileManager_Constructor:
			{
				fileManager fm; //creada instancia de fileManager
				//almacenarla para futuras llamadas a metodos
				clients[clientId]=fm;
				//empaquetar okay
				buffer.clear();//limpiar el buffer antes de reutilizarlo
				pack(buffer,OK_MSG);//almacenar mensaje
				sendMSG(clientId,buffer);	
			}break;
			case ListFiles_F:
			{
				//recuperar instancia de FM
					//desempaquetar params
				vector<string> res=clients[clientId].listFiles();
					//empaquetar resultado
					pack(buffer, res.size());
					//por cada string
					for(auto &s: res){
						//empaquetar string
						pack(buffer, s.size());
						packv(buffer, s.data());
					}
					
					//enviar
				
			}break;
		}
		//ejecutar funcion
	}
	closeConnection(clientId);
}