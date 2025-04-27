#include <iostream>
#include <string>
#include <stack>
using namespace std;

struct Cvor {                           
	string labela;
	string tip;
	int ocena;
	int stepen;
	int red;
	Cvor** naslednici = new Cvor*[10];
	int* ocene = new int[10];
	Cvor(string str,string t,int oc=0,int r=0) {
		labela = str;
		tip = t;
		ocena = oc;
		stepen = 0;
		red = r;
	}
};

struct Elem {               
	Cvor* pitanje;
	Elem* next;
	Elem(Cvor& pit) : pitanje(&pit), next(nullptr) {}
};

Elem* dodajPitanje(Elem* prvi,Cvor* c) {
	Elem* novi = new Elem(*c);
	Elem* tek = prvi;
	if (prvi == nullptr) {
		prvi = novi;
	}
	else {
		while (tek->next) {
			tek = tek->next;
		}
		tek->next = novi;
	}
	return prvi;
}

void ispisiPitanja(Elem* prvi) {
	Elem* tek = prvi;
	int i = 0;
	while (tek) {
		std::cout <<i<<". "<<tek->pitanje->labela<< " ";
		tek = tek->next;
		i++;
	}
}

Cvor* dodajOdgovor(Cvor* pit, Cvor* odg) {
	if (pit->stepen < 10) {
		pit->naslednici[pit->stepen] = odg;
		pit->ocene[pit->stepen] = odg->ocena;
		pit->stepen++;
		return pit;
	}
} 

Cvor* pretraziPitanja(Elem* prvi,string pit) {
	Cvor* pom = nullptr;
	Elem* tek = prvi;
	while (tek) {
		if (tek->pitanje->labela == pit) {
		    pom=tek->pitanje;
		}
		tek = tek->next;
	}
	return pom;
}

Cvor* pronalazakMax(Elem* prvi) {
	Elem* tek = prvi;
	Cvor* pom;
	stack<Cvor*>Stek;
	int max = 0;
	Cvor* maxOdg = nullptr;
	while (tek) {
		int brojac = 0;
		Stek.push(tek->pitanje);
		while (!Stek.empty()) {
			pom = Stek.top();
			Stek.pop();
			for (int i = pom->stepen-1;i >= 0;i--) {
					Stek.push(pom->naslednici[i]);
					brojac++;
			}		
		}
		if (brojac > max) {
			maxOdg = tek->pitanje;
		}
		tek = tek->next;
	}
	return maxOdg;
}

Cvor* pretraziOdgovore(Elem* pit, string odgovor) {
	Cvor* pom;
	stack<Cvor*>Stek;
	Cvor* odg = nullptr;
		Stek.push(pit->pitanje);
		while (!Stek.empty()) {
			pom = Stek.top();
			Stek.pop();
			if (pom->labela == odgovor) {
				odg = pom;
			}
			for (int i = pom->stepen - 1;i >= 0;i--) {
				Stek.push(pom->naslednici[i]);
			}
		}
	return odg;
}

Cvor* urediNiz(Cvor* c) {
	for (int i = 0;i < (c->stepen) - 1;i++) {
		for (int j = i + 1;j < c->stepen;j++) {
			if (c->naslednici[i]->ocena < c->naslednici[j]->ocena) {
				Cvor *pom = c->naslednici[i];
				c->naslednici[i] = c->naslednici[j];
				c->naslednici[j] = pom;
			}
		}
	}
	return c;
}

Cvor* urediStablo(Cvor* c) {
	stack<Cvor*>Stek;
	Cvor* pom;
	Stek.push(c);
		while (!Stek.empty()) {
			pom = Stek.top();
			Stek.pop();
			pom=urediNiz(pom);
			for (int i = pom->stepen - 1;i >= 0;i--) {
				Stek.push(pom->naslednici[i]);
			}
		}
	return c;
}

