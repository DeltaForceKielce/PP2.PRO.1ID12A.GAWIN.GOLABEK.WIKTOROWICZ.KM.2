#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <stdbool.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_color.h>
//#include <allegro5/allegro_mouse.h>


#define WIERSZE 8 // liczba wierszy
#define KOLUMNY 8 // liczba kolumn
#define MINY 10 // liczba min

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 700
#define ROZMIAR_KOMORKI (SCREEN_WIDTH / KOLUMNY)
#define FONT_SIZE 20


typedef struct {
	int ma_mine;
	int odkryta;
	int oflagowana;
	int liczba_sasiednich_min;
}pole;


double przegrany_czas(clock_t start, clock_t end) {
	return ((double)(end - start)) / CLOCKS_PER_SEC;
}


void policz_miny(int x, int y, pole tablica[WIERSZE][KOLUMNY], int tablica2[8][8]) {
	int rozmiar_tablicy = 8;
	int i, j;
	for (i = x - 1; i <= x + 1; i++) {
		for (j = y - 1; j <= y + 1; j++) {
			if (i >= 0 && i < rozmiar_tablicy && j >= 0 && j < rozmiar_tablicy && !(i == x && j == y)) {
				tablica2[i][j]++;
			}
		}
	}
}



void zaladowanie_planszy(pole tablica[WIERSZE][KOLUMNY], int tablica2[], int liczba_min) {

	// wyzerowane wszystkie pola planszy
	for (int i = 0; i < WIERSZE; i++) {
		for (int j = 0; j < KOLUMNY; j++) {
			tablica[i][j].ma_mine = 0;
			tablica[i][j].odkryta = 0;
			tablica[i][j].oflagowana = 0;
			tablica[i][j].liczba_sasiednich_min = 0;
		}
	}

	// wylosowanie polozenia min
	srand(time(0));
	int rozmiar_tablicy = 10;

	int ile_polozono = 0;
	while (ile_polozono < liczba_min) {
		int wylosowana_x = rand() % WIERSZE;
		int wylosowana_y = rand() % KOLUMNY;
		if (!tablica[wylosowana_x][wylosowana_y].ma_mine) {
			tablica[wylosowana_x][wylosowana_y].ma_mine = 1;
			ile_polozono++;

			policz_miny(wylosowana_x, wylosowana_y, tablica, tablica2);
		}
	}
}
void rysuj_plansze(pole tablica[WIERSZE][KOLUMNY], int tablica2[8][8],int licznik_oflagowanych,int licznik_bomb) {

	int pozostale_bomby = licznik_bomb - licznik_oflagowanych;
	al_flip_display();
	al_init_image_addon();
	al_init_font_addon();
	ALLEGRO_FONT* font = al_load_font("supercomic.ttf", FONT_SIZE, 0);
	al_clear_to_color(al_map_rgb(255, 255, 255));

	ALLEGRO_BITMAP* bomba = al_load_bitmap("bomba.png");
	ALLEGRO_BITMAP* obrazek0 = al_load_bitmap("obrazek0.png");
	ALLEGRO_BITMAP* obrazek1 = al_load_bitmap("obrazek1.png");
	ALLEGRO_BITMAP* obrazek2 = al_load_bitmap("obrazek2.png");
	ALLEGRO_BITMAP* obrazek3 = al_load_bitmap("obrazek3.png");
	ALLEGRO_BITMAP* obrazek4 = al_load_bitmap("obrazek4.png");
	ALLEGRO_BITMAP* obrazek5 = al_load_bitmap("obrazek5.png");
	ALLEGRO_BITMAP* obrazek6 = al_load_bitmap("obrazek6.png");
	ALLEGRO_BITMAP* obrazek7 = al_load_bitmap("obrazek7.png");
	ALLEGRO_BITMAP* obrazek8 = al_load_bitmap("obrazek8.png");
	ALLEGRO_BITMAP* flaga = al_load_bitmap("flaga.png");
	ALLEGRO_BITMAP* tlo_pozostalych_bomb= al_load_bitmap("pozostale_bomby.png");
	if (tlo_pozostalych_bomb ==NULL||flaga == NULL || bomba == NULL || obrazek0 == NULL || obrazek1 == NULL || obrazek2 == NULL || obrazek3 == NULL || obrazek4 == NULL || obrazek5 == NULL || obrazek6 == NULL || obrazek7 == NULL || obrazek8 == NULL) {
		fprintf(stderr, "B³¹d ³adowania obrazka.\n");
		return;
	}
	ALLEGRO_COLOR borderColor = al_map_rgb(255, 191, 0); // pomaranczowe obramowanie

	ALLEGRO_COLOR color = al_map_rgb(0, 0, 0); // domyyœlny kolor t³a komórki

	for (int i = 0; i < KOLUMNY; i++) {
		for (int j = 0; j < WIERSZE; j++) {
			int x = i * ROZMIAR_KOMORKI;
			int y = j * ROZMIAR_KOMORKI;
			int borderThickness = 5;


			if (tablica[i][j].odkryta == 0 && tablica[i][j].oflagowana == 0) {
				color = al_map_rgb(124, 252, 0); // Kolor t³a dla nieodkrytej komórki zielony
				al_draw_filled_rectangle(x, y, x + ROZMIAR_KOMORKI, y + ROZMIAR_KOMORKI, color);
			}
			if (tablica[i][j].oflagowana == 1) {
				al_draw_bitmap(flaga, x, y, 0);
			}


			al_draw_rectangle(x, y, x + ROZMIAR_KOMORKI, y + ROZMIAR_KOMORKI, borderColor, borderThickness);



			if (tablica[i][j].odkryta == 1) {
				if (tablica2[i][j] == 0)
					al_draw_bitmap(obrazek0, x, y, 0);
				else if (tablica2[i][j] == 1)
					al_draw_bitmap(obrazek1, x, y, 0);
				else if (tablica2[i][j] == 2)
					al_draw_bitmap(obrazek2, x, y, 0);
				else if (tablica2[i][j] == 3)
					al_draw_bitmap(obrazek3, x, y, 0);
				else if (tablica2[i][j] == 4)
					al_draw_bitmap(obrazek4, x, y, 0);
				else if (tablica2[i][j] == 5)
					al_draw_bitmap(obrazek5, x, y, 0);
				else if (tablica2[i][j] == 6)
					al_draw_bitmap(obrazek6, x, y, 0);
				else if (tablica2[i][j] == 7)
					al_draw_bitmap(obrazek7, x, y, 0);
				else
					al_draw_bitmap(obrazek8, x, y, 0);
			}

		}
	}

	color = al_map_rgb(124, 252, 0); // Kolor t³a dla nieodkrytej komórki zielony
	al_draw_filled_rectangle(0, 641, 640, 700, color);
	//al_draw_bitmap(tlo_pozostalych_bomb, 0, 640, 0);
	al_draw_textf(font, al_map_rgb(0, 0, 0), 320, 660, ALLEGRO_ALIGN_CENTER, "Pozostalych bomb: %d",pozostale_bomby);
	al_destroy_bitmap(bomba); 
	al_flip_display();
}

