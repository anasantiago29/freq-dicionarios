#ifndef RB_HPP
#define RB_HPP
#include <iostream>
#include <string>
#include <vector>

#define RED true
#define BLACK false

template <typename Key, typename Value>
// Node
struct Node {
    std::pair<Key, Value> par; // chave-valor
    bool color; // RED ou BLACK - true ou false
    Node<Key, Value>* parent; // ponteiro para o pai
    Node<Key, Value>* left; // ponteiro para filho esquerdo
    Node<Key, Value>* right; // ponteiro para filho direito

    // Construtor
    Node (std::pair<Key, Value> pr, bool c, Node<Key, Value>* p, Node<Key, Value>* l, Node<Key, Value>* r) {
        par = pr;
        color = c;
        parent = p;
        left = l;
        right = r;
    } 

};

template <typename Key, typename Value>
class RB {
private:
    // Ponteiro para a raiz da arvore
    Node<Key, Value>* m_root;

    // Ponteiro para o NIL
    Node<Key, Value>* m_nil;

    // Numero de rotacoes
    int rotacoes;

    /**
     * @brief Conta o numero de comparacoes de chaves necessarias 
     *        para montar a tabela de frequencias. Portanto, conta as 
     *        comparacoes na insercao.
     */
    int comparacoes;
    

    /**
     * @brief Realiza uma rotacao a direita em um nodo dado, e
     *        retorna a raiz resultante da rotacao
     * 
     * @param node := nodo pivo da rotacao
     */    
    void rightRotate(Node<Key, Value> *node) {
        Node<Key, Value>* y = node->left;
        node->left = y->right;
        if(y->right != m_nil) y->right->parent = node;
        y->parent = node->parent;
        if(node->parent == m_nil) m_root = y;
        else if(node == node->parent->right) node->parent->right = y;
        else node->parent->left = y;
        y->right = node;
        node->parent = y;
        rotacoes++;
    }

    /**
     * @brief Realiza uma rotacao a esquerda em um nodo dado, e
     *        retorna a raiz resultante da rotacao
     * 
     * @param node := nodo pivo da rotacao
     */
    void leftRotate(Node<Key, Value>* node) {
        Node<Key, Value>* y = node->right;
        node->right = y->left;
        if(y->left != m_nil) y->left->parent = node;
        y->parent = node->parent;
        if(node->parent == m_nil) m_root = y;
        else if(node == node->parent->left) node->parent->left = y;
        else node->parent->right = y;
        y->left = node;
        node->parent = y;
        rotacoes++;
    }

    /**
     * @brief Insere um par chave valor na arvore se a chave nao existir na mesma.
     *        Atualiza o valor associado a chave caso contrario.
     * 
     * @param p := par a ser inserido na arvore
     */
    void _insert(std::pair<Key,Value> p) {
        Node<Key, Value>* atual = m_root;
        Node<Key, Value>* pai = m_nil;
        while(atual != m_nil){ // enquanto nao chegar em uma folha
            pai = atual; // atualiza pai
            comparacoes++;
            // navego pra direita ou esquerda
            if(p.first < atual->par.first) atual = atual->left;
            else if(p.first > atual->par.first) atual = atual->right;
            else { // chave ja existente: atualizo o valor
                atual->par.second = p.second;
                return; // retorno
            }            
        }
        // crio o novo nodo, verifico se eh raiz, filho esquerdo ou filho direito
        Node<Key, Value>* novo = new Node<Key, Value>(p, RED, pai, m_nil, m_nil);
        if(pai == m_nil) {
            m_root = novo;
            m_root->color = BLACK;
            return;
        } else if(pai->par.first > p.first) pai->left = novo;
        else pai->right = novo;
        fixupInsert(novo); // rebalancear e recolorir
    }
    
