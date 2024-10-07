/*
C++ SQLite Demo
Jon Goncalves
*/

#include <sqlite3.h>
#include <iostream>
#include <string>

using namespace std;


static int createDB(const char* s);
static int createTable(const char* s);
static int deleteData(const char* s);
static int insertData(const char* s);
static int updateData(const char* s);
static int selectData(const char* s);
static int callback(void* NotUsed, int argc, char** argv, char** azColName);

int main()
{
    const char* dir = R"(students.db)";

    createDB(dir);

    createTable(dir);

    deleteData(dir);

    insertData(dir);

    updateData(dir);

    selectData(dir);

    return 0;
}


/*
┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
┃                                                                           ┃
┃   Función para crear la base de datos. Si no existe, la crea, y si        ┃
┃   existe, la abre.                                                        ┃
┃   Recibe como parámetro la ruta de la base de datos.                      ┃
┃   Retorna 0 si se ejecuta correctamente.                                  ┃
┃                                                                           ┃
┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
*/
static int createDB(const char* s)
{
    sqlite3* DB;
    int exit = sqlite3_open(s, &DB); 
    sqlite3_close(DB);               
    return 0;
}



/*
┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
┃                                                                           ┃
┃   Función para crear una tabla en la base de datos.                       ┃
┃   Si la tabla no existe, la crea. Si existe, no la modifica.              ┃
┃   Recibe como parámetro la ruta de la base de datos.                      ┃
┃   Retorna 0 si se ejecuta correctamente.                                  ┃
┃                                                                           ┃
┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
*/

static int createTable(const char* s)
{
    sqlite3* DB;
    char* messageError;


    string sql = "CREATE TABLE IF NOT EXISTS CONTACT("
        "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
        "NAME TEXT NOT NULL, "
        "SURNAME TEXT NOT NULL, "
        "AGE INT NOT NULL, "
        "PHONE INT NOT NULL);";

    int exit = sqlite3_open(s, &DB);
    if (exit != SQLITE_OK) {
        cerr << "Error opening database: " << sqlite3_errmsg(DB) << endl;
        return exit;
    }

    // Ejecuta el comando SQL para crear la tabla.
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
        cerr << "Error in createTable function: " << messageError << endl;
        sqlite3_free(messageError); 
    }
    else {
        cout << "Table created successfully." << endl;
    }

    sqlite3_close(DB); 
    return 0;
}

/*
┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
┃                                                                           ┃
┃   Función para insertar registros en la tabla CONTACT.                    ┃
┃   Inserta varios registros con datos predefinidos.                        ┃
┃   Recibe como parámetro la ruta de la base de datos.                      ┃
┃   Retorna 0 si se ejecuta correctamente.                                  ┃
┃                                                                           ┃
┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
*/


static int insertData(const char* s)
{
    sqlite3* DB;
    char* messageError;

    // SQL para insertar datos en la tabla CONTACT.
    string sql = "INSERT INTO CONTACT (NAME, SURNAME, AGE, PHONE) VALUES('Juan', 'Perez', 30, 123456789);"
        "INSERT INTO CONTACT (NAME, SURNAME, AGE, PHONE) VALUES('Maria', 'Gomez', 25, 987654321);"
        "INSERT INTO CONTACT (NAME, SURNAME, AGE, PHONE) VALUES('Carlos', 'Sanchez', 40, 555666777);"
        "INSERT INTO CONTACT (NAME, SURNAME, AGE, PHONE) VALUES('Laura', 'Martinez', 28, 444333222);";

    int exit = sqlite3_open(s, &DB);
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError); 
    if (exit != SQLITE_OK) {
        cerr << "Error in insertData function: " << messageError << endl;
        sqlite3_free(messageError); 
    }
    else {
        cout << "Records inserted Successfully!" << endl;
    }

    sqlite3_close(DB); 
    return 0;
}

/*
┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
┃                                                                           ┃
┃      Función para actualizar datos en la tabla CONTACT.                   ┃
┃      Modifica un registro específico según la condición dada.             ┃
┃      En este caso, cambia el nombre 'Juan' por 'Manuela'.                 ┃
┃      Recibe como parámetro la ruta de la base de datos.                   ┃
┃      Retorna 0 si se ejecuta correctamente.                               ┃
┃                                                                           ┃
┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
*/
static int updateData(const char* s)
{
    sqlite3* DB;
    char* messageError;

    // SQL para actualizar el nombre de 'Juan' a 'Manuela' en la tabla.
    string sql = "UPDATE CONTACT SET NAME = 'Manuela' WHERE NAME = 'Juan'";

    int exit = sqlite3_open(s, &DB); // Abre la base de datos.
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError); 
    if (exit != SQLITE_OK) {
        cerr << "Error in updateData function: " << messageError << endl;
        sqlite3_free(messageError); 
    }
    else {
        cout << "Records updated Successfully!" << endl;
    }

    sqlite3_close(DB); 
    return 0;
}

/*
┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
┃                                                                           ┃
┃   Función para eliminar los registros existentes en la tabla.             ┃
┃   Elimina todos los registros de la tabla CONTACT.                        ┃
┃   Recibe como parámetro la ruta de la base de datos.                      ┃
┃   Retorna 0 si se ejecuta correctamente.                                  ┃
┃                                                                           ┃
┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
*/
static int deleteData(const char* s)
{
    sqlite3* DB;
    char* messageError;

    // SQL para eliminar todos los registros de la tabla.
    string sql = "DELETE FROM CONTACT;";

    int exit = sqlite3_open(s, &DB); 
    exit = sqlite3_exec(DB, sql.c_str(), callback, NULL, &messageError); 
    if (exit != SQLITE_OK) {
        cerr << "Error in deleteData function: " << messageError << endl;
        sqlite3_free(messageError); 
    }
    else {
        cout << "Records deleted Successfully!" << endl;
    }

    sqlite3_close(DB); 
    return 0;
}

/*
┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
┃                                                                           ┃
┃   Función para seleccionar (consultar) y mostrar todos los registros      ┃
┃   de la tabla CONTACT.                                                    ┃
┃   Recibe como parámetro la ruta de la base de datos.                      ┃
┃   Retorna 0 si se ejecuta correctamente.                                  ┃
┃                                                                           ┃
┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
*/
static int selectData(const char* s)
{
    sqlite3* DB;
    char* messageError;

    // SQL para seleccionar todos los registros de la tabla.
    string sql = "SELECT * FROM CONTACT;";

    int exit = sqlite3_open(s, &DB); // Abre la base de datos.
    exit = sqlite3_exec(DB, sql.c_str(), callback, NULL, &messageError); // Ejecuta la consulta.

    if (exit != SQLITE_OK) {
        cerr << "Error in selectData function: " << messageError << endl;
        sqlite3_free(messageError); // Libera el mensaje de error si ocurre.
    }
    else {
        cout << "Records selected Successfully!" << endl;
    }

    sqlite3_close(DB); // Cierra la base de datos.
    return 0;
}

/*
┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
┃                                                                           ┃
┃      Función de callback para procesar y mostrar los resultados de las    ┃
┃      consultas SQL.                                                       ┃
┃      Se invoca para cada fila recuperada de una consulta.                 ┃
┃      Muestra el nombre de cada columna y su valor correspondiente.        ┃
┃                                                                           ┃
┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
*/


static int callback(void* NotUsed, int argc, char** argv, char** azColName)
{
    
    for (int i = 0; i < argc; i++) {
        
        cout << azColName[i] << ": " << (argv[i] ? argv[i] : "NULL") << endl;
    }
    cout << endl;
    return 0;
}