void rysuj_przegrana(pole tablica[WIERSZE][KOLUMNY], int tablica2[8][8]) {

	al_init_image_addon();
	al_init_font_addon();
	ALLEGRO_FONT* font = al_load_font("supercomic.ttf", FONT_SIZE, 0);
	al_clear_to_color(al_map_rgb(255, 255, 255));

	ALLEGRO_COLOR borderColor = al_map_rgb(255, 191, 0); // pomaranczowe obramowanie

	ALLEGRO_COLOR color = al_map_rgb(0, 0, 0); // domyyœlny kolor t³a komórki

	ALLEGRO_BITMAP* bomba = al_load_bitmap("bomba.png");
	ALLEGRO_BITMAP* obrazek0 = al_load_bitmap("obrazek0.png");
	ALLEGRO_BITMAP* obrazek1 = al_load_bitmap("obrazek1.png");
	ALLEGRO_BITMAP* obrazek2 = al_load_bitmap("obrazek2.png");
	ALLEGRO_BITMAP* obrazek3 = al_load_bitmap("obrazek3.png");
	ALLEGRO_BITMAP* obrazek4 = al_load_bitmap("obrazek4.png");
	ALLEGRO_BITMAP* obrazek5 = al_load_bitmap("obrazek5.png");
	ALLEGRO_BITMAP* obrazek6 = al_load_bitmap("obrazek6.png");
	ALLEGRO_BITMAP* obrazek7 = al_load_bitmap("obrazek7.png");
	ALLEGRO_BITMAP* obrazek8 = al_load_bitmap("obrazek8.png");
	ALLEGRO_BITMAP* flaga = al_load_bitmap("flaga.png");
	if (flaga == NULL || bomba == NULL || obrazek0 == NULL || obrazek1 == NULL || obrazek2 == NULL || obrazek3 == NULL || obrazek4 == NULL || obrazek5 == NULL || obrazek6 == NULL || obrazek7 == NULL || obrazek8 == NULL) {
		fprintf(stderr, "B³¹d ³adowania obrazka.\n");
		return;
	}


	for (int i = 0; i < KOLUMNY; i++) {
		for (int j = 0; j < WIERSZE; j++) {
			int x = i * ROZMIAR_KOMORKI;
			int y = j * ROZMIAR_KOMORKI;
			int borderThickness = 5;
			ALLEGRO_COLOR borderColor = al_map_rgb(255, 191, 0); // pomaranczowe obramowanie

			ALLEGRO_COLOR color = al_map_rgb(0, 0, 0); // Domyœlny kolor t³a komórki

			if (tablica[i][j].ma_mine == 1) {
				al_draw_bitmap(bomba, x, y, 0);
			}
			else {
				if (tablica2[i][j] == 0)
					al_draw_bitmap(obrazek0, x, y, 0);
				else if (tablica2[i][j] == 1)
					al_draw_bitmap(obrazek1, x, y, 0);
				else if (tablica2[i][j] == 2)
					al_draw_bitmap(obrazek2, x, y, 0);
				else if (tablica2[i][j] == 3)
					al_draw_bitmap(obrazek3, x, y, 0);
				else if (tablica2[i][j] == 4)
					al_draw_bitmap(obrazek4, x, y, 0);
				else if (tablica2[i][j] == 5)
					al_draw_bitmap(obrazek5, x, y, 0);
				else if (tablica2[i][j] == 6)
					al_draw_bitmap(obrazek6, x, y, 0);
				else if (tablica2[i][j] == 7)
					al_draw_bitmap(obrazek7, x, y, 0);
				else
					al_draw_bitmap(obrazek8, x, y, 0);
			}



			al_draw_rectangle(x, y, x + ROZMIAR_KOMORKI, y + ROZMIAR_KOMORKI, borderColor, borderThickness);





		}
	}
	color = al_map_rgb(124, 252, 0); // Kolor t³a dla nieodkrytej komórki zielony
	al_draw_filled_rectangle(0, 641, 640, 700, color);
	al_draw_textf(font, al_map_rgb(0, 0, 0), 320, 660, ALLEGRO_ALIGN_CENTER, "Przegrales!");
	al_destroy_bitmap(bomba); 
	al_flip_display();
	al_rest(10000.0);

}

