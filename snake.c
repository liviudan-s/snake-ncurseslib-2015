/* Programarea Calculatoarelor, seria CC
 * Tema2 - Snake
 */
#include <stdio.h>
#include <string.h>
#include <curses.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <sys/select.h>

#define FOREVER				1
#define INIT_ROW 			15	
#define INIT_COL			15
#define SELECT_EVENT		1
#define SELECT_NO_EVENT		0
#define HALF_A_SECOND		500000

typedef struct {

int row,col;

}snake;

typedef struct {

char nume[20];
int scor;
}tabela;

/* curses.h este necesar pentru biblioteca grafică ncurses
 * ctype.h este necesar pentru funcția tolower - man tolower pentru detalii
 * Inițial, steluța se va afla pe ecran la coordonatele (INIT_ROW, INIT_COL) 
 */

int main(void)
{
	int row = INIT_ROW, col = INIT_COL, new_row, new_col;
	int nrows, ncols,i,j=1,recognized_key=1,obstacole=0,speed_sec=0,speed_microsec=HALF_A_SECOND / 4,speed_increase_ratio=0,scor=0;
	float speed_delay_vertical=1;
	snake snake_piece[15],aux1,aux2;
	tabela tabela_pozitie[10];
	int snake_size=5;
	char c;
	int nfds, sel;
	fd_set read_descriptors;
	struct timeval timeout;
	
	FILE *f;
	
	f=fopen("highscores","rw");
	for(i=0;i<10;i++)
	{
		fscanf(f,"%s",tabela_pozitie[i].nume);
		fscanf(f,"%d",&tabela_pozitie[i].scor);
	}

	nfds = 1;
	FD_ZERO(&read_descriptors);
	FD_SET(0, &read_descriptors);
	timeout.tv_sec = speed_sec;
	timeout.tv_usec = speed_microsec;
	
	srand(time(NULL));	

	/* Se inițializează ecranul; initscr se va folosi în mod obligatoriu */
	WINDOW *wnd = initscr();
	
	/* getmaxyx - este un macro, nu o funcție, așă că nu avem adresă la parametri */
	/* Se va reține în nrows și ncols numărul maxim de linii și coloane */
	getmaxyx(wnd, nrows, ncols);

	/* Se șterge ecranul */
	clear();
	/* Se inhibă afișarea caracterelor introduse de la tastatură */
	noecho();
	/* Caracterele introduse sunt citite imediat - fără 'buffering' */
	cbreak();

	/* Se ascunde cursorul */	
	curs_set(0);
	
	start_color();
	init_pair(1,COLOR_RED,COLOR_BLACK);
	init_pair(7,COLOR_BLUE,COLOR_BLACK);
	init_pair(2,COLOR_MAGENTA,COLOR_BLACK);
	init_pair(3,COLOR_CYAN,COLOR_BLACK);
	init_pair(4,COLOR_WHITE,COLOR_BLACK);
	init_pair(5,COLOR_YELLOW,COLOR_BLACK);
	init_pair(6,COLOR_GREEN,COLOR_BLACK);

	mvaddstr(10, 10, "Joc cu obstacole : 1 ");
	mvaddstr(12, 10, "Joc fara obstacole : Apasati orice tasta ");
	refresh();
		
	while(FOREVER)
	{
		c = getchar();			
		
		if(c == '1')
			obstacole=1;
	
		break;
	}
	
	clear();
	
	attron(COLOR_PAIR(2));
	/* Se va afișa un mesaj la poziția formată din primii doi parametri - (par1, par2) */	
	mvaddstr(0, 0, "Puteti sa controlati sarpele folosind tastele:");
	mvaddstr(1, 2, "A - stanga");
	mvaddstr(2, 2, "D - dreapta");
	mvaddstr(3, 2, "W - sus");
	mvaddstr(4, 2, "S - jos");
	mvaddstr(5, 0, "Pentru iesire, apasati tasta Q.");
	attroff(COLOR_PAIR(3));
	
	mvaddstr(22,8,"Scor : ");
	move(22,15);
	printw("%d",scor);
	
	for(i=7;i<=70;i++) // Generare chenar pe orizontala
	{
		move(6,i);
		addch('-' | COLOR_PAIR(6) );
		move(21,i);
		addch('-' | COLOR_PAIR(6) );
	}
	for(i=7;i<=20;i++) // Generare chenar pe verticala
	{
		move(i,7);
		addch('|' | COLOR_PAIR(5) );
		move(i,70);
		addch('|' | COLOR_PAIR(5) );
	}
	
	snake_piece[1].row=row;	
	snake_piece[1].col=col;
		
	/* Se mută cursorul la poziția (row, col) */ 
	move(row, col);
	/* Se adaugă la poziția indicată de cursor caracterul '%' */
	addch('%');
	
	for(i=2;i<=snake_size;i++)
	{
		snake_piece[i].col=col-i+1;
		snake_piece[i].row=row;
		move(snake_piece[i].row,snake_piece[i].col);
		addch('=' |  COLOR_PAIR(3) );	
	}
	
	while(j)	
	{		
		aux1.row=rand() % 21 ;
		aux1.col=rand() % 70 ;
		if(aux1.row<7)
			aux1.row+=7;
		if(aux1.col<8)
			aux1.col+=8;
		if( ( mvinch(aux1.row,aux1.col) & A_CHARTEXT ) == ' ')
			j=0;	
	}
	j=1;
	move(aux1.row,aux1.col);
	addch('$' | COLOR_PAIR(7));

	if(obstacole)
	{		
		i=10;	
		while(i--)		
		{		
			while(j)
			{
				aux1.row=rand() % 21 ;
				aux1.col=rand() % 70 ;
				if(aux1.row<7)
					aux1.row+=7;
				if(aux1.col<8)
					aux1.col+=8;
				if( ( mvinch(aux1.row,aux1.col) & A_CHARTEXT ) == ' ')
					j=0;
			}
			j=1;
			move(aux1.row,aux1.col);
			addch('#' | COLOR_PAIR(1));
		}	
	}
	
	/* Se reflectă schimbările pe ecran */
	refresh();

	/* Rămânem în while până când se primește tasta q */
	while (FOREVER) {
		
		sel = select(nfds, &read_descriptors, NULL, NULL, &timeout);
	
		if (sel == SELECT_EVENT)
		{		
		
			c = getchar();

			if (tolower(c) == 'q') {
				break;
			}
	
			/* Se determină noua poziție, în funcție de tasta apăsată
			 * Nu putem depași nrows și ncols, sau linia 0/coloana 0.
			 */
			speed_delay_vertical=1;			
		
			switch (tolower(c)) {
			case 'a':
				new_row = row;
				if (col > 0)
					new_col = col - 1;
				break;
		
			case 'd':
				new_row = row;
				if (col + 1 < ncols)
					new_col = col + 1;
				break;
		
			case 'w':
				if (row > 0)
					new_row = row - 1;
				new_col = col;
				speed_delay_vertical=1.2;
				break;

			case 's':
				if (row + 1 < nrows)
					new_row = row + 1;
				new_col = col;
				speed_delay_vertical=1.2;
				break;
			default: recognized_key=0;
			}
		}	
		else if(sel == SELECT_NO_EVENT)
			{
				if(snake_piece[1].row == snake_piece[2].row)
				{					
					if(snake_piece[1].col > snake_piece[2].col)
					{ // case 'd'
						new_row = row;
						if (col + 1 < ncols)
							new_col = col + 1;
					}
					else { // case 'a'
							new_row = row;
							if (col > 0)
								new_col = col - 1;						
							}
				speed_delay_vertical=1;				
				}
				if(snake_piece[1].col == snake_piece[2].col)
				{
					//if(speed_delay_vertical==4)
					//{					
						if(snake_piece[1].row > snake_piece[2].row)				
						{ // case 's'
							if (row + 1 < nrows)
								new_row = row + 1;
							new_col = col;
						}
						else { // case 'w'
								if (row > 0)
									new_row = row - 1;
								new_col = col;
							  }
					speed_delay_vertical=1.4;
					//}
					//else speed_delay_vertical++;
					
				}
			}
	
		if(recognized_key)
		{
			
			if( ( mvinch(new_row, new_col) & A_CHARTEXT ) != ' ' && ( mvinch(new_row, new_col) & A_CHARTEXT ) != '$') // Se verifica daca in pozitia noua exista un obstacol
			{
				clear();
				mvaddstr(10,10,"Highscores:");
				for(i=0;scor<tabela_pozitie[i].scor;i++);
				
				for(j=0;j<=i;j++)
					tabela_pozitie[9-j]=tabela_pozitie[9-j-1];
				tabela_pozitie[i].scor=scor;
				for(j=0;j<10;j++)				
					mvprintw(10+j+1,10,"%d. %s    %d",j+1,tabela_pozitie[j].nume,tabela_pozitie[j].scor);				
				if(i==9)				
					move(10+i+1,10+4);
				else move(10+i+1,10+3);				
				getstr(tabela_pozitie[i].nume);
				for(i=0;i<10;i++)
				{
					fprintf(f,"%s",tabela_pozitie[i].nume);
					fprintf(f," %d",tabela_pozitie[i].scor);
				}

			}	
			
			/* Se șterge '%' din vechea poziție */
	//		move(row, col);
	//		addch(' ');

			if( ( mvinch(new_row, new_col) & A_CHARTEXT ) == '$')
			{
				scor++;

				mvaddstr(22,8,"Scor : ");
				move(22,15);
				printw("%d",scor);
				
				snake_size++;
				snake_piece[snake_size].row=snake_piece[snake_size-1].row-1;			
				snake_piece[snake_size].col=snake_piece[snake_size-1].col;
				while(j)	
				{		
					aux1.row=rand() % 21 ;
					aux1.col=rand() % 70 ;
					if(aux1.row<7)
						aux1.row+=7;
					if(aux1.col<8)
					aux1.col+=8;
					if( ( mvinch(aux1.row,aux1.col) & A_CHARTEXT ) == ' ')
					j=0;	
				}
				j=1;
				move(aux1.row,aux1.col);
				addch('$' | COLOR_PAIR(7) );
				speed_increase_ratio++;				
				if( speed_increase_ratio % 2 == 0 && speed_increase_ratio != 0)
				{
				if(speed_sec % 2 != 0 )
					j=0;
				speed_sec /= 2;
				speed_microsec /= 2;
				if(j==0)
					speed_microsec += HALF_A_SECOND;
				j=1;
				}
			}
					
			/* Se adaugă '%' în noua poziție */
			snake_piece[1].row=new_row;
			snake_piece[1].col=new_col;		
			move(new_row, new_col);
			addch('%');			
			
			aux2.row=row;
			aux2.col=col;			

			for(i=2;i<=snake_size;i++)
			{

				aux1=snake_piece[i];
				snake_piece[i]=aux2;						
				move(snake_piece[i].row,snake_piece[i].col);
				addch('=' | COLOR_PAIR(3) );
				aux2=aux1;
			}
	
			move(aux2.row,aux2.col);
			addch(' ');

			refresh();

			row = new_row;
			col = new_col;
		}

		recognized_key=1;

		FD_SET(0, &read_descriptors);
		timeout.tv_sec = speed_sec;
		timeout.tv_usec = speed_microsec * speed_delay_vertical;
	
	}

	/* Se închide fereastra ncurses */	
	endwin();

	return 0;
}
