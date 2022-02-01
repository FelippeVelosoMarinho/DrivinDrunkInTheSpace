#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

const float FPS = 100;  

const int SCREEN_W = 960;
const int SCREEN_H = 540;

const int NAVE_W = 100;
//const int NAVE_H = 100;
const int NAVE_H = 50;
const int VELOCIDADE_NAVE = 3;

const int NUM_TIROS = 1;
const int NUM_METEORO = 6;

ALLEGRO_COLOR COR_CENARIO;
ALLEGRO_BITMAP *image =  NULL;
ALLEGRO_SAMPLE *shot = NULL;
ALLEGRO_SAMPLE *morte = NULL;
ALLEGRO_SAMPLE *boom = NULL;

//ALLEGRO_BITMAP *gabigol = NULL;

typedef struct Nave {
	int ID;
	int x, y;
	int vel;
	int dir_x, dir_y;
	int borda_x;
	int borda_y;
	int vida;
	int score;
	ALLEGRO_BITMAP *skin;

	ALLEGRO_COLOR cor;
} Nave;

typedef struct Bloco {
	int x, y;
	int w, h;
	ALLEGRO_BITMAP *gabigol;
	ALLEGRO_COLOR cor;
} Bloco;

typedef struct Meteoro{
	int x, y;
	bool vida;
	int vel;	
	int size;
	ALLEGRO_COLOR cor;
} Meteoro;

typedef struct Tiro{
	int ID;
	int x;
	int y;
	bool vida;
	int size;
	int speed;
}Tiro;

typedef struct Chefao{
	int x, y;
	int vel;	
	int size;
	int w;
	int h;
	bool vida;
	ALLEGRO_COLOR cor;
	ALLEGRO_BITMAP *chefe;
} Chefao;

typedef struct TiroC{
	int x, y;
	int vel;	
	bool vida;
	int size;
	ALLEGRO_COLOR cor;
} TiroC;

int random(int n){
 return rand() %n;
}

int randInt(int min, int max) {
    return min + random(max - min + 1);

}


//Inicia elementos do jogo
void initGlobais(){
	
	COR_CENARIO = al_map_rgb(rand()%64, rand()%64, rand()%64);
	
}

void initBloco(Bloco *bloco){
	
	bloco->x = SCREEN_W + rand()%(SCREEN_W);
	bloco->y = rand()%(4*SCREEN_H/5);
	bloco->w = SCREEN_W + rand()%(SCREEN_W); 
	bloco->h = SCREEN_H/5 + rand()%(2*SCREEN_H/5); 
	bloco->cor = al_map_rgb(rand(), rand(), rand());
	bloco->gabigol = al_load_bitmap("Img\\gaigol.jpg");
	
}

void initNave(Nave *nave){
	
	nave->x = 10 + NAVE_W;
	nave->y = SCREEN_H/2;
	nave->dir_x = 0;
	nave->dir_y = 0;	
	nave->cor = al_map_rgb(192+rand()%64, 192+rand()%64, 192+rand()%64);
	nave->skin = al_load_bitmap("Img\\starShip3.png");
	nave->vel = VELOCIDADE_NAVE;
	nave->vida = 1;
}

void initMeteoro(Meteoro meteoro[]){
	for(int i = 0; i < NUM_METEORO; i++){
		meteoro[i].cor = al_map_rgb(rand(), rand(), rand());
		meteoro[i].vida = false;
		meteoro[i].vel = randInt(1,5);//192+rand()%64
		meteoro[i].size = randInt(15,60);
	}
}

void initTiro(Tiro tiro[], int tamanho,Nave nave){
	for(int i = 0; i < tamanho; i++){
		//tiro[i].ID = TIRO;
		tiro[i].x = nave.x + NAVE_W;
		tiro[i].y = nave.y + NAVE_H/2;
		tiro[i].speed = 10;
		tiro[i].size = 4;
		tiro[i].vida = false;
	}
}

//Desenha elementos do Jogo
void desenhaCenario() {//int x
	//al_clear_to_color(COR_CENARIO);
	al_draw_bitmap(image, 0, 0, 0);
}

void desenhaNave(Nave nave) {
	//al_draw_filled_rectangle(nave.x-NAVE_W/2, nave.y - 14, nave.x - 25, nave.y - 7, al_map_rgb(255, 255, 255));
	//al_draw_filled_rectangle(nave.x-NAVE_W/2, nave.y + 14, nave.x - 25, nave.y + 7, al_map_rgb(255, 255, 255));
	//al_draw_filled_triangle(nave.x, nave.y, nave.x - NAVE_W, nave.y - NAVE_H/2, nave.x - NAVE_W, nave.y + NAVE_H/2, nave.cor);
	al_draw_bitmap(nave.skin, nave.x - NAVE_W, nave.y- NAVE_H/2, 0);
}

void desenhaBloco(Bloco bloco){
	//al_draw_bitmap_region(bloco.gabigol,bloco.x, bloco.y, bloco.x + bloco.w, bloco.y + bloco.h, bloco.x, bloco.y, 0);
	al_draw_filled_rectangle(bloco.x, bloco.y, bloco.x + bloco.w, bloco.y + bloco.h, bloco.cor);
	
}

void desenhaMeteoro(Meteoro meteoro[]){
	for(int i = 0; i < NUM_METEORO; i++){
		if(meteoro[i].vida){
			al_draw_filled_circle(meteoro[i].x, meteoro[i].y, meteoro[i].size, meteoro[i].cor);//20
		}
	}
}

