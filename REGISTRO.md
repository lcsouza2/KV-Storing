Comecei o projeto planejando o esqueleto da aplicação e qual parte codificar primeiro, foi decidido começar pelas SSTables visto que são o core de SO e principal mecanismo no armazenamento dos dado

Usei o Gemini para esclarecer alguns conceitos e ajudar con algumas decisões arquiteturais, os prompts foram:

1. O meu é o tema 5, KV-Store com LSM tree, parece complexo e tenho dificuldade com alguns dos requisitos, pode aprofundar os conceitos de memtable e arvores AVL / treap? (PDF anexado)

2. Agora explique níveis de SSTables com índice esparso e Bloom filter; merge k-vias na compactação entre níveis.


Implementando a lib de logging precisei de uma forma de suportar strings formatadas, não sabia como fazer então pedi ajuda ao Gemini com ideias, usei o prompt:

```
*Codigo colado*
Como eu adiciono suport a strings formatadas?
```

Ele sugeriu usar a lib stdarg.h, que é uma lib padrão do C para lidar com argumentos variáveis em funções. Com ela, você pode criar funções que aceitam um número variável de argumentos, permitindo que você formate strings de maneira flexível.

Consegui implementar logging corretamente então parti para implementação das SSTables, implementei uma função para criar uma SSTable, que gera um arquivo físico no disco e retorna uma struct com informações sobre a SSTable criada. A função também gera o caminho do arquivo com base no nível da SSTable e no número de arquivos já criados nesse nível.

Criada e testada essa função, comecei a implementar árvores AVL para armazenar as chaves e valores na memória antes de serem persistidos em disco. O auto complete do Copilot no VSCode implementou algumnas coisas como rotação, mas eu ainda tinha dúvidas sobre o funcionamento do código. Então eu copiei o código gerado e pedi para o Gemini explicar o que cada parte fazia, ele explicou detalhadamente o funcionamento da árvore AVL, incluindo como as rotações funcionam para manter a árvore balanceada. Após entender o código eu separei o que era baixo nível e criei uma interface para a árvore AVL, além de corrigir alguns bugs que estavam presentes no código gerado pelo Copilot.
