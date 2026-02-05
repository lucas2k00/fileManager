#pragma once
#include "utils.h"
#include "filemanager.h"


typedef enum{
	FileManager_Constructor,
	FileManager_Destructor,
	FileManager_Constructor_parametros,
	ListFiles_F,
	
	OK_MSG

} msgTypes;

class GestorClientes{
	
	public:
	//Usado por el cliente
	//Mapa para almacenar y buscar datos de conexión
		static inline map<Persona*,connection_t > connections;
	//Usado por el servidor
	//Mapa para almacenar y buscar personas reservadas por clientes
		static inline map<int,Persona > clients; 
	//Método usado por el servidor para gestionar peticiones de invocación
	//de métodos recibido de programas tipo cliente.
	//Cada nuevo cliente conectado al sistema lleva asociado un "clientId" que 
	//se usará en sus comunicaciones
		static void atiendeCliente(int clientId);
};