    /**
     * @brief Promete, se necessario, rebalancear e recolorir a arvore
     *        apos a insercao de um nodo.
     * 
     * @param node := nodo pivo do rebalanceamento
     */
    void fixupInsert(Node<Key, Value>* novo) {
        while (novo->parent->color == RED){
            if(novo->parent == novo->parent->parent->left) { // Pai eh filho esquerdo
                Node<Key, Value>* tio = novo->parent->parent->right;
                if(tio->color == RED) { // Caso 1: tio vermelho
                    novo->parent->color = BLACK;
                    tio->color = BLACK;
                    novo->parent->parent->color = RED;
                    novo = novo->parent->parent;
                } else {
                    if(novo == novo->parent->right) { // Caso 2a: novo eh filho direito
                        novo = novo->parent;
                        leftRotate(novo);
                    }
                    novo->parent->color = BLACK; // Caso 3a: novo eh filho esquerdo
                    novo->parent->parent->color = RED;
                    rightRotate(novo->parent->parent);
                }
            } else { // Pai eh filho direito
                Node<Key, Value>* tio = novo->parent->parent->left;
                if(tio->color == RED) { // Caso 1: tio vermelho
                    novo->parent->color = BLACK;
                    tio->color = BLACK;
                    novo->parent->parent->color = RED;
                    novo = novo->parent->parent;
                } else {
                    if(novo == novo->parent->left) { // Caso 2b: novo eh filho esquerdo
                        novo = novo->parent;
                        rightRotate(novo);
                    }
                    novo->parent->color = BLACK; // Caso 3b: novo eh filho direito
                    novo->parent->parent->color = RED;
                    leftRotate(novo->parent->parent);
                }
            }
        }
        m_root->color = BLACK; // corrigir a raiz

    }

    /**
     * @brief Libera os nodos da arvore para a memoria, e 
     *        retorna NIL, indicando que a arvore ficou vazia
     * 
     * @param node := nodo a ser deletado, usado para navegar 
     *                para a esquerda e para a direita na arvore
     */
    Node<Key, Value>* _clear(Node<Key, Value>* node) {
        if(node != m_nil) {
            _clear(node->left);
            _clear(node->right);
            delete node;
        }
        return m_nil;
    }


