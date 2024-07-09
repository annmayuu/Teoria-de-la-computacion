#include <bits/stdc++.h>
#define endl '\n'
#define forn(i,n) for(int i=0;i<int(n);i++)
#define forr(i,a,b) for (int i = int(a); i <= int(b); i++)
#define fore(obj, x) for(const auto& x: obj)
#define pb push_back
typedef long long lli;
using namespace std;

/* RESUMEN DE LA IMPLEMENTACION:
-El automata se trata como un grafo dirigido, de esta forma un string se puede
 validar a partir de si existe un camino para llegar a un estado valido

-El caso trivial ocurre cuando el automata tiene una forma de arbol, ya que solo seria:
 1. en caso de que el nodo pueda aplicar arden, entonces se aplica y sustituye
 2. en caso de que no se pueda aplicar arden, solo se sustituya

-Hay variables globales para evitar problemas *****MUY FEOS***** de implementacion con recursion

-Hay muchos prints que estan comentados con comentario de una sola linea por lo que:

1. Para la descripcion de cosas, se utilizara SIEMPRE el comentario de varias lineas
2. Para los comentarios de prints se utilizara SIEMPRE el comentario de una linea

Cualquier cosa como pb y forn estan definidas al inicio del archivo
*/

/* VARIABLES GLOBALES
    a: conjunto de simbolos del alfabeto
    states: conjunto de estados que tiene el automata
    valid: conjunto de estados validos del automata
    solved: conjunto de estados resueltos* del automata
    path: conjunto de no
    DFA: Arreglo de mapas para definir el automata como un grafo (Todo el algoritmo se hace sobre esta)**
    k: estado extra (que se define como n+1, n siendo el numero total de estados) para indicar que es de aceptacion
    
    Para el DFA probablemente se pueda implementar como 2 matrices nxn con n siendo el numero de estados.
    Una matriz tendria que ser de conexiones (como un grafo o con implementacion de vector) y la segunda
    de las transiciones(que no veo una manera de implementarlo si no es una matriz nxn ya que pierdes los indices de 
    cada transicion) Pero intentalo como quieras

    DFA[i] indica el vector que describe los caminos para llegar al nodo j, y esta compuesto de la siguiente manera:
        si j indica el indice del nodo al que esta conectado entonces:
        j->*simbolo para realizar latransicion a j aqui*

    *Por estados resueltos me refiero a los que en el mapa esta unicamente depende de k
    **El algoritmo esta descrito mas abajo para tenerlo con calmita
*/

set<string> a;
set<int> states, valid, solved, path;
vector<map<int,string>> DFA;
int k;

/* Funciones Para revisar rapidamente si algo es algun tipo de dato, basicamente busca si esta en el set
RETURN TRUE si esta en el set
RETURN FALSE si no esta en el set
ejemplo isSolved(0) regresa false al inicio (ya que 0 no esta resuelto), pero inPath(0) regresa true al inicio
ya que 0 si ha sido visitado en el camino actual
*/

bool isSolved(int node){
    return solved.find(node)!=solved.end();
}

bool inPath(int node){
    return path.find(node)!=path.end();
}

/* Funcion para separar la linea en palabras 
toma en cuenta que esto implica que en el archivo todo esta separado por espacios
ejemplo:
double PI = 3.1416 , CteGrav = -6.674E-19 ; (todo separado por espacios)
"double", "PI", "=", "3.1416", "," , "CteGrav", "=", "-6.674E-19", ";"

Como lo hace:
-> Se manda a llamar con un string de argumento (line) que es la linea que queremos tokenizar
*/
vector<string> tokenizar(string line){
    int j=0;
    string current;
    vector<string> tokens;
    /*-> Verifica si empieza con espacio y si es asi lo quita*/
    while(line[j]==' '){
        j++;
    }
    //cout<<j<<" "<<line.size()-1<<endl;
    /*-> Empieza desde la posicion donde no hay espacio y añade el caracter hasta que encuentra espacio*/
    forr(i,j,line.size()-1){
        /*-> Cuando encuentra espacio añade al arreglo la palabra y se salta el espacio, vacia el string e inicia otra vez*/
            if(line[i]==' '){
                tokens.push_back(current);
                i++;
                current.clear();
            }
            current+=line[i];
        }
    /*-> Como sobra una palabra siempre la añade al final del for*/
    if(!current.empty()){
        tokens.push_back(current);
        current.clear();
    }
    return tokens;
}

