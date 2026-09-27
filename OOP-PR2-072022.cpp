#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <vector> 
#include <string>
#include <regex>
#include <mutex>
#include <thread>
#include <utility>

using namespace std;

const char* PORUKA = "\n-------------------------------------------------------------------------------\n"
"0. PROVJERITE DA LI PREUZETI ZADACI PRIPADAJU VASOJ GRUPI (G1/G2)\n"
"1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR\n"
"2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA CE BITI OZNACENO KAO TM\n"
"3. SPASAVAJTE PROJEKAT KAKO BI SE SPRIJECILO GUBLJENJE URADJENOG ZADATKA\n"
"4. ATRIBUTI, NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTICNI ONIMA KOJI SU KORISTENI U TESTNOM CODE-U, "
"OSIM U SLUCAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE "
"POMOCNE FUNKCIJE MOZETE IMENOVATI I DODAVATI PO ZELJI.\n"
"5. IZUZETAK BACITE SAMO U FUNKCIJAMA U KOJIMA JE TO NAZNACENO.\n"
"6. FUNKCIJE KOJE NE IMPLEMENTIRATE TREBAJU BITI OBRISANE (KAKO POZIV TAKO I DEFINICIJA)!\n"
"7. NA KRAJU ISPITA SVOJE RJESENJE KOPIRATE U .DOCX FAJL (IMENOVAN BROJEM INDEKSA)!\n"
"8. RJESENJA ZADATKA POSTAVITE NA FTP SERVER U ODGOVARAJUCI FOLDER!\n"
"9. NEMOJTE POSTAVLJATI VISUAL STUDIO PROJEKTE, VEC SAMO .DOCX FAJL SA VASIM RJESENJEM!\n"
"10.SVE NEDOZVOLJENE RADNJE TOKOM ISPITA CE BITI SANKCIONISANE!\n"
"11. ZA POTREBE TESTIRANJA, U MAINU, BUDITE SLOBODNI DODATI TESTNIH PODATAKA (POZIVA FUNKCIJA) KOLIKO GOD SMATRATE DA JE POTREBNO!\n"
"-------------------------------------------------------------------------------\n";

const char* crt = "\n-------------------------------------------\n";
enum Kriteriji { CISTOCA, USLUGA, LOKACIJA, UDOBNOST };

ostream& operator << (ostream& os, const Kriteriji& kriterij) {
	const char* ispisKriterija[] = { "CISTOCA", "USLUGA", "LOKACIJA", "UDOBNOST" };
	os << ispisKriterija[kriterij];
	return os;
}

char* GetNizKaraktera(const char* tekst) {
	if (tekst == nullptr)
		return nullptr;
	int vel = strlen(tekst) + 1;
	char* novi = new char[vel];
	strcpy_s(novi, vel, tekst);
	return novi;
}

unique_ptr<char[]> GetUniqueNizKaraktera(const char* tekst) {
	if (tekst == nullptr)
		return nullptr;
	int vel = strlen(tekst) + 1;
	unique_ptr<char[]> novi(new char[vel]);
	strcpy_s(novi.get(), vel, tekst);
	return novi;
}

bool ValidirajBrojPasosa(string broj) {
	regex pattern(R"(^[A-Z]{1,2}\d{3,4}[- ]?\d{2,4}$)");
	return regex_match(broj, pattern);
}

template<class T1, class T2>
class Kolekcija {
	T1* _elementi1;
	T2* _elementi2;
	int* _trenutno;
	bool _omoguciDupliranje;
public:
	Kolekcija(bool omoguciDupliranje = true) {
		_omoguciDupliranje = omoguciDupliranje;
		_trenutno = new int(0);
		_elementi1 = nullptr;
		_elementi2 = nullptr;
	}

	Kolekcija(const Kolekcija& obj) {
		_omoguciDupliranje = obj._omoguciDupliranje;
		_trenutno = new int(*obj._trenutno);
		_elementi1 = new T1[*_trenutno];
		_elementi2 = new T2[*_trenutno];
		for (int i = 0; i < *_trenutno; i++) {
			_elementi1[i] = obj._elementi1[i];
			_elementi2[i] = obj._elementi2[i];
		}
	}

