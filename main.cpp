#include <iostream>
#include <regex>

using namespace std;

int erro(string err = "")
{
    cout << err << endl;
    return -1;
}

int main(int argc, char const *argv[])
{
    regex outro("(.*)");
    regex quebras_espacos("\\n|\\t|\\s");
    regex comment("#(.*)#");
    regex digito("[0-9]");
    regex letra("[[:alpha:]]");
    regex chave_abre("\\{");
    regex chave_fecha("\\}");
    regex parent_abre("\\(");
    regex parent_fecha("\\)");
    regex str("(\"(.*)\")");
    regex inteiro("([0-9])+");
    regex duplo("([0-9])+(\\.)([0-9])+");
    regex numero("(((\\+)|(-))*)((([0-9])+)|(([0-9])+(\\.)([0-9])+))");
    regex mult("\\*|/|%");
    regex sum("-|\\+");
    regex op_arit("\\*|/|-|\\+");
    regex op_rel(">|<|(<=)|(>=)|(==)");
    regex op_atrib("=");
    regex op_log("!|(&&)|(\\|\\|)");

    if (argc < 2)
        return 1;
    const char *filename = argv[1];
    FILE *prog = fopen(filename, "r");
    if (!prog)
    {
        cout << "Erro ao abrir arquivo!\n";
        return 0;
    }

    int estado = 0;
    int test ;
    char c;
    string aux;
    while (true)
    {
        fread(&c, sizeof(char), 1, prog);
        aux = &c;
        
        if (feof(prog))
            break;
        switch (estado)
        {
        case 0:
        {
            if (regex_match(aux, letra))
                estado = 4;
            else if(regex_match(aux,digito))
                estado = 10;
            else if (regex_match(aux, mult))
                estado = 7;
            else if (regex_match(aux, sum))
                estado = 8;
            else if (regex_match(aux, quebras_espacos))
                estado = 0;
            else
            {

                switch (c)
                {
                case '#':
                    estado = 1;
                    break;
                case '$':
                    estado = 3;
                    break;
                case ';':
                    estado = 27;
                    break;
                case '"':
                    estado = 19;
                    break;
                case '{':
                    estado = 17;
                    break;
                case '}':
                    estado = 16;
                    break;
                case '(':
                    estado = 15;
                    break;
                case ')':
                    estado = 14;
                    break;
                case '=':
                    estado = 22;
                    break;
                case '<':
                    estado = 21;
                    break;
                case '>':
                    estado = 21;
                    break;
                case '&':
                    estado = 5;
                    break;
                case '!':
                    estado = 18;
                    break;
                case '|':
                    estado = 28;
                    break;
                default:
                    erro();
                    break;
                }
            }
            break;
        }
        case 1:
        {
            if(c == '#')
                estado = 2;
            else if(regex_match(aux,outro))
                estado = 1;
            break;
        }
        case 2:
        {
            //token comentario
            cout << "comentario" << endl;
            estado = 0;
            break;
        }
        case 3:
        {
            if (regex_match(aux, letra))
                estado = 4;
            else
                return erro("Identificador inválido");
            break;
        }
        case 4:
        {
            if(regex_match(aux,quebras_espacos) || c == ';')
                estado = 6;
            else if(regex_match(aux,letra) || regex_match(aux,digito))
                estado = 4;
            else
                return erro("Identificador inválido");
            break;
        }
        case 5:
        {
            if(c == '&')
                estado = 18;
            else
                return erro("Operador lógico inválido");
            break;
        }
        case 6:
        {
            //token id e reservadas
            cout << "id e reservadas" << endl;
            estado = 0;
            estado = 0;
            break;
        }
        case 7:
        {
            if(c == ' ')
                estado = 9;
            else
                return erro("Uso de operador inválido");
            break;
        }
        case 8:
        {
            if(c == ' ')
                estado = 9;
            else if(regex_match(aux,digito))
                estado = 10;
            else
                return erro("Uso de operador inválido");
            break;
        }
        case 9:
        {
            // token op arit
            cout << "op_arit" << endl;
            estado = 0;
            break;
        }
        case 10:
        {
            if (c == ' ' || c == ';')
                estado = 11;
            else if (c == '.')
                estado == 12;
            else
                return erro("Número inválido");
            break;
        }
        case 11: 
        {
            // token inteiro
            cout << "inteiro" << endl;
            estado = 0;
            break;
        }
        case 12:
        {
            if(regex_match(aux,digito))
                estado = 12;
            else if(c == ' ' || c == ';')
                estado = 13;
            break;
        }
        case 13:
        {
            // token duplo
            cout << "duplo" << endl;
            break;
        }
        case 14:
        {
            // token parent_fecha
            cout << ")" << endl;
            estado = 0;
            break;
        }
        case 15:
        {
            // token parent_abre
            cout << "(" << endl;
            estado = 0;
            break;
        }
        case 16:
        {
            // token chave fecha
            cout << "}" << endl;
            estado = 0;
            break;
        }
        case 17:
        {
            // token chave_abre
            cout << "{" << endl;
            estado = 0;
            break;
        }
        case 18:
        {
            if (c == ' ')
                estado = 25;
            else 
                return erro("Operador lógico inválido");
            break;
        }
        case 19:
        {
            if(regex_match(aux,outro))
                estado = 19;
            else if(c == '"')
                estado = 20;
            break;
        }
        case 20:
        {
            // token string
            cout << "string" << endl;
            estado = 0;
            break;
        }
        case 21:
        {
            if (c == '=')
                estado = 23;
            else if (c == ' ')
                estado = 26;
            else
                return erro("Operador relacional inválido");
            break;
        }
        case 22:
        {
            if (c == '=')
                estado = 23;
            else if (c == ' ')
                estado = 24;
            else
                return erro("Operador relacional inválido");

            break;
        }
        case 23:
        {
            if(regex_match(aux, outro))
                estado = 26;
            break;
        }
        case 24:
        {
            // token op_atrib
            cout << "op_atrib" << endl;
            estado = 0;
            break;
        }
        case 25:
        {
            // token op_log
            cout << "op_log" << endl;
            estado = 0;
            break;
        }
        case 26:
        {
            // token op_rel
            cout << "op_rel" << endl;
            estado = 0;
            break;
        }
        case 27:
        {
            // token pt_virg
            cout << "pt_virg" << endl;
            estado = 0;
            break;
        }
        case 28:
        {
            if(c == '|')
                estado = 18;
            else
                return erro("Operador lógico inválido");
            break;
        }
        }
    }

    return 0;
}