Cvor* povecajOcenu(Cvor& c,int i) {
	c.ocene[i]++;
	c.naslednici[i]->ocena++;
	return &c;
}

void ispisiStablo(Cvor& c) {
	Cvor* pom=&c;
	
	stack<Cvor*>Stek;
	int red=0;
	bool prviKom=true;
		Stek.push(pom);
		while (!Stek.empty()) {
			pom = Stek.top();
			Stek.pop();string ispis = "";
			for (int i = 0;i < pom->red;i++) { ispis += " "; }
            std::cout <<ispis<< pom->tip<< ": " << pom->labela;if (pom->tip != "Pitanje") { std::cout << "(ocena:" << pom->ocena << ") "; }
			std::cout << endl;
			for (int i = pom->stepen - 1;i >= 0;i--) {
				Stek.push(pom->naslednici[i]);
			}
		}
}

Cvor* izbrisiOdgKom(Cvor& c, int n) {
	stack<Cvor*>Stek;
	Cvor* pom = c.naslednici[n];
	for (int i = n;i < c.stepen;i++) {
		c.naslednici[i] = c.naslednici[i + 1];
		c.ocene[i] = c.ocene[i + 1];
	}
	c.stepen--;
	Stek.push(pom);
	while (!Stek.empty()) {
		pom = Stek.top();
		Stek.pop();
		for (int i = pom->stepen - 1;i >= 0;i--) {
			Stek.push(pom->naslednici[i]);
		}
		delete[]pom->naslednici;
	}
	return &c;
}

Elem* izbrisiStablo(Elem* prvi, int n) {
	Elem* tek;
	if (n == 0) { tek = prvi;prvi = prvi->next; }
	else {
		tek = prvi;
		Elem* pret = prvi;
		for (int i = 0;i < n;i++) {
			tek = tek->next;
			pret = (i == 0) ? prvi : pret->next;
		}
		pret->next = tek->next;
	}
	stack<Cvor*>Stek;
	Cvor* pom ;
	Stek.push(tek->pitanje);
	while (!Stek.empty()) {
		pom = Stek.top();
		Stek.pop();
		for (int i = pom->stepen - 1;i >= 0;i--) {
			Stek.push(pom->naslednici[i]);
		}
		delete[]pom->ocene;
		delete[]pom->naslednici;
	}
	delete tek;
	return prvi;
}

Elem* izbrisiStablo(Elem* el) {
	stack<Cvor*>Stek;
	Cvor* pom;
	Stek.push(el->pitanje);
	while (!Stek.empty()) {
		pom = Stek.top();
		Stek.pop();
		for (int i = pom->stepen - 1;i >= 0;i--) {
			Stek.push(pom->naslednici[i]);
		}
		delete[]pom->ocene;
		delete[]pom->naslednici;
	}
	el->pitanje = nullptr;
	return el;
}

void ispisiNaslednike(Cvor& c){ 
	for (int i = 0;i < c.stepen;i++) 
	{ std::cout << i << ". " << c.naslednici[i]->labela<<"(ocena: "<< c.naslednici[i]->ocena<< ") "; }
}

