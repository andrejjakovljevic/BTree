#include<iostream>
#include<algorithm>
#include<vector>
#include<utility>
#include<stack>
#include<fstream>
using namespace std;

class Bstablo
{
public:
	struct Cvor
	{
		int m;
		int* kljucevi;
		Cvor** pokazivaci;
		int vel;
		Cvor* parent;
		
		Cvor(int mm) : m(mm), vel(0), parent(nullptr) { kljucevi = new int[mm]; pokazivaci = new Cvor * [mm]; for (int i = 0; i < mm; i++) pokazivaci[i] = nullptr; }
		
		~Cvor() { delete[] kljucevi; delete[] pokazivaci; }

		bool nadji(int k)
		{
			for (int i = 0; i < vel; i++)
			{
				if (kljucevi[i] == k) return true;
			}
			return false;
		}

		Cvor* nadji_pok(int k)
		{
			if (vel == 0) return nullptr;
			for (int i = 0; i < vel; i++)
			{
				if (k < kljucevi[i]) return pokazivaci[i];
			}
			return pokazivaci[vel];
		}

		int dodaj_kljuc_nepuno(int k, Cvor* levi, Cvor* desni)
		{
			int r = -1;
			if (levi != nullptr) levi->parent = this;
			if (desni != nullptr) desni->parent = this;
			for (int i = 0; i < vel; i++)
			{
				if (k < kljucevi[i])
				{
					r = i;
					break;
				}
			}
			if (r == -1)
			{
				kljucevi[vel] = k; 
				pokazivaci[vel] = levi;
				pokazivaci[vel + 1] = desni;
				vel++;
				return vel - 1;
			}
			else
			{
				for (int i = vel - 1; i >= r; i--)
				{
					kljucevi[i + 1] = kljucevi[i];
				}
				for (int i = vel; i > r; i--)
				{
					pokazivaci[i + 1] = pokazivaci[i];
				}
				kljucevi[r] = k;
				pokazivaci[r + 1] = desni;
				pokazivaci[r] = levi;
				vel++;
				return r;
			}
		}

		Cvor* dodaj_kljuc_puno(int k, Cvor* levi, Cvor* desni)
		{
			int r = dodaj_kljuc_nepuno(k, levi, desni);
			Cvor* novi_levi = new Cvor(m);
			Cvor* novi_desni = new Cvor(m);
			Cvor* novi = new Cvor(m);
			for (int i = 0; i < (vel-1)/2; i++)
			{
				novi_levi->dodaj_kljuc_nepuno(kljucevi[i], pokazivaci[i], pokazivaci[i + 1]);
			}
			for (int i = (vel-1) / 2 + 1; i < vel; i++)
			{
				novi_desni->dodaj_kljuc_nepuno(kljucevi[i], pokazivaci[i], pokazivaci[i + 1]);
			}
			novi->dodaj_kljuc_nepuno(kljucevi[(vel-1) / 2], novi_levi, novi_desni);
			novi_levi->parent = novi;
			novi_desni->parent = novi;
			return novi;
		}

		friend ostream& operator<<(ostream& ut, Cvor* p)
		{
			for (int i = 0; i < p->vel; i++)
			{
				ut << p->kljucevi[i] << " ";
			}
			return ut;
		}

		void izbrisi_kljuc(int k, Cvor* pokaz)
		{
			int r;
			for (int i = 0; i < vel; i++)
			{
				if (k == kljucevi[i])
				{
					r = i; break;
				}
			}
			for (int i = r; i < vel - 1; i++)
			{
				kljucevi[i] = kljucevi[i + 1];
			}
			for (int i = r; i < vel; i++)
			{
				pokazivaci[i] = pokazivaci[i + 1];
			}
			pokazivaci[r] = pokaz;
			vel--;
		}

		Cvor* desni_brt()
		{
			Cvor* par = parent;
			if (par == nullptr) return nullptr;
			int r = -1;
			for (int i = 0; i < par->vel; i++)
			{
				if (par->kljucevi[i] > this->kljucevi[0])
				{
					r = i;
					break;
				}
			}
			if (r == -1) return nullptr;
			else return par->pokazivaci[r+1];
		}

