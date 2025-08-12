# Sentiment Analysis with Binary Trees (BST vs. AVL)

**INF01203 - Estruturas de Dados - UFRGS - Trabalho Final - 2023/2**

## About

The main objective is to implement a sentiment analysis application to compare the performance of two distinct data structures: a Binary Search Tree (BST) and an AVL Tree (a type of self-balancing binary search tree). The application calculates the "polarity" (positive or negative) of sentences based on a lexicon of words and their respective scores, measuring the efficiency of each tree by the total number of key comparisons during the search for words.

## Features

* **Sentiment Analysis:** Calculates a polarity score for a set of sentences.

* **Tree Storage:** The lexicon of words and scores is loaded into a binary tree for querying.

* **Two Implementations:** The project contains two versions of the program for comparison purposes:

  1. analise_sentimento_bst.c: Uses a standard Binary Search Tree.

  2. analise_sentimento_avl.c: Uses an AVL Tree to ensure balancing and optimize searches.

* **Comparison Count:** Measures and reports the total number of comparisons performed on the tree, serving as a performance metric.

* **Command Line Interface:** The program is run via a terminal, receiving input and output files as arguments.

* **Case-Insensitive:** Lexicon searches are not case-sensitive.

## How it works

1. **Lexicon Loading:** The program reads a text file (**lexico.txt**) containing words and their sentiment scores. Each word-score pair is inserted into the tree (BST or AVL). The words in the lexicon are the key, and the insertion order follows the order in which they appear in the file.

2. **Sentence Processing:** Next, the program reads a second file (**sentencas.txt**) line by line.

3. **Score Calculation:** For each sentence, the program divides it into words (tokens). Each word is searched in the tree. If a word is found, its score is added to the total sentence score.

4. **Output Generation:** Finally, the results are written to output files (**saida_bst.txt** and **saida_avl.txt**), containing each sentence, its calculated score, and, at the end of the file, the total number of comparisons performed during the searches.

* **lexico.txt** file:

```txt
abominavel -3.5
aceitar 1.6
adoro 3.0
alegre 2.5
bom 1.5
bonito 2.0
decepcionante -2.8
feliz 2.2
horrivel -4.0
incrivel 3.2
mau -1.8
maravilhoso 3.8
odio -3.0
pessimo -5.0
problema -1.5
ruim -2.5
terrivel -4.5
triste -2.0
```
* **sentencas.txt** file:

```txt
O atendimento foi pessimo e horrivel
Eu adoro este lugar ele é maravilhoso e incrivel
O filme foi bom mas o final foi decepcionante
Que dia feliz e alegre
O produto apresentou um problema e a qualidade é ruim
```

## How to Compile and Run

* **Compiling:**

A C compiler (like GCC) is required. Open the terminal at the root of the project and run the following commands to compile both versions of the program:

```bash
# To compile the Binary Search Tree (BST) version
gcc analise_sentimento_bst.c -o analise_bst

# To compile the AVL Tree version
gcc analise_sentimento_avl.c -o analise_avl
```
* **Running:**

To run, use the following format on the command line:

```bash
# Running the analysis with BST
./analise_bst lexico.txt sentencas.txt saida_bst.txt

# Running the analysis with AVL
./analise_avl lexico.txt sentencas.txt saida_avl.txt
```

After running, the files saida_bst.txt and saida_avl.txt will be created with the results.

## Performance and Results Analysis

The main performance evaluation criterion in this work is the number of comparisons required to find all the words in the sentences within the lexicon stored in the tree.

* **saida_bst.txt** file:

```txt
Score: -9.00 | Sentenca: O atendimento foi pessimo e horrivel
Score: 10.00 | Sentenca: Eu adoro este lugar ele é maravilhoso e incrivel
Score: -1.30 | Sentenca: O filme foi bom mas o final foi decepcionante
Score: 4.70 | Sentenca: Que dia feliz e alegre
Score: -4.00 | Sentenca: O produto apresentou um problema e a qualidade é ruim

--------------------------------------------------
Analise com Arvore Binaria de Busca (BST):
Numero total de comparacoes: 388
```

* **saida_avl.txt** file:

```txt
Score: -9.00 | Sentença: O atendimento foi pessimo e horrivel
Score: 10.00 | Sentença: Eu adoro este lugar ele é maravilhoso e incrivel
Score: -1.30 | Sentença: O filme foi bom mas o final foi decepcionante
Score: 4.70 | Sentença: Que dia feliz e alegre
Score: -4.00 | Sentença: O produto apresentou um problema e a qualidade é ruim

--------------------------------------------------
Analise com Arvore AVL:
Numero total de comparacoes: 160
```

The results clearly demonstrate the superiority of the AVL Tree. With 160 comparisons, it was more than 2.4 times more efficient than the BST, which required 388 comparisons for the same task. This is because the AVL tree remains balanced, ensuring that the search complexity is always close to O(log n). In contrast, the BST, when inserting lexical data (which may already be partially sorted), tended to be more unbalanced, approaching the worst-case search of O(n).


## License

Distributed under the MIT license. See `LICENSE.txt` for more information.

## Contact

Leonardo Santos - <leorsantos2003@gmail.com>