	Kolekcija& operator = (const Kolekcija& obj) {
		if (this != &obj) {
			delete[] _elementi1; _elementi1 = nullptr;
			delete[] _elementi2; _elementi2 = nullptr;
			delete _trenutno; _trenutno = nullptr;

			_omoguciDupliranje = obj._omoguciDupliranje;
			_trenutno = new int(*obj._trenutno);
			_elementi1 = new T1[*_trenutno];
			_elementi2 = new T2[*_trenutno];
			for (int i = 0; i < *_trenutno; i++) {
				_elementi1[i] = obj._elementi1[i];
				_elementi2[i] = obj._elementi2[i];
			}
		}
		return *this;
	}

	bool operator == (const Kolekcija& obj) {
		if (getTrenutno() != obj.getTrenutno())
			return false;
		for (size_t i = 0; i < obj.getTrenutno(); i++) {
			if (_elementi1[i] != obj._elementi1[i] || _elementi2[i] != obj._elementi2[i])
				return false;
		}
		return true;
	}

	~Kolekcija() {
		delete[] _elementi1; _elementi1 = nullptr;
		delete[] _elementi2; _elementi2 = nullptr;
		delete _trenutno; _trenutno = nullptr;
	}

	T1& getElement1(int lokacija)const { return _elementi1[lokacija]; }
	T2& getElement2(int lokacija)const { return _elementi2[lokacija]; }
	int getTrenutno()const { return *_trenutno; }

	friend ostream& operator<< (ostream& COUT, const Kolekcija& obj) {
		for (size_t i = 0; i < *obj._trenutno; i++)
			COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
		return COUT;
	}

	void AddElement(T1 el1, T2 el2) {
		if (_trenutno != nullptr) {
			if (_omoguciDupliranje == false) {
				for (size_t i = 0; i < getTrenutno(); i++) {
					if (_elementi1[i] == el1 && _elementi2[i] == el2)
						throw exception("Dupliciranje nije dozvoljeno!");
				}
			}
			T1* temp1 = _elementi1;
			T2* temp2 = _elementi2;
			_elementi1 = new T1[*_trenutno + 1];
			_elementi2 = new T2[*_trenutno + 1];
			for (size_t i = 0; i < *_trenutno; i++) {
				_elementi1[i] = temp1[i];
				_elementi2[i] = temp2[i];
			}
			_elementi1[*_trenutno] = el1;
			_elementi2[*_trenutno] = el2;
			delete[] temp1; temp1 = nullptr;
			delete[] temp2; temp2 = nullptr;
			(*_trenutno)++;
		}
		else throw exception("Niz je nullptr!");
	}

	Kolekcija InsertAt(int index, T1 el1, T2 el2) {
		if (index < 0 || index > getTrenutno())
			throw exception("Lokacija van opsega!");

		Kolekcija nova(_omoguciDupliranje);
		bool ubacen = false;
		for (size_t i = 0; i < getTrenutno(); i++) {
			if (i == index) {
				nova.AddElement(el1, el2);
				ubacen = true;
			}
			nova.AddElement(_elementi1[i], _elementi2[i]);
		}
		if (!ubacen && index == getTrenutno()) {
			nova.AddElement(el1, el2);
		}
		*this = nova;
		return nova;
	}
};

class Datum {
	int* _dan, * _mjesec, * _godina;
public:
	Datum(int dan = 1, int mjesec = 1, int godina = 2000) {
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
	}

	Datum(const Datum& obj) {
		_dan = new int(*obj._dan);
		_mjesec = new int(*obj._mjesec);
		_godina = new int(*obj._godina);
	}

	Datum& operator = (const Datum& obj) {
		if (this == &obj)
			return *this;
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
		_dan = new int(*obj._dan);
		_mjesec = new int(*obj._mjesec);
		_godina = new int(*obj._godina);
		return *this;
	}

	bool operator == (const Datum& d1) {
		return *_dan == *d1._dan && *_mjesec == *d1._mjesec && *_godina == *d1._godina;
	}

	int SumaDana()const {
		return *_godina * 365 + *_mjesec * 30 + *_dan;
	}

	~Datum() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
	}

	friend ostream& operator<< (ostream& COUT, const Datum& obj) {
		COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina;
		return COUT;
	}
};

