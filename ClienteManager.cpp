#include <iostream>
using namespace std;

#include "ClienteManager.h"
#include "TrabajoArchivo.h"
#include "TrabajoManager.h"

void ClienteManager::OrdenarPorFecha(Cliente *vec, int cantRegistros) {
	int mayor = 0;
	Cliente aux;
	for (int i = 0; i < cantRegistros - 1; i++) {
		mayor = i;
		for (int j = i + 1; j < cantRegistros; j++) {
			if (vec[j].getFechaAlta().toString("YYYY/MM/DD") > vec[mayor].getFechaAlta().toString("YYYY/MM/DD")) {
				mayor = j;
			}
		}
		if (i != mayor) {
			aux = vec[i];
			vec[i] = vec[mayor];
			vec[mayor] = aux;
		}
	}
}

void ClienteManager::OrdenarPorApellido(Cliente *vec, int cantRegistros) {
	int mayor = 0;
	Cliente aux;
	for (int i = 0; i < cantRegistros - 1; i++) {
		mayor = i;
		for (int j = i + 1; j < cantRegistros; j++) {
			if (vec[j].getApellido() > vec[mayor].getApellido()) {
				mayor = j;
			}
		}
		if (i != mayor) {
			aux = vec[i];
			vec[i] = vec[mayor];
			vec[mayor] = aux;
		}
	}
}


void ClienteManager::Cargar() {
	string dni, nombre, apellido, eMail, direccion, telefono, razonSocial;
	int dia, mes, anio;

	cout << "NOMBRE       : ";
	getline(cin, nombre);
	cout << "APELLIDO     : ";
	getline(cin, apellido);
	cout << "DNI          : ";
	getline(cin, dni);
	cout << "EMAIL        : ";
	getline(cin, eMail);
	cout << "DIRECCION    : ";
	getline(cin, direccion);
	cout << "TELEFONO     : ";
	getline(cin, telefono);
	cout << "RAZON SOCIAL : ";
	getline(cin, razonSocial);
	cout << "FECHA DE ALTA" << endl;
	cout << "DIA          : ";
	cin >> dia;
	cout << "MES          : ";
	cin >> mes;
	cout << "ANIO         :" ;
	cin >> anio;

    Fecha fechaAlta(dia,mes,anio);

	Cliente reg(dni, nombre, apellido, eMail, direccion, telefono, razonSocial,fechaAlta);
	reg.setFechaAlta(Fecha(dia, mes, anio));

	if (_archivo.Guardar(reg)) {
		cout << "Registro guardado existosamente!" << endl;
	} else cout << "Error al guardar el registro" << endl;
}

void ClienteManager::ListarTodos() {
	int cantRegistros = _archivo.GetCantidadRegistros();
	for (int i = 0; i < cantRegistros; i++) {
		Cliente cliente = _archivo.Leer(i);
		if (cliente.getEstado()) {
			ListarRegistro(cliente);
			cout << endl;
		}
	}
}

void ClienteManager::ListarRegistro(Cliente cliente) {
	cout << "NOMBRE        : " << cliente.getNombre() << endl;
	cout << "APELLIDO      : " << cliente.getApellido() << endl;
	cout << "DNI           : " << cliente.getDni() << endl;
	cout << "EMAIL         : " << cliente.getEmail() << endl;
	cout << "DIRECCION     : " << cliente.getDireccion() << endl;
	cout << "TELEFONO      : " << cliente.getTelefono() << endl;
	cout << "RAZON SOCIAL  : " << cliente.getRazonSocial() << endl;
	cout << "FECHA DE ALTA : " << cliente.getFechaAlta().toString("DD/MM/YYYY") << endl;
}

void ClienteManager::ListarPorDni() {
	int pos;
	string dni;
	cout << "INGRESAR DNI DEL CLIENTE: ";
	getline(cin, dni);
	pos = _archivo.Buscar(dni);
	if (pos >= 0) {
		ListarRegistro(_archivo.Leer(pos));
	} else cout << "No exsite registro con DNI " << dni << endl;
}