void desenhaTiro(Tiro tiro[], int tamanho, int tecla){

	for( int i = 0; i < tamanho; i++){
		if(tiro[i].vida)
			al_draw_filled_circle(tiro[i].x, tiro[i].y, tiro[i].size, al_map_rgb(255, 255, 255));
	}
}



//Atualiza os valores das variáveis declaradas no struct 
void atualizaNave(Nave *n){
	
	n->y += n->dir_y * n->vel;
	n->x += n->dir_x * n->vel;
	
}

void atualizaTiro(Tiro tiro[], int tamanho){
	for(int i = 0; i < tamanho; i++){
		if(tiro[i].vida){
			tiro[i].x += tiro[i].speed;
			if(tiro[i].x > SCREEN_W)
				tiro[i].vida = false;
		}
	}
}
void atualizaMeteoro(Meteoro meteoro[]){
	for(int i = 0; i < NUM_METEORO; i++){
		if(meteoro[i].vida){
			meteoro[i].x -= meteoro[i].vel;

			if(meteoro[i].x+meteoro[i].size < 0 && meteoro[i].y<0 && meteoro[i].y<SCREEN_H)
				meteoro[i].vida = false;
		}
	}
}

void atualizaBloco(Bloco *bloco){
	//int vel = 0;
	bloco->x -= 1;
	
	if(bloco->x + bloco->w < 0){
		initBloco(bloco);
	}
	
}

//Funções de movimento Tiro e Inimigo
void movimentaMeteoro(Meteoro meteoro[]){
	for(int i = 0; i < NUM_METEORO; i++){
		if(!meteoro[i].vida){
			if(rand() % 500 == 0){
				meteoro[i].vida = true;
				meteoro[i].x = SCREEN_W;
				meteoro[i].y = 30 + rand() % (SCREEN_H - 60);

				break;
			}
		}
	}
}

void atira(Tiro tiro[], int tamanho ,Nave nave, float tiro_duracao, bool tiro_avancado_duracao, int tecla, float inicio_tiro, bool segurando_tecla){
        switch(tecla) {
			//se a tecla for o espaço
			case ALLEGRO_KEY_SPACE:
                inicio_tiro = al_get_time();
                segurando_tecla = true;
			break;

		}
	for(int i = 0; i < tamanho; i++){
		/*if(!tiro[i].vida){
			if(tiro_duracao<tiro_avancado_duracao){
				tiro[i].vida = true;
				tiro[i].size++;
			}		
		}*/

		if(!tiro[i].vida){
			tiro[i].x = nave.x;
			tiro[i].y = nave.y;
			tiro[i].vida = true;
		
			shot = al_load_sample("Sound\\Laser_Shoot.ogg");
			al_play_sample(shot, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);

			break;
		}
	}
}

//função que prende a nave na tela
void mantemNaveNaTela(Nave *nave){
	if(nave->x > SCREEN_W){
		nave->x = SCREEN_W;
		//printf("Lado 1");	
	}
	if(nave->x-NAVE_W<0){
		nave->x = 0+NAVE_W;
		//printf("Lado 2");
	}
	if(nave->y+(NAVE_H/2)>SCREEN_H){
		nave->y = SCREEN_H-(NAVE_H/2);
		//- nave->borda_y
		//printf("Lado 3");
	}
	if(nave->y-(NAVE_H/2)<0){
		nave->y = 0+(NAVE_H/2);
		//nave->y = SCREEN_H + nave->borda_y;
		//printf("Lado 4");
	}	
}


 //Colisoes 

void colisaoTiroBloco(Tiro tiro[], Bloco bloco){
	for(int i = 0; i < NUM_TIROS; i++){
		if(tiro[i].x  > bloco.x && tiro[i].x-4 < bloco.x + bloco.w && tiro[i].y+4 > bloco.y && tiro[i].y-4 < bloco.y + bloco.h){	
			tiro[i].vida = false;
		}	
	}
}
void colisaoMeteoroBloco(Bloco bloco, Meteoro meteoro[]){
	
	for(int i = 0; i < NUM_METEORO; i++){
		if(meteoro[i].x+meteoro[i].size  > bloco.x /*- bloco.w */&& meteoro[i].x - meteoro[i].size < bloco.x + bloco.w && meteoro[i].y + meteoro[i].size > bloco.y /*- bloco.h*/ && meteoro[i].y - meteoro[i].size < bloco.y + bloco.h){
			meteoro[i].vida = false;
			//printf("meteoro: %d ", meteoro[i].vida);
		}
	}					
}

void colisaoTiro(Tiro tiro[], int tTam, Meteoro meteoro[], int *escore){
	//int score = 0;
	
	for(int i = 0; i < tTam; i++){
		if(tiro[i].vida){
			for(int j =0; j < NUM_METEORO; j++){
				if(meteoro[j].vida){
					if(tiro[i].x+tiro[i].size > (meteoro[j].x - meteoro[j].size) &&
						tiro[i].x-tiro[i].size < (meteoro[j].x + meteoro[j].size) &&
						tiro[i].y+tiro[i].size > (meteoro[j].y - meteoro[j].size) &&
						tiro[i].y-tiro[i].size < (meteoro[j].y + meteoro[j].size)){
						boom = al_load_sample("Sound\\Explosion.ogg");
						al_play_sample(boom, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);	
						if(tiro[i].size<16){
							tiro[i].vida = false;
						}	
						
						meteoro[j].vida = false;

						*escore += meteoro[j].size;

					}
				}
			}
		}
	}
}