class Komentar {
	char* _sadrzajKomentara;
	Kolekcija<Kriteriji, int>* _ocjeneKriterija;
public:
	Komentar(const char* sadrzajKomentara = "") {
		_sadrzajKomentara = GetNizKaraktera(sadrzajKomentara);
		_ocjeneKriterija = new Kolekcija<Kriteriji, int>;
	}

	Komentar(const Komentar& obj) {
		_sadrzajKomentara = GetNizKaraktera(obj._sadrzajKomentara);
		_ocjeneKriterija = new Kolekcija<Kriteriji, int>(*obj._ocjeneKriterija);
	}

	~Komentar() {
		delete[] _sadrzajKomentara; _sadrzajKomentara = nullptr;
		delete _ocjeneKriterija; _ocjeneKriterija = nullptr;
	}

	char* GetSadrzajKomentara() { return _sadrzajKomentara; }
	Kolekcija<Kriteriji, int>* GetOcjeneKriterija() { return _ocjeneKriterija; }

	Komentar& operator = (const Komentar& obj) {
		if (this != &obj) {
			delete[] _sadrzajKomentara; _sadrzajKomentara = nullptr;
			delete _ocjeneKriterija; _ocjeneKriterija = nullptr;
			_sadrzajKomentara = GetNizKaraktera(obj._sadrzajKomentara);
			_ocjeneKriterija = new Kolekcija<Kriteriji, int>(*obj._ocjeneKriterija);
		}
		return *this;
	}

	float getProsjecnaOcjena() const {
		if (_ocjeneKriterija->getTrenutno() == 0)
			return 0.0f;
		float zbroj = 0.0f;
		for (size_t i = 0; i < _ocjeneKriterija->getTrenutno(); i++) {
			zbroj += _ocjeneKriterija->getElement2(i);
		}
		return zbroj / _ocjeneKriterija->getTrenutno();
	}

	friend ostream& operator << (ostream& seeout, const Komentar& obj) {
		seeout << obj._sadrzajKomentara << endl;
		for (size_t i = 0; i < obj._ocjeneKriterija->getTrenutno(); i++) {
			seeout << obj._ocjeneKriterija->getElement1(i) << "("
				<< obj._ocjeneKriterija->getElement2(i) << ")" << endl;
		}
		seeout << "Prosjecna ocjena -> " << obj.getProsjecnaOcjena() << endl;
		return seeout;
	}

	void AddOcjenuKriterija(Kriteriji kriterij, int ocjena) {
		for (size_t i = 0; i < _ocjeneKriterija->getTrenutno(); i++) {
			if (kriterij == _ocjeneKriterija->getElement1(i))
				throw exception("Kriterij je vec ocijenjen!");
		}
		if (ocjena < 1 || ocjena > 10)
			throw exception("Ocjena nije validna!");

		_ocjeneKriterija->AddElement(kriterij, ocjena);
	}
};

class Gost {
	unique_ptr<char[]> _imePrezime;
	string _emailAdresa;
	string _brojPasosa;
public:
	Gost(const char* imePrezime, string emailAdresa, string brojPasosa) {
		_imePrezime = GetUniqueNizKaraktera(imePrezime);
		_emailAdresa = emailAdresa;
		_brojPasosa = ValidirajBrojPasosa(brojPasosa) ? brojPasosa : "NOT VALID";
	}

	Gost(const Gost& obj) {
		_imePrezime = GetUniqueNizKaraktera(obj._imePrezime.get());
		_emailAdresa = obj._emailAdresa;
		_brojPasosa = ValidirajBrojPasosa(obj._brojPasosa) ? obj._brojPasosa : "NOT VALID";
	}

	~Gost() {}

	string GetEmail() const { return _emailAdresa; }
	string GetBrojPasosa() const { return _brojPasosa; }
	char* GetImePrezime() const { return _imePrezime.get(); }

	friend ostream& operator<< (ostream& COUT, const Gost& obj) {
		COUT << obj._imePrezime.get() << " " << obj._emailAdresa << " " << obj._brojPasosa << endl;
		return COUT;
	}