		Cvor* levi_brt()
		{

			Cvor* par = parent;
			if (par == nullptr) return nullptr;
			int r = -1;
			for (int i = 0; i < par->vel; i++)
			{
				if (par->kljucevi[i] > this->kljucevi[0])
				{
					r = i;
					break;
				}
			}
			if (r == -1) return par->pokazivaci[par->vel - 1];
			else if (r == 0) return nullptr;
			else return par->pokazivaci[r - 1];
		}

		int min()
		{
			if (vel != 0)
			{
				return kljucevi[0];
			}
		}

		int max()
		{
			if (vel != 0)
			{
				return kljucevi[vel - 1];
			}
		}

		bool is_leaf()
		{
			if (vel != 0)
			{
				return (pokazivaci[0] == nullptr);
			}
			return false;
		}
	};

	Cvor* root;
	int m;

	Bstablo(int mm) : m(mm) { root = new Cvor(mm); }

	bool dodaj(int k)
	{
		Cvor* pom = root;
		while (!pom->nadji(k) && pom->nadji_pok(k) != nullptr)
		{
			pom = pom->nadji_pok(k);
		}
		if (pom->nadji(k)) return false;
		Cvor* levi = nullptr;
		Cvor* desni = nullptr;
		int ubac = k;
		while (true)
		{
			if (pom->vel < m - 1)
			{
				pom->dodaj_kljuc_nepuno(ubac, levi, desni);
				return true;
			}
			else
			{
				Cvor* prelom = pom->dodaj_kljuc_puno(ubac, levi, desni);
				Cvor* par = pom->parent;
				if (par == nullptr)
				{
					root = prelom;
					return true;
				}
				else
				{
					pom = par;
					ubac = prelom->kljucevi[0];
					levi = prelom->pokazivaci[0];
					desni = prelom->pokazivaci[1];
				}
			}
		}
		return true;
	}

	friend ostream& operator<<(ostream& ut, Bstablo* t)
	{
		vector< pair<Cvor*,bool> > v;
		v.push_back(make_pair(t->root,true));
		for (int i = 0; i < v.size(); i++)
		{
			pair<Cvor*,bool> pom = v[i];
			Cvor* tren = pom.first;
			if (tren != nullptr)
			{
				ut << tren;
				if (pom.second == true) ut << endl;
				else ut << "      ";
				for (int j = 0; j <= tren->vel; j++)
				{
					bool b;
					if (j == tren->vel && pom.second == true) b = true;
					else b = false;
					v.push_back(make_pair(tren->pokazivaci[j], b));
				}
			}
		}
		return ut;
	}

	~Bstablo()
	{
		vector<Cvor*> v;
		v.push_back(root);
		for (int i = 0; i < v.size(); i++)
		{
			Cvor* tren = v[i];
			if (tren != nullptr)
			{
				for (int j = 0; j <= tren->vel; j++)
				{
					v.push_back(tren->pokazivaci[i]);
				}
				delete tren;
			}
		}
	}

	bool nadji(int k)
	{
		Cvor* pom = root;
		while (pom != nullptr && !pom->nadji(k))
		{
			pom = pom->nadji_pok(k);
		}
		if (pom == nullptr) return false;
		else return true;
	}

	int visina()
	{
		Cvor* pom = root;
		int k = 0;
		while (pom != nullptr)
		{
			pom = pom->pokazivaci[0];
			k++;
		}
		return k - 1;
	}

	int broj_kljuceva()
	{
		vector<Cvor*> v;
		v.push_back(root);
		int s = 0;
		for (int i = 0; i < v.size(); i++)
		{
			Cvor* tren = v[i];
			if (tren != nullptr)
			{
				s += tren->vel;
				for (int j = 0; j <= tren->vel; j++)
				{
					v.push_back(tren->pokazivaci[j]);
				}
			}
		}
		return s;
	}

