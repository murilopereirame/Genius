#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef struct {
	int cor;
	struct NO* prox;
} NO;

typedef struct {
	NO* inicio;
} FILA;

void creditos();
void insereFila(FILA* f);
void geraSeq(int dificuldade, FILA* f);
int confereSeq(int seq[], int tam, FILA* f);
void reproduzirSequencia(FILA* f, int qtde);
void jogar(int dif);
void menu(int op);
void ganhou(int dif);
void perdeu();

int main() {
	srand(time(NULL));
	al_init();
	al_install_keyboard();

	ALLEGRO_EVENT_QUEUE* queue;
	queue = al_create_event_queue();
	al_register_event_source(queue, al_get_keyboard_event_source());

	ALLEGRO_DISPLAY* disp = al_create_display(800, 600);

	ALLEGRO_FONT* font = al_create_builtin_font();
	ALLEGRO_BITMAP* imagem = NULL;

	al_init_image_addon();
	al_init_primitives_addon();
	imagem = al_load_bitmap("GENIUS.png");
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_draw_bitmap(imagem, 300, 50, 0);
	al_draw_text(font, al_map_rgb(255, 255, 255), 400, 250, ALLEGRO_ALIGN_CENTER, "====Bem vindo ao GENIUS!====");
	al_draw_text(font, al_map_rgb(255, 255, 255), 400, 270, ALLEGRO_ALIGN_CENTER, "Tecle \"J\" para jogar.");
	al_draw_text(font, al_map_rgb(255, 255, 255), 400, 285, ALLEGRO_ALIGN_CENTER, "Tecle \"C\" para crÃ©ditos.");
	al_draw_text(font, al_map_rgb(255, 255, 255), 400, 550, ALLEGRO_ALIGN_CENTER, "Tecle \"E\" para sair.");

	al_flip_display();

	int selection = 0;

	while (!selection)
	{
		ALLEGRO_EVENT event;
		al_wait_for_event(queue, &event);
		if (event.type == ALLEGRO_EVENT_KEY_UP)
		{
			if (event.keyboard.keycode == ALLEGRO_KEY_C) {
				al_destroy_bitmap(imagem);
				menu(2);
			}
			else if (event.keyboard.keycode == ALLEGRO_KEY_J) {
				al_destroy_bitmap(imagem);
				menu(1);
			}
			else if (event.keyboard.keycode == ALLEGRO_KEY_E) {
				selection = 1;
			}
		}
	}
	return 0;
}

void menu(int op) {
	switch (op)
	{
	case 1:
		jogar(dificuldade());
		break;
	case 2:
		creditos();
		break;

	default:
		break;
	}

	return;
}

