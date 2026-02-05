#ifndef AVL_HPP
#define AVL_HPP
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <unicode/unistr.h> 
#include <unicode/ustream.h>
#include <unicode/ucnv.h> 

template <typename Key, typename Value>
// Node
struct NodeAVL {
    std::pair<Key, Value> par; // par chave valor
    int height; // altura do nodo
    NodeAVL<Key, Value>* left; // filho esquerdo
    NodeAVL<Key, Value>* right; // filho direito

    // Construtor
    NodeAVL(std::pair<Key, Value> p, int h, NodeAVL<Key, Value>* l, NodeAVL<Key, Value>* r) {
        par = p;
        height = h;
        left = l;
        right = r;
    }

};

template <typename Key, typename Value>
class AVL {
private:
    // Ponteiro para a raiz da arvore
    NodeAVL<Key, Value>* m_root;

    // Numero de rotacoes
    int rotacoes;

    /**
     * @brief Conta o numero de comparacoes de chaves necessarias 
     *        para montar a tabela de frequencias. Portanto, conta as 
     *        comparacoes na insercao.
     */
    int comparacoes;
    
    /**
     * @brief Retorna a altura de um nodo
     * 
     * @param node := nodo cuja altura vai ser retornada
     */
    int height(NodeAVL<Key, Value>* node) {
        if(node == nullptr) return 0;
        else return node->height;
    }

    /**
     * @brief Retorna o fator de balanceamento de um nodo
     * 
     * @param node := nodo cujo fator de balanceamento vai ser retornado
     */
    int balance(NodeAVL<Key, Value>* node) {
        return height(node->right) - height(node->left);
    }

    /**
     * @brief Realiza uma rotacao a direita em um nodo dado, e
     *        retorna a raiz resultante da rotacao
     * 
     * @param node := nodo pivo da rotacao
     */
    NodeAVL<Key, Value>* rightRotate(NodeAVL<Key, Value> *node) {
        NodeAVL<Key, Value>* aux = node->left;
        node->left = aux->right;
        aux->right = node;
        node->height = 1 + std::max(height(node->left), height(node->right));
        aux->height = 1 + std::max(height(aux->left), height(aux->right));
        rotacoes++;
        return aux;
    }

    /**
     * @brief Realiza uma rotacao a esquerda em um nodo dado, e
     *        retorna a raiz resultante da rotacao
     * 
     * @param node := nodo pivo da rotacao
     */
    NodeAVL<Key, Value>* leftRotate(NodeAVL<Key, Value>* node) {
        NodeAVL<Key, Value>* aux = node->right;
        node->right = aux->left;
        aux->left = node;
        node->height = 1 + std::max(height(node->left), height(node->right));
        aux->height = 1 + std::max(height(aux->left), height(aux->right));
        rotacoes++;
        return aux;
    }

    /**
     * @brief Insere um par chave valor na arvore se a chave nao existir na mesma.
     *        Atualiza o valor associado a chave caso contrario.
     * 
     * @param node := nodo atual: verifica se node->par.first eh igual, menor ou maior 
     *                que a chave a ser inserida.
     * @param p := par a ser inserido na arvore
     */
    NodeAVL<Key, Value>* _insert(NodeAVL<Key, Value>* node, std::pair<Key,Value> p) {
        if (node == nullptr){ // arvore vazia
            return new NodeAVL<Key, Value>(p, 1, nullptr, nullptr);
        }
        comparacoes++;
        if (node->par.first == p.first) { // chave repetida
            node->par.second = p.second; // atualiza o valor
            return node;
        }
        if (p.first < node->par.first){ // caso geral
            node->left = _insert(node->left, p);
        } else {
            node->right = _insert(node->right, p);
        }
        node = fixupInsert(node, p); // rebalancear a arvore
        return node;
    }
    
    /**
     * @brief Verifica se eh necessario realizar algum rebalanceamento na arvore
     *        apos a insercao de um nodo. Recalcula a altura do nodo dado.
     * 
     * @param node := nodo pivo do rebalanceamento
     * @param p := par pertencente a node
     */
    NodeAVL<Key, Value>* fixupInsert(NodeAVL<Key, Value>* node, std::pair<Key, Value> p) {
        int bal = balance(node);
        // Caso 1(a)
        if(bal == -2 && p.first < node->left->par.first) {
            return rightRotate(node);
        }
        // Caso 1(b)
        if(bal == -2 && p.first > node->left->par.first) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        // Caso 2(a)
        if(bal == 2 && p.first > node->right->par.first) {
            return leftRotate(node);
        }
        // Caso 2(b)
        if(bal == 2 && p.first < node->right->par.first) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
        // Caso node balanceado - recalcular altura
        node->height = 1 + std::max(height(node->left), height(node->right));
        return node;
    }