void zapisz_czas(int minuty, int sekundy) {
	FILE* plik;
	if (fopen_s(&plik, "ranking.txt", "a") != 0) {
		fprintf(stderr, "B³¹d otwarcia pliku.\n");
		return;
	}

	fprintf(plik, "%d min %d sec\n", minuty, sekundy);

	fclose(plik);
}

void po_wygranej(clock_t start) {
	clock_t end = clock();
	double czas_gry = przegrany_czas(start, end);
	al_init_image_addon();
	al_init_font_addon();
	ALLEGRO_FONT* font = al_load_font("supercomic.ttf", 30, 0);
	al_clear_to_color(al_map_rgb(255, 255, 255));

	ALLEGRO_BITMAP* tlo = al_load_bitmap("tlo.png");
	ALLEGRO_BITMAP* wygranko = al_load_bitmap("wygranko.png");
	if (wygranko == NULL) {
		fprintf(stderr, "B³¹d ³adowania obrazka.\n");
		return;
	}
	al_draw_bitmap(tlo, 0, 0, 0);
	al_draw_bitmap(wygranko, 70, 100, 0);
	int minuty = (int)(czas_gry / 60);
	int sekundy = (int)(czas_gry - minuty * 60);
	al_draw_textf(font, al_map_rgb(255, 255, 255), 360, 520, ALLEGRO_ALIGN_CENTER, "%d min %d sec", minuty, sekundy);
	zapisz_czas(minuty, sekundy);


	al_destroy_bitmap(wygranko); 
	al_flip_display();
	al_rest(10000.0);

}

