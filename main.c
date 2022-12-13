#include <SDL2/SDL.h>
#include <unistd.h>
#include "function.h"

#define WINDOW_WIDTH 700
#define WINDOW_HEIGHT 920
#define FPS 60

#define LIGNE 6
#define COL 10

#define ECARTA 20
#define ECARTO 20

#define LONGEUR 47
#define HAUTEUR 20

#define R 5


// pad
int pos_pad_x = 700/2-70;
int pos_pad_y = 900;
int speedx = 35;
int longeur_pad = 140;
int Drawpaddel();

// bloc
int Drawbloc();

int posbX;
int posbY;
int inittab();
int L = LIGNE;


// ball
int Drawballe();
double speedballx ;
double speedbally ;
int posballx = 350;
int posbally = 880;

// Colision
int Drawcol();
int DetecCol();

int life = 3;

// win
int win();


//	Reset
int reset();
int reset_game  = 0;

//****Structure**brique****//
struct brick
{
	int posbx;
	int posby;
	int posbx2;
	int posby2;
	int alive;
	int brick;
	
};

struct brick level[LIGNE][COL];

int Game_Over();
int game_over;
// Score
int score = 0;

void drawGame(){
    /* Ici je dessine mon jeu
     * exemple position x, y modifiés dans KeyPressed() et utilisés pour
     * pouvoir deplacer la figure à chaque boucle de gameLoop()
     */
    clear();
    
    reset();
    Game_Over();
    Drawbloc();
    Drawballe();
    Drawpaddel();
    Drawcol();
    DetecCol();
    win();

    actualize();
    usleep(1000000 / FPS); // 60 images par seconde | 1000000 = 1 seconde
}
void KeyPressed(SDL_Keycode touche){
    /** @brief event.key.keysym.sym renvoi la touche appuyé
     *
     */
    switch (touche) {
        // Voir doc SDL_Keycode pour plus de touches https://wiki.libsdl.org/SDL_Keycode
        // ici exemple ou appuyer sur les touches Q | D modifie x (position du carré dans drawGame())
        
        case SDLK_q:						/*	utlisation de la touche q	*/
            //touche q appuyé
            printf("Q\n");
            if(pos_pad_x > 10)				/*	on déplace le paddel vers la gauche	*/
            pos_pad_x = pos_pad_x - speedx;
            break;
        
        case SDLK_d:						/*	utlisation de la touche d	*/
            //touche d appuyé
            printf("D\n");
            if(pos_pad_x <= 530)			/*	on déplace le paddel vers la droite	*/
            pos_pad_x = pos_pad_x + speedx;
            break;

        case SDLK_SPACE:
            //touche espace appuyé
            printf("Espace\n");
            
 			speedballx = 0;
			speedbally = -2;
			game_over = 0;  

			break;

		case SDLK_r:
			//touche R reset  
			printf("Reset\n");
			reset_game = 1;



            break;

        case SDLK_p:
            //touche P appuyé
            printf("P\n");

			speedballx = 2;
			speedbally = -2;

            break;
            			
        case SDLK_ESCAPE:
            freeAndTerminate();
            break;
        default:
            break;
    }
}
void joyButtonPressed(){
}

void gameLoop() {
    int programLaunched = 1;
    while (programLaunched == 1) {
        // Boucle pour garder le programme ouvert
        // lorsque programLaunched est different de 1
        // on sort de la boucle, donc de la fonction
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            // boucle pour la gestion d'évenement
            switch (event.type) {
                /** event.type renvoi le type d'evenement qui se passe
                 * (si on appuie sur une touche, clique de souris...)
                 * en fonction du type d'évènement on à alors
                 * différentes information accessibles
                 * voir doc pour plus d'event https://wiki.libsdl.org/SDL_EventType
                 */
                case SDL_QUIT:
                    // quand on clique sur fermer la fénêtre en haut à droite
                    programLaunched = 0;
                    break;
                case SDL_MOUSEBUTTONUP:
                    /* clique de la souris
                     * event.motion.y | event.motion.x pour les positions de la souris
                     */
                    printf("position de la souris x : %d , y : %d\n", event.motion.x, event.motion.y);
                    break;
                case SDL_KEYDOWN:
                    KeyPressed(event.key.keysym.sym);
                    break;
                case SDL_JOYBUTTONDOWN:
                    break;
                default:
                    break;
            }
        }
        /* coder ici pour que le code s'execute après chaque évenement
         * exemple dessiner un carré avec position int x, int y qu'on peut
         * deplacer lorsqu'on appuie sur une touche
         */
        
        drawGame();
        
    }
}

int main(){
    /** @description 3 fonctions dans le main qui permettent de créer l'application et la maintenir ouverte :
     *  init(...) : initialiser la SDL/ fenêtre
     *  gameLoop() : boucle de jeu dans laquelle l'application reste ouverte
     *  freeAndTerminate() : quitte le programme proprement
     */
	reset();
    init(WINDOW_WIDTH, WINDOW_HEIGHT);

    game_over = 1;

    Game_Over();
    DetecCol();
    inittab();
    

    gameLoop();

    printf("Fin du programme\n");
    freeAndTerminate();
}

int reset()
{
	if(reset_game == 1)
	{
		clear();
		actualize();
		
	}
}