	Gost& operator = (const Gost& obj) {
		if (this != &obj) {
			_imePrezime = GetUniqueNizKaraktera(obj._imePrezime.get());
			_emailAdresa = obj._emailAdresa;
			_brojPasosa = ValidirajBrojPasosa(obj._brojPasosa) ? obj._brojPasosa : "NOT VALID";
		}
		return *this;
	}
};

mutex kljuc;

class Rezervacija {
	Datum _OD;
	Datum _DO;
	vector<Gost> _gosti;
	Komentar _komentar;
public:
	Rezervacija(Datum& OD, Datum& DO, Gost& gost) : _OD(OD), _DO(DO) {
		_gosti.push_back(gost);
	}

	vector<Gost>& GetGosti() { return _gosti; }
	Komentar GetKomentar() { return _komentar; }

	friend ostream& operator<< (ostream& COUT, Rezervacija& obj) {
		COUT << crt << "Rezervacija " << obj._OD << " - " << obj._DO << " za goste: " << endl;

		for (size_t i = 0; i < obj._gosti.size(); i++)
			COUT << "\t" << i + 1 << "." << obj._gosti[i];

		COUT << crt << "Komentar rezervacije: " << endl << obj._komentar << crt;
		return COUT;
	}

	bool AddGost(Gost newgost) {
		for (size_t i = 0; i < _gosti.size(); i++) {
			if (_gosti[i].GetEmail() == newgost.GetEmail() ||
				_gosti[i].GetBrojPasosa() == newgost.GetBrojPasosa())
				return false;
		}
		_gosti.push_back(newgost);
		return true;
	}

	void SetKomentar(Komentar komentar) {
		_komentar = komentar;
		int brojKomentaraSaOcjenomKriterijaManjomOdPet = 0;
		for (size_t i = 0; i < _komentar.GetOcjeneKriterija()->getTrenutno(); i++) {
			if (_komentar.GetOcjeneKriterija()->getElement2(i) < 5)
				brojKomentaraSaOcjenomKriterijaManjomOdPet++;
		}
		if (brojKomentaraSaOcjenomKriterijaManjomOdPet >= 2) {
			thread t(&Rezervacija::SendEmail, this);
			t.join();
		}
	}

	void SendEmail() {
		lock_guard<mutex> guard(kljuc);

		cout << "To: ";
		for (size_t i = 0; i < _gosti.size(); i++) {
			cout << _gosti[i].GetEmail() << (i + 1 == _gosti.size() ? "" : ";");
		}
		cout << endl << "Subject: Informacija\n" << endl << "Postovani,\n" << endl;
		cout << "Zaprimili smo Vase ocjene, a njihova prosjecna vrijednost je " << _komentar.getProsjecnaOcjena()
			<< ".\nZao nam je zbog toga, te ce Vas u najkracem periodu kontaktirati nasa Sluzba za odnose sa gostima.\n\n";
		cout << "Ugodan boravak Vam zelimo\nPuno pozdrava\n";
	}

	pair<int, int> GetBrojZnakova(string nazivFajla, string trazeniZnakovi) {
		ifstream fajl(nazivFajla);
		if (!fajl.is_open())
			return { 0, 0 };

		int ukupnoZnakova = 0;
		int trazeniZnakoviBrojac = 0;
		char ch;

		while (fajl.get(ch)) {
			ukupnoZnakova++;
			if (trazeniZnakovi.find(ch) != string::npos) {
				trazeniZnakoviBrojac++;
			}
		}
		fajl.close();
		return { ukupnoZnakova, trazeniZnakoviBrojac };
	}
};

const char* GetOdgovorNaPrvoPitanje() {
	cout << "Pitanje -> Pojasnite osnovne preduslove koji moraju biti ispunjeni da bi se realizovao polimorfizam?\n";
	return "Odgovor -> 1. Postojanje nasljedivanja (izvedene klase iz bazne klase).\n"
		   "           2. Postojanje virtuelnih funkcija u baznoj klasi (koristenjem kljucne rijeci 'virtual').\n"
		   "           3. Pozivanje virtuelnih funkcija preko pokazivaca ili reference na baznu klasu.";
}