void obsluga_klikniecia_l(int x, int y, pole tablica[WIERSZE][KOLUMNY], int tablica2[8][8],int licznik_bomb) {
	int kolumna = x;
	int wiersz = y;
	int licznik_oflagowanych = 0;

	// sprawdzenie czy klikniêcie jest w obszarze planszy
	if (kolumna >= 0 && kolumna < KOLUMNY && wiersz >= 0 && wiersz < WIERSZE) {
		if (tablica[kolumna][wiersz].odkryta == 0) {
			tablica[kolumna][wiersz].odkryta = 1;
			for (int i = 0; i < KOLUMNY; i++) {
				for (int j = 0; j < WIERSZE; j++) {
					if (tablica[i][j].oflagowana == 1 && tablica[i][j].ma_mine == 1) {
						licznik_oflagowanych++;
					}
				}
			}
			rysuj_plansze(tablica, tablica2,licznik_oflagowanych,licznik_bomb);
		}
	}
}

void obsluga_klikniecia_p(int x, int y, pole tablica[WIERSZE][KOLUMNY], int tablica2[8][8],int licznik_bomb) {
	int kolumna = x;
	int wiersz = y;
	int licznik_oflagowanych = 0;

	// czy w obszarze planszy
	if (kolumna >= 0 && kolumna < KOLUMNY && wiersz >= 0 && wiersz < WIERSZE) {
		// sprawdzenie czy pole jest odkryte
		if (tablica[kolumna][wiersz].odkryta == 0) {
			// sprawddzenie czy pole jest oflagowane
			if (tablica[kolumna][wiersz].oflagowana == 0) {
				tablica[kolumna][wiersz].oflagowana = 1;
				for (int i = 0; i < KOLUMNY; i++) {
					for (int j = 0; j < WIERSZE; j++) {
						if (tablica[i][j].oflagowana == 1) {
							licznik_oflagowanych++;
						}
					}
				}
			}
			else {
				tablica[kolumna][wiersz].oflagowana = 0;
				for (int i = 0; i < KOLUMNY; i++) {
					for (int j = 0; j < WIERSZE; j++) {
						if (tablica[i][j].oflagowana == 1) {
							licznik_oflagowanych++;
						}
					}
				}

			}
		}
	}

	rysuj_plansze(tablica, tablica2,licznik_oflagowanych,licznik_bomb);


}