void jogar(int dif) {
	FILA* fila = (FILA*)malloc(sizeof(FILA));
	ALLEGRO_FONT* font = al_create_builtin_font();
	fila->inicio = NULL;
	/*==Dificuldades==
	 * 20 FÃ¡cil 0
	 * 30 MÃ©dio 1
	 * 50 DifÃ­cil 2
	*/

	int tamSeq = 0;
	if (dif == 0)
		tamSeq = 20;
	else if (dif == 1)
		tamSeq = 30;
	else if (dif == 2)
		tamSeq = 50;

	al_clear_to_color(al_map_rgb(0, 0, 0));
	int qtde = 1;
	al_draw_filled_rectangle(300, 200, 400, 300, al_map_rgb(238, 173, 45)); //255 255 0 Q
	al_draw_filled_rectangle(300, 300, 400, 400, al_map_rgb(30, 114, 255)); //200 244 255 A
	al_draw_filled_rectangle(400, 200, 500, 300, al_map_rgb(34, 139, 34)); //70 255 107 W
	al_draw_filled_rectangle(400, 300, 500, 400, al_map_rgb(255, 0, 144)); //255 146 209 S

	al_draw_filled_rectangle(20, 450, 45, 475, al_map_rgb(238, 173, 45)); //255 255 0 Q
	al_draw_filled_rectangle(20, 480, 45, 505, al_map_rgb(30, 114, 255)); //200 244 255 A
	al_draw_filled_rectangle(20, 510, 45, 535, al_map_rgb(34, 139, 34)); //70 255 107 W
	al_draw_filled_rectangle(20, 540, 45, 565, al_map_rgb(255, 0, 144)); //255 146 209 S

	al_draw_text(font, al_map_rgb(255, 255, 255), 55, 460, ALLEGRO_ALIGN_CENTER, "Q");
	al_draw_text(font, al_map_rgb(255, 255, 255), 55, 490, ALLEGRO_ALIGN_CENTER, "A");
	al_draw_text(font, al_map_rgb(255, 255, 255), 55, 520, ALLEGRO_ALIGN_CENTER, "W");
	al_draw_text(font, al_map_rgb(255, 255, 255), 55, 550, ALLEGRO_ALIGN_CENTER, "S");

	al_flip_display();

	al_rest(0.5);
	geraSeq(dif, fila);

	reproduzirSequencia(fila, qtde);

	int gameover = 1;
	int reproduz = 0;
	int win = 0;
	while (gameover) {

		if (qtde == tamSeq) {
			gameover = 0;
			win = 1;
			break;
		}

		if (reproduz) {
			qtde += 1;
			reproduzirSequencia(fila, qtde);
			reproduz = 0;
		}
		int entradas[100];
		int confere = 0;
		int record = 0;
		while (!confere) {
			ALLEGRO_EVENT_QUEUE* queue;
			queue = al_create_event_queue();
			al_register_event_source(queue, al_get_keyboard_event_source());

			ALLEGRO_EVENT event;
			al_wait_for_event(queue, &event);

			if (event.type == ALLEGRO_EVENT_KEY_DOWN)
			{
				if (event.keyboard.keycode == ALLEGRO_KEY_Q) {
					entradas[record] = 2;
					record += 1;
					al_clear_to_color(al_map_rgb(000, 000, 000));
					al_draw_filled_rectangle(300, 200, 400, 300, al_map_rgb(255, 255, 0)); //225 255 0 Amarelo
					al_draw_filled_rectangle(300, 300, 400, 400, al_map_rgb(30, 114, 255)); //200 244 255 Azul
					al_draw_filled_rectangle(400, 200, 500, 300, al_map_rgb(34, 139, 34)); //70 255 107 Verde
					al_draw_filled_rectangle(400, 300, 500, 400, al_map_rgb(255, 0, 144)); //255 146 209 Pink

					al_draw_filled_rectangle(20, 450, 45, 475, al_map_rgb(238, 173, 45)); //255 255 0 Q
					al_draw_filled_rectangle(20, 480, 45, 505, al_map_rgb(30, 114, 255)); //200 244 255 A
					al_draw_filled_rectangle(20, 510, 45, 535, al_map_rgb(34, 139, 34)); //70 255 107 W
					al_draw_filled_rectangle(20, 540, 45, 565, al_map_rgb(255, 0, 144)); //255 146 209 S

					al_draw_text(font, al_map_rgb(255, 255, 255), 55, 460, ALLEGRO_ALIGN_CENTER, "Q");
					al_draw_text(font, al_map_rgb(255, 255, 255), 55, 490, ALLEGRO_ALIGN_CENTER, "A");
					al_draw_text(font, al_map_rgb(255, 255, 255), 55, 520, ALLEGRO_ALIGN_CENTER, "W");
					al_draw_text(font, al_map_rgb(255, 255, 255), 55, 550, ALLEGRO_ALIGN_CENTER, "S");

					al_flip_display();

					al_rest(0.5);
				}
				else if (event.keyboard.keycode == ALLEGRO_KEY_A) {
					entradas[record] = 1;
					record += 1;
					al_clear_to_color(al_map_rgb(000, 000, 000));
					al_draw_filled_rectangle(300, 200, 400, 300, al_map_rgb(238, 173, 45)); //225 255 0 Amarelo
					al_draw_filled_rectangle(300, 300, 400, 400, al_map_rgb(200, 244, 255)); //200 244 255 Azul
					al_draw_filled_rectangle(400, 200, 500, 300, al_map_rgb(34, 139, 34)); //70 255 107 Verde
					al_draw_filled_rectangle(400, 300, 500, 400, al_map_rgb(255, 0, 144)); //255 146 209 Pink

					al_draw_filled_rectangle(20, 450, 45, 475, al_map_rgb(238, 173, 45)); //255 255 0 Q
					al_draw_filled_rectangle(20, 480, 45, 505, al_map_rgb(30, 114, 255)); //200 244 255 A
					al_draw_filled_rectangle(20, 510, 45, 535, al_map_rgb(34, 139, 34)); //70 255 107 W
					al_draw_filled_rectangle(20, 540, 45, 565, al_map_rgb(255, 0, 144)); //255 146 209 S

					al_draw_text(font, al_map_rgb(255, 255, 255), 55, 460, ALLEGRO_ALIGN_CENTER, "Q");
					al_draw_text(font, al_map_rgb(255, 255, 255), 55, 490, ALLEGRO_ALIGN_CENTER, "A");
					al_draw_text(font, al_map_rgb(255, 255, 255), 55, 520, ALLEGRO_ALIGN_CENTER, "W");
					al_draw_text(font, al_map_rgb(255, 255, 255), 55, 550, ALLEGRO_ALIGN_CENTER, "S");

					al_flip_display();

					al_rest(0.5);


				}
				else if (event.keyboard.keycode == ALLEGRO_KEY_W) {
					entradas[record] = 3;
					record += 1;
					al_clear_to_color(al_map_rgb(000, 000, 000));
					al_draw_filled_rectangle(300, 200, 400, 300, al_map_rgb(238, 173, 45)); //225 255 0 Amarelo
					al_draw_filled_rectangle(300, 300, 400, 400, al_map_rgb(30, 114, 255)); //200 244 255 Azul
					al_draw_filled_rectangle(400, 200, 500, 300, al_map_rgb(70, 255, 107)); //70 255 107 Verde
					al_draw_filled_rectangle(400, 300, 500, 400, al_map_rgb(255, 0, 144)); //255 146 209 Pink

					al_draw_filled_rectangle(20, 450, 45, 475, al_map_rgb(238, 173, 45)); //255 255 0 Q
					al_draw_filled_rectangle(20, 480, 45, 505, al_map_rgb(30, 114, 255)); //200 244 255 A
					al_draw_filled_rectangle(20, 510, 45, 535, al_map_rgb(34, 139, 34)); //70 255 107 W
					al_draw_filled_rectangle(20, 540, 45, 565, al_map_rgb(255, 0, 144)); //255 146 209 S

					al_draw_text(font, al_map_rgb(255, 255, 255), 55, 460, ALLEGRO_ALIGN_CENTER, "Q");
					al_draw_text(font, al_map_rgb(255, 255, 255), 55, 490, ALLEGRO_ALIGN_CENTER, "A");
					al_draw_text(font, al_map_rgb(255, 255, 255), 55, 520, ALLEGRO_ALIGN_CENTER, "W");
					al_draw_text(font, al_map_rgb(255, 255, 255), 55, 550, ALLEGRO_ALIGN_CENTER, "S");

					al_flip_display();

					al_rest(0.5);


				}
				else if (event.keyboard.keycode == ALLEGRO_KEY_S) {
					entradas[record] = 4;
					record += 1;
					al_clear_to_color(al_map_rgb(000, 000, 000));
					al_draw_filled_rectangle(300, 200, 400, 300, al_map_rgb(238, 173, 45)); //225 255 0 Amarelo
					al_draw_filled_rectangle(300, 300, 400, 400, al_map_rgb(30, 114, 255)); //200 244 255 Azul
					al_draw_filled_rectangle(400, 200, 500, 300, al_map_rgb(34, 139, 34)); //70 255 107 Verde
					al_draw_filled_rectangle(400, 300, 500, 400, al_map_rgb(255, 146, 209)); //255 146 209 Pink

					al_draw_filled_rectangle(20, 450, 45, 475, al_map_rgb(238, 173, 45)); //255 255 0 Q
					al_draw_filled_rectangle(20, 480, 45, 505, al_map_rgb(30, 114, 255)); //200 244 255 A
					al_draw_filled_rectangle(20, 510, 45, 535, al_map_rgb(34, 139, 34)); //70 255 107 W
					al_draw_filled_rectangle(20, 540, 45, 565, al_map_rgb(255, 0, 144)); //255 146 209 S

					al_draw_text(font, al_map_rgb(255, 255, 255), 55, 460, ALLEGRO_ALIGN_CENTER, "Q");
					al_draw_text(font, al_map_rgb(255, 255, 255), 55, 490, ALLEGRO_ALIGN_CENTER, "A");
					al_draw_text(font, al_map_rgb(255, 255, 255), 55, 520, ALLEGRO_ALIGN_CENTER, "W");
					al_draw_text(font, al_map_rgb(255, 255, 255), 55, 550, ALLEGRO_ALIGN_CENTER, "S");

					al_flip_display();

					al_rest(0.5);


				}
				else if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
					gameover = 0;
					win = 0;
					break;
				}
			}
			al_clear_to_color(al_map_rgb(0, 0, 0));

			al_draw_filled_rectangle(300, 200, 400, 300, al_map_rgb(238, 173, 45)); //255 255 0 Q
			al_draw_filled_rectangle(300, 300, 400, 400, al_map_rgb(30, 114, 255)); //200 244 255 A
			al_draw_filled_rectangle(400, 200, 500, 300, al_map_rgb(34, 139, 34)); //70 255 107 W
			al_draw_filled_rectangle(400, 300, 500, 400, al_map_rgb(255, 0, 144)); //255 146 209 S

			al_draw_filled_rectangle(20, 450, 45, 475, al_map_rgb(238, 173, 45)); //255 255 0 Q
			al_draw_filled_rectangle(20, 480, 45, 505, al_map_rgb(30, 114, 255)); //200 244 255 A
			al_draw_filled_rectangle(20, 510, 45, 535, al_map_rgb(34, 139, 34)); //70 255 107 W
			al_draw_filled_rectangle(20, 540, 45, 565, al_map_rgb(255, 0, 144)); //255 146 209 S

			al_draw_text(font, al_map_rgb(255, 255, 255), 55, 460, ALLEGRO_ALIGN_CENTER, "Q");
			al_draw_text(font, al_map_rgb(255, 255, 255), 55, 490, ALLEGRO_ALIGN_CENTER, "A");
			al_draw_text(font, al_map_rgb(255, 255, 255), 55, 520, ALLEGRO_ALIGN_CENTER, "W");
			al_draw_text(font, al_map_rgb(255, 255, 255), 55, 550, ALLEGRO_ALIGN_CENTER, "S");

			al_flip_display();

			al_rest(0.8);
			if (record == qtde)
				confere = 1;
		}
		reproduz = 1;
		gameover = confereSeq(entradas, qtde, fila);
	}

	if (win) {
		ganhou(dif);
	}
	else {
		perdeu();
	}
	free(fila);
}

