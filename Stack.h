/* Copyright 2018 Petcu Monica-Alexandra */

#ifndef STACK_H_
#define STACK_H_

#include <string>
#include <fstream>

template <typename T>
struct Entry {
    T RobotID;
    std::string command;
    T x, y, NrBoxes;
    int Priority;
	Entry<T> () {
		RobotID = -1;
		x = -1;
		y = -1;
		NrBoxes = -1;
		command = "No command";
		Priority = -1;
	}

    Entry(T id, std::string commandType, T coordx, T coordy, T boxes,
    		int prior) {
    	this->RobotID = id;
    	this->x = coordx;
        this->y = coordy;
        this->NrBoxes = boxes;
        this->command = commandType;
        this->Priority = prior;
    }

	Entry<T>& operator=(const Entry<T>& a);
};
// Am supraincarcat operatorul "=" pentru a putea face atribuiri de Entry-uri
template <typename T>
	Entry<T>& Entry<T>::operator=(const Entry<T>& a) {
		if(this != &a) {
			RobotID = a.RobotID;
			x = a.x;
			y = a.y;
			NrBoxes = a.NrBoxes;
			command = a.command;
			Priority = a.Priority;
		}
		return *this;
	}

template <typename T>
class Stack {
 private:
    int defaultCapacity = 5;
    int expandFactor = 2;

    int numElements;
    int maxCapacity;
    Entry<T> *data;

 public:
    // Constructor
    Stack() {
        numElements = 0;
        maxCapacity = defaultCapacity;

        data = new Entry<T>[maxCapacity];
    }

    // Destructor
    ~Stack() {
        delete[] data;
    }

    // Adaug element la inceputul stivei si il initiez
    void addFirst(int id, std::string commandType, int coordx,
     int coordy, int boxes, int prior) {
    	Entry<T> *newEntry =
    		new Entry<T>(id, commandType, coordx, coordy, boxes, prior);
    	Entry<T> *newData = new Entry<T>[maxCapacity * expandFactor];
    	/* Verific daca exista elemente in lista, daca nu, primul element
    	o sa fie elementul introdus acum. Daca exista, si numarul de elemente
    	nu depaseste inca marimea maxima a stivei, il adaug pe prima pozitie,
    	mutand restul elementelor cu o pozitie la dreapta. Daca stiva este
    	plina, o realoc cu o dimensiune mai mare */
    	if (numElements == 0) {
    		numElements++;
    		data[0] = *newEntry;
    	} else if (numElements < maxCapacity) {
    				for (int i = numElements - 1; i >= 0; i--) {
    					data[i + 1] = data[i];
    				}
    				numElements++;
    				data[0] = *newEntry;
    			} else {
    				for (int i = 0; i < numElements; i++) {
    					newData[i] = data[i];
    				}
    				delete[] data;
    				maxCapacity *= expandFactor;
    				data = new Entry<T>[maxCapacity];
    				numElements++;
    				for (int i = 0; i < numElements; i++) {
    					data[i + 1] = newData[i];
    				}
    				data[0] = *newEntry;
    			}
    	delete[] newData;
    	delete newEntry;
    }

    // Elimin primul element al stivei
    void removeFirst() {
        if (numElements == 0) {
        	return;
        }
        for (int i = 0; i < numElements - 1; i++) {
        	data[i] = data[i + 1];
        }
        numElements--;
    }
    // Afisez primul element al stivei
    Entry<T> popFirst() {
    	Entry<T> first;
    	first = data[0];
    	return first;
    }

    // Verific daca stiva este goala
    bool isEmpty() {
        if (!numElements) {
        	return true;
    	}
    	return false;
    }

    // Returnez lungimea stivei (numarul de elemente din stiva)
    int size() {
        return numElements;
    }

    // Getters
    Entry<T> *getData() {
        return data;
    }
    // Returnez ID-ul robotului ce se afla pe prima pozitie din stiva
    T getId() {
    	return data[0].RobotID;
    }
    // Returnez comanda robotului ce se afla pe prima pozitie din stiva
    std::string getCommand() {
    	return data[0].command;
    }
    /* Returnez coordonata x unde trebuie sa execute comanda robotul ce se afla
    pe prima pozitie din stiva */
    T get_x() {
    	return data[0].x;
    }
    /* Returnez coordonata y unde trebuie sa execute comanda robotul ce se afla
    pe prima pozitie din stiva */
    T get_y() {
    	return data[0].y;
    }
    /* Returnez numarul de cutii asupra caruia robotul ce se afla pe prima
    pozitie din stiva trebuie sa faca executia */
    T getNrBoxes() {
    	return data[0].NrBoxes;
    }
    /* Returnez:
    - Pentru stiva undo - prioritatea comenzii pentru a putea sa o elimin
    de la inceputul sau sfarsitul listei in cazul comenzii ADD, sau pentru
    a o putea adauga inapoi la inceputul sau sfarsitul listei in cazul comenzii
    EXECUTE
    - Pentru stiva lst_exe_comm - numarul de cutii executate, de fapt, de
    robotul curent, intrucat exista posibilitatea de a nu avea numarul de cutii
    pentru DROP fata de comanda primita sau de a nu gasi numarul de cutii
    primit prin comanda GET
    - Pentru stiva time - returneaza timpul cat a stat in coada robotului sau
    comanda aflata la inceputul stivei
    */
    int getPriority() {
    	return data[0].Priority;
    }
    /* Returneaza cat timp a stat in coada ultima comanda executata de robotul
	cu ID-ul cautat */
    int getOldContor(int id) {
    	for (int i = 0; i < numElements; i++) {
        	if (data[i].RobotID == id) {
        		return data[i].Priority;
        	}
        }
    }
    // Afiseaza primul element al listei
    void printTop() {
    	if(numElements > 0) {
    		std::cout << data[0].RobotID << ": " << data[0].command
    		<< " " << data[0].x << " " << data[0].y <<
    		" " << data[0].Priority << "\n";
    	} else {
    		std::cout << "No command was executed" << "\n";
    	}
    }
};

#endif  // STACK_H_