	Cvor* nadji_sledbenika(int k)
	{
		Cvor* pom = root;
		while (!pom->nadji(k))
		{
			pom = pom->nadji_pok(k);
		}
		int r;
		for (int i = 0; i < pom->vel; i++)
		{
			if (k == pom->kljucevi[i])
			{
				r = i; break;
			}
		}
		Cvor* sin = pom->pokazivaci[r + 1];
		if (sin == nullptr) return sin;
		else
		{
			while (sin->pokazivaci[0] != nullptr)
			{
				sin = sin->pokazivaci[0];
			}
			return sin;
		}
	}

	bool izbrisi(int k)
	{
		Cvor* pom = root;
		int izbris = k;
		while (pom != nullptr && !pom->nadji(k))
		{
			pom = pom->nadji_pok(k);
		}
		if (pom == nullptr) return false;
		else
		{
			if (!pom->is_leaf())
			{
				Cvor* sled = nadji_sledbenika(k);
				for (int i = 0; i < pom->vel; i++)
				{
					if (k == pom->kljucevi[i]) pom->kljucevi[i] = sled->kljucevi[0];
				}
				pom = sled;
				izbris = sled->kljucevi[0];
			}
			Cvor* pokaz = nullptr;
			while (true)
			{
				if (pom->vel > (m - 1) / 2)
				{
					pom->izbrisi_kljuc(izbris, pokaz);
					return true;
				}
				else
				{
					Cvor* par = pom->parent;
					Cvor* desni = pom->desni_brt();
					if (desni != nullptr)
					{
						if (desni->vel > (m - 1) / 2)
						{
							int ide_gore = desni->min();
							int r = -1;
							int razdelnik = -1;
							for (int i = 0; i < par->vel; i++)
							{
								if (pom->kljucevi[0]<par->kljucevi[i] && desni->kljucevi[0]>par->kljucevi[i])
								{
									r = i;
									razdelnik = par->kljucevi[i];
									par->kljucevi[i] = ide_gore;
									break;
								}
							}
							Cvor *levi_sin_desnog = desni->pokazivaci[0];
							desni->izbrisi_kljuc(ide_gore, desni->pokazivaci[1]);
							pom->izbrisi_kljuc(izbris, pokaz);
							Cvor* desni_levog = pom->pokazivaci[pom->vel];
							pom->dodaj_kljuc_nepuno(razdelnik, desni_levog, levi_sin_desnog);
							return true;
						}
					}
					Cvor* levi = pom->levi_brt();
					if (levi != nullptr)
					{
						if (levi->vel > (m - 1) / 2)
						{
							int ide_gore = levi->max();
							int r = -1;
							int razdelnik = -1;
							for (int i = 0; i < par->vel; i++)
							{
								if (pom->kljucevi[0]>par->kljucevi[i] && levi->kljucevi[0]<par->kljucevi[i])
								{
									r = i;
									razdelnik = par->kljucevi[i];
									par->kljucevi[i] = ide_gore;
									break;
								}
							}
							Cvor* desni_sin_levog = levi->pokazivaci[levi->vel];
							levi->izbrisi_kljuc(ide_gore, levi->pokazivaci[levi->vel]);
							pom->izbrisi_kljuc(izbris, pokaz);
							Cvor* levi_sin_desnog = pom->pokazivaci[pom->vel];
							pom->dodaj_kljuc_nepuno(razdelnik, desni_sin_levog, levi_sin_desnog);
							return true;
						}
					}
					if (desni != nullptr)
					{
						pom->izbrisi_kljuc(izbris, pokaz);
						Cvor* novi = new Cvor(m);
						int razdelnik = -1;
						for (int i = 0; i < par->vel; i++)
						{
							if (pom->kljucevi[0]<par->kljucevi[i] && desni->kljucevi[0]>par->kljucevi[i])
							{
								razdelnik = par->kljucevi[i];
								break;
							}
						}
						for (int i = 0; i < pom->vel; i++)
						{
							novi->dodaj_kljuc_nepuno(pom->kljucevi[i], pom->pokazivaci[i], pom->pokazivaci[i + 1]);
						}
						novi->dodaj_kljuc_nepuno(razdelnik, pom->pokazivaci[pom->vel], desni->pokazivaci[0]);
						for (int i = 0; i < desni->vel; i++)
						{
							novi->dodaj_kljuc_nepuno(desni->kljucevi[i], desni->pokazivaci[i], desni->pokazivaci[i + 1]);
						}
						novi->parent = par;
						izbris = razdelnik;
						pokaz = novi;
						pom = par;
						continue;
					}
					else if (levi != nullptr)
					{
						pom->izbrisi_kljuc(izbris, pokaz);
						Cvor* novi = new Cvor(m);
						int razdelnik = -1;
						for (int i = 0; i < par->vel; i++)
						{
							if (pom->kljucevi[0] > par->kljucevi[i] && levi->kljucevi[0] < par->kljucevi[i])
							{
								razdelnik = par->kljucevi[i];
								break;
							}
						}
						for (int i = 0; i < levi->vel; i++)
						{
							novi->dodaj_kljuc_nepuno(levi->kljucevi[i], levi->pokazivaci[i], levi->pokazivaci[i + 1]);
						}
						novi->dodaj_kljuc_nepuno(razdelnik, levi->pokazivaci[levi->vel], pom->pokazivaci[0]);
						for (int i = 0; i < pom->vel; i++)
						{
							novi->dodaj_kljuc_nepuno(pom->kljucevi[i], pom->pokazivaci[i], pom->pokazivaci[i + 1]);
						}
						novi->parent = par;
						izbris = razdelnik;
						pokaz = novi;
						pom = par;
						continue;
					}
					else if (levi == nullptr && desni == nullptr)
					{
						if (pom->vel == 1)
						{
							root = pokaz;
							if (pokaz != nullptr) pokaz->parent = nullptr;
							return true;
						}
						else
						{
							pom->izbrisi_kljuc(izbris, pokaz);
							root = pom;
							return true;
						}
					}
				}
			}
		}
		return true;
	}

