#include <bits/stdc++.h>
#define endl '\n'
#define forn(i,n) for(int i=0;i<int(n);i++)
#define forr(i,a,b) for (int i = int(a); i <= int(b); i++)
#define fore(obj) for(const auto& element: obj)
typedef long long lli;
using namespace std;

//global vars
bool nLineComment=false;
bool castFlag=false;
bool correct=true;

//easy checking tokens
set<string> reserved{
    "class", "for", "if", "else", "while", "do", "switch", "case", "break",
    "continue", "return", "public", "private", "protected", "static", "final", "abstract", "native",
    "synchronized", "void", "true", "false", "null", "new", "this", "super", "extends", "implements",
    "interface", "package", "import", "throws", "throw", "try", "catch", "finally", "instanceof",
    "volatile", "transient", "const", "goto"
};
set<string> dType{
    "byte", "short", "int", "long", "float", "double", "char", "boolean"
};

set<string> aOper{
    "+", "-", "*", "/", "%"
};

set<string> asignOper{
    "+=" , "-=", "*=", "/=", "%="
};

set<string> logicOper{
    "==", "!=", "<", ">", ">=", "<="
};

vector<string> tokenizar(string line){
    int j=0;
    string current;
    vector<string> tokens;
    while(line[j]==' '){
        j++;
    }
    //cout<<j<<" "<<line.size()-1<<endl;
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

void printTokens(vector<string> tokens){
    forn(i,tokens.size()){
        cout<<tokens[i]<<endl;
    }
    cout<<tokens.size()<<endl<<endl;
}

bool isReserved(string token){
    return (reserved.find(token)!=reserved.end());
}
bool isdType(string token){
    return (dType.find(token)!=dType.end());
}
bool isAritOperator(string token){
    return (aOper.find(token)!=aOper.end());
}
bool isAsignOperator(string token){
    return (asignOper.find(token)!=asignOper.end());
}
bool isLogicOperator(string token){
    return (logicOper.find(token)!=logicOper.end());
}

bool validateName(string token){
    //cout<<"Name validation: "<<token<<endl;
    int ret=0;
    forn(i,token.size()){
        //cout<<token[i]<<" "<<i<<endl;
        switch (ret)
        {
        case 0:
            if((token[i]>='A' && token[i]<='Z') || (token[i]>='a' && token[i]<='z') || token[i]=='_' || token[i]=='$'){
                ret=1;
            }else{
                return false;
            }
            break;
        
        case 1:
            if((token[i]>='A' && token[i]<='Z') || (token[i]>='a' && token[i]<='z') || token[i]=='_' || token[i]=='$' || (token[i]>='0' && token[i]<='9')){
                ret=1;
            }else if(token[i]=='{' && i==token.size()-1){
                return true;
            }else{
                return false;
            }
        }
    }
    set<int> valid{1};
    return (valid.find(ret)!=valid.end());
}

//DFA to check if a number is valid
bool validateNumber(string token){
    castFlag=false;
    string cast,current;
    int ret=0;
    //cout<<"Num validation: "<<token<<endl;
    forn(i,token.size()){
        //cout<<i<<":"<<token[i]<<" q:"<<ret<<endl;
        current=token[i];
        if(castFlag){
            if(token[i]==')'){
                castFlag=false;
                if(!cast.empty() && !isdType(cast)){
                    return false;
                }
            }else{
                cast+=current;
            }
        }else if(token[i]=='('){
            castFlag=true;
        }else{
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
    }
    set<int> valid{2,4,7,9,10};
    return (valid.find(ret)!=valid.end());
}

bool bigDFA(vector<string> tokens, int end){
    bool nres=false, p=false;
    int q=0;
    //cout<<"end: "<<end<<endl;
    forn(i,tokens.size()){
        //cout<<i<<":"<<tokens[i]<<" q:"<<q<<endl;
        if(p){
            if(tokens[i]==")"){
                p=false;
            }
        }else if(nres && tokens[i]=="("){
            //cout<<"() after res"<<endl;
            p=true;
            nres=false;
        }else if(nLineComment){
            if(tokens[i]=="*/"){
                nLineComment=false;
                //cout<<"Comment end"<<endl;
            }
        }else if(tokens[i]=="/*"){
            //cout<<"Comment start"<<endl;
            nLineComment=true;
        }else if(tokens[i]=="//"){
            return true;
        }else if(tokens[i]=="{" || tokens[i]=="}"){
            return true;
        }else{
            switch(q){
                case 0:
                    if(isReserved(tokens[i])){
                        nres=true;
                        q=0;
                    }else if(isdType(tokens[i])){
                        nres=true;
                        q=1;
                    }else if(validateName(tokens[i])){
                        nres=true;
                        q=2;
                    }else{
                        return false;
                    }
                    break;

                case 1:
                    if(isdType(tokens[i])){
                        q=1;
                    }else if(validateName(tokens[i])){
                        q=2;
                    }else{
                        return false;
                    }
                    break;

                case 2:
                    if(tokens[i]=="=" || isAsignOperator(tokens[i])){
                        q=3;
                    }else if((tokens[i]==";" || tokens[i]=="{") && i==end){
                        return true;
                    }else{
                        return false;
                    }
                    break;

                case 3:
                    if(validateNumber(tokens[i])){
                        q=4;
                    }else if(validateName(tokens[i])){
                        q=4;
                    }else{
                        return false;
                    }
                    break;

                case 4:
                    if(isAritOperator(tokens[i]) || isLogicOperator(tokens[i])){
                        q=3;
                    }else if(tokens[i]==";" && i==end){
                        return true;
                    }else if(tokens[i]==","){
                        q=1;
                    }else{
                        return false;
                    }
                    break;
            }
        }
    }
    set<int> valid{0,1,2,5};
    return (valid.find(q)!=valid.end());
}

bool printDFA(vector<string> tokens, int start){
    char comillas='"';
    string s;
    s+=comillas;
    int q=0;
    forr(i,start,tokens.size()-1){
        //cout<<i<<":"<<tokens[i]<<" q:"<<q<<endl;
        switch (q){
        case 0:
            if(tokens[i]=="("){
                q=1;
            }else{
                return false;
            }
            break;

        case 1:
            if(tokens[i]==s){
                q=2;
            }else if(tokens[i]=="("){
                q=4;
            }else{
                return false;
            }
            break;

        case 2:
            if(tokens[i]==s){
                q=3;
            }else{
                q=2;
            }
            break;

        case 3:
            if(tokens[i]=="+"){
                q=1;
            }else if( tokens[i]==")"){
                q=6;
            }else{
                return false;
            }
            break;

        case 4:
            if(validateName(tokens[i])){
                q=5;
            }else if(validateNumber(tokens[i])){
                q=5;
            }else{
                return false;
            }
            break;

        case 5:
            if(isLogicOperator(tokens[i]) || isAritOperator(tokens[i])){
                q=4;
            }else if(tokens[i]==")"){
                q=3;
            }else{
                return false;
            }
            break;

        case 6:
            if(tokens[i]==";" && i==tokens.size()-1){
                q=7;
            }else{
                return false;
            }
            break;
        }
    }
    return q==7;
}

int main() {
    vector<string> tokens;
    int l=0;
    string cline, filename="entrada.java";
    bool ans;
    
    ifstream file(filename);
    if(!file.is_open()){
        cout<<"No hay archivo :("<<endl;
        return 1;
    }
    while(getline(file, cline)){
        //Make new line into tokens
        l++,ans=0;
        tokens.clear();
        tokens=tokenizar(cline);
        //printTokens(tokens);

        //call DFA to check the
        if(tokens[0]=="System.out.println"){
            ans=printDFA(tokens,1);
        }else{
            ans=bigDFA(tokens,tokens.size()-1);
        }
        
        if(!ans){
            correct=false;
            cout<<"Error en la linea "<<l<<endl;
            
        }
        else{
            //cout<<"Linea "<<l<<" correcta"<<endl;
        }
    }
    if(correct && !nLineComment){
        cout<<"No hay errores de analisis lexico en el archivo:"<<endl;
        cout<<filename<<endl;
    }else if(nLineComment){
        cout<<"Comentario sin cerrar"<<endl;
    }
    return 0;
}