int confereSeq(int seq[], int tam, FILA * f) {
	NO* aux = f->inicio;
	for (int i = 0; i < tam; i++) {
		if (seq[i] == aux->cor)
			aux = aux->prox;
		else
			return 0;
	}

	return 1;
}

void reproduzirSequencia(FILA * f, int qtde) {
	ALLEGRO_FONT* font = al_create_builtin_font();
	NO* aux = f->inicio;
	int i = 0;
	while (i < qtde) {
		al_clear_to_color(al_map_rgb(000, 000, 000));
		/* =====CORES=====
		 * Azul 1
		 * Amarelo 2
		 * Verde 3
		 * Rosa 4
		*/

		/*
			al_draw_filled_rectangle(300,200,400,300,al_map_rgb(238,173,45)); //255 255 0 Q
			al_draw_filled_rectangle(300, 300, 400, 400, al_map_rgb(30, 114, 255)); //200 244 255 A
			al_draw_filled_rectangle(400, 200, 500, 300, al_map_rgb(34, 139, 34)); //70 255 107 W
			al_draw_filled_rectangle(400, 300, 500, 400, al_map_rgb(255, 0, 144)); //255 146 209 S
		*/

		if (aux->cor == 1) {
			al_draw_filled_rectangle(300, 200, 400, 300, al_map_rgb(238, 173, 45)); //225 255 0 Amarelo
			al_draw_filled_rectangle(300, 300, 400, 400, al_map_rgb(200, 244, 255)); //200 244 255 Azul
			al_draw_filled_rectangle(400, 200, 500, 300, al_map_rgb(34, 139, 34)); //70 255 107 Verde
			al_draw_filled_rectangle(400, 300, 500, 400, al_map_rgb(255, 0, 144)); //255 146 209 Pink

			al_draw_filled_rectangle(20, 450, 45, 475, al_map_rgb(238, 173, 45)); //255 255 0 Q
			al_draw_filled_rectangle(20, 480, 45, 505, al_map_rgb(30, 114, 255)); //200 244 255 A
			al_draw_filled_rectangle(20, 510, 45, 535, al_map_rgb(34, 139, 34)); //70 255 107 W
			al_draw_filled_rectangle(20, 540, 45, 565, al_map_rgb(255, 0, 144)); //255 146 209 S

			al_draw_text(font, al_map_rgb(255, 255, 255), 55, 460, ALLEGRO_ALIGN_CENTER, "Q");
			al_draw_text(font, al_map_rgb(255, 255, 255), 55, 490, ALLEGRO_ALIGN_CENTER, "A");
			al_draw_text(font, al_map_rgb(255, 255, 255), 55, 520, ALLEGRO_ALIGN_CENTER, "W");
			al_draw_text(font, al_map_rgb(255, 255, 255), 55, 550, ALLEGRO_ALIGN_CENTER, "S");
		}
		else if (aux->cor == 2) {
			al_draw_filled_rectangle(300, 200, 400, 300, al_map_rgb(255, 255, 0)); //225 255 0 Amarelo
			al_draw_filled_rectangle(300, 300, 400, 400, al_map_rgb(30, 114, 255)); //200 244 255 Azul
			al_draw_filled_rectangle(400, 200, 500, 300, al_map_rgb(34, 139, 34)); //70 255 107 Verde
			al_draw_filled_rectangle(400, 300, 500, 400, al_map_rgb(255, 0, 144)); //255 146 209 Pink

			al_draw_filled_rectangle(20, 450, 45, 475, al_map_rgb(238, 173, 45)); //255 255 0 Q
			al_draw_filled_rectangle(20, 480, 45, 505, al_map_rgb(30, 114, 255)); //200 244 255 A
			al_draw_filled_rectangle(20, 510, 45, 535, al_map_rgb(34, 139, 34)); //70 255 107 W
			al_draw_filled_rectangle(20, 540, 45, 565, al_map_rgb(255, 0, 144)); //255 146 209 S

			al_draw_text(font, al_map_rgb(255, 255, 255), 55, 460, ALLEGRO_ALIGN_CENTER, "Q");
			al_draw_text(font, al_map_rgb(255, 255, 255), 55, 490, ALLEGRO_ALIGN_CENTER, "A");
			al_draw_text(font, al_map_rgb(255, 255, 255), 55, 520, ALLEGRO_ALIGN_CENTER, "W");
			al_draw_text(font, al_map_rgb(255, 255, 255), 55, 550, ALLEGRO_ALIGN_CENTER, "S");
		}
		else if (aux->cor == 3) {
			al_draw_filled_rectangle(300, 200, 400, 300, al_map_rgb(238, 173, 45)); //225 255 0 Amarelo
			al_draw_filled_rectangle(300, 300, 400, 400, al_map_rgb(30, 114, 255)); //200 244 255 Azul
			al_draw_filled_rectangle(400, 200, 500, 300, al_map_rgb(70, 255, 107)); //70 255 107 Verde
			al_draw_filled_rectangle(400, 300, 500, 400, al_map_rgb(255, 0, 144)); //255 146 209 Pink

			al_draw_filled_rectangle(20, 450, 45, 475, al_map_rgb(238, 173, 45)); //255 255 0 Q
			al_draw_filled_rectangle(20, 480, 45, 505, al_map_rgb(30, 114, 255)); //200 244 255 A
			al_draw_filled_rectangle(20, 510, 45, 535, al_map_rgb(34, 139, 34)); //70 255 107 W
			al_draw_filled_rectangle(20, 540, 45, 565, al_map_rgb(255, 0, 144)); //255 146 209 S

			al_draw_text(font, al_map_rgb(255, 255, 255), 55, 460, ALLEGRO_ALIGN_CENTER, "Q");
			al_draw_text(font, al_map_rgb(255, 255, 255), 55, 490, ALLEGRO_ALIGN_CENTER, "A");
			al_draw_text(font, al_map_rgb(255, 255, 255), 55, 520, ALLEGRO_ALIGN_CENTER, "W");
			al_draw_text(font, al_map_rgb(255, 255, 255), 55, 550, ALLEGRO_ALIGN_CENTER, "S");
		}

		else if (aux->cor == 4) {
			al_draw_filled_rectangle(300, 200, 400, 300, al_map_rgb(238, 173, 45)); //225 255 0 Amarelo
			al_draw_filled_rectangle(300, 300, 400, 400, al_map_rgb(30, 114, 255)); //200 244 255 Azul
			al_draw_filled_rectangle(400, 200, 500, 300, al_map_rgb(34, 139, 34)); //70 255 107 Verde
			al_draw_filled_rectangle(400, 300, 500, 400, al_map_rgb(255, 146, 209)); //255 146 209 Pink

			al_draw_filled_rectangle(20, 450, 45, 475, al_map_rgb(238, 173, 45)); //255 255 0 Q
			al_draw_filled_rectangle(20, 480, 45, 505, al_map_rgb(30, 114, 255)); //200 244 255 A
			al_draw_filled_rectangle(20, 510, 45, 535, al_map_rgb(34, 139, 34)); //70 255 107 W
			al_draw_filled_rectangle(20, 540, 45, 565, al_map_rgb(255, 0, 144)); //255 146 209 S

			al_draw_text(font, al_map_rgb(255, 255, 255), 55, 460, ALLEGRO_ALIGN_CENTER, "Q");
			al_draw_text(font, al_map_rgb(255, 255, 255), 55, 490, ALLEGRO_ALIGN_CENTER, "A");
			al_draw_text(font, al_map_rgb(255, 255, 255), 55, 520, ALLEGRO_ALIGN_CENTER, "W");
			al_draw_text(font, al_map_rgb(255, 255, 255), 55, 550, ALLEGRO_ALIGN_CENTER, "S");
		}

		al_flip_display();

		al_rest(1.5);

		al_clear_to_color(al_map_rgb(000, 000, 000));
		al_draw_filled_rectangle(300, 200, 400, 300, al_map_rgb(238, 173, 45)); //255 255 0 Q
		al_draw_filled_rectangle(300, 300, 400, 400, al_map_rgb(30, 114, 255)); //200 244 255 A
		al_draw_filled_rectangle(400, 200, 500, 300, al_map_rgb(34, 139, 34)); //70 255 107 W
		al_draw_filled_rectangle(400, 300, 500, 400, al_map_rgb(255, 0, 144)); //255 146 209 S

		al_draw_filled_rectangle(20, 450, 45, 475, al_map_rgb(238, 173, 45)); //255 255 0 Q
		al_draw_filled_rectangle(20, 480, 45, 505, al_map_rgb(30, 114, 255)); //200 244 255 A
		al_draw_filled_rectangle(20, 510, 45, 535, al_map_rgb(34, 139, 34)); //70 255 107 W
		al_draw_filled_rectangle(20, 540, 45, 565, al_map_rgb(255, 0, 144)); //255 146 209 S

		al_draw_text(font, al_map_rgb(255, 255, 255), 55, 460, ALLEGRO_ALIGN_CENTER, "Q");
		al_draw_text(font, al_map_rgb(255, 255, 255), 55, 490, ALLEGRO_ALIGN_CENTER, "A");
		al_draw_text(font, al_map_rgb(255, 255, 255), 55, 520, ALLEGRO_ALIGN_CENTER, "W");
		al_draw_text(font, al_map_rgb(255, 255, 255), 55, 550, ALLEGRO_ALIGN_CENTER, "S");

		al_flip_display();

		al_rest(0.5);
		aux = aux->prox;

		i++;
	}

	al_clear_to_color(al_map_rgb(0, 0, 0));

	al_draw_filled_rectangle(300, 200, 400, 300, al_map_rgb(238, 173, 45)); //255 255 0 Q
	al_draw_filled_rectangle(300, 300, 400, 400, al_map_rgb(30, 114, 255)); //200 244 255 A
	al_draw_filled_rectangle(400, 200, 500, 300, al_map_rgb(34, 139, 34)); //70 255 107 W
	al_draw_filled_rectangle(400, 300, 500, 400, al_map_rgb(255, 0, 144)); //255 146 209 S

	al_draw_filled_rectangle(20, 450, 45, 475, al_map_rgb(238, 173, 45)); //255 255 0 Q
	al_draw_filled_rectangle(20, 480, 45, 505, al_map_rgb(30, 114, 255)); //200 244 255 A
	al_draw_filled_rectangle(20, 510, 45, 535, al_map_rgb(34, 139, 34)); //70 255 107 W
	al_draw_filled_rectangle(20, 540, 45, 565, al_map_rgb(255, 0, 144)); //255 146 209 S

	al_draw_text(font, al_map_rgb(255, 255, 255), 55, 460, ALLEGRO_ALIGN_CENTER, "Q");
	al_draw_text(font, al_map_rgb(255, 255, 255), 55, 490, ALLEGRO_ALIGN_CENTER, "A");
	al_draw_text(font, al_map_rgb(255, 255, 255), 55, 520, ALLEGRO_ALIGN_CENTER, "W");
	al_draw_text(font, al_map_rgb(255, 255, 255), 55, 550, ALLEGRO_ALIGN_CENTER, "S");

	al_flip_display();
}