void liczenie_index(int mousex, int mousey, pole tablica[WIERSZE][KOLUMNY], int tablica2[8][8], int rodzaj_przycisku,int licznik_bomb) {
	int kolumna = mousex / ROZMIAR_KOMORKI;
	int wiersz = mousey / ROZMIAR_KOMORKI;


	if (rodzaj_przycisku == 1) {
		if (tablica[kolumna][wiersz].ma_mine == 1) {
			rysuj_przegrana(tablica, tablica2);
		}

		else
			obsluga_klikniecia_l(kolumna, wiersz, tablica, tablica2,licznik_bomb);
	}
	else {
		obsluga_klikniecia_p(kolumna, wiersz, tablica, tablica2,licznik_bomb);
	}
}

void wyswietlenie_planszy();

void sortuj_czasy() {
	FILE* plik;
	if (fopen_s(&plik, "ranking.txt", "r+") != 0) {
		fprintf(stderr, "B³¹d otwarcia pliku.\n");
		return;
	}

	int max_czasy = 100;  // Maksymalna liczba czasów w pliku zapisanych
	int* czasy = malloc(max_czasy * sizeof(int));
	int liczba_czasow = 0;

	int minuty, sekundy;
	while (fscanf_s(plik, "%d min %d sec", &minuty, &sekundy) == 2) {
		int czas = minuty * 60 + sekundy;
		czasy[liczba_czasow++] = czas;

		if (liczba_czasow >= max_czasy) {
			// zwiekszenie rozmiaru tablicy jesli jest iwecej niz 100
			max_czasy *= 2;
			czasy = realloc(czasy, max_czasy * sizeof(int));
		}
	}

	fclose(plik);

	// sortowaanie
	for (int i = 0; i < liczba_czasow - 1; i++) {
		for (int j = 0; j < liczba_czasow - i - 1; j++) {
			if (czasy[j] > czasy[j + 1]) {
				int temp = czasy[j];
				czasy[j] = czasy[j + 1];
				czasy[j + 1] = temp;
			}
		}
	}

	// zapisanie posortowanych czasów
	if (fopen_s(&plik, "ranking.txt", "w") != 0) {
		fprintf(stderr, "B³¹d otwarcia pliku.\n");
		free(czasy);
		return;
	}

	for (int i = 0; i < liczba_czasow; i++) {
		int czas = czasy[i];
		int minuty = czas / 60;
		int sekundy = czas % 60;
		fprintf(plik, "%d min %d sec\n", minuty, sekundy);
	}

	fclose(plik);
	free(czasy);
}

void wyswietlenie_tablicy_wynikow(ALLEGRO_DISPLAY* display, ALLEGRO_EVENT_QUEUE* event_queue) {
	al_clear_to_color(al_map_rgb(255, 255, 255));
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_flip_display();

	al_init_font_addon();
	ALLEGRO_FONT* font = al_load_font("supercomic.ttf", 20, 0);

	al_init_image_addon();
	ALLEGRO_BITMAP* tlo = al_load_bitmap("tlo.png");
	ALLEGRO_BITMAP* powrot = al_load_bitmap("powrot.png");

	if (powrot == NULL) {
		fprintf(stderr, "B³¹d ³adowania obrazka.\n");
		return;
	}
	al_draw_bitmap(tlo, 0, 0, 0);
	al_draw_bitmap(powrot, 530, 10, 0);
	al_flip_display();

	bool exit = false;
	ALLEGRO_EVENT event;
	while (!exit) {
		al_wait_for_event(event_queue, &event);

		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			exit = true; 
		}

		else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
			if (event.mouse.button == 1) {  // czy kliknieto lewy przycisk
				int mousex = event.mouse.x;
				int mousey = event.mouse.y;

				// ktory poziom trudnosci jest klikniety
				if (mousey >= 10 && mousey <= 110) {
					if (mousex >= 417 && mousex <= 630) {
						wyswietlenie_planszy(display,event_queue,font);

					}
				}
			}
		}
		sortuj_czasy();
		FILE* plik;
		if (fopen_s(&plik, "ranking.txt", "r") != 0) {
			fprintf(stderr, "B³¹d otwarcia pliku.\n");
			return;
		}

		char linia[100];
		int y = 50;  // pozycja Y dla pierwszego wiersza

		al_draw_textf(font, al_map_rgb(0, 0, 0), 20, 20, ALLEGRO_ALIGN_LEFT, "Ranking poprzednich czasow:");
		int miejsce = 1;

		while (fgets(linia, sizeof(linia), plik) != NULL) {
			// usuwanie pustych znakow nowej linii
			size_t length = strlen(linia);
			if (linia[length - 1] == '\n') {
				linia[length - 1] = '\0';
			}

			// wysweitlenie odczytanej linii
			al_draw_textf(font, al_map_rgb(0, 0, 0), 20, y, ALLEGRO_ALIGN_LEFT, "%d. %s", miejsce, linia);
			y += 30;  // zwiekszenie Y dla kolejnego wiersza
			miejsce++;
		}

		fclose(plik);
		al_flip_display();
	}

}

