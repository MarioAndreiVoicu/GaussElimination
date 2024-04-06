#include<iostream>
#include<vector>
#include<fstream>

void CitireMatrice(std::vector<std::vector<float>>& matrice)
{
	int nrLinii, nrColoane;

	std::ifstream fisier("matrice.txt");
	float element;

	fisier >> nrLinii >> nrColoane;  //se citesc de pe prima linie din fisier numarul de linii si de coloane ale matricei
	for (int i = 0; i < nrLinii; i++)   //citesc matricea
	{
		std::vector<float> vectorLinie;
		for (int j = 0; j < nrColoane; j++)
		{
			fisier >> element;
			vectorLinie.push_back(element);
		}
		matrice.push_back(vectorLinie);
	}
}

void AfisareMatrice(const std::vector<std::vector<float>>& matrice)
{
	for (int i = 0; i < matrice.size(); i++)
	{
		for (int j = 0; j < matrice[i].size(); j++)
			std::cout << matrice[i][j] << " ";
		std::cout << "\n";
	}
}

bool VerificareSistemIncompatibil(std::vector<std::vector<float>>& matrice)
{   //daca obtin ca 0 = ceva diferit de 0,atunci sistemul este incompatibil
	int j;
	for (int i = 0; i < matrice.size(); i++)  //verific ca fiecare linie sa nu contina 0 = ceva diferit de 0
	{
		float suma = 0;
		int ok = 0;  //ok ia valoarea 1 daca toate elementele de pe linie sunt zero

		for (j = 0; j < matrice[i].size() - 1; j++)
			if (matrice[i][j] != 0)
				ok = 1;

		float rezultat = matrice[i][j];
		if (ok == 0 && rezultat != 0)  //daca toate variabilele sunt 0 si rezultatul e diferit de 0 inseamna ca e fals si sistemul este incompatibil
			return false;
	}
	return true;
}


bool CautareUnuPeColoana(std::vector<std::vector<float>>& matrice, int iStart, int j)  //incerc sa gasesc un element de 1 pe o coloana de mai jos si sa interschimb coloanele ca sa aduc 1 in pozitia corecta
{
	for (int i = iStart + 1; i < matrice.size(); i++)  //parcurg de la iStart+1,adica de la urmatoarea linie pana la ultima linie si caut un element de 1
		if (matrice[i][j] == 1)
		{
			std::swap(matrice[iStart], matrice[i]);
			AfisareMatrice(matrice);
			std::cout << "\n";
			return true;
		}
	return false;
}

bool CautareElementNenulPeColoana(std::vector<std::vector<float>>& matrice, int iStart, int j)
{
	for (int i = iStart + 1; i < matrice.size(); i++)
		if (matrice[i][j] != 0)
		{
			std::swap(matrice[iStart], matrice[i]);
			AfisareMatrice(matrice);
			std::cout << "\n";
			return true;
		}
	return false;
}

void RezolvareLinie(std::vector<std::vector<float>>& matrice, int iLiniePrincipala, int iLinieSecundara, float coeficientLinie)
{
	for (int j = 0; j < matrice[iLiniePrincipala].size(); j++)  //parcurg linia pana la capat
		matrice[iLinieSecundara][j] = matrice[iLinieSecundara][j] - coeficientLinie * matrice[iLiniePrincipala][j];
}

void RezolvareColoana(std::vector<std::vector<float>>& matrice, int iLiniePrincipala, int jColoanaPrincipala)   /* iLiniePrincipala si jColoanaPrincipala sunt coordonatele elementului de 1 in jurul caruia formez zeorouri */
{
	for (int i = 0; i < matrice.size(); i++)
	{
		if (i != iLiniePrincipala)  //linia cu elementul de 1 nu trebuie schimbata,sar peste ea
		{
			float coeficientLinie;
			if (matrice[i][jColoanaPrincipala] >= 0) //daca elementul e pozitiv
			{
				coeficientLinie = matrice[i][jColoanaPrincipala] / matrice[iLiniePrincipala][jColoanaPrincipala];  //cu cat trebuie sa inmultesc linia ca sa obtin 0
			}
			else  //daca elementul e negativ,il consider pozitiv si pun - la coeficientLinie
			{
				float element = (-1) * matrice[i][jColoanaPrincipala];
				coeficientLinie = element / matrice[iLiniePrincipala][jColoanaPrincipala];  //cu cat trebuie sa inmultesc linia ca sa obtin 0
				coeficientLinie = coeficientLinie * (-1);
			}
			RezolvareLinie(matrice, iLiniePrincipala, i, coeficientLinie);
		}
	}
}