void geraSeq(int dificuldade, FILA * f) {
	if (dificuldade == 0) {
		for (int i = 0; i < 20; i++) {
			int cor = rand() % 4;

			if (f->inicio == NULL) {
				NO* aux = (NO*)malloc(sizeof(NO));
				aux->prox = NULL;
				aux->cor = cor + 1;
				f->inicio = aux;
			}
			else {
				insereFila(f);
			}
		}
	}
	else if (dificuldade == 1) {
		for (int i = 0; i < 30; i++) {
			int cor = rand() % 4;
			if (f->inicio == NULL) {
				NO* aux = (NO*)malloc(sizeof(NO));
				aux->prox = NULL;
				aux->cor = cor + 1;
				f->inicio = aux;
			}
			else {
				insereFila(f);
			}
		}
	}
	else if (dificuldade == 2) {
		for (int i = 0; i < 50; i++) {
			int cor = rand() % 4;
			if (f->inicio == NULL) {
				NO* aux = (NO*)malloc(sizeof(NO));
				aux->prox = NULL;
				aux->cor = cor + 1;
				f->inicio = aux;
			}
			else {
				insereFila(f);
			}
		}
	}
}

void insereFila(FILA * f) {
	NO* aux = f->inicio;
	while (aux->prox != NULL)
		aux = aux->prox;

	int cor = rand() % 4;
	NO * a2 = (NO*)malloc(sizeof(NO));
	a2->prox = NULL;
	a2->cor = cor + 1;
	aux->prox = a2;
}

