#include <iostream>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <ctime>
#include <cstring>
#include <cmath>

using namespace std;

void calcularFecha(float numeroFecha){

    time_t seconds = numeroFecha;

    // Transformar los segundos en una estructura tm
    struct tm *timeinfo = gmtime(&seconds);

    // Imprimir la fecha y hora en el formato dd/mm/aaaa hh:mm:ss
    cout << timeinfo->tm_mday << "/" << timeinfo->tm_mon + 1 << "/" << timeinfo->tm_year + 1900 << " ";
    cout << timeinfo->tm_hour << ":" << timeinfo->tm_min << ":" << timeinfo->tm_sec << endl;
}

int main()
{
    ifstream miArchivo;
    miArchivo.open("datos_examen.csv");
    string linea = "";
    float contador = 0;
    float sumaAccidente = 0;
    float sumaYPromAccidente = 0;
    float sumaFecha = 0;
    float sumatoriaXY = 0;
    float sumatoriaXCuadrado = 0;
    getline(miArchivo,linea);
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
        //Convertimos los componentes a números y asignamos a la estructura tm
        tm t;
        t.tm_year = atoi(year_str) - 1900;
        t.tm_mon = atoi(month_str) - 1;
        t.tm_mday = atoi(day_str);

        // Convertimos la estructura tm a un valor de tiempo en segundos
        time_t segundos = mktime(&t);
        sumaFecha = sumaFecha + segundos;
        getline(inputString,tempString,';');
        accidentes = atoi(tempString.c_str());
        contador = contador + 1;
        sumaAccidente = sumaAccidente + accidentes;
        sumatoriaXY = sumatoriaXY +(segundos * accidentes);
        sumatoriaXCuadrado = sumatoriaXCuadrado + (segundos*segundos);
        sumaYPromAccidente = sumaYPromAccidente + (accidentes*accidentes);
        linea = "";

    }
    float promedioAccidente = sumaAccidente/contador;
    float promeFecha = sumaFecha/contador;
    float nxy = contador * promeFecha * promedioAccidente;
    float nx2 = contador* promeFecha * promeFecha;
    float pendiente = (sumatoriaXY - nxy)/(sumatoriaXCuadrado - nx2);
    float ordenada = promedioAccidente - pendiente * promeFecha;
    float sumatoriaYCuadrado = sumaYPromAccidente;
    double resultErEs = sqrt((sumatoriaYCuadrado- ordenada*sumaAccidente-pendiente*sumatoriaXY)/(contador-2));
    cout <<"n: " <<contador << endl;
    cout <<"Y promedio: " << promedioAccidente << endl;
    cout << "X promedio: "  << promeFecha << endl;
    cout << "Sumatoria X*Y: " << sumatoriaXY << endl;
    cout << "n * X promedio * Y promedio: " << nxy << endl;
    cout << "sumatoria X Cuadrado: " << sumatoriaXCuadrado << endl;
    cout << "n* X promedio al cuadrado: " << nx2 << endl;
    cout << "pendiente (b) " << pendiente << endl;
    cout << "ordenada (a) " << ordenada << endl;
    cout << "sumatoria Y cuadrada: " << sumatoriaYCuadrado << endl;
    cout << "suma de y: " << sumaAccidente <<endl;
    cout << "Error Estandar: " << resultErEs;

    string diaSigiente = "2-09-2030";
    char* date_str = strdup(diaSigiente.c_str());
    char* day_str = strtok(date_str, "-");
    char* month_str = strtok(NULL, "-");
    char* year_str = strtok(NULL, "-");
        //Convertimos los componentes a números y asignamos a la estructura tm
    tm t;
    t.tm_year = atoi(year_str) - 1900;
    t.tm_mon = atoi(month_str) - 1;
    t.tm_mday = atoi(day_str);

        // Convertimos la estructura tm a un valor de tiempo en segundos
    time_t seg = mktime(&t);
    float ecuaciondelaRecta = ordenada - (pendiente * seg);
    cout << seg << endl;
    cout << "Prediccion: " << ecuaciondelaRecta << endl;
    return 0;
}