void colisaoMeteoroMeteoro(Meteoro meteoro[]){
	for(int i = 0; i < 1; i++){
		if(meteoro[i].vida /*&& meteoro[i].x<0*/){
			for(int j =0; j < 1; j++){
				if(meteoro[j].vida /*&& meteoro[j].x<0*/){
					if(meteoro[i].x + meteoro[i].size > meteoro[j].x - meteoro[j].size &&
						meteoro[i].x - meteoro[i].size < meteoro[j].x + meteoro[j].size &&
						meteoro[i].y + meteoro[i].size > meteoro[j].y - meteoro[j].size &&
						meteoro[i].y - meteoro[i].size < meteoro[j].y + meteoro[j].size){					
							meteoro[i].vida = false;
							meteoro[j].vida = false;
							printf("churinsques");
					}
				}
			}
		}
	}
}/*
void colisaoMeteoroMeteoro(Meteoro meteoro[]){
	for(int i = 0; i < NUM_METEORO; i++){
		if(meteoro[i].vida&& meteoro[i].x>SCREEN_W){
		
				
					if(meteoro[i].x + meteoro[i].size> (meteoro[i].x - meteoro[i].size) &&
						meteoro[i].x - meteoro[i].size < (meteoro[i].x + meteoro[i].size) &&
						meteoro[i].y + meteoro[i].size> (meteoro[i].y - meteoro[i].size) &&
						meteoro[i].y - meteoro[i].size< (meteoro[i].y + meteoro[i].size)){					
							meteoro[i].vida = false;
							printf("churinsques");
					}
				
			
		}
	}
}*/


