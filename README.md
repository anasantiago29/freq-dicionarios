# Contador de frequência utilizando Dicionários
## Status: em construção :hammer:
## 🎯 Contexto e Propósito
Este projeto nasceu da necessidade de resolver um desafio clássico de processamento de texto: **a contagem eficiente de ocorrências de palavras em grandes volumes de dados.**

### ⚠️ Problemática 1: O Desafio Prático
Como processar um arquivo `.txt` de tamanho considerável e contabilizar a frequência de cada palavra de forma rápida? 

**Solução:** Desenvolver um sistema em C++ fundamentado no conceito de **Dicionário** (*Map*), onde cada palavra atua como uma chave única associada ao seu número de repetições.

## 📘 Dicionários

Dicionários são estruturas de dados poderosas que armazenam pares de chave-valor. No contexto deste projeto, eles oferecem:

* **Versatilidade**: Permitem o uso de `strings` (as palavras) como chaves de busca.
* **Eficiência**: Operações de busca, inserção e remoção com complexidade média logarítmica ou até mesmo constante.
* **Relevância**: São a base para tecnologias essenciais como Bancos de Dados, caches de alto desempenho e sistemas de DNS.

### 🔍 Problemática 2: A Escolha da Estrutura Ideal
Embora o conceito de Dicionário seja claro, surge a dúvida: **qual estrutura de dados oferece o melhor desempenho para essa tarefa específica?**

**Solução:** Implementação e comparação de 4 dicionários com estruturas básicas diferentes, sendo duas árvores e duas tabelas de dispersão.
## 🌳 Árvores

De modo geral, as árvores selecionadas buscam manter a menor altura possível, dado que essa métrica dita a complexidade das operações. Cada uma, porém, resolve o desafio do balanceamento de forma distinta para garantir eficiência em buscas e inserções.

### Árvore AVL vs Árvore Rubro-Negra

As duas estruturas garantem complexidade de pior caso $O(\log n)$, mas diferem na frequência e na lógica de rebalanceamento.

#### **AVL**
* **Balanceamento Rígido**: Realiza rotações sempre que o fator de balanço (diferença entre as alturas das subárvores) foge do intervalo $\{-1, 0, 1\}$.
* **Foco em Busca**: Por ser mais estritamente balanceada, tende a ser ligeiramente mais rápida para consultas frequentes.
* **Implementação**: Mais intuitiva em suas rotações, utilizando 4 tipos básicos de rebalanceamento (Simples e Dupla, à Direita ou Esquerda).

#### **Rubro-Negra**
* **Balanceamento Baseado em Cores**: Utiliza a "cor" do nó (Red ou Black) para manter a árvore aproximadamente balanceada.
* **Foco em Inserção/Remoção**: Como é menos rígida que a AVL, costuma exigir menos rotações durante alterações frequentes no dicionário.
* **Estrutura**: Utiliza nós com ponteiros para o pai e apenas 2 tipos de rotações.
* **Possui 5 propriedades fundamentais**:
    1. Todo nó é **Vermelho** ou **Preto**.
    2. A **Raiz** da árvore é obrigatoriamente **Preta**.
    3. Todo nó **NIL** (folha externa) é **Preto**.
    4. Se um nó é **Vermelho**, seus filhos devem ser **Pretos** (não há vermelhos consecutivos).
    5. **Caminho Preto**: Para todo nó, todos os caminhos dele até as folhas descendentes possuem a mesma quantidade de nós pretos.

## 🔡 Tabelas de Dispersão

As tabelas de dispersão visam prover um acesso eficiente aos dados por meio de uma função de espalhamento (*hash*), sendo sua principal vantagem o tempo médio constante $O(1)$ para as operações de inserção, busca e remoção. Cada tabela implementada utiliza um tratamento de colisão diferente para lidar com chaves que resultam no mesmo índice.

### Chained Hash Table vs Open Addressing Hash Table

#### **Chained Hash Table (Encadeamento Externo)**
* **Tratamento de Colisão**: Utiliza listas ligadas (*chaining*) em cada posição da tabela para armazenar múltiplos elementos que colidem.
* **Alocação Dinâmica**: Cria nós conforme a necessidade em cada posição.
* **Fator de Carga**: Permite um fator de carga ($n/size$) maior que 1 sem falhas catastróficas, embora o desempenho degrade se as listas crescerem muito.
* **Flexibilidade**: A remoção é mais direta (procedimento padrão de listas ligadas).

#### **Open Addressing Hash Table (Endereçamento Aberto)**
Diferente do encadeamento, esta tabela busca por outra posição livre diretamente ao encontrar uma colisão.

* **Tratamento de Colisão**: Foi implementado o **Hashing Duplo**, que utiliza uma função hash secundária para determinar o intervalo de busca por uma nova posição.
* **Fator de Carga**: Exige que a tabela esteja parcialmente vazia para manter a eficiência. Geralmente opera com um fator máximo menor que $0.7$ (nesta implementação, foi utilizado $0.5$).
* **Performance**: Favorece o acesso direto à memória e busca rápida quando bem dimensionada.
* **Desvantagens**: A remoção exige tratamento especial (uso de marcadores de exclusão) e o desempenho diminui rapidamente conforme a taxa de ocupação aumenta.

## :rocket: Como Compilar e Executar

### :clipboard: Pré-requisitos
Você precisará de um compilador C++ instalado (como o `g++`) para processar a interface principal.

### :hammer_and_pick: Compilação e execução
Para compilar o projeto, abra o terminal na pasta raiz e execute o comando abaixo:

```
g++ main.cpp -licuuc -licuio -o main
```

O arquivo main do programa recebe dois parametros de linha de comando, um indicando qual o dicionário escolhido (AVL, Rubro-Negra, Chained Hash Table ou Open Addressing Hash Table) e o outro com o nome do arquivo .txt escolhido para contar as frequências. O padrão utilizado para essas entradas foi:
<div align="center">
<img width="400" height="auto" alt="t1" src="https://github.com/user-attachments/assets/99b97b74-cef7-41cb-b3b7-5ec4405c13e1" />
</div>

### :bulb: Documentação
Para uma compreensão aprofundada sobre as decisões de projeto, conclusões, testes executados e sanar dúvidas ou curiosidades, o **[relatório técnico](./relatorio.pdf)** do projeto está disponível para consulta.

### :woman_technologist: Autora e contato

* **Ana Beatriz Martins Santiago**.
* **Estudante do curso de Ciência da Computação**, Universidade Federal do Ceará (UFC) — Campus Quixadá.
* **E-mail**: anabsantiago0@gmail.com.