    /**
     * @brief Libera os nodos da arvore para a memoria, e 
     *        retorna nullptr, indicando que a arvore ficou vazia
     * 
     * @param node := nodo a ser deletado, usado para navegar 
     *                para a esquerda e para a direita na arvore
     */
    NodeAVL<Key, Value>* clear(NodeAVL<Key, Value>* node) {
        if(node != nullptr) {  
            clear(node->left);
            clear(node->right);
            delete node;
        }
        return nullptr;
    }


    /**
     * @brief Imprime a serializazao da arvore no terminal, em pre-ordem. 
     * 
     * @param node := nodo imprimido.
     */
    void print(NodeAVL<Key, Value>* node){
        if(node == nullptr) {
            std::cout<<" # ";
            return;
        }
        else {
            std::cout<<"{"<<node->par.first<<" : "<<node->par.second<<"} ";
            print(node->left);
            print(node->right);
        }
    }

    /**
     * @brief Registra os pares, em ordem alfabetica de chave, em um vector
     *        de pares. 
     * 
     * @param node := nodo imprimido.
     * @param saida := vector a ser preenchido.
     */
    void _in_order(NodeAVL<Key, Value>* node, std::vector<std::pair<Key, Value>>& saida){
        if(node == nullptr) return;
        _in_order(node->left, saida);
        saida.push_back(node->par);
        _in_order(node->right, saida);
    }

    /**
     * @brief Retorna um nodo se e somente se uma dada chave existir 
     *        na arvore. Retorna nullptr caso contrario.
     * 
     * @param node := nodo atual, o qual compara-se com a chave k,
     *                verificando se eh igual, menor ou maior que a mesma.
     * @param k := chave a ser buscada na arvore.
     */
    NodeAVL<Key, Value>* _contains(NodeAVL<Key, Value>* node, const Key& k) {
        // Caso de parada: k nao existe ou achei k
        if(node == nullptr || node->par.first == k) return node;
        
        // Caso recursivo: ir pra esquerda ou direita
        if(k < node->par.first) return _contains(node->left, k); 
        return _contains(node->right, k);
    }

    /**
     * @brief Busca a chave a ser removida da arvore.
     * 
     * @param node := nodo atual, o qual compara-se com a chave k,
     *                verificando se eh igual, menor ou maior que a mesma.
     * @param k := chave a ser buscada na arvore.
     */
    NodeAVL<Key, Value>* _remove(NodeAVL<Key, Value>* node, Key& k) {
        if(node == nullptr) // Arvore vazia ou chave nao encontrada
            return nullptr;
        if(node->par.first == k) // Chave encontrada
            return _removeNode(node);

        if(k < node->par.first) node->left = _remove(node->left, k);
        else node->right = _remove(node->right, k);
        
        node = fixupDelete(node);
        return node;
    }
    
    /**
     * @brief Remove um nodo da arvore e retorna o nodo que assumir seu lugar.
     * 
     * @param node := nodo que sera removido da arvore
     */
    NodeAVL<Key, Value>* _removeNode(NodeAVL<Key, Value> *node) {
        
        NodeAVL<Key, Value> *pai, *succ;
        if(node->right == nullptr) succ = node->left; // filho esquerdo assume
        else {      
            pai = node;
            succ = node->right;
            while(succ->left != nullptr) {
                pai = succ;
                succ = succ->left;
            }
            if(pai != node) {
                pai->left = succ->right;
                succ->right = node->right;
            }
            succ->left = node->left;
        }
        delete node;
        if(succ != nullptr) succ->height = 1 + std::max(height(succ->right), height(succ->left));
        return succ;
    }