void ClienteManager::ListarPorApellido() {
	int cantRegistros = _archivo.GetCantidadRegistros();
	string apellido;
	cout << "INGRESAR APELLIDO DEL CLIENTE: ";
	getline(cin, apellido);
	for (int i = 0; i < cantRegistros; i++) {
		Cliente cliente = _archivo.Leer(i);
		if (cliente.getApellido() == apellido && cliente.getEstado()) {
			ListarRegistro(cliente);
		}
	}
}

void ClienteManager::ListarOrdenadosPorFechaAlta() {
	int cantReg = _archivo.GetCantidadRegistros();

	Cliente *vec = new Cliente[cantReg];
	if (vec == nullptr) {
		cout << "Error" << endl;
		return;
	}

	for (int i = 0; i < cantReg; i++) {
		vec[i] = _archivo.Leer(i);
	}

	OrdenarPorApellido(vec, cantReg);

	for (int i = 0; i < cantReg; i++) {
		ListarRegistro(vec[i]);
		cout << endl;
	}

	delete []vec;
}

void ClienteManager::ListarOrdenadosPorApellido() {
	int cantReg = _archivo.GetCantidadRegistros();

	Cliente *vec = new Cliente[cantReg];
	if (vec == nullptr) {
		cout << "Error" << endl;
		return;
	}

	for (int i = 0; i < cantReg; i++) {
		vec[i] = _archivo.Leer(i);
	}

	OrdenarPorApellido(vec, cantReg);

	for (int i = 0; i < cantReg; i++) {
		ListarRegistro(vec[i]);
		cout << endl;
	}

	delete []vec;
}

void ClienteManager::EditarTelefono() {
	string dni, telefono;
	cout << "INGRESAR DNI DEL CLIENTE: ";
	getline(cin, dni);
	int pos = _archivo.Buscar(dni);
	if (pos >= 0) {
		Cliente reg = _archivo.Leer(pos);
		ListarRegistro(reg);
		cout << endl << "INGRESAR NUEVO TELEFONO: ";
		getline(cin, telefono);
		reg.setTelefono(telefono);
		if (_archivo.Guardar(reg)) {
			cout << "Registro guardado existosamente!" << endl;
		} cout << "Error al guardar el registro." << endl;
	} cout << "DNI inexistente." << endl;
}

void ClienteManager::Eliminar() {
	int pos;
	string dni;
	cout << "INGRESAR DNI DEL CLIENTE: ";
	getline(cin, dni);
	pos = _archivo.Buscar(dni);
	if (pos >= 0) {
		Cliente reg = _archivo.Leer(pos);
		ListarRegistro(reg);

		string opc;
		cout << endl << "Desea eliminar el registro? Ingresar 'S' para confirmar; 'N' para regresar.";
		cin.ignore();
		getline(cin, opc);

		if (opc == "S" || opc == "s") {
			reg.setEstado(false);

			if (_archivo.Guardar(reg)) {
				cout << "Registro eliminado exitosamente." << endl;
			} else cout << "Error al eliminar el registro." << endl;

		} else if (opc == "N" || opc == "n") {
			cout << "No se realizaron modificaciones." << endl;
		} else cout << "El valor ingresado es incorrecto" << endl;
	} else cout << "DNI inexistente." << endl;
}

void ClienteManager::HacerCopiaDeSeguridad() {
	int cantRegistros = _archivo.GetCantidadRegistros();
	Cliente *vec;

	vec = new Cliente[cantRegistros];
	if (vec == nullptr) {
		cout << "Error al realizar backup" << endl;
		return;
	}

	_archivo.Leer(vec, cantRegistros);
	_archivoBkp.Vaciar();
	if (_archivoBkp.Guardar(vec, cantRegistros)) {
		cout << "Backup realizado correctamente!" << endl;
	} else cout << "Error al realizar backup" << endl;

	delete []vec;
}

void ClienteManager::RestaurarCopiaDeSeguridad() {
	int cantRegistros = _archivoBkp.GetCantidadRegistros();
	Cliente *vec;

	vec = new Cliente[cantRegistros];
	if (vec == nullptr) {
		cout << "Error al realizar backup" << endl;
		return;
	}

	_archivoBkp.Leer(vec, cantRegistros);
	_archivo.Vaciar();
	if (_archivo.Guardar(vec, cantRegistros)) {
		cout << "Backup restaurado correctamente!" << endl;
	} else cout << "Error al restaurar backup" << endl;

	delete []vec;
}