const char* GetOdgovorNaDrugoPitanje() {
	cout << "Pitanje -> Pojasnite razloge koristenja kljucnih rijeci abstract i ciste virtualne metode, te razlike izmedju njih?\n";
	return "Odgovor -> Cista virtuelna metoda (virtual void f() = 0;) definise interfejs bez implementacije u baznoj klasi,\n"
		   "           primoravajuci izvedene klase da je implementiraju. Klasa koja sadrzi bar jednu cistu virtuelnu metodu\n"
		   "           postaje apstraktna klasa, sto znaci da se ne mogu kreirati njeni direktni objekti, vec sluzi kao sablon za izvedene klase.";
}

void main() {

	cout << PORUKA;
	cin.get();

	cout << GetOdgovorNaPrvoPitanje() << endl;
	cin.get();
	cout << GetOdgovorNaDrugoPitanje() << endl;
	cin.get();

	Datum
		datum19062022(19, 6, 2022),
		datum20062022(20, 6, 2022),
		datum30062022(30, 6, 2022),
		datum05072022(5, 7, 2022);

	int kolekcijaTestSize = 9;
	Kolekcija<int, int> kolekcija1(false);
	for (int i = 0; i <= kolekcijaTestSize; i++)
		kolekcija1.AddElement(i, i);
	cout << kolekcija1 << crt;

	try {
		kolekcija1.AddElement(3, 3);
	}
	catch (exception& err) {
		cout << err.what() << crt;
	}
	cout << kolekcija1 << crt;

	Kolekcija<int, int> kolekcija2 = kolekcija1.InsertAt(0, 10, 10);
	cout << kolekcija2 << crt;

	if (ValidirajBrojPasosa("BH235-532"))
		cout << "Broj pasosa validan" << endl;
	if (ValidirajBrojPasosa("B123321"))
		cout << "Broj pasosa validan" << endl;
	if (ValidirajBrojPasosa("B1252 521"))
		cout << "Broj pasosa validan" << endl;
	if (!ValidirajBrojPasosa("H4521"))
		cout << "Broj pasosa NIJE validan" << endl;
	if (!ValidirajBrojPasosa("b1252 521"))
		cout << "Broj pasosa NIJE validan" << endl;

	Gost denis("Denis Music", "denis@fit.ba", "BH235-532");
	Gost jasmin("Jasmin Azemovic", "jasmin@fit.ba", "B123321");
	Gost adel("Adel Handzic", "adel@edu.fit.ba", "B1252 521");
	Gost gostPasosNotValid("Ime Prezime", "korisnik@klix.ba", "H4521");

	Rezervacija rezervacija(datum19062022, datum20062022, denis);
	if (rezervacija.AddGost(jasmin))
		cout << crt << "Gost uspjesno dodan!" << crt;

	Komentar komentarRezervacija("Nismo pretjerano zadovoljni uslugom, a ni lokacijom.");
	komentarRezervacija.AddOcjenuKriterija(CISTOCA, 7);
	komentarRezervacija.AddOcjenuKriterija(USLUGA, 4);
	komentarRezervacija.AddOcjenuKriterija(LOKACIJA, 3);
	komentarRezervacija.AddOcjenuKriterija(UDOBNOST, 6);

	try {
		komentarRezervacija.AddOcjenuKriterija(UDOBNOST, 6);
	}
	catch (exception& err) {
		cout << err.what() << crt;
	}

	rezervacija.SetKomentar(komentarRezervacija);

	cout << rezervacija << endl;

	Rezervacija rezervacijaSaAdelom = rezervacija;
	if (rezervacijaSaAdelom.AddGost(adel))
		cout << "Gost uspjesno dodan!" << endl;
	if (!rezervacijaSaAdelom.AddGost(denis))
		cout << "Gost je vec dodan na rezervaciju!" << endl;

	cout << rezervacijaSaAdelom << endl;

	// Kreiranje testne datoteke za GetBrojZnakova
	ofstream testFajl("rezervacije.txt");
	testFajl << "denis+music*_";
	testFajl.close();

	pair<int, int> brojac = rezervacijaSaAdelom.GetBrojZnakova("rezervacije.txt", "*_+");
	cout << "Ukupno znakova u fajlu: " << brojac.first << endl;
	cout << "Ukupno trazenih znakova: " << brojac.second << endl;

	cin.get();
	system("pause>0");
}