void perdeu() {
	int action = 1;
	ALLEGRO_EVENT_QUEUE* queue;
	queue = al_create_event_queue();
	al_register_event_source(queue, al_get_keyboard_event_source());
	ALLEGRO_FONT* font = al_create_builtin_font();

	al_clear_to_color(al_map_rgb(0, 0, 0));

	al_draw_text(font, al_map_rgb(255, 255, 255), 400, 200, ALLEGRO_ALIGN_CENTER, "=====================DERROTA=====================");
	al_draw_text(font, al_map_rgb(255, 255, 255), 400, 210, ALLEGRO_ALIGN_CENTER, "||Que pena :(                                   ||");
	al_draw_text(font, al_map_rgb(255, 255, 255), 400, 220, ALLEGRO_ALIGN_CENTER, "||Tente uma novamente                           ||");
	al_draw_text(font, al_map_rgb(255, 255, 255), 400, 230, ALLEGRO_ALIGN_CENTER, "==================================================");
	al_draw_text(font, al_map_rgb(255, 255, 255), 400, 550, ALLEGRO_ALIGN_CENTER, "Tecle \"B\" para voltar.");

	al_flip_display();

	while (action) {
		ALLEGRO_EVENT event;
		al_wait_for_event(queue, &event);
		if (event.type == ALLEGRO_EVENT_KEY_UP)
		{
			if (event.keyboard.keycode == ALLEGRO_KEY_B) {
				action = 0;
				al_clear_to_color(al_map_rgb(0, 0, 0));
				ALLEGRO_BITMAP* imagem = NULL;
				imagem = al_load_bitmap("GENIUS.png");
				al_draw_bitmap(imagem, 300, 50, 0);
				al_draw_text(font, al_map_rgb(255, 255, 255), 400, 250, ALLEGRO_ALIGN_CENTER, "====Bem vindo ao GENIUS!====");
				al_draw_text(font, al_map_rgb(255, 255, 255), 400, 270, ALLEGRO_ALIGN_CENTER, "Tecle \"J\" para jogar.");
				al_draw_text(font, al_map_rgb(255, 255, 255), 400, 285, ALLEGRO_ALIGN_CENTER, "Tecle \"C\" para crÃ©ditos.");
				al_draw_text(font, al_map_rgb(255, 255, 255), 400, 550, ALLEGRO_ALIGN_CENTER, "Tecle \"E\" para sair.");

				al_flip_display();
				al_destroy_bitmap(imagem);
			}
		}
	}
}

