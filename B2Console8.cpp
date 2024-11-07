// Fait par Ashley, Mouhamed, Elie, Guillome

#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <vector>
#include <mutex>
#include <future>

std::mutex mt;

// Fonction de vérification de nombre premier, retourne vrai ou faux
bool verification(int nombreATraiter)
{
    // Vérifier si le nombre est premier
    if (nombreATraiter > 7 && nombreATraiter % 2 != 0 && nombreATraiter % 3 != 0 && nombreATraiter % 5 != 0 && nombreATraiter % 7 != 0)
    {
        return true;
    }
	else if (nombreATraiter == 2 || nombreATraiter == 3 || nombreATraiter == 5 || nombreATraiter == 7)
	{
		return true;
	}
	else
    {
        return false;
    }
}

// Fonction pour calculer le temps d'execution
void calculerTempsFinal(std::ofstream& fichier, std::chrono::time_point<std::chrono::system_clock> start)
{
	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_milliseconds = end - start;

	fichier << "Temps d'execution : " << elapsed_milliseconds.count() << std::endl;
	std::cout << "Temps d'execution : " << elapsed_milliseconds.count() << std::endl;
}

// Fonction pour écrire la série finale dans le fichier 
void ecrireDansLeFichier(std::ofstream& fichier, std::vector<int> serie)
{
	for (int i = 0; i < serie.size(); i++)
	{
		fichier << serie[i] << std::endl;
	}
}

// Fonction pour créer la liste, prend en paramètre la limite de la liste
void creationDeLaListe(int debut, int limite, std::vector<int>& serie)
{
	std::vector<int> serieThread;
	// Boucle pour parcourir les nombres jusqu'à la limite
	for (int i = debut; i < limite; i++)
	{
		if (verification(i))
		{
			// Ecriture du nombre dans le fichier si il est premier
			serieThread.push_back(i);
		}
	}
	mt.lock();
	serie.insert(serie.end(), serieThread.begin(), serieThread.end());
	mt.unlock();
}

void initialisationThreads(int debut, int limite, int nbThreads, std::vector<int>& serie)
{
	// Compter le temps
	auto start = std::chrono::system_clock::now();

	std::vector<std::thread> threads;
	// Initiliaser chaque thread
	for (int i = 0; i < nbThreads; i++)
	{
		int debutThread = i * (limite / nbThreads);
		int limiteThread = (i + 1) * (limite / nbThreads);
		threads.push_back(std::thread(creationDeLaListe, debutThread, limiteThread, std::ref(serie)));
	}
	// Join les threads
	for (int i = 0; i < nbThreads; i++)
	{
		threads[i].join();
	}

	// Ecrire la serie dans le fichier
	std::ofstream fichier("premiers.txt");
	ecrireDansLeFichier(fichier, serie);
	// Calculer le temps final
	calculerTempsFinal(fichier, start);
}

int main()
{
	// Récuperer les threads
	int nbThreads = std::thread::hardware_concurrency();
	// Créer la liste avec les threads
	std::vector<int> serie;
	
	std::cout << "10 000" << std::endl;
	initialisationThreads(0, 10000, nbThreads, serie);
	initialisationThreads(0, 10000, 1, serie);

	std::cout << "100 000" << std::endl;
	initialisationThreads(0, 100000, nbThreads, serie);
	initialisationThreads(0, 100000, 1, serie);

	std::cout << "1 000 000" << std::endl;
	initialisationThreads(0, 1000000, nbThreads, serie);
	initialisationThreads(0, 1000000, 1, serie);
	return 0;
}