int main() {
	Elem* prvi = nullptr;
	bool kraj = false;
	while (!kraj) {
		std::cout <<
			"1.Dodavanje novog pitanja" << endl <<
			"2.Dodavanje odgovora ili komentara" << endl <<
			"3.Pretraga pitanja" << endl <<
			"4.Pretraga odgovora"<<endl<<
			"5.Pitanje sa maksimalnim brojem odgovora i komentara" << endl <<
			"6.Povecanje ocene odgovora" << endl << 
			"7.Uredjivanje stabla" << endl <<
			"8.Ispis pitanja" << endl <<
			"9.Ispis pitanja i odgovarajucih odgovora i komentara" << endl <<
			"10.Brisanje odgovora ili komentara" << endl << 
			"11.Brisanje pitanja i odgovarajucih odgovora i komentara" << endl <<
			"12.Kraj programa" << endl << endl <<
			"Vas izbor?" << endl;
		int izbor;
		std::cin >> izbor;
		switch (izbor) {
		case 1: {
			string pitanje;
			std::cout << "Unesite pitanje: " << endl;
			std::cin.ignore();
			std::getline(std::cin, pitanje);
			Cvor* c = new Cvor(pitanje, "Pitanje");
			prvi = dodajPitanje(prvi, c);
			std::cout << endl;
			break;
		}
		case 2: {
			int oc;
			int r = 1;
			if (prvi == nullptr) { std::cout << "Ne mozeta uneti odgovor/komentar bez prethodnog pitanja" << endl;break; }
			else {
				string odgovor;
				std::cout << "Na koje pitanje zelita da unesete odgovor/komentar?(unesite indeks pitanja)" << endl;
				ispisiPitanja(prvi);std::cout << endl;
				int n;std::cin >> n;
				bool tacno = true;
				Elem* tek = prvi;
				while (n) {
					tek = tek->next;
					if (tek == nullptr) {
						std::cout << "Presli ste opseg pitanja" << endl << endl;
						tacno = false;
						break;
					}
					n--;
				}
				if (!tacno) { break; }
				std::cout << "Da li zelite na ovo pitanje da date odgovor(1 za da,0 za komentar u okviru pitanja)" << endl;
				std::cin >> n;
				if (n) {
					if (tek->pitanje->stepen == 10) {
						std::cout << "Unet je maksimalan broj odgovora za ovo pitanje" << endl << endl;
						break;
					}
					else {
						std::cout << "Unesite odgovor" << endl;
						std::cin.ignore();
						std::getline(std::cin, odgovor);
						std::cout << "Unesite ocenu za ovaj odgovor" << endl;
						std::cin >> oc;
						Cvor* c = new Cvor(odgovor, "Odgovor", oc,r);
						tek->pitanje = dodajOdgovor(tek->pitanje, c);
						std::cout << endl;
						break;
					}
				}
				else {
					Cvor* pom = tek->pitanje;
					if (pom->stepen == 0) { std::cout << "Ne postoji cvor na koji biste dodali komentar" << endl << endl;break; }
					else {
						while (tacno) {
							ispisiNaslednike(*pom);std::cout << endl;
							std::cout << "Da li zelite da dodate komentar na neki od ponudjenih odgovora(1 za da,0 za ne)" << endl;
							std::cin >> n;
							if (n) {
								std::cout << "Unesite na koji odgovor dodajete komentar(unesite indeks odgovora)" << endl;
								std::cin >> n;
								if (n > pom->stepen) { std::cout << "Prekoracili ste indeks odgovora" << endl << endl;break; }
								if (pom->stepen == 10) { std::cout << "Unet je maksimalan broj komentar za ovaj odgovor" << endl << endl;break; }
								else {
									std::cout << "Unesite komentar" << endl;
									std::cin.ignore();
									std::getline(std::cin, odgovor);
									std::cout << "Unesite ocenu za ovaj komentar" << endl;
									std::cin >> oc;r++;
									Cvor* c = new Cvor(odgovor, "Komentar", oc,r);
									pom->naslednici[n] = dodajOdgovor(pom->naslednici[n], c);
									std::cout << endl;
									break;
								}
							}
							else {
								std::cout << "U kom smeru zelite da idete ka listovima(unesite indeks odgovora)" << endl;
								std::cin >> n;
								if ((n > pom->stepen) || (pom->naslednici[n]->stepen == 0)) { std::cout << "Ne postoji takav cvor" << endl << endl;break; }
								else {
									pom = pom->naslednici[n];r++;
								}
							}
						}
					}
				}
			}
			break;
		}
		case 3: {
			std::cout << "Unesite pitanje za koje zelite da dobijete pokazivac: " << endl;
			string pitanje;std::cin.ignore();std::getline(std::cin, pitanje);
			Cvor* c = pretraziPitanja(prvi, pitanje);
			std::cout << "Adresa pokazivaca sa trazenim pitanjem je: " << c << endl << endl;
			break;
		}
		case 4: {
			if (prvi == nullptr) { std::cout << "Ne mozeta odraditi operaciju bez prethodnog pitanja" << endl << endl;break; }
			ispisiPitanja(prvi);
			std::cout <<endl<< "Za koje pitanje zelite da pretrazite odgovore (unesite indeks pitanja)" << endl;
			int n;cin >> n;	
			Elem* tek = prvi;
			bool tacno = true;
			while (n) {
				tek = tek->next;
				if (tek == nullptr) {
					std::cout << "Presli ste opseg pitanja" << endl << endl;
					tacno = false;
					break;
				}
				n--;
			}
			if (!tacno) { break; }
			std::cout << "Unesite koji odgovor/komentar zelite da pretrazite" << endl;
			string odg;cin.ignore();getline(cin, odg);
			Cvor* c = pretraziOdgovore(tek, odg);
			std::cout << "Trazeni pokazivac je na adresi: " << c<<endl<<endl;
			break;
		}
		case 5: {
			if (prvi == nullptr) { std::cout << "Ne mozeta odraditi operaciju bez prethodnog pitanja" << endl << endl;break; }
			Cvor* c = pronalazakMax(prvi);
			std::cout << "Pitanje sa najvecim brojem odgovora i komentara je: " << c->labela << endl << endl;
			break;}
		case 6: {
			int oc;
			if (prvi == nullptr) { std::cout << "Ne mozete povecati ocenu odgovora/komentara bez prethodnog pitanja" << endl;break; }
			else {
				string odgovor;
				std::cout << "U okviru kog pitanja zelite da povecavate ocenu?(unesite indeks pitanja)" << endl;
				ispisiPitanja(prvi);std::cout << endl;
				int n;std::cin >> n;
				bool tacno = true;
				Elem* tek = prvi;
				while (n) {
					tek = tek->next;
					if (tek == nullptr) {
						std::cout << "Presli ste opseg pitanja" << endl << endl;
						tacno = false;
						break;
					}
					n--;
				}
				if (!tacno) { break; }
				Cvor* pom = tek->pitanje;
				while (tacno) {
					if (pom->stepen == 0) { break;std::cout <<"Ne postoji odgovor kojem biste povecali ocenu"<<endl<<endl; }
					ispisiNaslednike(*pom);std::cout << endl;
					std::cout << "Da li zelite da povecate ocenu nekog od ponudjenih odgovora(1 za da,0 za komentar u okviru pitanja)" << endl;
					std::cin >> n;
					if (n) {
						std::cout << "Unesite indeks odgovora za povecanje" << endl;
						std::cin >> oc;
						if (pom->stepen < oc) {
							std::cout << "Uneli ste nepostojeci indeks" << endl << endl;break;
						}
						else {
							pom = povecajOcenu(*pom, oc);
							std::cout << endl;
							break;
						}
					}
					else {
						std::cout << "U kom smeru zelite da idete ka listovima(unesite indeks odgovora)" << endl;
						std::cin >> n;
						if (pom->naslednici[n]->stepen == 0) { std::cout << "Ne postoji takav cvor" << endl;break; }
						else {
							pom = pom->naslednici[n];
						}
					}
				}
				break;
			}
		case 7: {
			if (prvi == nullptr) { std::cout << "Ne mozeta odraditi operaciju bez prethodnog pitanja" << endl << endl;break; }
			ispisiPitanja(prvi);
			std::cout << endl << "Koje stablo zelite da uredite?(unesite indeks stabla)" << endl;
			int n;std::cin >> n;
			Elem* tek = prvi;
			bool tacno = true;
			while (n) {
				tek = tek->next;
				if (tek == nullptr) {
					std::cout << "Presli ste opseg pitanja" << endl << endl;
					tacno = false;
					break;
				}
				n--;
			}
			if (!tacno) { break; }
			tek->pitanje = urediStablo(tek->pitanje);
			std::cout << endl;
			break;
		}
		case 8:
			if (prvi == nullptr) { std::cout << "Ne mozeta odraditi operaciju bez prethodnog pitanja" << endl << endl;break; }
			ispisiPitanja(prvi);std::cout << endl << endl;
			break;
		case 9: {
			if (prvi == nullptr) { std::cout << "Ne mozeta odraditi operaciju bez prethodnog pitanja" << endl << endl;break; }
			ispisiPitanja(prvi);
			std::cout << endl << "Koje stablo zelite da ispisete?(unesite indeks stabla)" << endl;
			int n;std::cin >> n;
			Elem* tek = prvi;
			bool tacno = true;
			while (n) {
				tek = tek->next;
				if (tek == nullptr) {
					std::cout << "Presli ste opseg pitanja" << endl << endl;
					tacno = false;
					break;
				}
				n--;
			}if (tacno) {
				ispisiStablo(*tek->pitanje);
				std::cout << endl << endl;;
			}
			break;
		}
		case 10: {
			int oc;
			if (prvi == nullptr) { std::cout << "Ne mozete izbrisati odgovor/komentar bez prethodnog pitanja" << endl;break; }
			else {
				string odgovor;
				std::cout << "U okviru kog pitanja zelite da izbrisete odgovor/komentar?(unesite indeks pitanja)" << endl;
				ispisiPitanja(prvi);std::cout << endl;
				int n;std::cin >> n;
				bool tacno = true;
				Elem* tek = prvi;
				while (n) {
					tek = tek->next;
					if (tek == nullptr) {
						std::cout << "Presli ste opseg pitanja" << endl << endl;
						tacno = false;
						break;
					}
					n--;
				}	
				if (tacno) {
					Cvor* pom = tek->pitanje;
					while (tacno) {
						ispisiNaslednike(*pom);std::cout << endl;
						std::cout << "Da li zelite da izbrisete neki od ponudjenih odgovora(1 za da,0 za komentar u okviru pitanja)" << endl;
						std::cin >> n;
						if (n) {
							std::cout << "Unesite indeks odgovora za brisanje" << endl;
							std::cin >> oc;
							if (pom->stepen < oc) {
								std::cout << "Uneli ste nepostojeci indeks" << endl << endl;break;
							}
							else {
								pom = izbrisiOdgKom(*pom, oc);
								std::cout << endl;
								break;
							}
						}
						else {
							std::cout << "U kom smeru zelite da idete ka listovima(unesite indeks odgovora)" << endl;
							std::cin >> n;
							if (pom->naslednici[n]->stepen == 0) { std::cout << "Ne postoji takav cvor" << endl;break; }
							else {
								pom = pom->naslednici[n];
							}
						}
					}
				}
				break;
			}
		case 11: {
			if (prvi == nullptr) { std::cout << "Ne mozeta odraditi operaciju bez prethodnog pitanja" << endl << endl;break; }
			ispisiPitanja(prvi);std::cout << endl;
			std::cout << "Koje stablo zelite da izbrisete?" << endl;
			int n;std::cin >> n;int j = n;
			Elem* pom = prvi;
			bool tacno = true;
			while (j) {
				pom = pom->next;
				if (pom == nullptr) {
					std::cout << "Presli ste opseg pitanja" << endl << endl;
					tacno = false;
					break;
				}
				j--;
			}
			if (!tacno) { break; }
			prvi = izbrisiStablo(prvi, n);
			std::cout << endl;
			break;
		}
		case 12:
			kraj = true;
			Elem* stari;
			while (prvi) {
				stari = prvi;
				prvi = prvi->next;
				izbrisiStablo(stari);
				delete stari;
			}
			break;
		default:
			std::cout << "Nedozvoljen izbor\n" << endl;break;
		}
		}
		}
	}
}
