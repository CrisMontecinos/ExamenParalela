#include <iostream>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <ctime>
#include <cstring>
#include <cmath>
#include <omp.h>

using namespace std;

/**
 * Funcion para convertir fechas a segundos gracias .
 *
 * @param anio tipo de puntero que apunta a una cadena de caracteres
 * @param mes tipo de puntero que apunta a una cadena de caracteres
 * @param dia tipo de puntero que apunta a una cadena de caracteres
 * @return t fecha transformada a segundos
 */
double fecha_a_segundos(char* anio, char* mes, char* dia) {
  // Crear un objeto tm con la fecha dada
  tm fecha;
  fecha.tm_year = atoi(anio) - 1900;;
  fecha.tm_mon = atoi(mes) - 1;
  fecha.tm_mday = atoi(dia);

  // Convertir el objeto tm a un objeto time_t
  time_t t = mktime(&fecha);

  // Obtener el número de segundos transcurridos desde la medianoche del 1 de enero de 1970
  return t;
}

/**
 * Función que realiza una regresión lineal a partir de dos
 * vectores de enteros, x e y, y un contador que indica el tamaño
 * de ambos vectores. También recibe un flotante segundos para
 * hacer una estimación. La función utiliza un bucle for para recorrer
 * los elementos de los vectores x e y y calcular varios valores
 * necesarios para realizar la regresión lineal. También incluye una
 * directiva #pragma omp parallel for que indica que el bucle debe
 * ejecutarse de forma paralela utilizando múltiples hilos de ejecución.
 *  Finalmente, la función devuelve el resultado de la regresión lineal.
 *
 * @param x vector de enteros
 * @param y vectores de enteros
 * @param contador entero que indica el tamaño
 * @param segundos es un flotante con la fecha en segundos ingresada por el usuario para hacer la preddicion
 * @return ecuaciondelaRecta es el resultado de toda la operacion para
 */

float regresionLineal(vector<int> x,vector<int> y, int contador, float segundos ){

    //Declaracion de variables para hacer las sumatorias necesarias para la regresion lineal
        float sumaAccidente = 0;
        float sumaYPromAccidente = 0;
        float sumaFecha = 0;
        float sumatoriaXY = 0;
        float sumatoriaXCuadrado = 0;
    //Indica que el bucle debe ejecutarse de forma paralela utilizando múltiples hilos de ejecución.
 #pragma omp parallel //for reduction(+:sumaAccidente,sumatoriaXY,sumatoriaXCuadrado,sumaYPromAccidente,sumaFecha)
    for(int i = 0; i<contador; i++){

        sumaAccidente += y[i];
        sumatoriaXY += (x[i] * y[i]);
        sumatoriaXCuadrado += (y[i]*y[i]);
        sumaYPromAccidente += (x[i]*x[i]);
        sumaFecha +=x[i];
    }
    // se declaran todas los parametros necesarios para realizar los calculos de la regresion llineal
        float promedioAccidente = sumaAccidente/contador;
        float promeFecha = sumaFecha/contador;
        float nxy = contador * promeFecha * promedioAccidente;
        float nx2 = contador* promeFecha * promeFecha;
        float pendiente = (sumatoriaXY - nxy)/(sumatoriaXCuadrado - nx2);
        float ordenada = promedioAccidente - pendiente * promeFecha;
        float ecuaciondelaRecta = ordenada + (pendiente * segundos);
    //retorna la preddiccio de la ecuacion de la recta
    return ecuaciondelaRecta;
}
/**
* El código principal del programa comienza con la apertura de un archivo
* llamado datos_examen.csv utilizando un objeto ifstream llamado miArchivo.
* Se declaran dos vectores de enteros llamados x e y, un entero contador y un flotante segundos.
* Se utiliza un bucle while para leer cada línea del archivo hasta que se alcance el final del archivo.
* En los vectores x e y, se almacenan las fechas en segundos utilizando la funcion fecha_a_segundos,
* y los accidentes en el parametro x.
* Una vez que se han leído todas las líneas del archivo, se cierra el archivo y se le pide al
* usuario que ingrese una fecha en formato iso 8601 se valida un formato correcto.
* luego se utiliza la funcion regresionLineal donde se le pasan los paramatros x, y, condator y segundos
* que es la fecha transformada a segundos para poder realizar la regresion lineal, luego
* se imprime por pantalla la cantidad de accidentes predecidos y la fecha en la priso 8601.
*/


int main()
{
    ifstream miArchivo;

    miArchivo.open("datos_examen.csv");// se abre el archivo csv

    string linea = "";

    getline(miArchivo,linea);// salta la primera línea

        vector<int> x;
        vector<int> y;
        int contador;
        float segundos;

   while(getline(miArchivo,linea)){
        string fecha;
        int accidentes;

        string tempString = "";

        stringstream inputString(linea);//se toma una linea del archivo

        getline(inputString, fecha, ';');//separamos en un string fecha lo que se encontro hasta el primer ;

        //Dividimos la fecha en componentes utilizando strtok()
        char* data_str = strdup(fecha.c_str());
        char* dia_str = strtok(data_str, "-");
        char* mes_str = strtok(NULL, "-");
        char* anho_str = strtok(NULL, "-");

        x.push_back(fecha_a_segundos(anho_str , mes_str, dia_str));//vamos dejando fechas en segundos en el vector

        getline(inputString,tempString,';');//busca dentro lo q esta despes del ; hasta el siguente ; y lo deja en ina variable temporal de string
        accidentes = atoi(tempString.c_str());// transforma el la variable temporal a un entero

        y.push_back(accidentes);//agrega fechas al el vector y

        contador = contador + 1; //contador para saber la cantidad total de accidentes y fechas
        linea = "";

}
    miArchivo.close();//se cierra el archivo

    string Dia,Mes,Anho;

    string diaPrediccion;//se declara en un string
    while(true){
        cout << "ingrese una fecha formato ISO 8601(ej: 2022-12-19): " << endl;
        cin >> diaPrediccion;
        if (diaPrediccion.size() != 10 || diaPrediccion[4] != '-' || diaPrediccion[7] != '-' ){
              cout << "Por favor, ingrese una fecha valida" << endl;
              continue;
        }
    // Verifica si los componentes de la fecha son números válidos
    int _anhos, _meses, _dias;
        if (!(istringstream(diaPrediccion.substr(0, 4)) >> _anhos)||
            !(istringstream(diaPrediccion.substr(5, 2)) >> _meses)||
            !(istringstream(diaPrediccion.substr(8, 2)) >> _dias)) {
          cout << "Por favor, ingrese una fecha valida" << endl;
          continue;
        }
        if (_meses < 1 || _meses > 12) {
          cout << "Por favor, ingrese una fecha válida" << endl;
          continue;
            }
        char* date_str = strdup(diaPrediccion.c_str());
        char* year_str = strtok(date_str, "-");
        char* month_str = strtok(NULL, "-");
        char* day_str = strtok(NULL, "-");

        segundos = fecha_a_segundos(year_str , month_str, day_str);//transforma la fecha ingresada por el usuario a segundos
        break;
    }

    float ecuaciondelarecta =regresionLineal(x, y, contador, segundos);//Se obtiene la prediccion
    cout << "Fecha de la prediccion: " <<diaPrediccion << endl;//se muestra la fecha en iso 8601
    cout << "Cantidad de accidentes esperados: " << ecuaciondelarecta << endl; //se muestra la prediccion
    return 0;
}