/*		paddel		*/	/*	ici on crée le paddel	*/

int Drawpaddel(){ 
	
	changeColor( 112, 255, 144 );			/*	couleur du paddel	*/
	drawRect(pos_pad_x, pos_pad_y, longeur_pad, 10);/*	on crée le rectangle avec la fonction drawRect	*/
											/*	le pad bouge avec pos_pad qui modifié avec les touches 88 & 95	*/	

}

/*		Balle		*/ /*	ici on crée la balle	*/
int Drawballe(){

	
	posballx = posballx + speedballx;		/*	initialisation du mouvement de la balle	*/
	posbally = posbally + speedbally;		/*	on ajoute la vitesse de la balle à la position de la balle	*/
	
	changeColor( 255, posbally, 255 );		/*	couleur de la balle */
	drawCircle(posballx, posbally, R);		/*	on crée la structure de la balle avec la fonction drawCircle	*/
	
}

//******brique*********//
int inittab(){

	int i, j;

	 int posbx = ECARTA;
	 int posby = ECARTO;
	

		for( i = 0; i < L; i++){
			for( j = 0; j < COL; j++){

				level[i][j].alive = 2;
				level[i][j].posbx = posbx;
				level[i][j].posby = posby;
				level[i][j].posbx2 = posbx + LONGEUR;
				level[i][j].posby2 = posby + HAUTEUR;
				level[i][j].brick = level[i][j].alive;
				
			
			posbx = posbx + LONGEUR + ECARTA;

			}

		posbx = ECARTA;
		posby = posby + HAUTEUR + ECARTO ;

		}
}

int Drawbloc(){

	changeColor(0,51,102);
	int i, j;
	//DetecCol();
		
	
	for( i = 0; i < L; i++){
		for( j = 0; j < COL; j++){
			if(level[i][j].alive >= 1){
				
			changeColor(level[i][j].posbx,level[i][j].posby,200);

				if (level[i][j].alive == 1)
				{
					changeColor(255, posballx, 0);
				}
			drawRect(level[i][j].posbx,level[i][j].posby, LONGEUR, HAUTEUR);
			level[i][j].brick++;
			}	
		}
	}
}



//*******Game**Over*******//
int Game_Over(){

	if( posbally > 920 ){
		printf("sorti de zone\n");

		life--;
		posballx = 350;
		posbally = 880;
		speedballx = 0;
		speedbally = 0;
		game_over = 1;
	}

	if (life == 0)
	{
		sprite(WINDOW_WIDTH/2-211, 410, "image/gameover.bmp");
	}
}



//*******Colision**mur**&**pad*****//

int Drawcol(){
		
	Drawballe();
	
	//**Mur**//
	if( posballx + speedballx < R | posballx + speedballx > 690){
		speedballx = -speedballx;
	} 
		
	if( posbally + speedbally < R){
		speedbally = -speedbally;
	}
	

	
	//**Pad**//
	if( posbally + R >= pos_pad_y && 
		posbally - R <= pos_pad_y + 10 &&
		posballx + R >= pos_pad_x && 
		posballx - R <= pos_pad_x + 140){
			
		


			if(pos_pad_x <= posballx || posballx >= pos_pad_x + 10){
				printf("position extreme gauche\n");
				speedbally = -5;
				speedballx = -2;
			}
			
			if(pos_pad_x + 11 <= posballx || posballx >= pos_pad_x + 65){
				printf("position gauche\n");
				speedbally = -2;
				speedballx = -2;
				
			}
			
			if(pos_pad_x + 66 <= posballx || posballx >= pos_pad_x + 75){
				printf("position centre\n");
				speedballx = 0;
				speedbally = -2; 
				
			}
			
			if(pos_pad_x + 76 <= posballx || posballx >= pos_pad_x + 129){
				printf("position droite\n");
				speedbally = -2;
				speedballx = 2;
				
			}
				
			if(pos_pad_x + 130 <= posballx  || posballx >= pos_pad_x + 140){
				printf("position extreme droite\n");
				speedbally = -5;
				speedballx = 2;
				
				
			}
			
			//speedbally = -speedbally;	
		

		
	}
}

//********Detec**Collision******//


int DetecCol(){
	
	int i, j;

	for( i = 0; i < L; i++){
		for( j = 0; j < COL; j++){
					
				
					if  ((posballx + R >= level[i][j].posbx ) && 
						 (posballx - R <= level[i][j].posbx2) &&
						 (posbally + R >= level[i][j].posby ) && 
						 (posbally - R <= level[i][j].posby2)){

						if( level[i][j].alive >= 1 ){

							if (level[i][j].posbx == posballx || level[i][j].posbx2 == posballx )
							{
								speedballx = -4;
							}
							if (level[i][j].posby == posballx || level[i][j].posby2 == posballx )
							{
								speedballx = -4;
							}

							speedbally = -speedbally;
							printf("brique\n");
							level[i][j].alive--;
							
						
						}
					}
				
		}

	}	
}

/*		win 		*/
int win(){
	
	

	for( int i = 0; i < L; i++){
		for( int j = 0; j < COL; j++){

			if (level[i][j].brick == 0)
			{
				printf("win\n");
			}
		}
	}
}