void ganhou(int dif) {
	int action = 1;
	ALLEGRO_EVENT_QUEUE* queue;
	queue = al_create_event_queue();
	al_register_event_source(queue, al_get_keyboard_event_source());
	ALLEGRO_FONT* font = al_create_builtin_font();

	al_clear_to_color(al_map_rgb(0, 0, 0));

	al_draw_text(font, al_map_rgb(255, 255, 255), 400, 200, ALLEGRO_ALIGN_CENTER, "======================VITÃ“RIA=====================");
	al_draw_text(font, al_map_rgb(255, 255, 255), 400, 210, ALLEGRO_ALIGN_CENTER, "||ParabÃ©ns! VocÃª entrou para o hall da fama!    ||");

	if (dif == 0) {
		al_draw_text(font, al_map_rgb(255, 255, 255), 400, 220, ALLEGRO_ALIGN_CENTER, "||VocÃª passou pela dificuldade: fÃ¡cil           ||");
		al_draw_text(font, al_map_rgb(255, 255, 255), 400, 230, ALLEGRO_ALIGN_CENTER, "||Tente uma nova difÃ­culdade!                   ||");
	}
	else if (dif == 1) {
		al_draw_text(font, al_map_rgb(255, 255, 255), 400, 220, ALLEGRO_ALIGN_CENTER, "||VocÃª passou pela dificuldade: mÃ©dio           ||");
		al_draw_text(font, al_map_rgb(255, 255, 255), 400, 230, ALLEGRO_ALIGN_CENTER, "||VocÃª esta quase se tornando uma lenda!        ||");
	}
	else {
		al_draw_text(font, al_map_rgb(255, 255, 255), 400, 220, ALLEGRO_ALIGN_CENTER, "||VocÃª passou pela dificuldade: DÃFICIL         ||");
		al_draw_text(font, al_map_rgb(255, 255, 255), 400, 230, ALLEGRO_ALIGN_CENTER, "||VocÃª conseguiu! Agora vocÃª Ã© uma lenda!       ||");
	}

	al_draw_text(font, al_map_rgb(255, 255, 255), 400, 240, ALLEGRO_ALIGN_CENTER, "==================================================");
	al_draw_text(font, al_map_rgb(255, 255, 255), 400, 550, ALLEGRO_ALIGN_CENTER, "Tecle \"B\" para voltar.");

	al_flip_display();

	while (action) {
		ALLEGRO_EVENT event;
		al_wait_for_event(queue, &event);
		if (event.type == ALLEGRO_EVENT_KEY_UP)
		{
			if (event.keyboard.keycode == ALLEGRO_KEY_B) {
				action = 0;
				al_clear_to_color(al_map_rgb(0, 0, 0));
				ALLEGRO_BITMAP* imagem = NULL;
				imagem = al_load_bitmap("GENIUS.png");
				al_draw_bitmap(imagem, 300, 50, 0);
				al_draw_text(font, al_map_rgb(255, 255, 255), 400, 250, ALLEGRO_ALIGN_CENTER, "====Bem vindo ao GENIUS!===");
				al_draw_text(font, al_map_rgb(255, 255, 255), 400, 270, ALLEGRO_ALIGN_CENTER, "Tecle \"J\" para jogar.");
				al_draw_text(font, al_map_rgb(255, 255, 255), 400, 285, ALLEGRO_ALIGN_CENTER, "Tecle \"C\" para crÃ©ditos.");
				al_draw_text(font, al_map_rgb(255, 255, 255), 400, 550, ALLEGRO_ALIGN_CENTER, "Tecle \"E\" para sair.");

				al_flip_display();

				al_destroy_bitmap(imagem);
			}
		}
	}
}