/*Por si quieres imprimir algo para debugear o algo asi*/
void printTokens(vector<string> tokens){
    forn(i,tokens.size()){
        cout<<tokens[i]<<endl;
    }
    cout<<tokens.size()<<endl<<endl;
}

void printMap(map<int,string> node, int id){
    cout<<"mapa "<<id<<":"<<endl;
    fore(node,x){
        cout<<x.first<<" -> "<<x.second<<endl;
    }
    return;
}

/* Funciones para llenar el alfabeto (aFill), los estados (sFill) y los estados de aceptacion (vFill)
    -Para todos:
        RECIBE: La linea que contiene la informacion correspondiente directo desde la lectura de archivo
        o sea, sin procesar.

        REGRESA: Nada kjaskjaskja pq es variable global

        Cada funcion manda a tokenizar la linea primero para poder procesar los tokens de forma mas sencilla

        tokens: vector de strings que contiene las palabras separadas
*/

/* aFill:
    Como el alfabeto siempre viene separado por espacios (yo le quite la , entre simbolos) entonces
    por cada token simplemente es insertarlo al set
*/
void aFill(string line){
    vector<string> tokens=tokenizar(line);
    forn(i,tokens.size()){
        a.insert(tokens[i]);
    }
}

/* sFill:
    Variables:
        -cId: entero auxiliar para almacenar el numero de nodo que estamos procesando actualmente

    Como la informacion viene en el siguiente formato:
        X *numero de nodo*
    Entonces debemos de separar el simbolo de transicion y el numero de nodo.

    Para esto se hace basicamente un atoi desde la posicion 1 del token (arreglo indexado en 0)
    y el resultado se inserta en states

    Mucha acrobacia porque no pude hacer funcionar el atoi :c

    NOTA: Como obtenemos el numero maximo que tiene un estado, de una vez declaramos el valor de k como 
    el numero maximo que obtuvimos del ciclo + 1 (ya que este estado es garantizado que no existe y es 
    NECESARIO para la forma en la que lo implemente) 
*/
void sFill(string line){
    vector<string> tokens=tokenizar(line);
    int cID,max;
    forn(i,tokens.size()){
        cID=tokens[i][1]-'0';
        forr(j,2,tokens[i].size()-1){
            cID*=10;
            cID+=cID=tokens[i][j]-'0';
        }
        states.insert(cID);
        max=i;
    }
    k=max+1;
}

/* vFill:
    Hace exactamente lo mismo que sFill pero para la linea de estados de aceptacion, probablemente pude haberlo puesto en 1
    sola funcion xd
*/
void vFill(string line){
    vector<string> tokens=tokenizar(line);
    int cID;
    forn(i,tokens.size()){
        cID=tokens[i][1]-'0';
        forr(j,2,tokens[i].size()-1){
            cID*=10;
            cID+=tokens[i][j]-'0';
        }
        valid.insert(cID);
    }
}

/* Funcion para filtrar los simbolos

    RECIBE: una linea completa del archivo de texto que se lee

    REGRESA: un vector de strings deseado descrito en el ultimo parrafo de este comentario xd

    Como la entrada no me deja procesarla directamente, entonces debo filtrar para que 
    X0 = 0X1 + 1X2 => 0X1 , 1X2

    Si tengo la entrada tokenizada como {0X1, 0X2} entonces ya puedo convertir de una manera
    similar a como hice los estados de aceptacion.
*/

vector<string> cleanUpInput(string line){
    vector<string> ans, tokens=tokenizar(line);
    string s="+", d="/";
    forr(i,2,tokens.size()-1){
        if(tokens[i]!=s && tokens[i]!=d){
            ans.pb(tokens[i]);
        }
    }
    return ans;
}

/* Funcion para Combinar 2 expresiones (cada una es un string)
    
    RECIBE: 2 strings (expr1, expr2) 

    REGRESA: 1 solo string donde es los 2 combinados

    Basicamente lo unico que hace es agregar un parentesis al inicio, ponerle un + entre
    las 2 expresiones y ponerle un parentesis al final

    Toma en cuenta que pone un espacio entre cada palabra y como todas las expresiones que ingresan aqui
    ya vienen con ese formato, siempre se conserva que las concatenaciones se tomaran como un solo token
    y la suma esa se puede separar por tokens (donde el primero y el ultimo sera un parentesis)
*/
string mergeExpression(string expr1, string expr2){
    string ans="( ", merged;
    merged+=expr1;
    merged+=" + ";
    merged+=expr2;
    forn(i,merged.size()){
        ans+=merged[i];
    }
    ans+=" )";
    return ans;
}