	int broj_cvorova()
	{
		vector<Cvor*> v;
		v.push_back(root);
		int s = 0;
		for (int i = 0; i < v.size(); i++)
		{
			Cvor* tren = v[i];
			if (tren != nullptr)
			{
				s++;
				for (int j = 0; j <= tren->vel; j++)
				{
					v.push_back(tren->pokazivaci[j]);
				}
			}
		}
		return s;
	}

	int prosecan_br_cvorova()
	{
		return broj_kljuceva() / broj_cvorova();
	}

	void sredi_drvo()
	{
		vector< pair<Cvor*, bool> > v;
		v.push_back(make_pair(root, true));
		int br_kljuceva = 0;
		int br_cvorova = 0;
		for (int i = 0; i < v.size(); i++)
		{
			Cvor* tren = v[i].first;
			if (tren != nullptr)
			{
				br_kljuceva += tren->vel;
				br_cvorova++;
				for (int j = 0; j <= tren->vel; j++)
				{
					bool b = false;
					if (v[i].second == true && j == tren->vel) b = true;
					v.push_back(make_pair(tren->pokazivaci[j], b));
				}
				if (v[i].second == true)
				{
					int prosecan = br_kljuceva / br_cvorova;
					if (br_kljuceva % br_cvorova != 0) prosecan++;
					for (int j = i - br_cvorova + 1; j <= i; j++)
					{
						Cvor* curr = v[j].first;
						Cvor* desni = curr->desni_brt();
						if (desni != nullptr)
						{
							while (curr->vel > prosecan)
							{
								int ide_gore = curr->max();
								int r = -1;
								int razdelnik = -1;
								Cvor* par = curr->parent;
								for (int i = 0; i < par->vel; i++)
								{
									if (curr->kljucevi[0]<par->kljucevi[i] && desni->kljucevi[0]>par->kljucevi[i])
									{
										r = i;
										razdelnik = par->kljucevi[i];
										par->kljucevi[i] = ide_gore;
										break;
									}
								}
								Cvor* levi_sin_desnog = desni->pokazivaci[0];
								curr->izbrisi_kljuc(ide_gore, curr->pokazivaci[curr->vel]);
								Cvor* desni_levog = curr->pokazivaci[curr->vel];
								desni->dodaj_kljuc_nepuno(razdelnik, desni_levog, levi_sin_desnog);
							}
						}
					}
					for (int j = i; j >= i - br_cvorova + 1; j--)
					{
						Cvor* curr = v[j].first;
						Cvor* levi = curr->levi_brt();
						if (levi != nullptr)
						{
							while (curr->vel > prosecan)
							{
								int ide_gore = curr->min();
								int r = -1;
								int razdelnik = -1;
								Cvor* par = curr->parent;
								for (int i = 0; i < par->vel; i++)
								{
									if (curr->kljucevi[0] > par->kljucevi[i] && levi->kljucevi[0] < par->kljucevi[i])
									{
										r = i;
										razdelnik = par->kljucevi[i];
										par->kljucevi[i] = ide_gore;
										break;
									}
								}
								Cvor* levi_sin_desnog = curr->pokazivaci[0];
								curr->izbrisi_kljuc(ide_gore, curr->pokazivaci[1]);
								Cvor* desni_levog = levi->pokazivaci[levi->vel];
								levi->dodaj_kljuc_nepuno(razdelnik, desni_levog, levi_sin_desnog);
							}
						}
					}
					br_cvorova = 0;
					br_kljuceva = 0;
				}
			}
		}
	}
};