void creditos() {
	int action = 1;
	ALLEGRO_EVENT_QUEUE* queue;
	queue = al_create_event_queue();
	al_register_event_source(queue, al_get_keyboard_event_source());
	ALLEGRO_FONT* font = al_create_builtin_font();

	al_clear_to_color(al_map_rgb(0, 0, 0));

	al_draw_text(font, al_map_rgb(255, 255, 255), 400, 250, ALLEGRO_ALIGN_CENTER, "==================GENIUS==================");
	al_draw_text(font, al_map_rgb(255, 255, 255), 400, 270, ALLEGRO_ALIGN_CENTER, "||Murilo Rocha Pereira     RA: 181250934||");
	al_draw_text(font, al_map_rgb(255, 255, 255), 400, 280, ALLEGRO_ALIGN_CENTER, "||Willian Sotocorno Mendes RA: 181256479||");
	al_draw_text(font, al_map_rgb(255, 255, 255), 400, 300, ALLEGRO_ALIGN_CENTER, "==========================================");
	al_draw_text(font, al_map_rgb(255, 255, 255), 400, 550, ALLEGRO_ALIGN_CENTER, "Tecle \"B\" para voltar.");

	al_flip_display();

	while (action) {
		ALLEGRO_EVENT event;
		al_wait_for_event(queue, &event);
		if (event.type == ALLEGRO_EVENT_KEY_UP)
		{
			if (event.keyboard.keycode == ALLEGRO_KEY_B) {
				action = 0;
				al_clear_to_color(al_map_rgb(0, 0, 0));
				ALLEGRO_BITMAP* imagem = NULL;
				imagem = al_load_bitmap("GENIUS.png");
				al_draw_bitmap(imagem, 300, 50, 0);
				
				al_draw_text(font, al_map_rgb(255, 255, 255), 400, 250, ALLEGRO_ALIGN_CENTER, "====Bem vindo ao GENIUS!===");
				al_draw_text(font, al_map_rgb(255, 255, 255), 400, 270, ALLEGRO_ALIGN_CENTER, "Tecle \"J\" para jogar.");
				al_draw_text(font, al_map_rgb(255, 255, 255), 400, 285, ALLEGRO_ALIGN_CENTER, "Tecle \"C\" para crÃ©ditos.");
				al_draw_text(font, al_map_rgb(255, 255, 255), 400, 550, ALLEGRO_ALIGN_CENTER, "Tecle \"E\" para sair.");

				al_flip_display();
				al_destroy_bitmap(imagem);
			}
		}
	}
}

