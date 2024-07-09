#include <bits/stdc++.h>
#include <regex>
#define endl '\n'
using namespace std;

int main()
{
    //global
    int opc,opc1,i,j,k;
    set<string> baseAlphabet;
    string letterInLetters, element;

    //1-Generacion de alfabeto
    char start,end;
    int total;
    string letters;

    //2-Validacion de cadenas
    string w1,w2;
    set<char> w1Check, w2Check;
    int flag1,flag2;

    //3-Subsecuencia => subcadena/prefijo/sufijo
    int notSubSec, notSubStr, yesPrefix, yesSuffix;
    stack<char> checkWord;
    int pointw1, pointw2, aux1, aux2;
    bool flag, flags;
    //4-Generacion de lenguajes de forma aleatoria
    int wordNumber, wordSize;
    int randlower,randupper,randlim;
    string randLetter, generatedWord;
    set<string> lang1, lang2;

    //5-Diferencia de lenguajes
    int foundflag;
    set<string> dlang;

    //6-Potencia de un alfabeto
    int potencia, negflag;
    string summedUpLetter;
    set<string> invertedAlphabet, raisedAlfabet, tempAlfabet;
    //7-Validacion de vocales con regex
    regex pattern1("^[-+]?\\d*\\.?\\d+\\s*[-+]\\s*\\d*\\.?\\d+i$");
    regex pattern2("^[-+]?\\d*\\.?\\d+\\s*[-+]+i\\s*\\d*\\.?\\d$");
    string checkString;
    
    //Ingresar la opcion y redirigir a las funciones
    do{
        cout<<"Ingrese la opcion deseada (main): ";
        cin>>opc;
        switch (opc)
        {
        case 0:
            break;
            
        case 1:
            /* Crear un alfabeto DONE*/
            
            cout<<"Ingrese la opcion deseada (submenu opc=1): ";
            cin>>opc1;
            baseAlphabet.clear();
            switch (opc1)
            {
            case 1:
                //por rangos
                cout<<"Ingrese el inicio: ";
                cin>>start;
                cout<<"Ingrese el final: ";
                cin>>end;
                cout<<"Alfabeto generado: ";
                for(char ch=start; ch<=end;ch++){
                    letters=ch;
                    baseAlphabet.insert(letters);
                    cout<<ch<<" ";
                }
                cout<<endl;
                break;
            case 2:
                //1 por 1
                cout<<"Ingrese el numero de caracteres que va a ingresar: ";
                cin>>total;
                cout<<"Ingrese los strings separados por espacios: ";
                for(i=0;i<total;i++){
                    cin>>letters;
                    baseAlphabet.insert(letters);
                }
                cout<<"Alfabeto generado: ";
                for(const auto& element:baseAlphabet){
                    cout<<element<<" ";
                }
                cout<<endl;
                break;
            
            default:
                break;
            }
            break;
        case 2:
            /* Validacion de cadenas sobre el alfabeto DONE*/
            cout<<"Ingrese la primer cadena: ";
            cin>>w1;
            cout<<"Ingrese la segunda cadena: ";
            cin>>w2;
            flag1=1;
            flag2=1;
            for(i=0;i<w1.size();i++){
                letterInLetters=w1[i];
                auto it=baseAlphabet.find(letterInLetters);
                if(it==baseAlphabet.end()){
                    flag1=0;
                    break;
                }
            }
            if(flag1){
                cout<<"cadena 1 valida";
            }else{
                cout<<"cadena 1 invalida en: "<<w1[i];
            }
            cout<<endl;
            for(i=0;i<w2.size();i++){
                letterInLetters=w2[i];
                auto it=baseAlphabet.find(letterInLetters);
                if(it==baseAlphabet.end()){
                    flag2=0;
                    break;
                }
            }
            if(flag2){
                cout<<"cadena 2 valida";
            }else{
                cout<<"cadena 2 invalida en: "<<w2[i];
            }
            cout<<endl;
            break;
        case 3:
            /* Verificar si w1 es prefijo, sufijo, subcadena o subsecuencia de w2 DONE*/
            cout<<"w1: "<<w1<<" w2: "<<w2<<endl;
            if(w1.size()==0 || w2.size()==0){
                cout<<"No se pudo realizar, una o mas cadenas estan vacias"<<endl;
                break;
            }else if(w1.size()>w2.size()){
                cout<<"No se pudo realizar, w1 es mas grande que w2"<<endl;
                break;
            }else if(w1==w2){
                cout<<"Es subsecuencia, subcadena, prefijo y sufijo"<<endl;
                break;
            }
            //Revisar si w1 es una subsecuencia de w2 alo ola
            reverse(w1.begin(),w1.end());
            //cout<<"cadena reverseada"<<endl;
            for(i=0;i<w1.size();i++){
                checkWord.push(w1[i]);
            }
            //cout<<"ingresada a la pila"<<endl;
            for(i=0;i<w2.size();i++){
                if(checkWord.empty()){
                    //cout<<"pila vacia"<<endl;
                    break;
                }
                if(w2[i]==checkWord.top()){
                    checkWord.pop();
                }
            }
            //cout<<"checeo terminado"<<endl;
            if(!checkWord.empty()){
                cout<<"No es subsecuencia, subcadena, prefijo o sufijo"<<endl;
                break;
            }
            //por lo menos es subsecuencia || verificar si esta seriada para decir que es subcadena
            reverse(w1.begin(),w1.end());
            notSubSec=1, yesPrefix=0, yesSuffix=0,flag=0, flags=0;
            //iterar sobre w2 hasta encontrar w1[0] w1 debe de caber en el resto de w2
            //verificar si es subcadena y guardar los pointers
            //si no rompe notSubstr=0
            //si rompe verificar el siguiente elemento de w2 que sea w1[0], si llega al final del loop no es subcadena
            //cout<<"checked subsecuence"<<endl;
            for(i=0;i<=(w2.size()-w1.size());i++){
                //flag=0, flags=0;
                if(w2[i]==w1[0]){
                    //encontramos un posible inicio
                    for(j=0;j<w1.size();j++){
                        //cout<<"compared: "<<w2[i+j]<<" "<<w1[j]<<endl;
                        if(w2[i+j]!=w1[j]){
                            //cout<<"ent cond"<<endl;
                            flags=1;
                            flag=1;
                            //cout<<"flag"<<endl;
                            break;
                        }
                        //out<<"found shit"<<endl;
                        flags=0;
                        flag=0;
                    }
                    if(flags==0){
                        //cout<<"assigned two pointers"<<endl;
                        aux1=i;
                        aux2=j;
                        break;
                    }
                }
            }
            if(flag){
                cout<<"Es Subsecuencia"<<endl;
                break;
            }
            //es subcadena solo hay que verificar donde
            if(aux1==0){
                /*Solo empieza en 0*/
                cout<<"Es Subsecuencia, Subcadena propia y Prefijo propio"<<endl;
            }else if(aux2==(w2.size()-1)){
                /*Solo termina en size-1*/
                cout<<"Es Subsecuencia, Subcadena propia y Sufijo propio"<<endl;
            }else{ /*Esta entre toda la cadena*/
                cout<<"Es Subsecuencia y Subcadena propia"<<endl;
            }
            break;
        case 4:
            /* Generar lenguajes DONE
            agarrar longitud
            forn(i,wordNumber)
                forn(j,wordSize)
                agarrar letra random*/
            cout<<"Ingrese el numero de palabras: ";
            cin>>wordNumber;
            cout<<"Ingrese la longitud de las palabras: ";
            cin>>wordSize;
            if(wordNumber>pow(baseAlphabet.size(), wordSize)){
                cout<<"No hay suficientes combinaciones"<<endl;
                break;
            }
            lang1.clear();
            while(lang1.size()<wordNumber){
                generatedWord="";
                for(j=0;j<wordSize;j++){
                    //make random number that is in the set
                    randlim=rand()%(baseAlphabet.size());
                    k=0;
                    for(auto it=baseAlphabet.begin();it!=baseAlphabet.end();++it){
                        if(k==randlim){
                            generatedWord+=*it;
                            break;
                        }
                        k++;
                    }
                }
                lang1.insert(generatedWord);
            }
            cout<<"Lenguaje 1 generado: ";
            for(const auto& word:lang1){
                cout<<word<<" ";
            }
            cout<<endl;
            lang2.clear();
            while(lang2.size()<wordNumber){
                generatedWord="";
                for(j=0;j<wordSize;j++){
                    //make random number that is in the set
                    randlim=rand()%(baseAlphabet.size());
                    k=0;
                    for(auto it=baseAlphabet.begin();it!=baseAlphabet.end();++it){
                        if(k==randlim){
                            generatedWord+=*it;
                            break;
                        }
                        k++;
                    }
                }
                lang2.insert(generatedWord);
            }
            cout<<"Lenguaje 2 generado: ";
            for(const auto& word:lang2){
                cout<<word<<" ";
            }
            cout<<endl;
            break;
        case 5:
            /* Diferencia de lenguajes assume we have both generated DONE*/
            /*iterador - 2 pointers uno sobre cada lenguaje
            if == nada  alt: search for element if not found then insert
            if diferente meter a dlang*/
            dlang.clear();
            for(const auto& word1: lang1){
                foundflag=0;
                for(const auto& word2: lang2){
                    if(word1==word2){
                        foundflag=1;
                        break;
                    }
                }
                if(foundflag!=1){
                    dlang.insert(word1);
                } 
            }
            cout<<"Diferencia de l1-l2: ";
            for(const auto& word: dlang){
                cout<<word<<" ";
            }
            cout<<endl;
            break;
        case 6:
            /* Potencia de un alfabeto  DONE*/
            //n=0 => se vacia
            //n>0 => combinas
            //n<0 => inviertes todo y LUEGO combinas
            negflag=0;
            cout<<"Ingrese la potencia para elevar: ";
            cin>>potencia;
            if(potencia==0){
                baseAlphabet.clear();
            }else if(potencia<0){
                negflag=1;
                potencia=-(potencia);
            }
            for(const auto& thingy: baseAlphabet){
                raisedAlfabet.insert(thingy);
            }
            for(i=1;i<potencia;i++){
                for(const auto& word1: baseAlphabet){
                    for (const auto& word2: raisedAlfabet) {
                        summedUpLetter=word1+word2;
                        tempAlfabet.insert(summedUpLetter);
                    }
                }
                raisedAlfabet.clear();
                for(const auto& word: tempAlfabet){
                    raisedAlfabet.insert(word);
                }
                tempAlfabet.clear();
            }
            cout<<"Alfabeto nuevo: ";
            if(negflag){
                for (const auto& word:raisedAlfabet) {
                    element=word;
                    reverse(element.begin(),element.end());
                    invertedAlphabet.insert(element);
                    cout<<element<<" ";
                }
                cout<<endl<<"tamaño: "<<raisedAlfabet.size()<<endl;
                break;
            }        
            if(raisedAlfabet.size()==0){
                cout<<"lambda";
            }else{
                for(const auto& thingy: raisedAlfabet){
                    cout<<thingy<<" ";
                }
            }
            cout<<endl<<"tamaño: "<<raisedAlfabet.size()<<endl;
            raisedAlfabet.clear();
            break;
        case 7:
            /* validacion de vocales en orden con REGEX -- DONE */
            /*formato propuesto: 1 solo numero  x+-iy*/
            cout<<"Ingrese el numero complejo en la forma x+-iy: ";
            cin>>checkString;
            if(regex_match(checkString,pattern1)||regex_match(checkString,pattern2)){
                cout<<"Palabra Correcta";
            }else{
                cout<<"Palabra Incorrecta";
            }
            cout<<endl;
            break;
        default:
            cout<<"Opcion no valida"<<endl;
            break;
        }
    }while(opc!=0);
    
    return 0;
}