/* Funcion para llenar lo que seria el equivalente al vector de conexiones para el nodo i 

    RECIBE: el vector de strings de que nodos tiene conexion (toma en cuenta que es dinamico y puede ser de la
    forma conect={1X0, 0X3} en vez de {1X0, vacio, vacio, 0X3})

    REGRESA: Un mapa con key tipo entero y value tipo string que representa las conexiones del nodo i.

    **key siempre sera el nodo al que quier llegar y value el caracter o string que debo leer para llegar ahi**

    Variables
        -net: mapa auxiliar que se devuelve al final de la funcion+
        -path: string auxiliar para separar la trancision
        -pathEnd: bandera para saber si ya termine de obtener path
        -ID: variable para almacenar el numero de nodo

    como el string es de la forma: *string de trancision* X *ID del nodo*
    entonces podemos realizar un greedy asi:

        -Empiezo almacenando en path lo que vea hasta que llego a la X
        -Cuando llegue a la X entonces activo la bandera para decir que ya termine
        -Si esta la bandera activada simplemente hago un atoi de lo que reste del string

    al final path y ID se agregan al mapa asi:
    key: ID, value: path

    Este proceso se hace para todos los elementos de conect*

    Al final se regresa net y ese sera el mapa que representa las conexiones para el nodo i

    *Si existe una key repetida, entonces se combinan las expresiones que acabo de calcular y la que
    ya estaba anteriormente en el mapa. Esto porque no podemos poner la clave 2 veces y asi se reduce
    la ER final
*/
map<int,string> makeNet(vector<string> conect){
    map<int,string> net;
    string path;
    bool pathEnd;
    int ID;
    forn(i,conect.size()){
        pathEnd=false;
        ID=0;
        path.clear();
        forn(j,conect[i].size()){
            if(pathEnd){
                ID*=10;
                ID+=conect[i][j]-'0';
            }else if(conect[i][j]=='X'){
                pathEnd=true;
            }else{
                path+=conect[i][j];
            }
        }
        if(net.find(ID)!=net.end()){
            net[ID]=mergeExpression(net[ID], path);
        }else{
            net[ID]=path;
        }
    }
    return net;
}

/* Funcion para aplicar EL Teorema de Arden

    def Teorema de arden:
        Si el nodo tiene conexion con si mismo (que sea de la forma Xi=rXi+s) entonces:
        Xi puede ser escrito como r*s ya que puedo pasar t veces por el nodo Xi sin salir de ahi

        Toma en cuenta que s puede ser una expresion compuesta

    RECIBE: el id del nodo que voy a resolver

    REGRESA: nada pq trabaja sobre variable global

    Variables
        -node: auxiliar para no trabajar directo sobre la global pq me da miedo

    Se copia el valor r de la expresion (que es value de id en el mapa) y se encierra entre
    parentesis y se le agrega la cerradura de kleene (*)

    Se borra la entrada de id en el mapa para evitar repetidos

    Se pueden poner 2 casos:
        1. Si el mapa queda vacio despues de la operacion entonces agregamos k y le ponemos lo que hayamos calculado de r*s

        2. Si el mapa no queda vacio, entonces se debe de concatenar la entrada  para cada
        elemento de s, ya que s es una expresion compuesta.
        
    (2) se hace para evitar que quede sin solucion debido a un caso como Xi = r*(s1(Xi+1)+ s2(Xi+2))

    Despues simplemente se asigna en la variable global el resultado

*/
void solveNode(int id){
    cout<<"Solving: "<<id<<endl;
    map<int,string> node=DFA[id];
    if(node[id]==""){
        return;
    }
    string obj="(",aux;
    obj+=node[id]+")*";
    node.erase(id);
    if(node.empty()){
        node[k]=obj;
    }else{
        fore(node,pair){
            aux=obj+node[pair.first];
            node[pair.first]=aux;
        }
    }
    DFA[id]=node;
    printMap(DFA[id],id);
    return;
}