void RezolvareElementUnu(std::vector<std::vector<float>>& matrice, int i, int j)  //rezolva cazul in care trebuie sa transofme elementul in 1 prin inmultire cu inversul
{
	float invers = 1 / matrice[i][j];

	for (int j1 = 0; j1 < matrice[i].size(); j1++)
		if (matrice[i][j1] != 0)
			matrice[i][j1] = matrice[i][j1] * invers;

	AfisareMatrice(matrice);
	std::cout << "\n";

	RezolvareColoana(matrice, i, j);
}

bool Rezolvare(std::vector<std::vector<float>>& matrice, std::vector<int>& vectorNecunoscuteSecundare)
{
	int i = 0, j = 0;

	if (matrice.size() == 1)  //adaug cazul in care matricea are o singura linie
	{
		if (!VerificareSistemIncompatibil(matrice))
			return false;

		while (j < matrice[0].size() - 1 && matrice[i][j] == 0)
		{
			vectorNecunoscuteSecundare[j]++;
			j++;
		}
		if (j == matrice[0].size() - 1)  //aici se rezolva cazul 0=0 
		{
			std::cout << "\n\n\n\n0=0 \n\n\n\n";
			return true;
		}

		RezolvareElementUnu(matrice, i, j);

		for (int jNou = j + 1; jNou < matrice[0].size() - 1; jNou++)
			vectorNecunoscuteSecundare[jNou]++;

		return true;
	}

	while (i != matrice.size() - 1)
	{
		if (matrice[i][j] != 0)  //daca elementul este diferit de 0,atunci continui rezolvarea
		{
			if (matrice[i][j] == 1)  //daca elementul este 1,trec la pasul urmator
			{
				RezolvareColoana(matrice, i, j);
				i++;
				j++;
				AfisareMatrice(matrice);
				std::cout << "\n";
			}
			else  //daca elementul nu este 1,atunci caut un element de 1 pe o linie de mai jos
			{
				if (CautareUnuPeColoana(matrice, i, j))  //daca gaseste 1, il pune in pozitia corecta si rezolva coloana
				{
					RezolvareColoana(matrice, i, j);
					AfisareMatrice(matrice);
					std::cout << "\n";
				}
				else  //daca nu gaseste 1 mai jos,transforma elementul curent nenul in 1 si rezolva coloana
				{
					RezolvareElementUnu(matrice, i, j);
				}
			}
		}
		else //daca elementul este 0,atunci caut un 1 in jos
		{
			if (CautareUnuPeColoana(matrice, i, j))  //daca gaseste 1,il pune in pozitia corecta si rezolva coloana
			{
				RezolvareColoana(matrice, i, j);
				i++;
				j++;
				AfisareMatrice(matrice);
				std::cout << "\n";
			}
			else   //daca nu a gasait un element de 1 pe coloana,verifica daca exista orice element nenul pe coloana in jos
			{
				if (CautareElementNenulPeColoana(matrice, i, j))  //daca a gasit un element nenul,il transforma in 1 si continua rezolvarea
				{
					RezolvareElementUnu(matrice, i, j);
					/*	AfisareMatrice(matrice);
						std::cout << "\n";*/
				}
				else
				{
					while (matrice[i][j] == 0 && !CautareElementNenulPeColoana(matrice, i, j))  //cat timp nu gaseste un element nenul pe coloana,se duce pe coloana din dreapta pana cand gaseste o coloana care contine un element nenul sau pana la finalul sistemului
					{
						if (j == matrice[i].size() - 1)
						{
							i = matrice.size() - 1;  //ca sa iasa din toata structura
							break;
						}
						vectorNecunoscuteSecundare[j]++;
						j++;
					}
				}
			}
		}
		if (!VerificareSistemIncompatibil(matrice))
		{
			AfisareMatrice(matrice);
			std::cout << "\n";
			return false;
		}
	}

	if (matrice[i][j] == 0)  //pentru cazul in care ajunge la ultimua linie,dar elementul este 0 si coloana e a unei variabile care devine necunoscuta secundara,deci trebuie sa caut un element nenul pe urmatoarele coloane
		while (matrice[i][j] == 0 && !CautareElementNenulPeColoana(matrice, i, j))  //cat timp nu gaseste un element nenul pe coloana,se duce pe coloana din dreapta pana cand gaseste o coloana care contine un element nenul sau pana la finalul sistemului
		{
			if (j == matrice[i].size() - 1)
			{
				break;
			}
			vectorNecunoscuteSecundare[j]++;
			j++;
		}

	if (matrice[i][j] != 0)
		RezolvareElementUnu(matrice, i, j);
	AfisareMatrice(matrice);

	if (j < matrice[i].size() - 1)  //poate sa parcurga matricea ca pe  una patratica si sa piarda necunoscute secundare,cum e cazul pentru   1 2 -1 0 0    ,,ajungle la matrice[1][1] si se opreste fara sa retina necunoscutele secundare z si t
	{																																	//	  0 1 - 1 1 0
		for (int j2 = j + 1; j2 < matrice[i].size() - 1; j2++)
			vectorNecunoscuteSecundare[j2]++;
	}

	return true;
}

