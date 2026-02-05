#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <vector>
#include <cctype>
#include <unicode/unistr.h>
#include <unicode/ustream.h>
#include <unicode/ucnv.h>
#include <unicode/uchar.h>
#include <chrono>
#include "hash_unistr.hpp"
#include "AVL.hpp"
#include "RB.hpp"
#include "HashC.hpp"
#include "HashO.hpp"
using namespace std;

/**
* @brief Recebe uma referencia de UnicodeString constante, 
*        e remove os caracteres que nao sao alfabeticos ou
*        hifen.
* @param str := UnicodeString que sera "limpa"
* @return resultado := recebe os caracteres permitidos da str e eh retornado.  
 */
icu::UnicodeString delete_pont(const icu::UnicodeString& str) {
    icu::UnicodeString resultado;
    for (int i = 0; i < str.length(); ++i) {
        UChar32 c = str.char32At(i);
        if (u_isalpha(c) || (c == 0x002D && u_isalpha(str.char32At(i-1)) && u_isalpha(str.char32At(i+1)))) { // 0x002D é o código do hífen
            resultado.append(c);
        }
    }
    return resultado;
}

/**
* @brief Cria a tabela de frequencias em um arquivo dado, a partir de outro
*        arquivo tambem passado por parametro. 
* @param dict := estrutura escolhida pelo user para construir a tabela.
* @param file := ifstream para o arquivo que será lido.
* @param saida := ofstream de saida, onde a tabela sera escrita.
*/
template <typename T>
void make_dict(T& dict, ifstream& file, ofstream& saida){
    saida << setw(35) << setfill(' ') << right << "Tabela de frequencias\n";
    saida << "Chave : Frequencia\n";
    auto ini = chrono::high_resolution_clock::now();
    string key;
    while(file >> key) {
        icu::UnicodeString key_unicode = icu::UnicodeString::fromUTF8(key);
        key_unicode.toLower();
        key_unicode = delete_pont(key_unicode);
        if(!key_unicode.isEmpty()) dict[key_unicode] = dict[key_unicode] + 1;
    }
    vector<pair<icu::UnicodeString, int>> pares;
    dict.in_order(pares);
    for(auto p : pares){
        saida << p.first << " : " << to_string(p.second) << "\n";
    }
    auto fim = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(fim - ini);
    cout<<"Tabela criada com sucesso na pasta \"saida\"!\n";
    saida << setw(35) << setfill(' ') << right << "Tempo de construcao: " << duration.count() << " milisegundos\n";
}

/**
* @brief Funcao main: recebe uma escolha por um dicionario e 
*        o nome de um arquivo .txt (incluindo o .txt), e cria uma tabela
*        de frequencias utilizando a estrutura escolhida para o dicionario.
*/
int main(int /* argc */, char *argv[]){
    string dict = argv[1]; // dicionario escolhido

    // se o dicionario nao eh valido
    while(dict != "AVL" && dict != "RB" && dict != "HashC" && dict != "HashO"){
        cout<<"Dicionario invalido. Escreva uma das opcoes abaixo:\nAVL\t\tRB\nHashC\t\tHashO\n>> ";
        cin>>dict;
    }

    string filename = argv[2]; // arquivo escolhido
    ifstream file("entrada/" + filename);

    // se o arquivo nao consegue ser aberto
    while(!file.is_open()) {
        file.open(filename);
        if(!file.is_open()){
            cout<<"Erro ao abrir arquivo. Digite o nome de um arquivo valido escolhido:\n>> ";
            cin>>filename;
            file.open("entrada/" + filename);
        }
    }

    // Arquivo escolhido : OK
    // Estrutura escolhida : OK
    if(dict == "AVL"){
        ofstream saida("saida/resultadoAVL.txt");
        AVL<icu::UnicodeString, int> t;
        make_dict(t, file, saida);
        saida << setw(30) << setfill(' ') << right <<"Numero de rotacoes: " << t.getRotacoes() << "\t\tNumero de comparacores: "<< t.getComparacoes() << "\n";
    } else if(dict == "RB"){
        ofstream saida("saida/resultadoRB.txt");
        RB<icu::UnicodeString, int> t;
        make_dict(t, file, saida);
        saida << setw(30) << setfill(' ') << right <<"Numero de rotacoes: " << t.getRotacoes() << "\t\tNumero de comparacores: "<< t.getComparacoes() << "\n";
    } else if(dict == "HashC"){
        ofstream saida("saida/resultadoHashC.txt");
        HashC<icu::UnicodeString, int> t;
        make_dict(t, file, saida);
        saida << setw(30) << setfill(' ') << right <<"Fator de carga medio: " << t.load_factor_medio() << "\t\tNumero de comparacores: "<< t.get_comparacoes() << "\n";
    } else if(dict == "HashO"){
        ofstream saida("saida/resultadoHashO.txt");
        HashO<icu::UnicodeString, int> t;
        make_dict(t, file, saida);
        saida << setw(30) << setfill(' ') << right <<"Fator de carga medio: " << t.load_factor_medio() << "\t\tNumero de comparacores: "<< t.get_comparacoes() << "\n";
    }
    return 0;
}