void rozgrywka(pole tablica[WIERSZE][KOLUMNY], int tablica2[10][10], ALLEGRO_DISPLAY* display, ALLEGRO_FONT* font, ALLEGRO_EVENT_QUEUE* event_queue, int liczba_min, clock_t start) {

	rysuj_plansze(tablica, tablica2,0,liczba_min);

	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_mouse_event_source());

	bool exit = false;
	ALLEGRO_EVENT event;
	ALLEGRO_MOUSE_STATE state;
	while (!exit) {
		al_wait_for_event(event_queue, &event);

		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			exit = true; 
		}
		else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
			if (event.mouse.button & 1) {  // czy jest lewy
				int mousex = event.mouse.x;
				int mousey = event.mouse.y;


				liczenie_index(mousex, mousey, tablica, tablica2, 1,liczba_min);


			}
			else if (event.mouse.button & 2) {  // czy jest prawy
				int mousex = event.mouse.x;
				int mousey = event.mouse.y;


				liczenie_index(mousex, mousey, tablica, tablica2, 2,liczba_min);

				int licznik_oflagowanych = 0;
				for (int i = 0; i < KOLUMNY; i++) {
					for (int j = 0; j < WIERSZE; j++) {
						if (tablica[i][j].oflagowana == 1 && tablica[i][j].ma_mine == 1) {
							licznik_oflagowanych++;
						}
					}
				}

				if (licznik_oflagowanych == liczba_min) {
					po_wygranej(start);
					exit = true;
				}

			}
		}
	}

	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
	al_destroy_font(font);
}

