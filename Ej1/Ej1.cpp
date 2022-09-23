/*Dado el archivo “ALUMNOS.dat” con los datos personales de alumnos ordenado por legajo, con el siguiente
diseño:

a.1 Legajo                       a.2 Apellido y nombre ( 30 caracteres)
a.3 Domicilio (20 caracteres)    a.4 Código postal
a.5 Teléfono (10 caracteres)     a.6 Año de ingreso

Y otro archivo sin orden llamado “NOVEDADES.dat”, con cantidad máxima de registros es 100. Posee las
actualizaciones (altas, bajas, y modificaciones) a ser aplicadas, donde cada registro contiene además de todos los
campos de Alumnos.dat un código de operación (‘A’= Alta, ‘B’= Baja, ‘M’= Modificación).
 Se pide desarrollar todos los pasos necesarios para realizar un programa que genere un archivo actualizado
“ALUMACTU.dat” con el mismo diseño.*/

#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;

struct AlumnosDesactualizados
{
    int Legajo,CodPostal,Ingreso,Telefono;
    char NombreCompleto[30],Domicilio[20];
};

struct AlumnosActualizados
{
    int Legajo,CodPostal,Ingreso,Telefono;
    char NombreCompleto[30],Domicilio[20],CodDeOperacion[1];
};

int CargaDeDatos(FILE*ArchivoB,AlumnosActualizados v[]);
void Burbujeo(AlumnosActualizados v[],unsigned Volumen);
void Punto1(FILE*ArchivoA,FILE*ArchivoC,AlumnosActualizados v[],unsigned Volumen);

int main()
{
    int Volumen;
    FILE*ArchivoA = fopen("ALUMNOS.dat","rb");

    if(ArchivoA == NULL)
    {
        cout << "ERROR" <<endl;
    }
    else
    {
        FILE*ArchivoB = fopen("NOVEDADES.dat","rb");

        if(ArchivoB == NULL )
        {
            cout << "ERROR" <<endl;
        }
        else
        {
            AlumnosActualizados v[100];
            FILE*ArchivoC = fopen("ALUMACTU.dat","wb");

            Volumen = CargaDeDatos(ArchivoB,v);

            Burbujeo(v,Volumen);

            Punto1(ArchivoA,ArchivoC,v,Volumen);

            fclose(ArchivoA);
            fclose(ArchivoB);
            fclose(ArchivoC);
        }
    }

    return 0;
}

int CargaDeDatos(FILE*ArchivoB,AlumnosActualizados v[])
{
    int i = 0;
    AlumnosActualizados A;

    fread(&A,sizeof(AlumnosActualizados),1,ArchivoB);

    while(!feof(ArchivoB) && i < 100)
    {
        v[i].Legajo = A.Legajo;

        strcpy(v[i].NombreCompleto,A.NombreCompleto);
        strcpy(v[i].Domicilio,A.Domicilio);
        strcpy(v[i].CodDeOperacion,A.CodDeOperacion);

        v[i].CodPostal = A.CodPostal;
        v[i].Telefono = A.Telefono;
        v[i].Ingreso = A.Ingreso;

        i++;

        fread(&A,sizeof(AlumnosActualizados),1,ArchivoB);
    }

    return i;
}

void Burbujeo(AlumnosActualizados v[],unsigned Volumen)
{
    AlumnosActualizados Aux;
    unsigned i = 1, j;
    bool Cambio;

    do
    {
        Cambio = false;

        for(j = 0;j < Volumen - i;j++)
        {
            if(v[j].Legajo > v[j+1].Legajo)
            {
                Aux = v[j];
                v[j] = v[j+1];
                v[j+1] = Aux;
                Cambio = true;
            }
        }

        i++;
    }
    while(i < Volumen && Cambio);
}

void Punto1(FILE*ArchivoA,FILE*ArchivoC,AlumnosActualizados v[],unsigned Volumen)
{
    int i = 0;
    AlumnosDesactualizados A;

    fread(&A,sizeof(AlumnosDesactualizados),1,ArchivoA);

    while(!feof(ArchivoA) && i < Volumen)
    {
        if(A.Legajo < v[i].Legajo)
        {
            fwrite(&A,sizeof(AlumnosDesactualizados),1,ArchivoC);
            fread(&A,sizeof(AlumnosDesactualizados),1,ArchivoA);
        }
        else
        {
            if(A.Legajo == v[i].Legajo)
            {
                fwrite(&v[i],sizeof(AlumnosActualizados),1,ArchivoC);
                fread(&A,sizeof(AlumnosDesactualizados),1,ArchivoA);

                i++;
            }
            else
            {
                fwrite(&v[i],sizeof(AlumnosActualizados),1,ArchivoC);

                i++;
            }
        }
    }

    while(!feof(ArchivoA))
    {
        fwrite(&A,sizeof(AlumnosDesactualizados),1,ArchivoC);
        fread(&A,sizeof(AlumnosDesactualizados),1,ArchivoA);
    }

    for(int k = i;k < Volumen;k++)
    {
        fwrite(&v[k],sizeof(AlumnosActualizados),1,ArchivoC);
    }
}
