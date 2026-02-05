#include "filemanager.h"
#include "gestorClientes.h"
#include "utils.h"
#include <iostream>
	
FileManager::FileManager(){
	//buffer
	vector<unsigned char> buffer;
	//iniciar conexion
	auto conn=initClient("127.0.0.1",5000);

	//empaquetar
	pack(buffer,FileManager_Constructor);
	//enviar
	sendMSG(conn.serverId, buffer);
	//recibir respuesta
	buffer.clear(); //limpiar buffer antes de reutilizarlo
	recvMSG(conn.serverId,buffer);
	//si no ok, error
	if(unpack<msgTypes>(buffer) !=OK_MSG)
		cout<<"ERROR....";
	
	//almacenar conexion
	GestorClientes::connections[this]=conn;
}


FileManager::~FileManager(){
	//Buscar datos de conexión
	auto conn=GestorClientes::connections[this];
	//buffer
	vector<unsigned char> buffer;

	//Empaquetar invocación de destructor
	pack(buffer,FileManager_Destructor);
	//Enviar
	sendMSG(conn.serverId, buffer);
	//recibir confirmación de ejecución
	buffer.clear(); //limpiar buffer antes de reutilizarlo
	recvMSG(conn.serverId,buffer);

	//cerrar conexion
	closeConnection(conn.serverId);
	//eliminar datos de conexión
	GestorClientes::connections.erase(this);
}


FileManager::FileManager(string path){
	//buffer
	vector<unsigned char> buffer;
	//iniciar conexion
	auto conn=initClient("127.0.0.1",5000);

	//empaquetar
	pack(buffer,FileManager_Constructor_parametros);
	pack(buffer,(int)path.size());//Empaquetar tamaño del path
	packv(buffer,(char*)path.data(),(int)path.size()); //Empaquetar letras del path
	//enviar
	sendMSG(conn.serverId, buffer);
	//recibir respuesta
	buffer.clear(); //limpiar buffer antes de reutilizarlo
	recvMSG(conn.serverId,buffer);
	//si no ok, error
	if(unpack<msgTypes>(buffer) !=OK_MSG)
		cout<<"ERROR....";
	
	//almacenar conexion
	GestorClientes::connections[this]=conn;
}


vector<string> FileManager::listFiles(){
	
	vector<unsigned char> buffer;
	vector<string> res;
	auto conn=GestorClientes::connections[this];
	
	//crear peticion de invocacion
		//empaquetar tipo
		pack(buffer,ListFiles_F);
		//empaquetar parametros (si es necesario)
	//enviar
	sendMSG(conn.serverId,buffer);
	//recibir resultado
	recvMSG(conn.serverId,buffer);
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


void FileManager::readFile(string fileName, vector<unsigned char> &data){
	//buffer
	vector<unsigned char> buffer;
	//Buscar datos de conexión
	auto conn=GestorClientes::connections[this];

	//crear peticion de invocacion
		//empaquetar tipo
		pack(buffer,ReadFiles_F);
		//empaquetar parametros
		pack(buffer,(int)fileName.size());
		packv(buffer,(char*)fileName.data(),(int)fileName.size());
	//enviar
	sendMSG(conn.serverId,buffer);
	//recibir resultador
	buffer.clear();
	recvMSG(conn.serverId,buffer);

	if(buffer.size()==0)
	{
			cout<<"ERROR: "<<__FILE__<<":"<<__LINE__<<
						"Tamaño de mensaje inválido"<<endl;							//error
	}
    else {
        //desempaquetar
		int size=unpack<int>(buffer); //QUITADO EL LONG INT, ahora funciona porque???
        
        data.resize(size);
        unpackv(buffer, data.data(), size);
    }

}


void FileManager::writeFile(string fileName, vector<unsigned char> &data){
	//buffer
	vector<unsigned char> buffer;
	//Buscar datos de conexión
	auto conn=GestorClientes::connections[this];

	//crear peticion de invocacion
		//empaquetar tipo
		pack(buffer,WriteFiles_F);
		//empaquetar parametros
		pack(buffer,(int)fileName.size());
		packv(buffer,(char*)fileName.data(),(int)fileName.size());
		pack(buffer,(int)data.size());
		packv(buffer,data.data(),(int)data.size());
	//enviar
	sendMSG(conn.serverId,buffer);
	//recibir resultador
	buffer.clear();
	recvMSG(conn.serverId,buffer);

	if(buffer.size()==0)
	{
			cout<<"ERROR: "<<__FILE__<<":"<<__LINE__<<
						"Tamaño de mensaje inválido"<<endl;							//error
	}
	else{	
		msgTypes ack=unpack<msgTypes>(buffer);
		if(ack!=OK_MSG)
			cout<<"ERROR: "<<__FILE__<<":"<<__LINE__<<
						"Tipo de mensaje inválido"<<endl;
	}
}