Bstablo* t = nullptr;

int main()
{
	while (true)
	{
		cout << "1. Napravi stablo" << endl;
		cout << "2. Visina stabla" << endl;
		cout << "3. Broj kluceva u stablu" << endl;
		cout << "4. Ispisi stablo" << endl;
		cout << "5. Umetni kljuc" << endl;
		cout << "6. Obrisi kljuc" << endl;
		cout << "7. Brisi stablo" << endl;
		cout << "8. Broj cvorova" << endl;
		cout << "9. Sredi" << endl;
		cout << "10. Napravi na osnovu datoteke" << endl;
		int k;
		cin >> k;
		if (k == 1)
		{
			cout << "Unesi stepen stabla" << endl;
			int m; cin >> m;
			t = new Bstablo(m);
		}
		if (k == 10)
		{
			delete t;
			cout << "Unesite stepen stabla" << endl;
			int m; cin >> m;
			t = new Bstablo(m);
			cout << "Unesite broj kljuceva" << endl;
			int n; cin >> n;
			ifstream dat;
			cout << "Unesite ime datoteke" << endl;
			string s; cin >> s;
			dat.open(s);
			if (!dat.is_open()) exit(1);
			for (int i = 0; i < n; i++)
			{
				int a;
				dat >> a;
				t->dodaj(a);
			}
			dat.close();
		}
		else
		{
			if (t == nullptr)
			{
				cout << "Nema stabla!" << endl;
				continue;
			}
			if (k == 2) cout << t->visina() << endl;
			if (k == 3) cout << t->broj_kljuceva() << endl;
			if (k == 4) cout << t << endl;
			if (k == 5)
			{
				int x;
				cout << "Unesite sta se ubacuje" << endl; cin >> x;
				t->dodaj(x);
			}
			if (k == 6)
			{
				int x;
				cout << "Unesti sta se brise" << endl; cin >> x;
				t->izbrisi(x);
			}
			if (k == 7)
			{
				delete t;
				t = nullptr;
			}
			if (k == 8) cout << t->broj_cvorova() << endl;
			if (k == 9)
			{
				t->sredi_drvo();
				cout << t;
			}
		}
	}
	/*t = new Bstablo(5);
	for (int i = 1; i <= 19; i++)
	{
		t->dodaj(i);
	}
	cout << t;
	t->sredi_drvo();
	cout << t;*/
}