bool NecunoscuteSecundare(std::vector<int>& vectorNecunoscuteSecundare)
{
	for (int i = 0; i < vectorNecunoscuteSecundare.size(); i++)
		if (vectorNecunoscuteSecundare[i] > 0)
			return true;
	return false;
}

void AfisareNecunoscuteSecundare(std::vector<int>& vectorNecunoscuteSecundare)
{
	for (int i = 0; i < vectorNecunoscuteSecundare.size(); i++)
	{
		if (vectorNecunoscuteSecundare[i] > 0)
		{
			switch (i)
			{
			case 0:
				std::cout << "x,";
				break;
			case 1:
				std::cout << "y,";
				break;
			case 2:
				std::cout << "z,";
				break;
			case 3:
				std::cout << "t,";
				break;
			}
		}
	}
}

bool VerificareNecunoscuteSecundarePeLinie(std::vector<std::vector<float>>& matrice, int i)
{  //verific pe linie daca exista necunoscute secundare
	int count = 0; //numar cate valori diferite de 0 se afla pe linie si daca exista mai mult de o valoare nenula,atunci exista necunoscute secundare
	for (int j = 0; j < matrice[i].size(); j++)
	{
		if (matrice[i][j] != 0)
			count++;
	}
	return count > 1;  //returneaza true daca exista necunoscute secundare si false daca nu exista necunoscute secundare
}

bool VerificareLinieDeZero(std::vector<std::vector<float>>& matrice, int i)
{
	for (int j = 0; j < matrice[i].size(); j++)
		if (matrice[i][j] != 0)
			return false;
	return true;    //returneaza true daca linia este formata numai din 0 si false daca linia contine cel putin un element nenul
}

float VerificareLinieDoarCuNecunoscutaSecundara(std::vector<std::vector<float>>& matrice, int i, int jPunctPrincipal)
{  //daca am o linie care contine o singura valoare in afara de cea de pe coloana rezultatelor si acea valoarea nu se afla pe pozitia elememtului de 1 principal,atunci pe acea pozitie se afla necunoscuta secundara si valoarea de pe pozitie reprezinta cantitatea de acea necunoscuta secundara
	int count = 0;
	int pozJ;
	for (int j = 0; j < matrice[i].size() - 1; j++)
	{
		if (matrice[i][j] != 0 && j != jPunctPrincipal)
		{
			count++;
			pozJ = j;
		}
	}
	if (count == 1)
		return matrice[i][pozJ];
	else
		return 0;
}

void AfisareTipNecunoscutaSecundara(int j)
{
	if (j == 0)
		std::cout << "alfa";  //daca x e necunoscuta secundara,e notat cu alfa
	if (j == 1)
		std::cout << "beta"; //daca y e necunoscuta secundara,e notat cu alfa
	if (j == 2)
		std::cout << "gamma"; //daca z e necunoscuta secundara,e notat cu alfa
	if (j == 3)
		std::cout << "delta"; //daca t e necunoscuta secundara,e notat cu alfa
	if (j == 4)
		std::cout << "epsilon"; //daca u e necunoscuta secundara,e notat cu alfa
}

void AfisareTipVariabila(int i)
{
	if (i == 0)
		std::cout << "x";  //daca x e necunoscuta secundara,e notat cu alfa
	if (i == 1)
		std::cout << "y"; //daca y e necunoscuta secundara,e notat cu alfa
	if (i == 2)
		std::cout << "z"; //daca z e necunoscuta secundara,e notat cu alfa
	if (i == 3)
		std::cout << "t"; //daca t e necunoscuta secundara,e notat cu alfa
	if (i == 4)
		std::cout << "k"; //daca u e necunoscuta secundara,e notat cu alfa
}

