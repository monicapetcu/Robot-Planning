/* Copyright 2018 Petcu Monica-Alexandra */

#ifndef DEQUE_H_
#define DEQUE_H_

#include <iostream>
#include <string>
#include <fstream>

template <typename T>
struct Node {
    Node<T> *next;
    Node<T> *prev;

    T x, y;
    T NrBoxes;
    std::string command;
    int contor;

    Node(std::string commandType, T coordx, T coordy, T boxes, int ctr) {
        next = NULL;
        prev = NULL;
        this->x = coordx;
        this->y = coordy;
        this->NrBoxes = boxes;
        this->command = commandType;
        this->contor = ctr;
    }
};

template <typename T>
class Deque {
 private:
    Node<T> *head;
    Node<T> *tail;
    int numElements;

 public:
    // Constructor
    Deque() {
        head = NULL;
        tail = NULL;
        numElements = 0;
    }

    // Destructor
    ~Deque() {
        Node<T> *current = head;

        while (current != tail) {
            head = head->next;
            delete head->prev;
            current = head;
        }
        delete current;
    }

    // Adaug un nod nou la sfarsitul listei si il initializez
    void addLast(std::string commandType, T coordx, T coordy, T boxes,
            int ctr) {
        Node<T> *nod = new Node<T>(commandType, coordx, coordy, boxes, ctr);
        nod->next = NULL;

        if ((head == NULL) && (tail == NULL)) {
            nod->prev = NULL;
            head = tail = nod;
            numElements++;
        } else {
            nod->prev = tail;
            tail->next = nod;
            tail = nod;
            numElements++;
        }
    }

    // Adaug un nod nou la inceputul listei si il initializez
    void addFirst(std::string commandType, T coordx, T coordy, T boxes,
            int ctr) {
        Node<T> *nod = new Node<T>(commandType, coordx, coordy, boxes, ctr);
        nod->prev = NULL;

        if ((head == NULL) && (tail == NULL)) {
            nod->next = NULL;
            head = tail = nod;
            numElements++;
        } else {
            nod->next = head;
            head->prev = nod;
            head = nod;
            numElements++;
        }
    }

    // Sterg ultimul nod al listei
    void removeLast() {
        if ((head == tail) && (head != NULL) && (tail != NULL)) {
            Node<T> *aux = head;
            delete aux;
            head = tail = NULL;
        } else if (!((head == NULL) && (tail == NULL))) {
                Node<T> *aux = tail;
                tail = tail->prev;
                tail->next = NULL;
                delete aux;
                numElements--;
        }
    }

    // Sterg primul nod al listei
    void removeFirst() {
        if ((head == tail) && (head != NULL) && (tail != NULL)) {
            Node<T> *aux = head;
            delete aux;
            head = NULL;
            tail = NULL;
        } else if (!((head == NULL) && (tail == NULL))) {
                Node<T> *aux = head;
                head = head->next;
                head->prev = NULL;
                delete aux;
                numElements--;
            }
    }
    // Verific daca lista este goala
    bool isEmpty() {
        return ((head == NULL) && (tail == NULL));
    }
    // Returnez lungimea listei (numarul de noduri al listei)
    int size() {
        return numElements;
    }
    // Returnez comanda primului element din lista pentru EXECUTE si UNDO
    std::string getCommand() {
        return head->command;
    }
    // Returnez numarul de cutii al comenzii primite initial
    T getNrBoxes() {
        return head->NrBoxes;
    }
    // Returneaza coordonata x (linia) unde trebuie sa faca actiunea
    T get_x() {
        return head->x;
    }
    // Returneaza coordonata y (coloana) unde trebuie sa faca actiunea
    T get_y() {
        return head->y;
    }
    /* Mareste contorul fiecarei comenzi din lista, adica mareste timpul
    cat a stat in lista */
    void updateContor() {
        Node<T> *current = head;
        while (current != NULL) {
            current->contor = current->contor + 1;
            current = current->next;
        }
    }
    // Returneaza timpul primei comenzi
    int getContor() {
        return head->contor;
    }
    /* Afiseaza comenzile listei, locatia unde se produce actiunea si numarul
    de cutii ce trebuiesc executate, pentru fiecare element din lista, in
    ordine de la primul la ultimul */
    void print() {
        Node<T> *current = head;
        while (current != tail) {
            std::cout << current->command << " " << current->x
            << " " << current->y << " " << current->NrBoxes << "; ";
            current = current->next;
        }
        std::cout << current->command << " " << current->x
        << " " << current->y << " " << current->NrBoxes << "\n";
    }
};

#endif  // DEQUE_H_