    /**
     * @brief Verifica se eh necessario realizar algum rebalanceamento na arvore
     *        apos a remocao de um nodo. Recalcula a altura do nodo dado.
     * 
     * @param node := nodo pivo do rebalanceamento
     */
    NodeAVL<Key, Value>* fixupDelete(NodeAVL<Key, Value> *node) {
        int bal = balance(node);
        // 4 casos de desbalanceamento
        if(bal > 1 && balance(node->right) >= 0) { // Caso 1a: Peso na direita -> direita
            return leftRotate(node);
        }
        else if(bal > 1 && balance(node->right) < 0) { // Caso 2a: Peso na direita -> esquerda
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
        else if(bal < -1 && balance(node->left) <= 0) { // Caso 1b: Peso na esquerda -> esquerda
            return rightRotate(node);
        }
        else if(bal < -1 && balance(node->left) > 0) { // Caso 2b: Peso na esquerda -> direita
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Recalcular altura de node
        node->height = 1 + std::max(height(node->left), height(node->right));
        return node;
    }

    /**
     * @brief Conta e retorna o numero de nodos na arvore.
     */
    int _size(NodeAVL<Key, Value>* node){
        if(node == nullptr) return 0;
        return 1 + _size(node->left) + _size(node->right);
    }

public:
    // Construtor padrao: cria uma arvore vazia.
    AVL() {
        rotacoes = 0;
        comparacoes = 0;
        m_root = nullptr;
    }

    // Destrutor: libera todos os nodos da arvore
    // para a memoria
    ~AVL() {
        clear();
    }

    /**
     * @brief Retorna o total de rotacoes realizadas na arvore.
     */
    int getRotacoes(){
        return rotacoes;
    }

    /**
     * @brief Retorna o total de comparacoes realizadas na construcao da tabela
     *        de frequencias.
     */
    int getComparacoes(){
        return comparacoes;
    }

    /**
     * @brief Retorna o numero de elementos da arvore
     */
    int size(){
        return _size(m_root);
    }


    /**
     * @brief Promete inserir um par dado na arvore.
     * 
     * @param p := par a ser inserido.
     */
    void insert(std::pair<Key, Value> p) {
        m_root = _insert(m_root, p);
    }

    // Funcao que libera os nodos da arvore para a memoria.
    // Atualiza m_root para nullptr, indicando que a arvore ficou vazia.
    void clear() {
        m_root = clear(m_root);
        rotacoes = 0;
        comparacoes = 0;
    }

    /**
     * @brief Retorna true se e somente se a arvore estiver vazia.
     *        Retorna false caso contrario.
     */
    bool empty(){
        return m_root == nullptr;
    }


    // Imprime a serializacao da arvore
    void print() {
        std::cout<<"\nPrint: \n";
        print(m_root);
        std::cout<<"\n";
    }

    // Registra os pares em ordem crescente de chave no vector vec
    void in_order(std::vector<std::pair<Key, Value>>& vec){
        _in_order(m_root, vec);
    }


    /**
     * @brief Retorna true se e somente se uma chave dada estiver
     *        na arvore. Retorna false caso contrario.
     * 
     * @param k := chave a ser buscada
     */
    bool contains(Key k) {
        NodeAVL<Key, Value>* aux = _contains(m_root, k);
        return aux != nullptr;
    }
 
    /**
     * @brief Promete remover o par, cuja chave eh igual a k, da arvore.
     * 
     * @param k := chave a ser buscada
     */
    void remove(Key k) {
        if(contains(k)) m_root = _remove(m_root, k);
        else std::cout<<"Chave inexistente\n";
    }

    /**
     * @brief Sobrecarga do operador colchete. Recebe uma chave e
     *        retorna uma referencia para o valor associado a mesma.
     *        Caso a chave nao esteja na arvore, insere a mesma com um valor
     *        associado padrao (construtor padrao de Value) e retorna uma referencia
     *        para o valor associado.
     * @param k := chave a ser buscada.
     */
    Value& operator[](const Key& k) {
        NodeAVL<Key, Value>* nodo = _contains(m_root, k);
        if(nodo != nullptr) return nodo->par.second;
        insert(std::make_pair(k, Value()));
        return (_contains(m_root, k))->par.second;
    }

    /**
     * @brief Versao constante da sobrecarga do operador colchete. 
     *        Recebe uma chave e retorna uma referencia constante
     *        para o valor associado a mesma. Lanca uma excecao 
     *        invalid_argument caso nao encontre a chave.
     * 
     * @param k := chave a ser buscada.
     */
    const Value& operator[](const Key& k) const {
        NodeAVL<Key, Value>* nodo = _contains(m_root, k);
        if(nodo != nullptr) return nodo->par.second;
        throw std::invalid_argument("A chave dada nao esta na arvore");
    }
    
};
#endif
    