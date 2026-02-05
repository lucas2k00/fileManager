#include "filemanager.h"
#include "gestorClientes.h"

	
FileManager(){
	vector<unsigned char> buffer;
	auto conn=initClient("127.0.0.1",5000);
	//empaquetar
	pack(buffer,FileManager_Constructor);
	//enviar
	sendMSG(conn.serverId, buffer);
	//recibir respuesta
	recvMSG(conn.serverId,buffer);
	//si no ok, error
	if(unpack<MSGTypes>(buffer) !=OK_MSG)
		cout<<"ERROR....";
	
	//almacenar conexion
	connections[this]=conn.serverId;
}

~FileManager(){ //ESTA TERMINADO??????????
	closeConnection(conn.serverId);
}

FileManager(string path); //TO DOOO

vector<string> listFiles(){
	
	vector<unsigned char> buffer;
	vector<string> res;
	auto serverID=connections[this];
	
	//crear peticion de invocacion
		//empaquetar tipo
		pack(buffer,ListFiles_F);
		//empaquetar parametros (si es necesario en este caso NO)
	//enviar
	sendMSG(serverID,buffer);
	//recibir resultado
	recvMSG(serverID,buffer);
	//desempaquetar
	int size=unpack<long int>(buffer);
	res.resize(size);
	
	for(auto &s: res){
		s.resize(unpack<long int>(buffer));
		unpackv(buffer,s.data(),s.size());
	}
	//retornar
	return res;
}

void readFile(string fileName, vector<unsigned char> &data); //TO DOOO
void writeFile(string fileName, vector<unsigned char> &data); //TO DOOO