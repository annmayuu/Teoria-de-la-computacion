#include <bits/stdc++.h>
#define endl '\n'
#define forn(i,n) for(int i=0;i<int(n);i++)
#define forr(i,a,b) for (int i = int(a); i <= int(b); i++)
#define fore(obj, element) for(const auto& element: obj)
#define pb push_back
typedef long long int lli;
using namespace std;

//Variables globales otra vez
set<string> reserved{
    "auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum", 
	"extern", "float", "for", "goto", "if", "inline", "int", "long", "register", "restrict", "return", 
	"short", "signed", "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned", "void", 
	"volatile", "while", "_Alignas", "_Alignof", "_Atomic", "_Bool", "_Complex", "_Decimal128", "_Decimal32", 
	"_Decimal64", "_Generic", "_Imaginary", "_Noreturn", "_Static_assert", "_Thread_local", "__func__"
};
set<string> aOper{
    "+", "-", "*", "/", "%"
};
//funciones para revisar (otra vez) cosas rapidamente
bool isReserved(string token){
    return (reserved.find(token)!=reserved.end());
}
bool isAritOperator(string token){
    return (aOper.find(token)!=aOper.end());
}
bool validateName(string token){
	if(isReserved(token)){
		return false;
	}
    int ret=0;
    forn(i,token.size()){
        switch (ret)
        {
        case 0:
            if((token[i]>='A' && token[i]<='Z') || (token[i]>='a' && token[i]<='z') || token[i]=='_'){
                ret=1;
            }else{
                return false;
            }
            break;
        
        case 1:
            if((token[i]>='A' && token[i]<='Z') || (token[i]>='a' && token[i]<='z') || token[i]=='_' || (token[i]>='0' && token[i]<='9')){
                ret=1;
            }else if(token[i]=='{' && i==token.size()-1){
                return true;
            }else{
                return false;
            }
        }
    }
    return (ret==1);
}
bool validateNumber(string token){
    string cast,current;
    int ret=0;
    forn(i,token.size()){
        current=token[i];
            switch(ret){
            case 0:
                if(token[i]=='+'|| token[i]=='-'){
                    ret=1;
                }else if(token[i]=='0'){
                    ret=8;
                }else if(token[i]>='1' && token[i]<='9'){
                    ret=2;
                }else{
                    return false;
                }
                break;

            case 1:
                if(token[i]=='0'){
                    ret=8;
                }else if(token[i]>='1' && token[i]<='9'){
                    ret=2;
                }else{
                    return false;
                }
                break;

            case 2:
                if(token[i]>='0' && token[i]<='9'){
                    ret=2;
                }else if(token[i]=='.'){
                    ret=3;
                }else if(token[i]=='E' || token[i]=='e'){
                    ret=5;
                }else{
                    return false;
                }
                break;
            
            case 3:
                if(token[i]>='0' && token[i]<='9'){
                    ret=4;
                }else{
                    return false;
                }
                break;
            
            case 4:
                if(token[i]>='0' && token[i]<='9'){
                    ret=4;
                }else if(token[i]=='E' || token[i]=='e'){
                    ret=5;
                }else{
                    return false;
                }
                break;

            case 5:
                if(token[i]=='+'||token[i]=='-'){
                    ret=6;
                }else if(token[i]>='1' && token[i]<='9'){
                    ret=7;
                }else{
                    return false;
                }
                break;

            case 6:
                if(token[i]>='1' && token[i]<='9'){
                    ret=7;
                }else{
                    return false;
                }
                break;
            
            case 7:
                if(token[i]>='0' && token[i]<='9'){
                    ret=7;
                }else{
                    return false;
                }
                break;
            
            case 8:
                if(token[i]>='1' && token[i]<='7'){
                    ret=10;
                }else if(token[i]=='x' || token[i]=='X'){
                    ret=9;
                }else if(token[i]=='.'){
                    ret=3;
                }else{
                    return false;
                }
                break;

            case 9:
                if((token[i]>='0' && token[i]<='9')|| (token[i]>='A' && token[i]<='F')){
                    ret=9;
                }else{
                    return false;
                }
                break;

            case 10:
                if(token[i]>='0' && token[i]<='7'){
                    ret=10;
                }else{
                    return false;
                }
                break;
            }
    }
    set<int> valid{2,4,7,9,10};
    return (valid.find(ret)!=valid.end());
}
bool checkfinal(vector<string> tokens){
	return(tokens[tokens.size()-1]==";");
}

vector<string> tokenizar(string line){
    int j=0;
    string current;
    vector<string> tokens;
    while(line[j]==' '){
        j++;
    }
    forr(i,j,line.size()-1){
            if(line[i]==' '){
                tokens.push_back(current);
                i++;
                current.clear();
            }
            current+=line[i];
        }
    if(!current.empty()){
        tokens.push_back(current);
        current.clear();
    }
    return tokens;
}
vector<string> removeFinal(vector<string> tokens){
	vector<string> aux;
	forn(i,tokens.size()-1){
		aux.pb(tokens[i]);
	}
	return aux;
}
//Automata de pila
bool AP(vector<string> tokens){
	stack<int> balanced;
	int q=0;
	forn(i,tokens.size()){
		switch (q)
		{
		case 0:
			if(validateName(tokens[i])){
				q=1;
			}else{
				return false;
			}
			break;

		case 1:
			if(tokens[i]=="="){
				q=2;
			}else{
				return false;
			}
			break;

		case 2:
			if(tokens[i]=="("){
				balanced.push(1);
			}else if(tokens[i]==")" && !balanced.empty()){
				balanced.pop();
			}else if(validateName(tokens[i])){
				q=3;
			}else if(validateNumber(tokens[i])){
				q=4;
			}else{
				return false;
			}
			break;

		case 3:
			if(tokens[i]=="("){
				balanced.push(1);
			}else if(tokens[i]==")" && !balanced.empty()){
				balanced.pop();
			}else if(tokens[i]=="="){
				q=2;
			}else if(isAritOperator(tokens[i])){
				q=5;
			}else{
				return false;
			}
			break;

		case 4:
			if(tokens[i]=="("){
				balanced.push(1);
			}else if(tokens[i]==")" && !balanced.empty()){
				balanced.pop();
			}else if(isAritOperator(tokens[i])){
				q=5;
			}else{
				return false;
			}
			break;

		case 5:
			if(tokens[i]=="("){
				balanced.push(1);
			}else if(tokens[i]==")" && !balanced.empty()){
				balanced.pop();
			}else if(validateName(tokens[i]) || validateNumber(tokens[i])){
				q=4;
			}else{
				return false;
			}
			break;
		}
	}
	set<int> valid={3,4};
	return ((valid.find(q)!=valid.end())&&balanced.empty());
}
int main() {
    vector<string> aux, tokens;
    int l=0;
	bool flag=true;
    string cline, filename="valid.txt";
    ifstream file(filename);
    if(!file.is_open()){
        cout<<"No hay archivo :("<<endl;
        return 1;
    }
    while(getline(file, cline)){
        l++;
        aux.clear();
        aux=tokenizar(cline);
		if(!checkfinal(aux)){
			cout<<"Falta ; en la linea "<<l<<endl;
			flag=false;
		}else{
			tokens=removeFinal(aux);
			if(!AP(tokens)){
				cout<<"Error en la linea "<<l<<endl;
				flag=false;
			}
        }
    }
	if(flag){
		cout<<"No hay errores"<<endl;
	}
    return 0;
}