/* Funcion para sustituir una variable en otra
    Como al resolver el sistema de ecuaciones en esencia lo que se hace es
    concatenar el coeficiente que tenia con todos los terminos que voy a sustutuir de la
    variable Xi (si Xi= aXj+bXj+1 => abXi = ab(aXj+bX(j+1))

    Entonces puedo distribuir la concatenacion de la siguiente manera
    Xi= aXj+bXj+1 => abXi = abaXj+abbX(j+1) Y asi no caigo en un problema donde no se puede resolver con
    solveNode (ya que la funcion no admitiria esa entrada)

    Como se trabaja con mapas, y cada entrada en el mapa es equivalente al termino *trancision*Xj,
    entonces podemos concatenar el valor del mapa original con todas las entradas del mapa que
    voy a sustituir

    ***En otras palabras para sustituir Xi en otra ecuacion debo de combinar los mapas***

    Para combinar los mapas se concatena el valor de Xi con todas las entradas del mapa para Xi

    Variables:
        -aux: auxiliar del mapa al que voy a sustituir Xi pq me da miedo hacerlo sobre la global
        -src: auxiliar del mapa de Xi que voy a sustituir en aux
        -obj: string para guardar la trancision para llegar a Xi desde el mapa aux
        -p: string para factorizar puede o no ser que este extra y se pueda reusar obj

    Se obtiene la transicion para llegar a Xi desde aux y se guarda en obj

    Se borra la entrada de Xi en aux para que no exista repetido

    Por cada elemento del mapa:
    
    Caso 1 No existe la clave Xj en aux:
        concateno aux y el valor que ya habia, el string 
        resultante se almacena como el valor de esa clave en el mapa aux

    Caso 2 Existe la clave Xj en aux:
        Primero se concatena obj con el valor de la clave repetida (o sea la que voy a sustituir)
        y luego se combina la expresion

    De esta manera copiamos las claves existentes en el mapa de Xi, concatenadas
    con la transicion para llegar a Xi a aux

    Al terminar de copiar debemos verificar si se puede aplicar teormea de arden al mapa
    (o resolverlo puede ser que puedas simplemente mandar a llamar a solveNode)
*/
void mergeMaps(int target ,int source){
    cout<<"source:"<<source<<" target:"<<target<<endl;
    map<int,string> aux=DFA[target], src=DFA[source];
    string obj, p;
    cout<<"Original map:"<<endl;
    printMap(aux,target);
    /*Obtener el valor a concatenar*/
    obj=aux[source];
    cout<<"Obj ready: "<<obj<<endl;
    aux.erase(source);
    cout<<"source deleted"<<endl;
    /*Concatenar para cada elemento de src*/
    fore(src,x){
        /*Si ya existe el elemento que voy a sustituir entonces
        factorizo la expresion para poder sustituir*/
        if(aux.find(x.first)!=aux.end()){
            aux[x.first]=mergeExpression(obj+src[x.first], aux[x.first]);
        }else{
            aux[x.first]=obj+src[x.first];
        }
    }
    cout<<"Checking map:"<<endl;
    printMap(aux,target);
    if(valid.find(target)!=valid.end()){
        /*Reviso si es posible aplicar Arden y es un estado de aceptacion*/
        if(aux.size()==2 && (aux.find(target)!=aux.end())){
            p=aux[target];
            p+="*";
            aux.erase(target);
            aux[k]=p;
        }
    }else{
        if(aux.size()==1 && (aux.find(target)!=aux.end())){
            /*Reviso si es posible aplicar Arden*/
            p=aux[target];
            p+="*";
            aux.erase(target);
            aux[k]=p;
        }
    }
    DFA[target]=aux;
    return;
}

