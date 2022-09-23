/*Un organismo municipal de la provincia de Buenos Aires quiere hacer un programa para
llevar un control sobre las infracciones de tránsito cometidas en el último año, por los no más
de 1000 vehículos registrados en su jurisdicción.
Para ello cuenta con los siguientes archivos:
“Vehículos.dat” con un registro por vehículo. Cada registro contiene:
• Patente (alfanumérico de 6 caracteres)
• Apellido y nombre del titular (alfanumérico de 30 caracteres).
• Año patentamiento.
 
“Infracciones.dat” con un registro por cada infracción cometida en la provincia de Buenos
Aires en el último año, ordenado por código de infracción. Si el vehículo que ha cometido la
infracción tiene más de 20 años se le aplica un descuento del 20%. Cada registro contiene:
• Código de infracción (entero)
• Patente (alfanumérico de 6 caracteres)
• Día (entero)
• Mes (entero)
• Importe (real)

Se pide.
1. Generar un archivo “InfracMunicipio.dat” con un registro por vehículo registrado en
el municipio que haya cometido infracciones en el último año en la provincia de
Buenos Aires. Cada registro debe contener:
• Patente
• Apellido y nombre del titular
• Importe total por infracciones cometidas

2. Informar por cada código de infracción la cantidad de vehículos registrados en el
municipio que la hayan cometido e importe total abonado por todos los vehículos del
municipio que hayan cometido dicha infracción.

3. Informar la cantidad de vehículos registrados en el municipio que no han registrado
infracciones en el último año*/

#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;

struct Vehiculos
{
    int FechaDePatentamiento;
    char Patente[6],NombreCompleto[30];
};

struct Infracciones
{
    int CodDeInfraccion,Dia,Mes;
    float Importe;
    char Patente[6];
};

struct InfraccionesActualizadas
{
    int TotalPorInfracciones = 0;
    char Patente[6],NombreCompleto[30];
};

void Inicializar(int w[]);
void CargaDeDatos(FILE*ArchivoA,InfraccionesActualizadas v[],int FechaDePatentamiento[]);
void Punto1(FILE*ArchivoB,FILE*ArchivoC,InfraccionesActualizadas v[],int FechaDePatentamiento[],int &CantidadDeVehiculosSinInfraccion);
int BusquedaSecuencial(InfraccionesActualizadas v[],Infracciones &I);
void Punto2(FILE*ArchivoB,InfraccionesActualizadas v[]);

int main()
{
    FILE*ArchivoA = fopen("Vehículos.dat","rb");
    FILE*ArchivoB = fopen("Infracciones.dat","rb");

    if(ArchivoA == NULL || ArchivoB == NULL)
    {
        cout << "ERROR" <<endl;
    }
    else
    {
        int CantidadDeVehiculosSinInfraccion = 0,FechaDePatentamiento[1000];
        InfraccionesActualizadas v[1000];
        FILE*ArchivoC = fopen("InfracMunicipio.dat","wb");

        Inicializar(FechaDePatentamiento);

        CargaDeDatos(ArchivoA,v,FechaDePatentamiento);

        Punto1(ArchivoA,ArchivoC,v,FechaDePatentamiento,CantidadDeVehiculosSinInfraccion);

        fseek(ArchivoB,0,SEEK_SET);

        Punto2(ArchivoB,v);

        cout << "Cantidad de vehículos registrados en el municipio que no han registrado infracciones en el último año: " << CantidadDeVehiculosSinInfraccion <<endl;
    }

    return 0;
}

void Inicializar(int FechaDePatentamiento[])
{
    for(int i = 0;i < 1000;i++)
    {
        FechaDePatentamiento[i] = 0;
    }
}

void CargaDeDatos(FILE*ArchivoA,InfraccionesActualizadas v[],int FechaDePatentamiento[])
{
    int i = 0;
    Vehiculos V;

    fread(&V,sizeof(Vehiculos),1,ArchivoA);

    while(!feof(ArchivoA) && i < 1000)
    {
        strcpy(v[i].Patente,V.Patente);
        strcpy(v[i].NombreCompleto,V.NombreCompleto);
        FechaDePatentamiento[i] = V.FechaDePatentamiento;

        i++;
    }
}

void Punto1(FILE*ArchivoB,FILE*ArchivoC,InfraccionesActualizadas v[],int FechaDePatentamiento[],int &CantidadDeVehiculosSinInfraccion)
{
    int Pos;
    Infracciones I;

    fread(&I,sizeof(Infracciones),1,ArchivoB);

    while(!feof(ArchivoB))
    {
        Pos = BusquedaSecuencial(v,I.Patente);

        if(Pos != -1)
        {
            if((2022 - FechaDePatentamiento[Pos]) == 20)
            {
                I.Importe = I.Importe - ((20*I.Importe)/100);
                v[Pos].TotalPorInfracciones = v[Pos].TotalPorInfracciones + I.Importe;
            }
            else
            {
                v[Pos].TotalPorInfracciones = v[Pos].TotalPorInfracciones + I.Importe;
            }

            fread(&I,sizeof(Infracciones),1,ArchivoB);
        }

        fread(&I,sizeof(Infracciones),1,ArchivoB);
    }

    for(int i = 0;i < 1000;i++)
    {
        if(v[i].TotalPorInfracciones != 0)
        {
            fwrite(&v[i],sizeof(InfraccionesActualizadas),1,ArchivoC);
        }
        else
        {
            CantidadDeVehiculosSinInfraccion++;
        }
    }
}

int BusquedaSecuencial(InfraccionesActualizadas v[],Infracciones &I);
{
    unsigned i = 0;

    while(i < 1000 && strcmp(v[i].Patente,I.Patente) != 0)
    {
         i++;
    }

    if(i == 1000)
    {
        return -1;
    }
    else
    {
        return i;
    }
}

void Punto2(FILE*ArchivoB,InfraccionesActualizadas v[])
{
    int CodDeInfraccion,Pos,TotalPorVehiculo = 0,CantVehiculos = 0;
    Infracciones I;

    fread(&I,sizeof(Infracciones),1,ArchivoB);

    while(!feof(ArchivoB))
    {
        CodDeInfraccion = I.CodDeInfraccion;

        do
        {
            Pos = BusquedaSecuencial(v,I.Patente);

            if(Pos != -1)
            {
                TotalPorVehiculo = TotalPorVehiculo + I.Importe;
                CantVehiculos++;
            }

            fread(&I,sizeof(Infracciones),1,ArchivoB);
        }
        while(!feof(ArchivoB) && CodDeInfraccion == I.CodDeInfraccion);

        cout << "Codigo de infracion: " << CodDeInfraccion << " - " << "Total abonado por todos los vehículos que cometieron dicha infraccion: " << TotalPorVehiculo << " - " << "Cantidad de vehiculos que cometieron dicha infraccion: " << CantVehiculos <<endl;
        cout << "---------------------------------" <<endl;

        TotalPorVehiculo = 0;
        CantVehiculos = 0;
     }
}


