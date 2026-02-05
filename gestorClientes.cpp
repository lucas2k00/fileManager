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
		auto tipo=unpack<msgTypes>(buffer);
		switch(tipo){ //crear un caso para cada tipo de mensaje
			case FileManager_Constructor:
			{
				FileManager fm; //creada instancia de fileManager
				//almacenarla para futuras llamadas a metodos
				clients[clientId]=fm;
				//empaquetar okay
				buffer.clear();//limpiar el buffer antes de reutilizarlo
				pack(buffer,OK_MSG);//almacenar mensaje
				//en este punto termina la invocación, el mensaje se envía
				//al final del cuerpo switch-case
			}break;
			case ListFiles_F:
			{
				//recuperar instancia de FM
					//desempaquetar params
				vector<string> res=clients[clientId].listFiles();
				//empaquetar resultado
				buffer.clear();
				pack(buffer, res.size());
				//por cada string
				for(auto &s: res){
					//empaquetar string
					pack(buffer, s.size());
					packv(buffer, (char*)s.data(),(int)s.size());
				}
				//en este punto termina la invocación, el mensaje se envía
				//al final del cuerpo switch-case	
			}break;
			case FileManager_Destructor:
			{
				//Invocamos el destructor eliminando la instancia de persona
				//de la lista de clientes
				clients.erase(clientId);
				//acabamos el bucle "while" que mantiene la conexión con el cliente
				salir=true;
				//retornar un mensaje ACK
				buffer.clear();//limpiar el buffer antes de reutilizarlo
				pack(buffer,OK_MSG);//almacenar mensaje
				//en este punto termina la invocación, el mensaje se envía
				//al final del cuerpo switch-case
			}break;
			case FileManager_Constructor_parametros:
			{
				string path;

				path.resize(unpack<int>(buffer));
				unpackv<char>(buffer,(char*)path.data(),(int)path.size());

				FileManager fm(path);
				clients[clientId]=fm;
				//empaquetar okay
				buffer.clear();//limpiar el buffer antes de reutilizarlo
				pack(buffer,OK_MSG);//almacenar mensaje
				//en este punto termina la invocación, el mensaje se envía
				//al final del cuerpo switch-case
			}break;
			case ReadFiles_F:
			{
				string fichero; //Variables para almacenar fichero
				//Reconstruimos el fichero desempaquetando sus datos
				//- Desempaquetamos y redimensionamos el tamaño del fichero
				fichero.resize(unpack<int>(buffer));
				//desempaquetamos
				unpackv<char>(buffer,(char*)fichero.data(),(int)fichero.size());
				vector<unsigned char> fileData;
				clients[clientId].readFile(fichero,fileData);

				buffer.clear();
				pack(buffer,(int)fileData.size());
				packv(buffer,fileData.data(),(int)fileData.size());
				//en este punto termina la invocación, el mensaje se envía
				//al final del cuerpo switch-case
			}break;
			case WriteFiles_F:
			{
				string fileName;

				fileName.resize(unpack<int>(buffer));
				unpackv<char>(buffer,(char*)fileName.data(),(int)fileName.size());

				int dataSize = unpack<int>(buffer);
				vector<unsigned char> fileData(dataSize);
				unpackv<unsigned char>(buffer, fileData.data(),dataSize);

				clients[clientId].writeFile(fileName, fileData);

				//empaquetar okay
				buffer.clear();//limpiar el buffer antes de reutilizarlo
				pack(buffer,OK_MSG);//almacenar mensaje
				//en este punto termina la invocación, el mensaje se envía
				//al final del cuerpo switch-case
			}break;
			case OK_MSG: //No debemos recibir mensajes de ACK, ERROR
			default:
			{
			//default: mensaje error
				cout<<"ERROR: "<<__FILE__<<":"<<__LINE__<<
						"Tipo de mensaje inválido"<<endl;
			}break;
		};
		//En cada caso, se debe rellenar el buffer de respuesta
		//Una vez relleno, se envía al cliente:
		sendMSG(clientId,buffer);
	}
	closeConnection(clientId);
}