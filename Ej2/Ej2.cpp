/*Una biblioteca maneja la siguiente información:

a) un archivo de libros “Libros.dat”, ordenado por código de libro y con el siguiente diseño:
a.1 Código del libro                     a.2 título del libro (30 caract)
a.3 cantidad de ejemplares               a.4 ubicación (3 caract)
a.5 código de la editorial (1..200)      a.6 autor (25 caract)

b) un archivo de editoriales “Editoriales.dat” ordenado alfabéticamente por nombre de la editorial, con el
siguiente diseño.
b.1 nombre de la editorial (25 caracteres)    b.2 código de la editorial (1..200)

c) un archivo, ”Consultas.dat”, de consultas realizadas durante el primer semestre del año. Los diferentes libros
consultados no superan los 1000, y el diseño del registro es el siguiente:
c.1 código del libro     c.2 fecha de consulta (aaaammdd)

Se pide realizar la metodología necesaria para obtener un programa que:
1) Emita un listado con los libros que tuvieron como mínimo 20 consultas en cada mes del semestre, con el
siguiente diseño, ordenado por código:

Código del Libro Título Autor Editorial Consultas en el Semestre
 1 2 3 4 5 6
 999999 xxxxxxxxx xxxxxxxxx xxxxxxxxx 999 999 999 999 999 999
 999999 xxxxxxxxx xxxxxxxxx xxxxxxxxx 999 999 999 999 999 999 

2) Grabe un archivo ordenado de mayor a menor por cantidad de libros consultados por editorial, con el siguiente
diseño:
2.1 nombre de la editorial (25 caracteres)
2.2 cantidad de libros que fueron consultados en el semestre */

#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;

struct Libros
{
    int CodDeLibro,CantDeEjemplares,CodDeEditorial;
    char TituloDelLibro[30],Ubicacion[3],AutorDelLibro[25];
};

struct Editoriales
{
    int CodDeEditorial;
    char NombreDeLaEditorial[25];
};

struct Consultas
{
    int CodDelLibro,FechaDeConsulta;
};

struct EditorialesActualizadas
{
    int TotalDeConsultasPorEditorial = 0;
    char NombreDeLaEditorial[25];
};

void InicializarVector(int CodDeLibros[]);
void InicializarMatriz(int ConsultaPorMes[][6]);
void CargaDeDatosArchivoB(FILE*ArchivoB,EditorialesActualizadas ea[]);
int CargaDeDatosArchivoC(FILE*ArchivoC,int CodDeLibros[],int ConsultaPorMes[][6]);
int BusquedaSecuencial(int CodDeLibros[],int Bus);
void Punto1(FILE*ArchivoA,EditorialesActualizadas ea[],int CodDeLibros[],int ConsultaPorMes[][6],int Volumen);
int BusquedaSecuencialArchivoA(FILE*ArchivoA,Libros &L,int Bus);
void Burbujeo(EditorialesActualizadas ea[]);
void Punto2(FILE*ArchivoD,EditorialesActualizadas ea[]);

int main()
{
    FILE*ArchivoA = fopen("Libros.dat","rb");
    FILE*ArchivoB = fopen("Editoriales.dat","rb");
    FILE*ArchivoC = fopen("Consultas.dat","rb");

    if(ArchivoA == NULL || ArchivoB == NULL || ArchivoC == NULL)
    {
        cout << "ERROR" <<endl;
    }
    else
    {
        int CodDeLibros[1000],ConsultaPorMes[1000][6],Volumen;
        EditorialesActualizadas ea[200];
        FILE*ArchivoD = fopen("ConsultasPorEditorial.dat","wb");

        InicializarVector(CodDeLibros);
        InicializarMatriz(ConsultaPorMes);

        CargaDeDatosArchivoB(ArchivoB,ea);
        Volumen = CargaDeDatosArchivoC(ArchivoC,CodDeLibros,ConsultaPorMes);

        Punto1(ArchivoA,ea,CodDeLibros,ConsultaPorMes,Volumen);

        Burbujeo(ea);

        Punto2(ArchivoD,ea);
    }

    return 0;
}

void InicializarVector(int CodDeLibros[])
{
    for(int i = 0;i < 1000;i++)
    {
        CodDeLibros[i] = 0;
    }
}

void InicializarMatriz(int ConsultaPorMes[][6])
{
    for(int i = 0;i < 1000;i++)
    {
        for(int j = 0;j < 6;j++)
        {
            ConsultaPorMes[i][j] = 0;
        }
    }
}