int dificuldade() {
	int action = 1;
	ALLEGRO_EVENT_QUEUE* queue;
	queue = al_create_event_queue();
	al_register_event_source(queue, al_get_keyboard_event_source());
	ALLEGRO_FONT* font = al_create_builtin_font();

	al_clear_to_color(al_map_rgb(0, 0, 0));

	al_draw_text(font, al_map_rgb(255, 255, 255), 400, 200, ALLEGRO_ALIGN_CENTER, "===========DIFICULDADE============");
	al_draw_text(font, al_map_rgb(255, 255, 255), 400, 210, ALLEGRO_ALIGN_CENTER, "||Tecle \"0\" para fÃ¡cil.       ||");
	al_draw_text(font, al_map_rgb(255, 255, 255), 400, 220, ALLEGRO_ALIGN_CENTER, "||Tecle \"1\" para mÃ©dio.       ||");
	al_draw_text(font, al_map_rgb(255, 255, 255), 400, 230, ALLEGRO_ALIGN_CENTER, "||Tecle \"2\" para difÃ­cil.     ||");
	al_draw_text(font, al_map_rgb(255, 255, 255), 400, 240, ALLEGRO_ALIGN_CENTER, "=============================");
	al_draw_text(font, al_map_rgb(255, 255, 255), 400, 550, ALLEGRO_ALIGN_CENTER, "Tecle \"B\" para voltar.");

	al_flip_display();

	while (action) {
		ALLEGRO_EVENT event;
		al_wait_for_event(queue, &event);
		if (event.type == ALLEGRO_EVENT_KEY_UP)
		{
			if (event.keyboard.keycode == ALLEGRO_KEY_B) {
				action = 0;
				al_clear_to_color(al_map_rgb(0, 0, 0));
				ALLEGRO_BITMAP* imagem = NULL;
				imagem = al_load_bitmap("GENIUS.png");
				al_draw_bitmap(imagem, 300, 50, 0);
				al_draw_text(font, al_map_rgb(255, 255, 255), 400, 250, ALLEGRO_ALIGN_CENTER, "====Bem vindo ao GENIUS!===");
				al_draw_text(font, al_map_rgb(255, 255, 255), 400, 270, ALLEGRO_ALIGN_CENTER, "Tecle \"J\" para jogar.");
				al_draw_text(font, al_map_rgb(255, 255, 255), 400, 285, ALLEGRO_ALIGN_CENTER, "Tecle \"C\" para crÃ©ditos.");
				al_draw_text(font, al_map_rgb(255, 255, 255), 400, 550, ALLEGRO_ALIGN_CENTER, "Tecle \"E\" para sair.");

				al_flip_display();
				al_destroy_bitmap(imagem);

				return -1;
			}
			else if (event.keyboard.keycode == ALLEGRO_KEY_PAD_0 || event.keyboard.keycode == ALLEGRO_KEY_0) {
				action = 0;
				return 0;
			}
			else if (event.keyboard.keycode == ALLEGRO_KEY_PAD_1 || event.keyboard.keycode == ALLEGRO_KEY_1) {
				action = 0;
				return 1;
			}
			else if (event.keyboard.keycode == ALLEGRO_KEY_PAD_2 || event.keyboard.keycode == ALLEGRO_KEY_2) {
				action = 0;
				return 2;
			}
		}
	}
}