/* Funcion para obtener la ER de manera recursiva
    RECIBE: 
        -el mapa del nodo i(aunque esta extra porque tienes el id del nodo)
        -el id del nodo que voy a resolver

    REGRESA:
        En teoria un void? porque nunca te regresa un mapa como tal, pero si trabaja
        con ellos sobre una variable global porque es mas facil que estar
        enviando mapas cada vez que yo llame a la funcion

    
    ************ALGORITMO PARA OBTENER LA EXPRESION REGULAR DE UN AFD USANDO TEOREMA DE ARDEN************

    Se considera lo siguiente:
    
    1. El automata puede ser visto como un grafo dirigido
    2. Solo existen las siguientes operaciones:
        -Sustituir una ecuacion en otra
        -Usar teorema de arden para tener la forma Xi=r*s
    3. El programa no puede pensar la manera optima de sustituir

    Debido a que no podemos usar G-J o Cramer para resolver, tenemos que usar el metodo
    de sustitucion y solo existe una operacion que es Arden.

    Como se menciono al inicio el caso trivial es cuando sustituir no tiene mayor complejidad.
    Eso ocurre cuando el automata tiene una estructura de Arbol n-ario (sin considerar
    los caminos que llevan de un nodo a el mismo).

    Si el automata tiene esta estructura, entonces la solucion esta en llegar a las hojas del abrol
    y regresar los resultados ya sea cuando se aplica Arden o simplemente se sustituye
    para que los padres queden en terminos de si mismos y asi se pueda aplicar Arden en algun punto
    
    ***Entonces se debe llegar a un automata con estructura de arbol n-ario para poder resolver la ER***

    Para llegar a la estructura de arbol se considera el siguiente ejemplo de un grafo

    X1->X2
    X2->X3
    X3->X4
    X4->X1

    Como podemos ver esto no es un arbol ya que un arbol es un grafo que no tiene ciclos.
    Para transformarlo a abrol hay que romper el ciclo y si tomamos X1 como raiz entonces debemos 
    quitar la conexion de X4->X1

    De esta manera aseguramos que el recorrido sea:
    X1->X2->X3->X4 y se detenga

    Para poder hacer esto se puede mantener una lista de los nodos que ya visite, de esta manera
    en cuanto me encuentre un nodo j que ya visite dentro de las conexiones de un nodo i, sabre que si
    paso por ahi estaria entrando en un ciclo (ya que se paso por j antes)

    Asi se asegura que el recorrido sobre un automata no tendra ciclos aunque el automata si los tenga.

    El inicio del recorrido siempre esta en X0 por lo que es mas sencillo implementar esta idea.

    Para el recorrido se sigue el siguiente algoritmo:
        -Por cada nodo al que entro se agrega a la lista de visitados
        -Ir a la primera conexion que no este resuelta o visitada todavia
        -Si no existe tal conexion entonces se borra de la lista de visitados y:
            1. Si es posible usar Arden en el nodo, se usa y luego regresamos y sustituimos en el nodo padre
            2. Si no es posible, regresamos y sustituimos en el nodo padre

    Se puede apreciar el recorrido quitandole comentario los prints de Current node, Back to y Return From a la funcion

    tambien podemos ver como combina los mapas quitandole comentario a printMap(DFA[start], start);
*/
void getExpr(map<int,string> node, int start){
    path.insert(start);
    cout<<"Current node:"<<start<<endl;
    fore(node,pair){
        if(!isSolved(pair.first) && !inPath(pair.first)){
            getExpr(DFA[pair.first], pair.first);
            cout<<"Back to: "<<start<<endl;
            mergeMaps(start,pair.first);
            cout<<"Merged map:"<<endl;
            printMap(DFA[start], start);
        }
    }
    cout<<"Finished checking: "<<start<<endl;
    if(node.find(start)!=node.end()){
        solveNode(start);
    }
    path.erase(start);
    cout<<"Return from: "<<start<<endl;
}

int main() {
    /*Variables para lectura de archivo*/
    string cline, filename;
    cout<<"Archivo: ";
    cin>>filename;
    /*Contador de q linea voy*/
    int l=1;

    /*auxiliar(es) para lidiar con el formato culero*/
    vector<string> clean;
    map<int,string> current;

    /*Verificar que exista el archivo*/
    ifstream file(filename);
    if(!file.is_open()){
        cout<<"No hay archivo :("<<endl;
        return 1;
    }
    /*Si existe el archivo leer linea por linea*/
    while(getline(file, cline)){
        if(l==1){
            /*Lectura del alfabeto en la linea 1*/
            aFill(cline);
            cout<<"Alfabeto:";
            fore(a, x){
                cout<<" "<<x;
            }
            cout<<endl;
        }else if(l==2){
            /*Lectura de los estados en la linea 2*/
            sFill(cline);
            cout<<"states:";
            fore(states, x){
                cout<<" "<<x;
            }
            cout<<endl;
        }else if(l==3){
            /*Lectura de estados aceptados en la linea 3*/
            vFill(cline);
            cout<<"Valid:";
            fore(valid, x){
                cout<<" "<<x;
            }
            cout<<endl;
        }else{
            /*Lectura de la informacion del grafo en la linea 3+i*/
            clean=cleanUpInput(cline);
            current=makeNet(clean);
            /*Si es estado de agregacion se agrega n+1 con transicion lambda*/
            if(valid.find(k)!=valid.end()){
                current[k]="/";
            }
            /*Se agrega el mapa obtenido al automata*/
            DFA.pb(current);
            printMap(current, l-4);
            cout<<"Ecuacion para X"<<l-4<<":"<<endl;
            fore(current, pair){
                if(pair.first==k){
                    cout<<pair.second;
                }else{
                    cout<<pair.second<<"X"<<pair.first<<" + ";
                }
            }
            cout<<endl<<endl;
        }
        l++;
    }
    /*Se agrega el estado n+1 a la lista de estados resueltos*/
    solved.insert(k);
    getExpr(DFA[0], 0);
    cout<<"Expresion Regular:"<<endl;
    printMap(DFA[0],0);
    return 0;
}