int colisaoMeteoro(Meteoro meteoro[], Bloco bloco, Nave nave){
	
	for(int i = 0; i <NUM_METEORO; i++){
		if(meteoro[i].vida){
			if(meteoro[i].x + meteoro[i].size >= nave.x- NAVE_W && meteoro[i].x-meteoro[i].size <= nave.x){
				if(meteoro[i].y + meteoro[i].size >= nave.y - NAVE_H/2 && meteoro[i].y - meteoro[i].size <= nave.y + NAVE_H/2){
					//se a nave colidiu com um meteoro, atualiza o valor do jogo
					printf("colisao nave meteoro");
					nave.vida--;
					printf(" VIDA: %d ",nave.vida);
					morte = al_load_sample("Sound\\morte.ogg");
					al_play_sample(morte, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
					if(nave.vida == 0){
						return 1;
					}
				}
			}			 
		}
	}

	if(nave.x  > bloco.x && nave.x - NAVE_W < bloco.x + bloco.w && nave.y + (NAVE_H/2) > bloco.y && nave.y - (NAVE_H/2) < bloco.y + bloco.h){
		printf("colisao nave bloco");
		nave.vida--;
		morte = al_load_sample("Sound\\morte.ogg");
		al_play_sample(morte, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
		//printf(" VIDA: %d ",nave.vida);
		if(nave.vida == 0){
			return 1;
		}
		return 1;//+ NAVE_W
	}else{
		return 0;
	}
}

//Score e recorde
void score(ALLEGRO_FONT *size_32, int time, int *escore){
	//int score = time;
	Tiro tiro[1];
	Meteoro meteoro[NUM_METEORO];

	//int excore = excore + *escore;
	
	al_draw_textf(size_32,al_map_rgb(255,255,255),0,0,0,"SCORE %d", *escore);
	al_draw_textf(size_32,al_map_rgb(255,255,255),SCREEN_W/2-100,SCREEN_H-95,0,"TEMPO %d", time);
	if(time=100){
		
	}

}

void tiro2(Tiro tiro[],bool teclaApertada, int dispara){

	if(teclaApertada && dispara){
		for(int i = 0; i < NUM_TIROS; i++){
			printf("balacobaco");
			if(tiro[i].size<16)
				tiro[i].size++;	
			}	
		
	}
	
}

void crypt(char *array){
    int i;
    char secret[] = {22,54,15,45,65,33,12,54,10,34}; //colocar o tamanho do secret[] na linha de baixo
	int tam_secret = 10;
	int array_size = strlen(array);
    for(i = 0; i < array_size; i++){
        array[i] ^= secret[i%tam_secret]; 
	}
}

float getRecorde(){
	FILE *arq;
	char x[20];
	arq = fopen("recorde","r");
	if(arq != NULL){
		fscanf(arq,"%s",x);
		fclose(arq);
	}
	crypt(x);
	return (float)atof(x);
}
 
void setRecorde(float x){
	char str[20];
	FILE *arq;
	arq = fopen("recorde","w+");
	sprintf(str, "%f", x);
	crypt(str);
	fprintf(arq,"%s",str);
	fclose(arq);
} 

void initChefao(Chefao *chefao){
	chefao->x = SCREEN_W-95;
	chefao->y = 0;
	chefao->w = SCREEN_W; 
	chefao->h = SCREEN_H; 
	chefao->cor = al_map_rgb(0, 0, 0);
	chefao->vida = 1;
	chefao->chefe = al_load_bitmap("Img\\cops2.png");	
}

void desenhaChefao(Chefao chefao){
	al_draw_filled_rectangle(chefao.x, chefao.y, chefao.x + chefao.w, chefao.y + chefao.h, chefao.cor);
	al_draw_bitmap(chefao.chefe, chefao.x, chefao.y, 0);
}

int colisaoChefao(Chefao chefao, Nave nave, Tiro tiro[],int *vidaChefe){
	if(nave.x  > chefao.x && nave.x - NAVE_W < chefao.x + chefao.w && nave.y + (NAVE_H/2) > chefao.y && nave.y - (NAVE_H/2) < chefao.y + chefao.h){
		printf("colisao nave chefe");
		nave.vida--;
		if(nave.vida == 0){
				return 1;
		}else{
		return 0;
		}
	}
	
	
	for(int i = 0; i < NUM_TIROS; i++){
		
		if(tiro[i].vida){
			if(chefao.vida){
				if(tiro[i].x  > chefao.x && tiro[i].x-4 < chefao.x + chefao.w && tiro[i].y+4 > chefao.y && tiro[i].y-4 < chefao.y + chefao.h){	
					tiro[i].vida = false;
					*vidaChefe-=1;
					if(tiro[i].size>=15){
						*vidaChefe-=4;
					}
					//chefao.vida--;
				}				
			}
			
		}

	}	
	return 0;

}
int chefaoVida(Chefao chefao, int *vidaChefe, int tempo, int *escore){
	
	if(*vidaChefe <= 0){
		boom = al_load_sample("Sound\\Explosion.ogg");
		al_play_sample(boom, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
		chefao.vida = false;
		return 1;
	}

	return 0;
}
void printaChefeVida(ALLEGRO_FONT *size_32, int *vidaChefe){ 
	al_draw_textf(size_32,al_map_rgb(255,255,255),SCREEN_W-200,0,0,"BOSS %d", *vidaChefe);
}


void initTiroC(TiroC tiroc[]){
	for(int i = 0; i < 1; i++){
		//tiro[i].ID = TIRO;
		tiroc[i].x = randInt(SCREEN_W,SCREEN_W-95);
		tiroc[i].y = randInt(0,SCREEN_H);
		tiroc[i].vel = 8;
		tiroc[i].size = 25;
		tiroc[i].vida = false;
		tiroc[i].cor = al_map_rgb(255, 255, 255);
	}
}

void atualizaTiroC(TiroC tiroc[], int tamanho){
	for(int i = 0; i < tamanho; i++){
		if(tiroc[i].vida){
			tiroc[i].x -= tiroc[i].vel;
			if(tiroc[i].x < 0)
				tiroc[i].vida = false;
		}
	}
}
void movimentaTiroC(TiroC tiroc[]){
	for(int i = 0; i < 5; i++){
		if(!tiroc[i].vida){
			//if(rand() % 500 == 0){
				tiroc[i].vida = true;
				tiroc[i].x = SCREEN_W-95;
				tiroc[i].y = 30 + rand() % (SCREEN_H - 60);

				break;
			//}
		}
	}
}
void desenhaTiroC(TiroC tiroc[], int tamanho){
	
		
	for( int i = 0; i < tamanho; i++){
		if(tiroc[i].vida)
			al_draw_filled_circle(tiroc[i].x, tiroc[i].y, tiroc[i].size, al_map_rgb(255, 255, 255));
	}
}

int colisaoTiroC(TiroC tiroc[], Nave nave, Bloco bloco){

	
	for(int i = 0; i <NUM_TIROS; i++){
		//if(tiroc[i].vida){
			if(tiroc[i].x + tiroc[i].size >= nave.x- NAVE_W && tiroc[i].x <= nave.x){
				if(tiroc[i].y + tiroc[i].size >= nave.y - NAVE_H/2 && tiroc[i].y - tiroc[i].size <= nave.y + NAVE_H/2){
					//se a nave colidiu com um meteoro, atualiza o valor do jogo
					printf("colisao nave tirozao");
					nave.vida--;
					
					morte = al_load_sample("//Sound/morte.ogg");
					//printf(" VIDA: %d ",nave.vida);
					printf(" tchurururiri");
					if(nave.vida == 0){
						return 1;
					}else{
						return 0;
					}
				}
			}			 
		//}
	}
	
	for(int i = 0; i < NUM_TIROS; i++){
		if(tiroc[i].x  > bloco.x && tiroc[i].x - tiroc[i].size < bloco.x + bloco.w && tiroc[i].y + tiroc[i].size > bloco.y/*bloco.y - bloco.h*/ && tiroc[i].y - tiroc[i].size < bloco.y + bloco.h){
			tiroc[i].vida = false;
			//printf("meteoro: %d ", meteoro[i].vida);
		}
	}

	return 0;
}

void colisaoTiroCMeteoro(TiroC tiroc[], Meteoro meteoro[]){
	for(int i = 0; i < NUM_TIROS; i++){
		if(tiroc[i].vida){
			for(int j =0; j < NUM_METEORO; j++){
				if(meteoro[j].vida){
					if(tiroc[i].x+tiroc[i].size > (meteoro[j].x - meteoro[j].size) &&
						tiroc[i].x-tiroc[i].size < (meteoro[j].x + meteoro[j].size) &&
						tiroc[i].y+tiroc[i].size > (meteoro[j].y - meteoro[j].size) &&
						tiroc[i].y-tiroc[i].size < (meteoro[j].y + meteoro[j].size)){	
						meteoro[j].vida = false;
					}
				}
			}
		}
	}
}


 float dist(int x1, int y1, int x2, int y2){//distancia pra acionar o modo de batalha
	 return sqrt(pow(x1 - x2, 2)+ pow(y1 -y2,2));
	 
 }

int main(int argc, char **argv){
	
	srand(time(NULL));
	
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_TIMER *timer2 = NULL;
	
	ALLEGRO_SAMPLE *sample = NULL;
	
	//MENU
	ALLEGRO_BITMAP *menu = NULL;
	ALLEGRO_BITMAP *tutorial = NULL;
	ALLEGRO_BITMAP *botaoPlay = NULL;
	ALLEGRO_BITMAP *botaoOptions = NULL;	
	
	//ALLEGRO_SAMPLE *shot = NULL;
	ALLEGRO_SAMPLE *boom = NULL;
	ALLEGRO_SAMPLE *song = NULL;
	
	ALLEGRO_SAMPLE *siren = NULL;
	ALLEGRO_SAMPLE_INSTANCE *songInstance = NULL;
	ALLEGRO_SAMPLE_INSTANCE *songInstance2 = NULL;
	
	
	//----------------------- rotinas de inicializacao ---------------------------------------
	//inicializa o Allegro
	if(!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}
	
    //inicializa o módulo de primitivas do Allegro
    if(!al_init_primitives_addon()){
		fprintf(stderr, "failed to initialize primitives!\n");
        return -1;
    }	
	
	//inicializa o modulo que permite carregar imagens no jogo
	if(!al_init_image_addon()){
		fprintf(stderr, "failed to initialize image module!\n");
		return -1;
	}
   
	//cria um temporizador que incrementa uma unidade a cada 1.0/FPS segundos
    timer = al_create_timer(1.0 / FPS);
    if(!timer) {
		fprintf(stderr, "failed to create timer!\n");
		return -1;
	}
	
	timer2 = al_create_timer(1.0 / FPS);
    if(!timer2) {
		fprintf(stderr, "failed to create timer2!\n");
		return -1;
	}
 
	//cria uma tela com dimensoes de SCREEN_W, SCREEN_H pixels
	display = al_create_display(SCREEN_W, SCREEN_H);
	if(!display) {
		fprintf(stderr, "failed to create display!\n");
		al_destroy_timer(timer);
		return -1;
	}

	//instala o teclado
	if(!al_install_keyboard()) {
		fprintf(stderr, "failed to install keyboard!\n");
		return -1;
	}

	//inicializa o modulo allegro que carrega as fontes
	al_init_font_addon();

	//inicializa o modulo allegro que entende arquivos tff de fontes
	if(!al_init_ttf_addon()) {
		fprintf(stderr, "failed to load tff font module!\n");
		return -1;
	}
	
	//carrega o arquivo arial.ttf da fonte Arial e define que sera usado o tamanho 32 (segundo parametro)
    /*ALLEGRO_FONT *size_32 = al_load_font("arial.ttf", 32, 1);   
	if(size_32 == NULL) {
		fprintf(stderr, "font file does not exist or cannot be accessed!\n");
	}*/
    ALLEGRO_FONT *size_32 = al_load_font("8-BIT WONDER.ttf", 32, 1);   
	if(size_32 == NULL) {
		fprintf(stderr, "font file does not exist or cannot be accessed!\n");
	}	

 	//cria a fila de eventos
	event_queue = al_create_event_queue();
	if(!event_queue) {
		fprintf(stderr, "failed to create event_queue!\n");
		al_destroy_display(display);
		return -1;
	}
	/*gabigol = al_load_bitmap("Img\\gaigol.png");
	if(!gabigol) {
		fprintf(stderr, "Falha ao criar o gabigol!\n");
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	}	*/
	
	menu = al_load_bitmap("Img\\MenuTela.png");
	if(!menu) {
		fprintf(stderr, "Falha ao criar o menu bitmap!\n");
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	}		
	
    // Botão Play
  botaoPlay = al_load_bitmap("Img\\botaoPlay.png");
  if(!botaoPlay) {
    fprintf(stderr, "Falha ao criar o botao larguraBotaoPlay bitmap!\n");
    al_destroy_display(display);
    al_destroy_timer(timer);
    return -1;
  }

  float xBotaoPlay = SCREEN_W/2 - 90;
  float yBotaoPlay = SCREEN_H/2 + 100 ;

  // Botão Options
  botaoOptions = al_load_bitmap("Img\\Botoes.png");
  if(!botaoOptions) {
    fprintf(stderr, "Falha ao criar o botao Continue bitmap!\n");
    al_destroy_display(display);
    al_destroy_timer(timer);
    return -1;
  }	
  float xBotaoOptions = SCREEN_W/2 - 90;
  float yBotaoOptions = SCREEN_H/2 + 170; 

	image = al_load_bitmap("./Img/background2.png");
	if(!image) {
		fprintf(stderr, "Falha ao criar o background!\n");
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	 }
	tutorial = al_load_bitmap("./Img/MenuOptions.png");
	if(!tutorial) {
		fprintf(stderr, "Falha ao criar a imagem de tutorial!\n");
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	 }
	
	//incializa o audio
	al_install_audio();

	//incializa o decodificador de audio
    al_init_acodec_addon();

	//shot = al_load_sample("./Sound/Laser_Shoot.ogg");
	song = al_load_sample("./Sound/tema.ogg");
	siren = al_load_sample("Sound\\Siren.ogg");
	/*sample = al_load_sample("Sound\\tema.ogg");
	if(!sample){
		fprintf(stderr, "Falha ao carregar som\n");
		return -1;
	}*/
	 
   
	al_reserve_samples(40);
	songInstance = al_create_sample_instance(song);
	songInstance2 = al_create_sample_instance(siren);
	al_set_sample_instance_playmode(songInstance, ALLEGRO_PLAYMODE_LOOP);
	al_set_sample_instance_playmode(songInstance2, ALLEGRO_PLAYMODE_LOOP);
	
	al_attach_sample_instance_to_mixer(songInstance, al_get_default_mixer());
	al_attach_sample_instance_to_mixer(songInstance2, al_get_default_mixer());
	/*
	/* Loop the sample until the display closes. */
	//al_play_sample(sample, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP,NULL);   


	//registra na fila os eventos de tela (ex: clicar no X na janela)
	al_register_event_source(event_queue, al_get_display_event_source(display));
	//registra na fila os eventos de tempo: quando o tempo altera de t para t+1
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	
	//registra na fila os eventos de teclado (ex: pressionar uma tecla)
	al_register_event_source(event_queue, al_get_keyboard_event_source()); 	

	initGlobais();
	
	Nave nave;
	initNave(&nave);
	
	Tiro tiros[5];
	initTiro(tiros, NUM_TIROS, nave);
	
	TiroC tiroc[5];
	initTiroC(tiroc);
	
	Meteoro meteoro[NUM_METEORO];
	initMeteoro(meteoro);
	
	Bloco bloco;
	initBloco(&bloco);
	
	Chefao chefao;
	initChefao(&chefao);

	//inicia o temporizador
	al_start_timer(timer);
	int escore = 0;
	
	int vidaChefe = 10;
	
	bool done = false;
	bool render = false;
	
	int playing = 1;
	bool menuPlay = true;
	int jogoInicia = 0;	
	bool noMenu = true;
	bool naOpcao = false;
	bool continuar = false; 
	
	bool segurando_tecla = true;
	float tiro_avancado_duracao = 1.00;
	float inicio_tiro = 0;	
	
	bool chefe = 0;
	int dispara =0;

	while(playing) {
		ALLEGRO_EVENT ev;
		//espera por um evento e o armazena na variavel de evento ev
		al_wait_for_event(event_queue, &ev);

		//se o tipo de evento for um evento do temporizador, ou seja, se o tempo passou de t para t+1
		if(ev.type == ALLEGRO_EVENT_TIMER) {
			
		// -------- Menu --------------
		
		if (menuPlay){
		  // Desenha o Menu Principal
		  al_play_sample_instance(songInstance);
		  
		  al_draw_bitmap(menu, 0, 0, 0);
		  al_draw_bitmap_region(botaoPlay, 144, 0, 144, 52, xBotaoPlay, yBotaoPlay, 0);
		  al_draw_bitmap_region(botaoOptions, 0, 0, 144, 52, xBotaoOptions, yBotaoOptions, 0);

		  menuPlay = false;
		  
		  
		  while(noMenu){
			//tocarSom("tema.ogg");
			//introducao = false;  
			ALLEGRO_EVENT ev;
			al_wait_for_event(event_queue, &ev);
			int foiP = 0;

			if(ev.type == ALLEGRO_EVENT_KEY_DOWN) { 

			  switch(ev.keyboard.keycode) {

				case(ALLEGRO_KEY_UP):
				  al_draw_bitmap_region(botaoPlay, 144, 0, 144, 52, xBotaoPlay, yBotaoPlay, 0); //Altera os sprites para que pareça que esteja selecionados
				  al_draw_bitmap_region(botaoOptions, 0, 0, 144, 52, xBotaoOptions, yBotaoOptions, 0);
				  continuar = false;
				  //jogoInicia = 1; 
				  break;

				  case(ALLEGRO_KEY_DOWN):
				  al_draw_bitmap_region(botaoPlay, 0, 0, 144, 52, xBotaoPlay, yBotaoPlay, 0);
				  al_draw_bitmap_region(botaoOptions, 144, 0, 144, 52, xBotaoOptions, yBotaoOptions, 0);
				  continuar = true;
				  playing = 0;
				  break;
				
				case( ALLEGRO_KEY_W):
				  al_draw_bitmap_region(botaoPlay, 144, 0, 144, 52, xBotaoPlay, yBotaoPlay, 0); 
				  al_draw_bitmap_region(botaoOptions, 0, 0, 144, 52, xBotaoOptions, yBotaoOptions, 0);
				  continuar = false;
				  //jogoInicia = 1; 
				  break;

				  case(ALLEGRO_KEY_S):
				  al_draw_bitmap_region(botaoPlay, 0, 0, 144, 52, xBotaoPlay, yBotaoPlay, 0);
				  al_draw_bitmap_region(botaoOptions, 144, 0, 144, 52, xBotaoOptions, yBotaoOptions, 0);
				  continuar = true;
				  playing = 0;
				  break;				  
				  
				  case(ALLEGRO_KEY_ENTER):

				  if(!continuar){
					  while(!foiP){
						al_draw_bitmap(tutorial, 0, 0, 0);
						al_flip_display();

						ALLEGRO_EVENT ev;
						al_wait_for_event(event_queue, &ev);	
						if(ev.type == ALLEGRO_EVENT_KEY_DOWN) { 
						  switch(ev.keyboard.keycode) { 

							case ALLEGRO_KEY_P:
							foiP = 1;
							noMenu = false;
							jogoInicia = 1; 
							break;
						  }
						}
					  }
					}else{	
					noMenu = false;
					printf("fiuk");
					//playing=0;
					//jogoInicia = 1;
                }
				  break;
				} 
			  }	  

			  al_flip_display();

			  if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) { //Verifica se o botão "X" da janela foi pressionado e fecha o jogo
				playing = 0;
				break;
			  }
			}
		}
		
		//Jogo rodando
			
		
		if(jogoInicia = 1){
			al_start_timer(timer2);
			
			//al_play_sample_instance(songInstance);
			
			desenhaCenario();
			
			atualizaBloco(&bloco);
			
			desenhaBloco(bloco);
			
			atualizaNave(&nave);

			mantemNaveNaTela(&nave);
			
			desenhaNave(nave);
			
			atualizaMeteoro(meteoro);
			
			desenhaMeteoro(meteoro);
			
			movimentaMeteoro(meteoro);
			
			atualizaTiro(tiros, NUM_TIROS);
			
			desenhaTiro(tiros, NUM_TIROS,ev.keyboard.keycode);	
			
			int vra = (int)(al_get_timer_count(timer2)/FPS);
			
			
			score(size_32, vra, &escore);
			
			
			//colissao(meteoro, nave, tiros, bloco);
			colisaoMeteoroBloco(bloco, meteoro);
			
			colisaoMeteoroMeteoro(meteoro);
			
			colisaoTiroBloco(tiros,bloco);
			
			colisaoTiro(tiros, NUM_TIROS, meteoro, &escore);	
			
			if(vra == 60){
				//printf("envis presnli");
				chefe = 1;
			}
			
			if(colisaoMeteoro(meteoro,bloco,nave)==1){
				
				al_play_sample(morte, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
				char texto_fim_1[20];
				char texto_fim_2[20];
				char texto_fim_3[20];
				int recorde = getRecorde();

				
				sprintf(texto_fim_1, "");
				sprintf(texto_fim_3, "Recorde %.2d", recorde);	
				if(escore > recorde){
					sprintf(texto_fim_1, "Novo recorde");
					sprintf(texto_fim_3, "");	
					setRecorde(escore);
				}
				
				sprintf(texto_fim_2, "Score %.2d", escore);	
				//colore toda a tela de preto
				al_clear_to_color(al_map_rgb(0,0,0));
			
				al_draw_text(size_32, al_map_rgb(255, 255, 255), SCREEN_W/2, (SCREEN_H/2)-140, ALLEGRO_ALIGN_CENTRE , texto_fim_1);
				al_draw_text(size_32, al_map_rgb(255, 255, 255), SCREEN_W/2, SCREEN_H/2-20, ALLEGRO_ALIGN_CENTRE , texto_fim_2);
				al_draw_text(size_32, al_map_rgb(255, 255, 255), SCREEN_W/2, SCREEN_H/2+100, ALLEGRO_ALIGN_CENTRE , texto_fim_3);
				
				//reinicializa a tela
				al_flip_display();	
				al_rest(3);		
				
				playing=0;
				/*noMenu=true;
				menuPlay = true;
				jogoInicia = 0;*/
			}
		}		
		
			if(chefe==1){
				desenhaChefao(chefao); 
				printaChefeVida(size_32, &vidaChefe);
				
				al_play_sample_instance(songInstance2);
				
				desenhaTiroC(tiroc, NUM_TIROS); 
				atualizaTiroC(tiroc, NUM_TIROS);		
				
				movimentaTiroC(tiroc); 
				
				colisaoTiroCMeteoro(tiroc, meteoro);

				
				if(colisaoTiroC(tiroc,nave,bloco)==1){
					printf("Falesceu!");
					al_play_sample(morte, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
				
					char texto_fim_1[20];
					char texto_fim_2[20];
					char texto_fim_3[20];
					int recorde = getRecorde();
					
					sprintf(texto_fim_1, "");
					sprintf(texto_fim_3, "Recorde %.2d", recorde);	
					if(escore > recorde){
						sprintf(texto_fim_1, "Novo recorde");
						sprintf(texto_fim_3, "");	
						setRecorde(escore);
					}
					
					sprintf(texto_fim_2, "Score %.2d", escore);	
					//colore toda a tela de preto
					al_clear_to_color(al_map_rgb(0,0,0));
				
					al_draw_text(size_32, al_map_rgb(255, 255, 255), SCREEN_W/2, (SCREEN_H/2)-140, ALLEGRO_ALIGN_CENTRE , texto_fim_1);
					al_draw_text(size_32, al_map_rgb(255, 255, 255), SCREEN_W/2, SCREEN_H/2-20, ALLEGRO_ALIGN_CENTRE , texto_fim_2);
					al_draw_text(size_32, al_map_rgb(255, 255, 255), SCREEN_W/2, SCREEN_H/2+100, ALLEGRO_ALIGN_CENTRE , texto_fim_3);
					
					//reinicializa a tela
					al_flip_display();	
					al_rest(3);						
					
					playing=0;
					/*noMenu=true;
					menuPlay = true;
					jogoInicia = 0;*/
				} 
				
				//colisaoTiroC(tiroc, bloco);
				//chefaoVida(chefao, &vidaChefe);
				int vra = (int)(al_get_timer_count(timer2)/FPS);
				if(chefaoVida(chefao,&vidaChefe,vra, &escore)==1){
					
					chefe = 0;
					char texto_fim_1[20];
					char texto_fim_2[20];
					char texto_fim_3[20];
					char texto_fim_4[20];
					int recorde = getRecorde();
					
					sprintf(texto_fim_1, "");
					sprintf(texto_fim_4, "VOCE VENCEU");
					sprintf(texto_fim_3, "Recorde %.2d", recorde);	
					escore+=10000;
					if(escore > recorde){
						sprintf(texto_fim_1, "Novo recorde");
						sprintf(texto_fim_3, "");	
						setRecorde(escore);
					}
					
					sprintf(texto_fim_2, "Score %.2d", escore);	
					//colore toda a tela de preto
					al_clear_to_color(al_map_rgb(0,0,0));
				
					al_draw_text(size_32, al_map_rgb(255, 255, 255), SCREEN_W/2, (SCREEN_H/2)-140, ALLEGRO_ALIGN_CENTRE , texto_fim_1);
					al_draw_text(size_32, al_map_rgb(255, 255, 255), SCREEN_W/2, (SCREEN_H/2)-180, ALLEGRO_ALIGN_CENTRE , texto_fim_4);
					al_draw_text(size_32, al_map_rgb(255, 255, 255), SCREEN_W/2, SCREEN_H/2-20, ALLEGRO_ALIGN_CENTRE , texto_fim_2);
					al_draw_text(size_32, al_map_rgb(255, 255, 255), SCREEN_W/2, SCREEN_H/2+100, ALLEGRO_ALIGN_CENTRE , texto_fim_3);
					
					//reinicializa a tela
					al_flip_display();	
					al_rest(3);						
					
					playing=0;
					/*noMenu=true;					
					menuPlay = true;
					jogoInicia = 0;*/
				}	
				
				if(colisaoChefao(chefao, nave, tiros, &vidaChefe)==1){
				printf("Falesceu!");
				al_play_sample(morte, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
				//printf(" Vida do chefao: %d ", vidaChefe);

				//printf(" Vida do chefao: %d ", &vidaChefe);
				char texto_fim_1[20];
				char texto_fim_2[20];
				char texto_fim_3[20];
				int recorde = getRecorde();

				
				sprintf(texto_fim_1, "");
				sprintf(texto_fim_3, "Recorde %.2d", recorde);	
				if(escore > recorde){
					sprintf(texto_fim_1, "Novo recorde");
					sprintf(texto_fim_3, "");	
					setRecorde(escore);
				}
				
				sprintf(texto_fim_2, "Score %.2d", escore);	
	
				al_clear_to_color(al_map_rgb(0,0,0));
			
				al_draw_text(size_32, al_map_rgb(255, 255, 255), SCREEN_W/2, (SCREEN_H/2)-140, ALLEGRO_ALIGN_CENTRE , texto_fim_1);
				al_draw_text(size_32, al_map_rgb(255, 255, 255), SCREEN_W/2, SCREEN_H/2-20, ALLEGRO_ALIGN_CENTRE , texto_fim_2);
				al_draw_text(size_32, al_map_rgb(255, 255, 255), SCREEN_W/2, SCREEN_H/2+100, ALLEGRO_ALIGN_CENTRE , texto_fim_3);
				
				//reinicializa a tela
				al_flip_display();	
				al_rest(3);		
				
				playing=0;
				/*noMenu=true;
				menuPlay = true;
				jogoInicia = 0;*/
				}
				//vidaChefao();
			}			
			
			
			//atualiza a tela (quando houver algo para mostrar)
			al_flip_display();

			if(al_get_timer_count(timer)%(int)FPS == 0){
				printf("\n%d segundos se passaram...", (int)(al_get_timer_count(timer)/FPS));
			}
				
		}
		//se o tipo de evento for o fechamento da tela (clique no x da janela)
		else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			playing = 0;
		}
		//se o tipo de evento for um pressionar de uma tecla
		else if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			
			switch(ev.keyboard.keycode) {
				
				case ALLEGRO_KEY_W:
						nave.dir_y--;
				break;	
				case ALLEGRO_KEY_S:
						nave.dir_y++;
				break;	
				case ALLEGRO_KEY_A:
					nave.dir_x--;
				break;	
				case ALLEGRO_KEY_D:
						nave.dir_x++;
				break;	
				case ALLEGRO_KEY_SPACE:
					//al_play_sample(shot, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
					atira(tiros, NUM_TIROS, nave, inicio_tiro, segurando_tecla,ev.keyboard.keycode, segurando_tecla, tiro_avancado_duracao);
						inicio_tiro = al_get_time();
						segurando_tecla = true;	
						if(segurando_tecla)
							printf("shambles");
						
						dispara = 0;
					
				break;				
			}
			
			//imprime qual tecla foi
			//printf("\ncodigo tecla: %d", ev.keyboard.keycode);
		}
		else if(ev.type == ALLEGRO_EVENT_KEY_UP) {
			
			switch(ev.keyboard.keycode) {
				
				case ALLEGRO_KEY_W:
					nave.dir_y++;
				break;	
				case ALLEGRO_KEY_S:
					nave.dir_y--;
				break;	
				case ALLEGRO_KEY_A:
					nave.dir_x++;
				break;	
				case ALLEGRO_KEY_D:
					nave.dir_x--;
				break;	
				case ALLEGRO_KEY_SPACE:
						segurando_tecla = false;	
						//duracao_tiro = al_get_time() - inicio_tiro;
						if(!segurando_tecla)
							printf("shambles2");
						
						dispara = 1;
						//tiro2(tiros, segurando_tecla, dispara);
					
				break;					
			}
		}
		
	} //fim do while
	
     
	//procedimentos de fim de jogo (fecha a tela, limpa a memoria, etc)
	al_destroy_sample(shot);
	al_destroy_sample(boom);
	al_destroy_sample(song);
	al_destroy_sample_instance(songInstance);	
	al_destroy_bitmap(tutorial);
	al_destroy_bitmap(image);
	al_destroy_bitmap(menu);
	al_destroy_bitmap(botaoPlay );
	al_destroy_bitmap(botaoOptions);
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
   
 
	return 0;
}
