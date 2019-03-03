/* Copyright 2018 Petcu Monica-Alexandra */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Stack.h"
#include "Deque.h"

struct Robot {
	Deque<int> coada;
	int currentBoxes = 0;
	/* Contorul reprezinta cat timp a stat o comanda in coada robotului pana sa
	fie executata */
	int contor = 0;
};

int main() {
	// retin intr-o stiva comenzile executate
	Stack<int> lst_exe_comm;
	// retin intr-o stiva comenzile si cat timp a trecut pana au fost executate
	Stack<int> time;
	// retin intr-o stiva comenzile de tip ADD si EXECUTE
	Stack<int> undo;
	int i, j, n, lin, col, x, y, NrBoxes, RobotID;
	int Priority;
	std::string command;
	std::ifstream fin("robots.in");
	std::ofstream fout("robots.out");
	fin >> n >> lin >> col;
	int map[lin][col];
	Robot *robots = new Robot[n];
	// Creez matricea ce reprezinta harta
	for (i = 0; i < lin; i++) {
		for (j = 0; j < col; j++) {
			fin >> map[i][j];
		}
	}
	// Cat timp mai am ce citi din fisier, citesc pe rand comenzile
	while (fin >> command) {
		// Comanda ADD_GET_BOX
		if (command == "ADD_GET_BOX") {
			fin >> RobotID >> x >> y >> NrBoxes >> Priority;
			if (!Priority) {
				robots[RobotID].coada.addFirst("GET", x, y, NrBoxes, 0);
			} else {
				robots[RobotID].coada.addLast("GET", x, y, NrBoxes, 0);
			}
			undo.addFirst(RobotID, "GET", x, y, NrBoxes, Priority);
		}
		// Comanda ADD_DROP_BOX
		if (command == "ADD_DROP_BOX") {
			fin >> RobotID >> x >> y >> NrBoxes >> Priority;
			if (!Priority) {
				robots[RobotID].coada.addFirst("DROP", x, y, NrBoxes, 0);
			} else {
				robots[RobotID].coada.addLast("DROP", x, y, NrBoxes, 0);
			}
			undo.addFirst(RobotID, "DROP", x, y, NrBoxes, Priority);
		}
		// Comanda EXECUTE
		if (command == "EXECUTE") {
			fin >> RobotID;
			if (robots[RobotID].coada.isEmpty()) {
				freopen("robots.out", "a", stdout);
				std::cout << "EXECUTE: No command to execute" << "\n";
				fclose(stdout);
			} else {
				command = robots[RobotID].coada.getCommand();
				NrBoxes = robots[RobotID].coada.getNrBoxes();
				x = robots[RobotID].coada.get_x();
				y = robots[RobotID].coada.get_y();
				undo.addFirst(RobotID, "EXECUTE", x, y, NrBoxes, 0);
				// Verific daca EXECUTE trebuie sa fac pe un GET
				if (command == "GET") {
					if (map[x][y] <= NrBoxes) {
						robots[RobotID].currentBoxes += map[x][y];
						lst_exe_comm.addFirst(RobotID, "GET", x, y,
							NrBoxes, map[x][y]);
						time.addFirst(RobotID, "GET", x, y,
							NrBoxes, robots[RobotID].coada.getContor());
						map[x][y] = 0;
					} else {
						lst_exe_comm.addFirst(RobotID, "GET", x, y, NrBoxes, NrBoxes);
						robots[RobotID].currentBoxes += NrBoxes;
						time.addFirst(RobotID, "GET", x, y, NrBoxes,
						 robots[RobotID].coada.getContor());
						map[x][y] -= NrBoxes;
					}
				}
				// Verific daca EXECUTE trebuie sa fac pe un DROP
				if (command == "DROP") {
					if (robots[RobotID].currentBoxes <= NrBoxes) {
						map[x][y] += robots[RobotID].currentBoxes;
						lst_exe_comm.addFirst(RobotID, "DROP", x, y,
							NrBoxes, robots[RobotID].currentBoxes);
						time.addFirst(RobotID, "DROP", x, y,
							NrBoxes, robots[RobotID].coada.getContor());
						robots[RobotID].currentBoxes = 0;
					} else {
						map[x][y] += NrBoxes;
						robots[RobotID].currentBoxes -= NrBoxes;
						lst_exe_comm.addFirst(RobotID, "DROP", x, y, NrBoxes, NrBoxes);
						time.addFirst(RobotID, "DROP", x, y, NrBoxes,
						 robots[RobotID].coada.getContor());
					}
				}
				robots[RobotID].contor = robots[RobotID].coada.getContor();
				robots[RobotID].coada.removeFirst();
			}
		}
		// Comanda PRINT_COMMANDS
		if (command == "PRINT_COMMANDS") {
			fin >> RobotID;
			if (robots[RobotID].coada.isEmpty()) {
				freopen("robots.out", "a", stdout);
				std::cout << "PRINT_COMMANDS: No command found" << "\n";
				fclose(stdout);
			} else {
				freopen("robots.out", "a", stdout);
				std::cout << "PRINT_COMMANDS: " << RobotID << ": ";
				robots[RobotID].coada.print();
				fclose(stdout);
			}
		}
		// Comanda LAST_EXECUTED_COMMAND
		if (command == "LAST_EXECUTED_COMMAND") {
			if (lst_exe_comm.isEmpty()) {
				freopen("robots.out", "a", stdout);
				std::cout << "LAST_EXECUTED_COMMAND: No command was executed"
					<< "\n";
				fclose(stdout);
			} else {
				freopen("robots.out", "a", stdout);
				std::cout << "LAST_EXECUTED_COMMAND: ";
				lst_exe_comm.printTop();
				fclose(stdout);
			}
		}
		// Comanda UNDO
		if (command == "UNDO") {
			if (undo.isEmpty()) {
				freopen("robots.out", "a", stdout);
				std::cout << "UNDO: No History" << "\n";
				fclose(stdout);
			} else {
				// Verific daca fac UNDO pe un ADD_GET_BOX sau ADD_DROP_BOX
				if (undo.getCommand() == "GET" || undo.getCommand() == "DROP") {
					RobotID = undo.getId();
					Priority = undo.getPriority();
					if (Priority) {
						robots[RobotID].coada.removeLast();
					} else {
						robots[RobotID].coada.removeFirst();
					}
				// Verific daca fac UNDO pe un EXECUTE
				} else if (undo.getCommand() == "EXECUTE") {
					RobotID = undo.getId();
					// Verific daca EXECUTE-ul facea GET
					if (lst_exe_comm.getCommand() == "GET") {
						x = lst_exe_comm.get_x();
						y = lst_exe_comm.get_y();
						robots[RobotID].coada.addFirst("GET", x, y,
							lst_exe_comm.getPriority(), 0);
						map[x][y] += lst_exe_comm.getPriority();
						robots[RobotID].currentBoxes -= lst_exe_comm.getPriority();
					// Verific daca EXECUTE-ul facea DROP
					} else if (lst_exe_comm.getCommand() == "DROP") {
						x = lst_exe_comm.get_x();
						y = lst_exe_comm.get_y();
						robots[RobotID].coada.addFirst("DROP", x, y,
							lst_exe_comm.getPriority(), 0);
						map[x][y] -= lst_exe_comm.getPriority();
						robots[RobotID].currentBoxes += lst_exe_comm.getPriority();
					}
					/* Daca am facut UNDO pe EXECUTE, il scot din stivele time
					si lst_exe_comm */
					time.removeFirst();
					lst_exe_comm.removeFirst();
					robots[RobotID].contor = time.getOldContor(RobotID);
				}
				// Elimin ultima comanda din stiva undo
				undo.removeFirst();
			}
		}
		// Comanda HOW_MUCH_TIME
		if(command == "HOW_MUCH_TIME") {
			if(lst_exe_comm.isEmpty()) {
				freopen("robots.out", "a", stdout);
				std::cout << "HOW_MUCH_TIME: No command was executed" << "\n";
				fclose(stdout);
			} else {
				freopen("robots.out", "a", stdout);
				std::cout << "HOW_MUCH_TIME: " <<
				 robots[lst_exe_comm.getId()].contor << "\n";
				fclose(stdout);
			}
		}
		// Comanda HOW_MANY_BOXES
		if(command == "HOW_MANY_BOXES") {
			fin >> RobotID;
			freopen("robots.out", "a", stdout);
			std::cout << "HOW_MANY_BOXES: " << robots[RobotID].currentBoxes << "\n";
			fclose(stdout);
		}
		/* Maresc timpul in care a stat fiecare comanda in coada fiecarui
		robot */
		for (int i = 0; i < n; i++) {
			if (!(robots[i].coada.isEmpty())) {
				robots[i].coada.updateContor();
			}
		}
	}

	freopen("robots.out", "a", stdout);
	std::cout << "\n";
	fclose(stdout);
	delete[] robots;
	fin.close();
	fout.close();
	return 0;
}
