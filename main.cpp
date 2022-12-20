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
 * @return un valor de tipo time_t
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
 * Esta función realiza una regresión lineal a partir de dos vectores de enteros, x e y, y
 * un contador que indica el tamaño de ambos vectores. También recibe un flotante segundos
 * para hacer la estimacion .
 *
 * @param x vector de enteros
 * @param y vectores de enteros
 * @param contador entero que indica el tamaño
 * @param segundos es un flotante con los
 * @return ecuaciondelaRecta
 */

float regresionLineal(vector<int> x,vector<int> y, int contador, float segundos ){

    float sumaAccidente = 0;
    float sumaYPromAccidente = 0;
    float sumaFecha = 0;
    float sumatoriaXY = 0;
    float sumatoriaXCuadrado = 0;
 #pragma omp parallel for reduction(+:sumaAccidente,sumatoriaXY,sumatoriaXCuadrado,sumaYPromAccidente,sumaFecha)
    for(int i = 0; i<contador; i++){
        sumaAccidente += y[i];
        sumatoriaXY += (x[i] * y[i]);
        sumatoriaXCuadrado += (y[i]*y[i]);
        sumaYPromAccidente += (x[i]*x[i]);
        sumaFecha +=x[i];
    }
    float promedioAccidente = sumaAccidente/contador;
    float promeFecha = sumaFecha/contador;
    float nxy = contador * promeFecha * promedioAccidente;
    float nx2 = contador* promeFecha * promeFecha;
    float pendiente = (sumatoriaXY - nxy)/(sumatoriaXCuadrado - nx2);
    float ordenada = promedioAccidente - pendiente * promeFecha;
    float ecuaciondelaRecta = ordenada + (pendiente * segundos);

    return ecuaciondelaRecta;
}

int main()
{
    ifstream miArchivo;

    miArchivo.open("datos_examen.csv");

    string linea = "";

    getline(miArchivo,linea);// salta la primera línea

        vector<int> x;
        vector<int> y;
        int contador;
        float segundos;

//#pragma omp parallel
   while(getline(miArchivo,linea)){
        string fecha;
        int accidentes;

        string tempString = "";

        stringstream inputString(linea);

        getline(inputString, fecha, ';');

        //Dividimos la fecha en componentes utilizando strtok()
        char* date_str = strdup(fecha.c_str());
        char* day_str = strtok(date_str, "-");
        char* month_str = strtok(NULL, "-");
        char* year_str = strtok(NULL, "-");

        x.push_back(fecha_a_segundos(year_str , month_str, day_str));

        getline(inputString,tempString,';');
        accidentes = atoi(tempString.c_str());

        y.push_back(accidentes);

        contador = contador + 1;
        linea = "";

}
miArchivo.close();

    string Dia,Mes,Anho;

    string diaPrediccion;
    while(true){
    cout << "ingrese una fecha formato: " << endl;
    cin >> diaPrediccion;
    if (diaPrediccion.size() != 10 || diaPrediccion[4] != '/' || diaPrediccion[7] != '/' ){
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

    segundos = fecha_a_segundos(year_str , month_str, day_str);
    break;
    }

    float ecuaciondelarecta =regresionLineal(x, y, contador, segundos);

    cout << "Prediccion: " << ecuaciondelarecta << " Fecha de la prediccion: " <<diaPrediccion << endl;
    return 0;
}
