/** main.cpp
 *
 *  Startet Hauptmen� gibt die Auswahl der Verf�gbaren Matrixoperationen an und startet dann eben jene
 *
 */

#include <iostream>
#include <string>
#include <stdio.h>      /* printf, fgets */
#include <stdlib.h>     /* atoi */
#include <CMatrix>


using namespace std;

int main()
{
    CMatrix2<int> m(5,5);
    string e;                                           //In e wird die eingabe als String gespeichert
    int b = 0;                                          //b ist der wert der f�r die do while schleife als bedingung benutzt
    cout<<"Haptmenue:"<<endl;
    cout<<"Funktion             - Eingabe"<<endl;
    cout<<"Matrix Addition      - 1"<<endl;
    cout<<"Matrix Subtraktion   - 2"<<endl;
    cout<<"Matrix Multiplikation- 3"<<endl;
    cout<<"Skalarmultiplikation - 4"<<endl;
    cout<<"Gauss-Elimination    - 5"<<endl;
    cout<<"Determinante         - 6"<<endl;
    cout<<"Inversion            - 7"<<endl;
    do{                                                 //eingabe die �ber die Auszuf�hrende Funktion entscheidet
        cout<<"Eingabe:";
        cin>>e;
        b= atoi(e.c_str());
    }
    while(b!=(1||2||3||4||5||6||7));



    cout<<e<<endl;
    return 0;
                                                        //Was ist Gr�n und wenn man auf den Knopf dr�ckt Rot ?
}