void wyswietlenie_planszy(ALLEGRO_DISPLAY* display,ALLEGRO_EVENT_QUEUE* event_queue,ALLEGRO_FONT* font) {
	al_install_mouse();

	// rejestracja zdarzenia okna
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_mouse_event_source());


	bool poziom_wybrany = false;
	int poziom_trudnosci = 0;

	al_init_image_addon();
	ALLEGRO_BITMAP* tlo = al_load_bitmap("tlo.png");
	ALLEGRO_BITMAP* menu = al_load_bitmap("menu.png");
	ALLEGRO_BITMAP* latwy = al_load_bitmap("latwy.png");
	ALLEGRO_BITMAP* sredni = al_load_bitmap("sredni.png");
	ALLEGRO_BITMAP* trudny = al_load_bitmap("trudny.png");
	ALLEGRO_BITMAP* tablica_wynikow = al_load_bitmap("tablica_wynikow.png");


	if (latwy == NULL || sredni == NULL || trudny == NULL || tablica_wynikow == NULL || menu == NULL) {
		fprintf(stderr, "B³¹d ³adowania obrazka.\n");
		return;
	}

	while (!poziom_wybrany) {
		al_clear_to_color(al_map_rgb(255, 255, 255)); // czysczenie tla

		// rozne poziomy trudnosci
		al_draw_bitmap(tlo, 0, 0, 0);
		al_draw_bitmap(menu, 70, 100, 0);
		al_draw_bitmap(latwy, 120, 214, 0);
		al_draw_bitmap(sredni, 120, 315, 0);
		al_draw_bitmap(trudny, 120, 416, 0);
		al_draw_bitmap(tablica_wynikow, 120, 517, 0);

		al_flip_display();


		ALLEGRO_EVENT event;
		al_wait_for_event(event_queue, &event);

		if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
			if (event.mouse.button == 1) {
				int mousex = event.mouse.x;
				int mousey = event.mouse.y;

				// ktory poziom kliniety
				if (mousey >= 214 && mousey <= 314) {
					if (mousex >= 213 && mousex <= 426) {
						poziom_trudnosci = 1;
						poziom_wybrany = true;
					}
				}
				else if (mousey >= 315 && mousey <= 415) {
					if (mousex >= 213 && mousex <= 426) {
						poziom_trudnosci = 2;
						poziom_wybrany = true;
					}
				}
				else if (mousey >= 416 && mousey <= 516) {
					if (mousex >= 213 && mousex <= 426) {
						poziom_trudnosci = 3;
						poziom_wybrany = true;
					}
				}
				else if (mousey >= 517 && mousey <= 617) {
					if (mousex >= 112 && mousex <= 527) {
						poziom_trudnosci = 4;
						poziom_wybrany = true;
					}
				}
			}
		}
	}


	int liczba_min = 0;
	if (poziom_trudnosci == 1) {
		liczba_min = 1;
		pole tablica[WIERSZE][KOLUMNY];
		int tablica2[WIERSZE][KOLUMNY] = { 0 };
		zaladowanie_planszy(tablica, tablica2, liczba_min);
		al_flip_display();
		clock_t start = clock();
		rozgrywka(tablica, tablica2, display, font, event_queue, liczba_min, start);

	}
	else if (poziom_trudnosci == 2) {
		liczba_min = 10;
		pole tablica[WIERSZE][KOLUMNY];
		int tablica2[WIERSZE][KOLUMNY] = { 0 };
		zaladowanie_planszy(tablica, tablica2, liczba_min);
		al_flip_display();
		clock_t start = clock();
		rozgrywka(tablica, tablica2, display, font, event_queue, liczba_min, start);
	}
	else if (poziom_trudnosci == 3) {
		liczba_min = 60;
		pole tablica[WIERSZE][KOLUMNY];
		int tablica2[WIERSZE][KOLUMNY] = { 0 };
		zaladowanie_planszy(tablica, tablica2, liczba_min);
		al_flip_display();
		clock_t start = clock();
		rozgrywka(tablica, tablica2, display, font, event_queue, liczba_min, start);
	}
	else {
		wyswietlenie_tablicy_wynikow(display, event_queue);
	}

}


void start_okna() {
	ALLEGRO_DISPLAY* display = NULL;
	ALLEGRO_EVENT_QUEUE* event_queue = NULL;
	ALLEGRO_FONT* font = NULL;


	if (!al_init()) {
		fprintf(stderr, "B³¹d inicjalizacji Allegro.\n");
		return;
	}

	display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
	if (!display) {
		fprintf(stderr, "B³¹d tworzenia okna.\n");
		return;
	}

	event_queue = al_create_event_queue();
	if (!event_queue) {
		fprintf(stderr, "B³¹d tworzenia kolejki zdarzeñ.\n");
		al_destroy_display(display);
		return;
	}

	al_init_font_addon();
	al_init_ttf_addon();
	al_init_primitives_addon();

	font = al_load_font("supercomic.ttf", FONT_SIZE, 0);
	if (!font) {
		fprintf(stderr, "B³¹d ³adowania czcionki.\n");
		al_destroy_display(display);
		al_destroy_event_queue(event_queue);
		return;
	}
	wyswietlenie_planszy(display,event_queue,font);

}

int main() {
	start_okna();
}