void CargaDeDatosArchivoB(FILE*ArchivoB,EditorialesActualizadas ea[])
{
    Editoriales E;

    fread(&E,sizeof(Editoriales),1,ArchivoB);

    while(!feof(ArchivoB))
    {
        strcpy(ea[E.CodDeEditorial - 1].NombreDeLaEditorial,E.NombreDeLaEditorial);

        fread(&E,sizeof(Editoriales),1,ArchivoB);
    }
}

int CargaDeDatosArchivoC(FILE*ArchivoC,int CodDeLibros[],int ConsultaPorMes[][6])
{
    int i = 0,Pos,Anois,Mes;
    Consultas C;

    fread(&C,sizeof(Consultas),1,ArchivoC);

    while(!feof(ArchivoC) && i < 1000)
    {
        Pos = BusquedaSecuencial(CodDeLibros,C.CodDelLibro);

        if(Pos == -1)
        {
            CodDeLibros[i] = C.CodDelLibro;

            Anois = C.FechaDeConsulta / 10000;
            Mes = (C.FechaDeConsulta - (Anois * 10000)) / 100;

            ConsultaPorMes[i][Mes - 1] = ConsultaPorMes[i][Mes - 1] + 1;

            i++;
        }
        else
        {
            Anois = C.FechaDeConsulta / 10000;
            Mes = (C.FechaDeConsulta - (Anois * 10000)) / 100;

            ConsultaPorMes[Pos][Mes - 1] = ConsultaPorMes[Pos][Mes - 1] + 1;
        }

        fread(&C,sizeof(Consultas),1,ArchivoC);
    }

    return i;
}

int BusquedaSecuencial(int CodDeLibros[],int Bus)
{
    unsigned i = 0;

    while(i < 1000 && CodDeLibros[i] != Bus)
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

void Punto1(FILE*ArchivoA,EditorialesActualizadas ea[],int CodDeLibros[],int ConsultaPorMes[][6],int Volumen)
{
    int Verificador = 0,VerificadorDeExistenciaDeLibro;
    Libros L;

    for(int i = 0;i < Volumen;i++)
    {
        for(int j = 0;j < 6;j++)
        {
            if(ConsultaPorMes[i][j] >= 20)
            {
                Verificador++;
            }
        }

        VerificadorDeExistenciaDeLibro = BusquedaSecuencialArchivoA(ArchivoA,L,CodDeLibros[i]);

        if(VerificadorDeExistenciaDeLibro != -1)
        {
            if(Verificador == 6)
            {
                 cout << "Codigo del Libro: " << CodDeLibros[i] << " - " << "Titulo del libro: " << L.TituloDelLibro << " - " << "Editorial: " << ea[L.CodDeEditorial - 1].NombreDeLaEditorial << " - " << "Consultas en Enero: " << ConsultaPorMes[i][0] << " - " << "Consultas en Febrero: " << ConsultaPorMes[i][1] << " - " << "Consultas en Marzo: " << ConsultaPorMes[i][2] << " - " << "Consultas en Abril: " << ConsultaPorMes[i][3] << " - " << "Consultas en Mayo: " << ConsultaPorMes[i][4] << " - " << "Consultas en Junio: " << ConsultaPorMes[i][5] <<endl;
            }

            ea[L.CodDeEditorial - 1].TotalDeConsultasPorEditorial = ea[L.CodDeEditorial - 1].TotalDeConsultasPorEditorial + 1;
        }

        Verificador = 0;

        fseek(ArchivoA,sizeof(Libros),SEEK_SET);
    }
}

int BusquedaSecuencialArchivoA(FILE*ArchivoA,Libros &L,int Bus)
{
    int i = 0;

    fread(&L,sizeof(Libros),1,ArchivoA);

    while(!feof(ArchivoA) && L.CodDeLibro != Bus)
    {
        i++;

        fread(&L,sizeof(Libros),1,ArchivoA);
    }

    if(feof(ArchivoA))
    {
        return -1;
    }
    else
    {
        return i;
    }
}

void Burbujeo(EditorialesActualizadas ea[])
{
    EditorialesActualizadas Aux;
    unsigned i = 0, j;
    bool Cambio;

    do
    {
        Cambio = false;

        for(j = 0;j < 200 - i;j++)
        {
            if(ea[j].TotalDeConsultasPorEditorial < ea[j+1].TotalDeConsultasPorEditorial)
            {
                Aux = ea[j];
                ea[j] = ea[j+1];
                ea[j+1] = Aux;
                Cambio = true;
            }
        }

        i++;
    }
    while(i < 200 && Cambio);
}

void Punto2(FILE*ArchivoD,EditorialesActualizadas ea[])
{
    for(int i = 0;i < 200;i++)
    {
        fwrite(&ea[i],sizeof(EditorialesActualizadas),1,ArchivoD);
    }
}