void AfisareSolutieLinie(std::vector<std::vector<float>>& matrice, int i, int jPunctPrincipal)
{
	if (VerificareNecunoscuteSecundarePeLinie(matrice, i)) //daca exista necunoscute secundare pe linie
	{
		for (int j = 0; j < matrice[i].size() - 1; j++)  //afisez cu sens schimbat valoarea din matrice inmultita cu tipul necunoscutei secundare
		{
			if (matrice[i][j] != 0 && j != jPunctPrincipal) //daca elementul nenul nu este cel al necunoscutei principale,atunci vaoloarea de pe pozitia respectiva o sa o trec cu semn schimbat in solutie
			{
				if (matrice[i][j] < 0)
					std::cout << " +" << (-1) * matrice[i][j] << "*";
				else
					std::cout << " " << (-1) * matrice[i][j] << "*";
				AfisareTipNecunoscutaSecundara(j);
			}					//daca elementul nenul este cel al necunoscutei principale,il ignor
		}

		if (matrice[i][matrice[i].size() - 1] != 0)
		{
			if (matrice[i][matrice[i].size() - 1] < 0)
				std::cout << matrice[i][matrice[i].size() - 1];
			else
				std::cout << " + " << matrice[i][matrice[i].size() - 1];
		}
	}
	else if (matrice[i][jPunctPrincipal] != 0)  //daca nu am necunoscute secundare pe linie si am o necunoscuta principala,afisez direct valoarea ei din coloana rezultatelor
	{
		std::cout << matrice[i][matrice[i].size() - 1];
	}
}

void AfisareSolutie(std::vector<std::vector<float>>& matrice, std::vector<int>& vectorNecunoscuteSecundare)
{
	std::cout << "S={";
	int i = 0, j = 0;
	int pozitieVector = 0;

	if (!NecunoscuteSecundare(vectorNecunoscuteSecundare))  //daca nu exista necnunoscute secundare,se afiseaza direct
	{
		std::cout << matrice[0][matrice[0].size() - 1];
		for (int i = 1; i < vectorNecunoscuteSecundare.size(); i++)
			std::cout << ", " << matrice[i][matrice[i].size() - 1];
		std::cout << "}";
	}
	else while (pozitieVector != vectorNecunoscuteSecundare.size())
	{
		if (vectorNecunoscuteSecundare[pozitieVector] == 0)   //daca variabila nu e necunoscuta secundara,o afisez in functie de necunoscutele secundare
		{
			AfisareSolutieLinie(matrice, i, j);
			i++;
			j++;
		}
		else  //daca am necunoscuta secundara,atunci o afisez simplu,direct
		{
			AfisareTipNecunoscutaSecundara(pozitieVector);
			j++;
		}
		if (pozitieVector == vectorNecunoscuteSecundare.size() - 1)  //daca a ajuns la afisarea ultimului element,nu mai pune virgula
		{
			std::cout << "}";
		}
		else
			std::cout << ", ";

		pozitieVector++;
	}
}

bool RezolvareMatrice(std::vector<std::vector<float>>& matrice, std::vector<int>& vectorNecunoscuteSecundare)
{
	if (Rezolvare(matrice, vectorNecunoscuteSecundare))
	{
		std::cout << "\nMatricea finala este\n";
		AfisareMatrice(matrice);
		std::cout << "\n\n";
		AfisareSolutie(matrice, vectorNecunoscuteSecundare);

		if (NecunoscuteSecundare(vectorNecunoscuteSecundare))
		{
			std::cout << "\nNecunoscutele secundare sunt: ";
			AfisareNecunoscuteSecundare(vectorNecunoscuteSecundare);
			std::cout << "unde: ";
			for (int i = 0; i < vectorNecunoscuteSecundare.size(); i++)
				if (vectorNecunoscuteSecundare[i] > 0)
				{
					std::cout << "\n";
					AfisareTipVariabila(i);
					std::cout << " este ";
					AfisareTipNecunoscutaSecundara(i);
				}
			std::cout << "\n\n\n\n";
		}
	}
	else
	{
		std::cout << "Sistem incompatibil!\n\n\n\n";
		return false;
	}
	return true;  //returneaza true daca sistemul are solutie,adica daca este sistem compatibil
}

int main()
{
	std::vector<std::vector<float>> matrice;
	CitireMatrice(matrice);
	std::vector<int> vectorNecunoscuteSecundare1(matrice[0].size() - 1, 0);  //retin necunoscutele secundare intr-un vector si spre exemplu daca y este necunoscuta secundara,incrementez vector[1] ,daca z este necunoscuta secundara incrementez vector[2]
	RezolvareMatrice(matrice, vectorNecunoscuteSecundare1);

	return 0;
}