    /**
     * @brief Imprime a serializazao da arvore no terminal, em pre-ordem. 
     * 
     * @param node := nodo imprimido.
     */
    void print(Node<Key, Value>* node){
        if(node == m_nil) {
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
    void _in_order(Node<Key, Value>* node, std::vector<std::pair<Key, Value>>& saida){
        if(node == m_nil) return;
        _in_order(node->left, saida);
        saida.push_back(node->par);
        _in_order(node->right, saida);
    }


    /**
     * @brief Retorna um nodo se e somente se uma dada chave existir 
     *        na arvore. Retorna NIL caso contrario.
     * 
     * @param k := chave a ser buscada na arvore.
     */
    Node<Key, Value>* search(Key k) {
        Node<Key, Value>* atual = m_root;
        // Enquanto nao achar k e nao chegar no fim da arvore
        while(atual != m_nil && atual->par.first != k){ 
            if(k < atual->par.first){
                atual = atual->left;
            } else {
                atual = atual->right;
            }
        }
        return atual;
    }
    
    /**
     * @brief Remove um nodo da arvore e retorna o nodo que assumir seu lugar.
     * 
     * @param z := nodo que sera removido da arvore
     */
    void _removeNode(Node<Key, Value> *z) {
        Node<Key, Value>* y; // nodo a ser removido fisicamente
        Node<Key, Value>* x; // filho de y
        
        // Caso 1: Se pelo menos um dos filhos de z eh NIL,
        // o outro filho assume
        if(z->left == m_nil || z->right == m_nil){
            y = z;
        }
        else { // Caso 2: z tem exatamente dois filhos diferente de NIL
            y = z->right;
            while(y->left != m_nil){ // navega ate o sucessor de z
                y = y->left;
            }
        }
        // y sempre tem um filho NIL, e 
        // x sempre eh o outro filho
        if(y->left != m_nil) x = y->left;
        else x = y->right;
        
        x->parent = y->parent; // atualiza pai

        if(y->parent == m_nil)  m_root = x;
        else {
            if(y == y->parent->left) y->parent->left = x;
            else y->parent->right = x;
        }
        // copia a chave
        if(y != z) z->par = y->par;
        // precisa rebalancear/recolorir?
        if(y->color == BLACK) fixupDelete(x);
        delete y;
    }


    /**
     * @brief Promete, se necessario, rebalancear e recolorir a arvore
     *        apos a remocao de um nodo.
     * 
     * @param node := nodo pivo do rebalanceamento
     */
    void fixupDelete(Node<Key, Value> *x) {
        while(x != m_root && x->color != BLACK){
            if(x == x->parent->left){ // x eh filho esquerdo
                Node<Key, Value>* w = x->parent->right;
                // Caso 1: w eh vermelho
                if(w->color == RED){
                    w->color = BLACK;
                    x->parent->color = RED;
                    leftRotate(x->parent);
                    w = x->parent->right;
                } 
                // Caso 2: w eh preto e tem dois filhos pretos
                if(w->left->color == BLACK && w->right->color == BLACK){
                    w->color = RED;
                    if(x->parent->color == RED) { // se pai eh vermelho
                        x->parent->color = BLACK; // recolore
                        return; // acabou
                    }
                    x = x->parent;
                } else {
                    // Caso 3: filho direito de w eh preto
                    if(w->right->color == BLACK){
                        w->left->color = BLACK;
                        w->color = RED;
                        rightRotate(w);
                        w = x->parent->right;
                    }
                    // Caso 4: filho direito de w eh vermelho
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->right->color = BLACK;
                    leftRotate(x->parent);
                    x = m_root;
                }
            } else {
                Node<Key, Value>* w = x->parent->left;
                // Caso 1: w eh vermelho
                if(w->color == RED){
                    w->color = BLACK;
                    x->parent->color = RED;
                    rightRotate(x->parent);
                    w = x->parent->left;
                } 
                // Caso 2: w eh preto e tem dois filhos pretos
                if(w->right->color == BLACK && w->left->color == BLACK){
                    w->color = RED;
                    if(x->parent->color == RED) { // se pai eh vermelho
                        x->parent->color = BLACK; // recolore
                        return; // acabou
                    }
                    x = x->parent;
                } else {
                    // Caso 3: filho esquerdo de w eh preto
                    if(w->left->color == BLACK){
                        w->right->color = BLACK;
                        w->color = RED;
                        leftRotate(w);
                        w = x->parent->left;
                    }
                    // Caso 4: filho esquerdo de w eh vermelho
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->left->color = BLACK;
                    rightRotate(x->parent);
                    x = m_root;
                }
            } 
        }
        x->color = BLACK;
    }

    /**
     * @brief Conta e retorna o numero de nodos na arvore.
     */
    int _size(Node<Key, Value>* node){
        if(node == m_nil) return 0;
        return 1 + _size(node->left) + _size(node->right);
    }

public:
    // Construtor padrao: cria uma arvore vazia
    RB() {
        rotacoes = 0;
        comparacoes = 0;
        m_nil = new Node<Key, Value>(std::pair<Key, Value>(), BLACK, nullptr, nullptr, nullptr);
        m_nil->parent =  m_nil->left = m_nil->right = m_nil;
        m_root = m_nil;
    }
    
    // Destrutor: libera todos os nodos da arvore
    // para a memoria
    ~RB() {
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
        _insert(p);
    }

    // Funcao que libera os nodos da arvore para a memória.
    // Atualiza m_root para NIL, indicando que a arvore ficou vazia.
    void clear() {
        rotacoes = 0;
        m_root = _clear(m_root);
    }

    /**
     * @brief Retorna true se e somente se a arvore estiver vazia.
     *        Retorna false caso contrario.
     */
    bool empty(){
        return m_root == m_nil;
    }

    void print(){
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
    bool contains(const Key k) {
        return search(k) != m_nil;
    }
 
    /**
     * @brief Promete remover o par, cuja chave eh igual a k, da arvore.
     * 
     * @param k := chave a ser buscada
     */
    void remove(Key k) {
        Node<Key, Value>* aux = search(k);
        if(aux == m_nil) std::cout<<"Chave inexistente\n";
        else _removeNode(aux);
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
        Node<Key, Value>* nodo = search(k);
        if(nodo != m_nil) return nodo->par.second;
        insert(std::make_pair(k, Value()));
        return (search(k))->par.second;
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
        Node<Key, Value>* nodo = search(k);
        if(nodo != m_nil) return nodo->par.second;
        throw std::invalid_argument("A chave dada nao esta na arvore");
    }
    
};
#endif
    