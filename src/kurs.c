/*
 * kurs.c
 *
 *  Created on: 12 нояб. 2018 г.
 *      Author: FED
 */

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <Windows.h>
#include <string.h>
#include <time.h>

/*
 * макроподстановки
 */
#define SHIFR 8
#define KOD 3
#define FIO 16
#define TIME 5
#define K_LINE 19
#define STR_1 "выход"
#define STR_2 "назад"
#define TAB printf("%14c",' ');
#define TAB_G 25
#define WIDTH_M 28
#define WIDTH_M_2 30
#define BACKGROUND 7
#define BACKGROUND_M 8
#define BACKGROUND_M_P 11//14//7
#define TEXT_COLOR 0
#define TEXT_COLOR_M_P 5 //5
#define TEXT_COLOR_M 0
#define BACKGROUND_PRINT 8
#define BACKGROUND_PRINT_P 7
#define TEXT_COLOR_PRINT_P 5
/*
 * структуры
 */
struct inf{
	int num,shif,kod,t_ob,t_cp;
	char fio[FIO];
};
struct line{
	struct inf data;
	struct line *next_left,*next_right;
};
struct inf_2{
	struct inf ish;
	int perc;
};
struct line_2{
	struct inf_2 data;
	struct line_2 *next_left,*next_right;
};

/*
 * Глобальные переменные
 */
HANDLE h_stdin;
DWORD fdwSaveOldMode;
/*
 * прототипы
 */
void processing();
int menu(char item[][29], int k, int num_m);   //функция основного меню
void print_menu_inside(char *str,int k, int x,int y);  //печать выбранного пункта меню
void print_menu_outside(char *str,int k, int x,int y);  //печать печать пункта меню
VOID ErrorExit(LPSTR); //обработка ошибки подключения режимов
void print_menu_inside_l2(char *str, int k);  //печать выбранного пункта меню нижнего уровня
void print_menu_outside_l2(char *str,int k); // печать пункта меню нижнего уровня
void gotoxy(int x,int y);  //позиционирование в окне консоли
int menu_level_2(char item[][29], int k, int num_m, int x, int y); //меню нижнего уровня
void cls(int background);   //очистка экрана
void window(int x, int y, int width, int hight, int background); //печать прямоугольного блока
int mouse_event_processing_menu(char item[][29],int k, int *n, COORD pos); //обработка событий мыши в меню
int key_event_processing_menu(char item[][29], int k, int *n, int key);  //обработка события нажатия клавиши
int mouse_event_processing_menu_l2(char item[][29],int k, int *n, COORD pos, COORD pos0);  //обработкасобытия мыши в меню нижнего уровня
int key_event_processing_menu_l2(char item[][29], int k, int *n, int key, COORD pos);  //обработка события нажатия клавиши в меню нижнего уровня
int vvod(struct inf *zap);  //ввод данных с клавиатуры
int get_int(char *str, int k);  //получение int с клавиатуры
int get_mas(char *str,char *buf, int k);  //получение строки с клавиатуры
struct line *dob_1(struct line *right,struct line **left, struct inf z);//добавление одного элемента в конец
void org_line(struct line **left, struct line **right);  //организация списка
void vivod_zap(struct line *el);  //вывод одной записи
int form_save_load(char *str, char *filename);  //форма для сохранения/загрузки
int read_bin(struct line **left,struct line **right);  //считывание из бинарного файла
int save_bin(struct line *temp); //сохранение в бинарный файл
int save_text(struct line *temp);  //сохранение в текстовый файл
int read_text(struct line **left,struct line **right);  //считывание из текстового файла
void save_load(struct line **left, struct line **right); //сохранение/загрузка
void print_str(char *str, int x, int y, int background, int text_color);  //печать строки по координатам с цветом
void del(struct line **left);  //удаление списка
void del_key_fio(struct line **left,struct line **right);  //удаление 1го элемента по фамилии
void sort_fio(struct line *left);  //сортировка по фамилии
void sort_shif(struct line *left);  //сортировка по шифру
void sort_kod(struct line *left);  //сортировка по коду
void sort_t_ob(struct line *left);  //сортировка по общему времени
void sort_t_cp(struct line *left);  //сортировка по времени центрального процессора
void desort(struct line *left);  //обратная сортировка
void show_cap_print();  //печать шапки таблицы
void print(struct line **left, struct line **right);  //просмотр списка - скроллинг
void print_el_inside(struct line *el,int k,HANDLE h);  //печать выделенной записи
void print_el_outside(struct line *el, int k);  //печать записи
void show_page_table(struct line *top, int k);  //вывод записей на экран
void manipulation(struct line *el, struct line **left, struct line **right); //манипуляция с выбранным элементом
void change(struct line *el); //корректировка
void show_cap_change();  //печать шапки для корректировки
void del_record(struct line *el, struct line **left, struct line **right);  //удаление эл-та из списка
struct line *exist(struct line *temp); //поиск по фамилии
void show_exist(struct line *left);  //вывод результата поиска
struct line_2 *dob_1_rez(struct line_2 *right,struct line_2 **left, struct inf_2 z); //добавление одного элемента в обработку
struct inf_2 create_inf(struct inf z);  //расчет обработки
void create_line_2(struct line_2 **left,struct line_2 **right,struct line *temp); //создание базы обработки
void vivod_zap_rez(struct line_2 *el);  //вывод записи списка обработки
void del_line_2(struct line_2 **left);  //удаление списка обработки
void print_line_2(struct line_2 *left, struct line_2 *right); //просмотр списка обработки
void rezult(struct line_2 **left, struct line_2 **right, struct line *lleft);  //обработка нажатия пункта обработки
int save_rezult_text(struct line_2 *left); //сохранение обработки в файл
int cat();  //вывод котика:)
void welcome();  //приветствие
int enquiry_exit();  //запрос на выход
void help();  //справка
void error_warning_ok(char *str);  //вывод сообщения с ОК
int error_warning_yes_no(char *str);  //вывод сообщения с ДА НЕТ
/*
 * Основная функция
 */
int main()
{
	DWORD  fdwMode;
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(h, (WORD) ((BACKGROUND << 4) | TEXT_COLOR));
	system("chcp 1251>nul");
	char title[]="Разработано dfgwh";
	CharToOem(title,title);
	SetConsoleTitle(title);
	system("mode con cols=80 lines=25");
	h_stdin = GetStdHandle(STD_INPUT_HANDLE);
	/*if(h_stdin== INVALID_HANDLE_VALUE)
		ErrorExit("GetStdHandle");*/
	/*if(!*/GetConsoleMode(h_stdin, &fdwSaveOldMode);/*)*/
		/*ErrorExit("GetConsoleMode");*/
	fdwMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
	/*if(!*/SetConsoleMode(h_stdin, fdwMode)/*)
		ErrorExit("SetConsoleMode")*/;
	cat();
	welcome();
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD) ((BACKGROUND << 4) | TEXT_COLOR));
	processing();
	SetConsoleMode(h_stdin, fdwSaveOldMode);
	return 0;
}
void processing()
{
	struct line *left=0, *right=0;
	struct line_2 *left_2=0, *right_2=0;
	char item[][29]={
					 "Создание/Добавление",
					 "Загрузка/сохранение",
					 "Удаление/Сортирвка/Поиск",
					 "Просмотр beta",
					 "Обработка"
					};
	char item1[][29]={
					 "Создание новой базы",
					 "Добавление записи в конец"
	};
	char item2[][29]={
					 "Удаление по Ф.И.О.",
					 "Сортировка",
					 "Обратная сортировка",
					 "Поиск по Ф.И.О."
	};
	char item3[][29]={
					 "сорт. по шифру",
					 "сорт. по коду",
					 "сорт. по Ф.И.О.",
					 "сорт. по общему времени",
					 "сорт. по времени ЦП"
	};
	int num_m=0,k=5;
  	while(1)
  	{
  		switch(menu(item,k,num_m))
  		{
  			case -5: help(); break;
  			case 1:
  				switch(menu_level_2(item1,2,0,40,3+3*0+(20-3*k)/2))
  				{
  				case 1:org_line(&left,&right);num_m=0;break;
  				case 2:
  					cls(BACKGROUND);
  					num_m=0;
  					printf("*==============================================================================*");
  					printf("                   добавление одной записи в конец списка\n");
  					printf("*==============================================================================*");
  					struct inf *zap=(struct inf*)malloc(sizeof(struct inf));
  					if (vvod(zap)==-1)
  					{
  						free(zap);
  						break;
  					}
  					right=dob_1(right,&left,*zap);
  					free(zap);
  					//MessageBox(0,"Новая запись добавленна","",MB_OK|MB_TOPMOST|MB_TASKMODAL);
  					error_warning_ok("Новая запись добавленна");
  					break;
  				}
  				num_m=0;
  				break;
  			case 2:
  				save_load(&left,&right);num_m=1; break;
  			case 3:
  				num_m=2;
  				if(!left)
  				{
  					//MessageBox(0,"База данных пуста","",MB_OK|MB_ICONWARNING|MB_TOPMOST|MB_TASKMODAL);
  					error_warning_ok("База данных пуста");
  	  				break;
  				}
  				switch(menu_level_2(item2,4,0,40,3+3*2+(20-3*k)/2))
  				{
  				case 1:
  					del_key_fio(&left,&right);
  					//MessageBox(0,"Элемент удален","",MB_OK|MB_TOPMOST|MB_TASKMODAL);
  					error_warning_ok("Элемент удален");
  					break;
  				case 2:
  	  				if (left->next_right)
  	  					switch(menu_level_2(item3,5,0,50,5+3*2+(20-3*k)/2))
  	  					{
  	  					case 1:sort_shif(left);break;
  	  					case 2:sort_kod(left);break;
  	  					case 3:sort_fio(left);break;
  	  					case 4:sort_t_ob(left);break;
  	  					case 5:sort_t_cp(left);break;
  	  					}
  	  					//sort_fio(left);
  	  				//MessageBox(0,"Список отсортирован","",MB_OK|MB_TOPMOST|MB_TASKMODAL);
  	  				error_warning_ok("Список отсортирован");
  	  				break;
  				case 3:
  	   				if (left->next_right)
  	  					desort(left);
  	   				//MessageBox(0,"Произведена обратная сортировка","",MB_OK|MB_TOPMOST|MB_TASKMODAL);
  	   				error_warning_ok("Список отсортирован");
  	  			  	break;
  				case 4:
  					show_exist(left);
  					break;
  				}
  				break;
  			case 4:
			  	print(&left,&right);
			  	num_m=3;
				break;
  			case 5:
  				if(!left)
  				{
  					//MessageBox(0,"База данных пуста\nОбработка невозможна","",MB_OK|MB_ICONWARNING|MB_TOPMOST|MB_TASKMODAL);
  					error_warning_ok("База данных пуста|Обработка невозможна");
  					num_m=4;
  	  				break;
  				}
  				else rezult(&left_2,&right_2,left);
  				num_m=4;
  				break;
  			case 0:
  				cls(0);
  				cat();
  				if(!enquiry_exit())
  				{
  					num_m=0;
  					break;
  				}
  				else
  				{
  	  				del(&left);
  	  				del_line_2(&left_2);
  	  				return;
  				}
  		}
  	}
}
/*
 *
 * =================================================================
 * ================    Функции для работы с меню    ================
 * =================================================================
 *
 */
/*
 *  Меню - первый уровень
 *  Выводит пункты меню размером 3*WIDTH_M
 *  Сдвигает меню по горизонтали на TAB_G
 *  Вертикально устанавливает - верхний край по формуле (20-3*k)/2,
 *  где k - количество пунктов меню
 *  item[][29] - массив строк пунктов меню
 *  num_m - номер первоначально - выбранного пункта
 */
int menu(char item[][29], int k, int num_m)
{
	int i;
	char key;
	int n=num_m+1,tab_v=(20-3*k)/2;
    DWORD cNumRead, j;
    COORD pos;
    INPUT_RECORD irInBuf[128];
	cls(BACKGROUND);
	print_str("<-",2,1,15,12);
	print_str("СПРАВКА",70,23,15,12);
	print_str("(f1/fn+f1)",69,24,BACKGROUND,TEXT_COLOR);
	for (i=0;i<k;i++)
	{
		gotoxy(TAB_G,tab_v+3*i+1);print_menu_outside(item[i],WIDTH_M,TAB_G,tab_v+3*i+1);
	}
	gotoxy(TAB_G,tab_v+(n-1)*3+1); print_menu_inside(item[num_m],WIDTH_M,TAB_G,tab_v+(n-1)*3+1);
	while(1)
	{
		key=-1;
        if (! ReadConsoleInput(
                h_stdin,      // input buffer handle
                irInBuf,     // buffer to read into
                128,         // size of read buffer
                &cNumRead) ) // number of records read
            ErrorExit("ReadConsoleInput");
		for (j = 0; j < cNumRead; j++)
		{
		    if (irInBuf[j].EventType==MOUSE_EVENT)
		    {
		    	if (irInBuf[j].Event.MouseEvent.dwEventFlags==DOUBLE_CLICK)
		    	{
		    		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), irInBuf[j].Event.MouseEvent.dwMousePosition);
		    		pos=irInBuf[j].Event.MouseEvent.dwMousePosition;
		    		if (pos.X>=TAB_G&&pos.Y>=(tab_v+n)&&pos.X<=(TAB_G+WIDTH_M)&&pos.Y<=(tab_v+(n+1)*k))
		    		{
		    			key=13;
		    			break;
		    		}
		    		else continue;
		    	}
		    	if (!irInBuf[j].Event.MouseEvent.dwEventFlags)
		        {
		            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), irInBuf[j].Event.MouseEvent.dwMousePosition);
		            pos=irInBuf[j].Event.MouseEvent.dwMousePosition;
		            key=mouse_event_processing_menu(item,k,&n,pos);
		            if (key==-1) continue;
		            break;
		        }
		    }
		    if (irInBuf[j].EventType==KEY_EVENT)
		    {
		    	if(irInBuf[j].Event.KeyEvent.bKeyDown)
		    	{
		    		key=key_event_processing_menu(item,k,&n,irInBuf[j].Event.KeyEvent.wVirtualKeyCode);
		    		if (key==-1) continue;
		    		break;
		    	}
		    	continue;
		    }
		}
		if (key==-5) return -5;
		if (key==13) return n;
		if (key==0) return 0;
	}
	return 0;
}
VOID ErrorExit (LPSTR lpszMessage)
{
    fprintf(stderr, "%s\n", lpszMessage);
    SetConsoleMode(h_stdin, fdwSaveOldMode);
    ExitProcess(0);
}
/*
 * печать выделенного пункта меню - первого уровня
 * *str - строка с пунктом меню
 * k - ширина пункта меню
 * hConsole - ескриптор консоли
 */
void print_menu_inside(char *str, int k, int x, int y)
{
	int o;
	o=(k-strlen(str))/2;
          //изменение цвета текста и фона
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD) ((BACKGROUND_M_P << 4) | TEXT_COLOR_M_P));
	printf("|%*c|",k,' ');
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD) ((BACKGROUND_M << 4) | TEXT_COLOR_M));
	gotoxy(x,y+1);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD) ((BACKGROUND_M_P << 4) | TEXT_COLOR_M_P));
	printf("|%*c",o,' ');
	printf("%-*s|",k-o,str);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD) ((BACKGROUND_M << 4) | TEXT_COLOR_M));
	gotoxy(x,y+2);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD) ((BACKGROUND_M_P << 4) | TEXT_COLOR_M_P));
	printf("|%*c|",k,' ');
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD) ((BACKGROUND << 4) | TEXT_COLOR_M));
}
/*
 * печать пункта меню первого уровня (не выделенного)
 * *str - строка с пунктом меню
 * k - ширина пункта меню
 */
void print_menu_outside(char *str,int k, int x, int y)
{
	int o;
	o=(k-strlen(str))/2;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD) ((BACKGROUND_M << 4) | TEXT_COLOR_M));
	printf("|%*c|",k,' ');
	gotoxy(x,y+1);
	printf("|%*c",o,' ');
	printf("%-*s|",k-o,str);
	gotoxy(x,y+2);
	printf("|%*c|",k,' ');
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD) ((BACKGROUND << 4) | TEXT_COLOR_M));
}
/*
 * печать выделенного пункта меню - второго уровня
 * *str - строка с пунктом меню
 * k - ширина пункта меню
 * hConsole - ескриптор консоли
 */
void print_menu_inside_l2(char *str, int k)
{
	int o;
	o=(k-strlen(str))/2;
          //изменение цвета текста и фона
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD) ((BACKGROUND_M_P << 4) | TEXT_COLOR_M_P));
	printf("|");
	printf("%*c",o,' ');
	printf("%-*s|",k-o,str);
   //восстановление старых настроек
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD) ((BACKGROUND << 4) | TEXT_COLOR_M));
}
/*
 * печать пункта меню второго уровня (не выделенного)
 * *str - строка с пунктом меню
 * k - ширина пункта меню
 */
void print_menu_outside_l2(char *str,int k)
{
	int o;
	o=(k-strlen(str))/2;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD) ((BACKGROUND_M << 4) | TEXT_COLOR_M));
	printf("|");
	printf("%*c",o,' ');
	printf("%-*s|",k-o,str);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD) ((BACKGROUND << 4) | TEXT_COLOR_M));
}
/*
 *  Меню уровень 2
 *  item[][29] - массив пунктов меню
 *  k - количество пунктов меню
 *  num_m - номер начального пункта меню
 *  x y - координаты
 */
int menu_level_2(char item[][29], int k, int num_m, int x, int y)
{
	int i;
	char key;
	int n=num_m+1;
    DWORD cNumRead, j;
	x-=5;
	y++;
	int f=0;
    COORD pos,pos0={x,y};
    INPUT_RECORD irInBuf[128];
	//HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE); //дескриптор стандартного вывода консоли
	for (i=0;i<k;i++)
	{
		gotoxy(x,y+i);print_menu_outside_l2(item[i],30);
	}
	gotoxy(x,y); print_menu_inside_l2(item[num_m],30);
	while(1)
	{
		key=-2;
        if (! ReadConsoleInput(
                h_stdin,      // input buffer handle
                irInBuf,     // buffer to read into
                128,         // size of read buffer
                &cNumRead) ) // number of records read
            ErrorExit("ReadConsoleInput");
		for (j = 0; j < cNumRead; j++)
		{
		    if (irInBuf[j].EventType==MOUSE_EVENT)
		    {
		    	//if (!f){f++; continue;}
		    	if (irInBuf[j].Event.MouseEvent.dwEventFlags==DOUBLE_CLICK)
		    	{
		    		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), irInBuf[j].Event.MouseEvent.dwMousePosition);
		    		pos=irInBuf[j].Event.MouseEvent.dwMousePosition;
		    		if (pos.X>=x&&pos.Y>=y&&pos.X<=(x+WIDTH_M_2)&&pos.Y<=(y+(n-1)*k))
		    		{
		    			key=13;
		    			break;
		    		}
		    		else if (key==-1)
		    		{
		    			key=0;
		    			break;
		    		}
		    		else continue;
		    	}
		    	if (!irInBuf[j].Event.MouseEvent.dwEventFlags)
		        {
		            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), irInBuf[j].Event.MouseEvent.dwMousePosition);
		            pos=irInBuf[j].Event.MouseEvent.dwMousePosition;
		            //printf("button press %i %i\n", pos.X,pos.Y);
		            key=mouse_event_processing_menu_l2(item,k,&n,pos,pos0);
		            if (f&&key==-1) key=0;
		            else if (!f) {f++; continue;}
		            break;
		        }
		    }
		    if (irInBuf[j].EventType==KEY_EVENT)
		    {
		    	if(irInBuf[j].Event.KeyEvent.bKeyDown)
		    	{
		    		pos=irInBuf[j].Event.MouseEvent.dwMousePosition;
		    		key=key_event_processing_menu_l2(item,k,&n,irInBuf[j].Event.KeyEvent.wVirtualKeyCode,pos0);
		    		if (key==-1)
		    			continue;
		    		break;
		    	}
		    	continue;
		    }
		}
		if (key==-5) return -5;
		if (key==13) return n;
		if (key==0) return 0;
	}
}
/*
 *  Обработка события клика мышки
 *  item[][29] - массив строк пунктов меню
 *  k - количество пунктов меню
 *  *n - номер выбранного пункта меню
 *  pos - координаты в которых произошел щелчек мыши
 */
int mouse_event_processing_menu(char item[][29],int k, int *n, COORD pos)
{
	int decition=-1, tab_v=(20-3*k)/2, i, hight_p=3;
	if (pos.X>=70&&pos.X<=76&&pos.Y==23) return -5;
	if (pos.X<=4&&pos.Y<=2) return 0;
	else if (pos.X>=TAB_G&&pos.Y>=(tab_v+1)&&pos.X<=(TAB_G+WIDTH_M+1)&&pos.Y<=(tab_v+hight_p*k))
	{
		for(i=0;i<k;i++)
			if (pos.X>=TAB_G&&pos.Y>(tab_v+hight_p*i)&&pos.X<=(TAB_G+WIDTH_M+1)&&pos.Y<=(tab_v+hight_p*(i+1)))
				decition=i+1;
	}
	else return decition;
	gotoxy(TAB_G,tab_v+3*(*n)-2); print_menu_outside(item[*n-1],WIDTH_M,TAB_G,tab_v+3*(*n)-2);
	(*n)=decition;
	gotoxy(TAB_G,tab_v+3*(*n)-2); print_menu_inside(item[*n-1],WIDTH_M,TAB_G,tab_v+3*(*n)-2);
	return decition;
}
/*
 *  Обработка события нажатия клавиши
 *  item[][29] - массив строк пунктов меню
 *  k - количество пунктов меню
 *  *n - номер выбранного пункта меню
 *  key - виртуальный код нажатой клавиши
 */
int key_event_processing_menu(char item[][29], int k, int *n, int key)
{
	int tab_v=(20-3*k)/2;
	switch(key)
	{
	case 40://80:
		gotoxy(TAB_G,tab_v+3*(*n)-2);print_menu_outside(item[(*n)-1],WIDTH_M,TAB_G,tab_v+3*(*n)-2);
		(*n)++;
		if ((*n)>k) (*n)=1;
		gotoxy(TAB_G,tab_v+3*(*n)-2);print_menu_inside(item[(*n)-1],WIDTH_M,TAB_G,tab_v+3*(*n)-2);
		return (*n);
	case 38://72:
		gotoxy(TAB_G,tab_v+3*(*n)-2);print_menu_outside(item[(*n)-1],WIDTH_M,TAB_G,tab_v+3*(*n)-2);
		(*n)--;
		if ((*n)<1) (*n)=k;
		gotoxy(TAB_G,tab_v+3*(*n)-2);print_menu_inside(item[(*n)-1],WIDTH_M,TAB_G,tab_v+3*(*n)-2);
		return (*n);
	case 13: case 39: return (13);
	case 27: return 0;
	case 112:case 91: return -5;
	default: return -1;
	}
}
/*
 *  Обработка события клика мыши в меню 2го уровня
 *  item[][29] - массив строк пунктов меню
 *  k - количество пунктов меню
 *  *n - номер выбранного пункта меню
 *  pos - координаты в которых инициализировано событие
 *  pos0 - координаты левого верхнего угла меню второго уровня
 */
int mouse_event_processing_menu_l2(char item[][29],int k, int *n, COORD pos, COORD pos0)
{
	int decition=-1, i;
	//printf("%i %i\n%i %i",pos0.X,pos0.Y,pos0.X+WIDTH_M_2,pos0.Y+(*n)*k-1);
	//getch();
	if (pos.X>=70&&pos.X<=76&&pos.Y==23) return -5;
	if (pos.X<=4&&pos.Y<=2) return 0;
	else if (pos.X>=pos0.X && pos.Y>=(pos0.Y) && pos.X<=(pos0.X+WIDTH_M_2) && pos.Y<=(pos0.Y+(*n)*k-1))
	{
		for(i=0;i<k;i++)
			if (pos.X>=pos0.X&&pos.Y>=(pos0.Y+i)&&pos.X<=(pos0.X+WIDTH_M_2)&&pos.Y<=(pos0.Y+(i)))
			{
				decition=i+1;
				break;
			}
	}
	else return decition;
	gotoxy(pos0.X,pos0.Y+(*n)-1); print_menu_outside_l2(item[(*n)-1],30);
	(*n)=decition;
	gotoxy(pos0.X,pos0.Y+(*n)-1); print_menu_inside_l2(item[(*n)-1],30);
	return decition;
}
/*
 *  Обработка события нажатия клавиши в меню 2го уровня
 *  item[][29] - массив строк пунктов меню
 *  k - количество пунктов меню
 *  *n - номер выбранного пункта меню
 *  key - виртуальный код нажатой клавиши
 *  pos - координаты левого верхнего угла вызванного меню 2го уровня
 */
int key_event_processing_menu_l2(char item[][29], int k, int *n, int key, COORD pos)
{
	switch(key)
	{
	case 40://80:
		gotoxy(pos.X,pos.Y+(*n)-1);print_menu_outside_l2(item[(*n)-1],30);
		(*n)++;
		if ((*n)>k) (*n)=1;
		gotoxy(pos.X,pos.Y+(*n)-1);print_menu_inside_l2(item[(*n)-1],30);
		return (*n);
	case 38://72:
		gotoxy(pos.X,pos.Y+(*n)-1);print_menu_outside_l2(item[(*n)-1],30);
		(*n)--;
		if ((*n)<1) (*n)=k;
		gotoxy(pos.X,pos.Y+(*n)-1);print_menu_inside_l2(item[(*n)-1],30);
		return (*n);
	case 13:case 39: return 13;
	case 27:case 37: return 0;
	case 112:case 91: return -5;
	default: return -1;
	}
}
/*
 *
 */
void gotoxy(int x,int y)
{

	COORD position={x,y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),position);
}
void show()
{
    DWORD cNumRead, j;
    INPUT_RECORD irInBuf[128];
    COORD pos;
    while (1)
    {
        // Wait for the events.
        if (! ReadConsoleInput(
                h_stdin,      // input buffer handle
                irInBuf,     // buffer to read into
                128,         // size of read buffer
                &cNumRead) ) // number of records read
            ErrorExit("ReadConsoleInput");
		for (j = 0; j < cNumRead; j++)
		{
		    if (irInBuf[j].EventType==MOUSE_EVENT)
		    	if (!irInBuf[j].Event.MouseEvent.dwEventFlags)
		        {
		            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), irInBuf[j].Event.MouseEvent.dwMousePosition);
		            pos=irInBuf[j].Event.MouseEvent.dwMousePosition;
		            printf("button press %i %i\n", pos.X,pos.Y);
		            if (pos.X<4&&pos.Y<4)
		                return;

		        }
		}
    }
}
void window(int x, int y, int width, int hight, int background)
{
	HANDLE h=GetStdHandle(STD_OUTPUT_HANDLE);
	WORD m;
	int i;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (GetConsoleScreenBufferInfo(h,&csbi))
		m=csbi.wAttributes;
	for(i=0;i<(hight);i++)
	{
		gotoxy(x,y+i);
		SetConsoleTextAttribute(h, (WORD) ((background << 4) | 0));
		printf("%*c",width,' ');
		SetConsoleTextAttribute(h, m);
	}
}
void cls(int background)
{
	window(0,0,80,25,background);
	COORD pos={0,0};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);
}
/*
 * заполнение записи
 */
int vvod(struct inf *zap)
{
	struct inf z;
	fflush(stdin);
	gotoxy(20,10);
	z.shif=get_int("шифр(8 цифр): ",SHIFR);
	if (z.shif==-1) return -1;
	gotoxy(20,11);
	z.kod=get_int("код(3 цифры): ",KOD);
	if (z.kod==-1) return -1;
	fflush(stdin);
	gotoxy(20,12);
	if (get_mas("Ф.И.О.(<= 15 символов): ",z.fio,15)==-1) return -1;
	fflush(stdin);
	while(1)
	{
		print_str("                                                             ",20,13,BACKGROUND,TEXT_COLOR);
		print_str("                                                             ",20,14,BACKGROUND,TEXT_COLOR);
		gotoxy(20,13);
		if ((z.t_ob=get_int("общее время(<= 5 цифр): ",EOF))==-1) return -1;
		gotoxy(20,14);
		if ((z.t_cp=get_int("время ЦП(<= 5 цифр): ",EOF))==-1) return -1;
		if (z.t_ob>z.t_cp) break;
		print_str("общее время должно быть больше времени центрального процессора",10,20,10,0);
	}
	*zap=z;
	return 0;
}
/*
 * получение int
 * *str - строка, которая просит что-то ввести
 * k - требуемое количество вводимых символов
 * если k<0 то требует ввести <= TIME
 */
int get_int(char *str, int k)
{
	int key=0,i=0,j=0;
	printf(str);
	fflush(stdin);
	if (k>0)
	{
		while(j<k||key!=13)
		{
			key=getch();
			if (key==27)
			{
				if(MessageBox(0,"Прервать ввод?","",MB_YESNO|MB_TOPMOST|MB_TASKMODAL|MB_DEFBUTTON1)==IDNO)
					continue;
				else
					return -1;
			}
			if (key==8)
			{
				if (j<1)
					continue;
				j--;
				i/=10;
				printf("\b  \b%c",8);
			}
			else if (j<k&&key>='0'&&key<='9')
			{
				printf("%c",key);
				i=i*10+(key-'0');
				j++;
			}
		}
	}
	else
	{
		while((key=getch())!=13)
		{
			if (key==27)
			{
				if(MessageBox(0,"Прервать ввод?","",MB_YESNO|MB_TOPMOST|MB_TASKMODAL|MB_DEFBUTTON1)==IDNO)
					continue;
				else
					return -1;
			}
			if (key==8)
			{
				if (j<1)
					continue;
				j--;
				i/=10;
				printf("\b  \b%c",8);
				continue;
			}
			else if (j<TIME&&key>='0'&&key<='9')
			{
				printf("%c",key);
				i=i*10+(key-'0');
				j++;
			}
		}
	}
	printf("\n");
	fflush(stdin);
	return i;
}
/*
 * получение строки
 * записывает в *buf
 * *str - строка, которая просит что-то ввести
 * k - максимальная длина получаемой строки
 */
int get_mas(char *str,char *buf, int k)
{
	int i=0;
	printf(str);
	buf[0]=0;
	while((buf[i]=getch())!=13)
	{
		if (buf[i]==27)
		{
			if(MessageBox(0,"Прервать ввод?","",MB_YESNO|MB_TOPMOST|MB_TASKMODAL|MB_DEFBUTTON1)==IDNO)
				continue;
			else
				return -1;
		}
		if (buf[i]==8||buf[i]==75)
		{
			if (i<1) continue;
			i--;
			printf("\b \b");
			continue;
		}
		if(i<k)
		{
			printf("%c",buf[i]);
			i++;
		}
	}
	buf[i]='\0';
	return 0;
}
/*
 * проверка строки на наличие лишних символов
 * если встречается что-то кроме чисел возвращает -1
 * иначе 0
 */
int prov(char *buf)
{
	int i;
	for(i=0;buf[i]!='\0';i++)
	{
		if (buf[i]<'0'||buf[i]>'9') //'0':48 '9':57
			return -1;
	}
	return 0;
}
/*
 * добавление в конец
 */
struct line *dob_1(struct line *right,struct line **left, struct inf z)
{
	struct line *el=(struct line*)malloc(sizeof(struct line));
	el->data=z;
	el->next_right=0;
	if (!right)
	{
		el->data.num=1;
		el->next_left=0;
		right=el;
		*left=el;
		return right;
	}
	el->data.num=(right->data.num)+1;
	right->next_right=el;
	el->next_left=right;
	right=el;
	return right;
}
/*
 * организация списка
 */
void org_line(struct line **left, struct line **right)
{
	struct line *el=(struct line*)malloc(sizeof(struct line));
	cls(BACKGROUND);
	printf("*==============================================================================*");
	printf("                             организация списка\n");
	printf("*==============================================================================*");
	struct inf z;
	if (vvod(&z)==-1)
		return;
	el->data=z;
	el->data.num=1;
	el->next_right=0;
	el->next_left=0;
	del(left);
	*right=0;
	*left=el;
	*right=el;
	while(1)
	{
		if(!error_warning_yes_no("Добавить новый элемент?"))
			break;
		else
		{
			cls(BACKGROUND);
			printf("*==============================================================================*");
			printf("                             организация списка\n");
			printf("*==============================================================================*");
			if (vvod(&z)==-1)
				return;
			*right=dob_1(*right,left,z);
		}
	}
}
/*
 * вывод записи
 * без перехода на новую строку!!!
 */
void vivod_zap(struct line *el)
{
	printf("%8i|%5i|%15s|%13i|%31i|",el->data.shif,el->data.kod,el->data.fio,el->data.t_ob,el->data.t_cp);
}
/*
 * форма для сохранения / считывания
 */
int form_save_load(char *str, char *filename)
{
	int maxline=256;
	int i, tab_v=4, tab_g=15;
	cls(BACKGROUND);
	window(tab_g-1,tab_v,52,14,1);
	window(tab_g,tab_v+1,50,3,BACKGROUND_M_P);
	window(tab_g,tab_v+4,50,9,1);
	window(tab_g+9,tab_v+5,39,7,15);
	print_str(str,tab_g+1,tab_v+2,BACKGROUND_M_P,TEXT_COLOR_M);
	print_str("Путь",tab_g+1,tab_v+5,1,15);
	print_str("к файлу",tab_g+1,tab_v+6,1,15);
	int k=tab_v+5;
	gotoxy(tab_g+10,k);
	i=0;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD) ((15 << 4) | 0));
	while((filename[i]=getch())!=13)
	{
		if (filename[i]==27)
		{
			if(MessageBox(0,"Прервать ввод?","",MB_YESNO|MB_DEFBUTTON1|MB_TOPMOST|MB_TASKMODAL)==IDNO)
				continue;
			else
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD) ((BACKGROUND << 4) | TEXT_COLOR));
				return -1;
			}
		}
		if (filename[i]==8)
		{
			filename[i]='\0';
			if (i<1) continue;
			i--;
			if(!((i+1)%38)) gotoxy(tab_g+10+38,--k);
			printf("\b \b");
		}
		else if(i<(maxline-1))
		{
			printf("%c",filename[i]);
			i++;
			if(i==38||((!(((i+1)-39)%38))&&i>39)) gotoxy(tab_g+10,++k);
		}
	}
	filename[i]='\0';
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD) ((BACKGROUND << 4) | TEXT_COLOR));
	return 0;
}
/*
 * считывание из бинарного файла
 */
int read_bin(struct line **left,struct line **right)
{
	int n;
	FILE *f;
	struct inf z;
	char *filename=(char*)malloc(sizeof(char)*256);
	if (form_save_load("           загрузка из бинарного файла           ",filename)==-1)
		return -1;
	f=fopen(filename,"rb");
	if (!f) return 1;
	del(left);
	*right=0;
	while(1)
	{
		n=fread(&z,sizeof(struct inf),1,f);
		if (n<1) break;
		*right=dob_1(*right,left,z);
	}
	gotoxy(0,22); TAB printf("список был загружен из файла: %s\n",filename);
	getch();
    fclose(f);
    free(filename);
	return 0;
}
/*
 * сохранение в бинарного файл
 */
int save_bin(struct line *temp)
{
	FILE *f;
	char *filename=(char*)malloc(sizeof(char)*256);
	if (form_save_load("           сохранение в бинарный файл            ",filename)==-1)
		return -1;
	f=fopen(filename,"wb");
	if (!f) return 1;
    while(temp)
    {
    	fwrite(&temp->data,sizeof(struct inf),1,f);
    	temp=temp->next_right;
    }
    gotoxy(0,22); TAB printf("список сохранен: %s\n",filename);
    getch();
    fclose(f);
    free(filename);
    return 0;
}
/*
 * сохранение в текстовый файл
 */
int save_text(struct line *temp)
{
	FILE *f;
	int maxline=256;
	char *filename=(char*)malloc(sizeof(char)*maxline);
	if (form_save_load("           сохранение в текстовый файл           ",filename)==-1)
		return -1;
	f=fopen(filename,"wt");
	if (!f) return 1;
	fputs("*----------------------------------------*\n",f);
	fputs("|        |   |               |   время   |\n",f);
	fputs("|  шифр  |код|     Ф.И.О.    |-----------|\n",f);
	fputs("|        |   |               |общее| ц.п.|\n",f);
	fputs("|----------------------------------------|",f);
    while(temp)
    {
    	fprintf(f,"\n|%8i|%3i|%15s|%5i|%5i|",temp->data.shif,temp->data.kod,
    			temp->data.fio,temp->data.t_ob,temp->data.t_cp);
    	temp=temp->next_right;
    }
    gotoxy(0,22); TAB printf("список сохранен: %s\n",filename);
    getch();
    fclose(f);
    free(filename);
    return 0;
}
/*
 * считывание из текстового файла
 */
int read_text(struct line **left,struct line **right)
{
	FILE *f;
	struct inf z;
	char *filename=(char*)malloc(sizeof(char)*256),str[50],buf[16];
	int i,j,n,k;
	if (form_save_load("           загрузка из текстового файла          ",filename)==-1)
		return -1;
	for(i=0;filename[i]!='\0'&&filename[i]!='.';i++);
	if (filename[i]=='.'&&filename[i+1]=='d') return 1;
	f=fopen(filename,"rt");
	if (!f) return 1;
	del(left);
	*right=0;
	fgets(str,44,f);
	fgets(str,44,f);
	fgets(str,44,f);
	fgets(str,44,f);
	fgets(str,44,f);
	while(!feof(f))
	{
		fflush(stdin);
		fgets(str,44,f);
		buf[0]='\0';
			for(i=1,j=0;str[i]!='|';j++,i++)
				buf[j]=str[i];
			buf[j]='\0';
			z.shif=atoi(buf);
			buf[0]='\0';
			i++;
			for(j=0;str[i]!='|';j++,i++)
				buf[j]=str[i];
			buf[j]='\0';
			z.kod=atoi(buf);
			buf[0]='\0';
			i++;
			for(j=0;str[i]!='|';j++,i++)
				buf[j]=str[i];
			buf[j]='\0';
			for(k=0,n=0;k<16;k++,n++)
			{
				if (buf[k]==' ')
				{	for(j=k;j<15;j++)
						buf[j]=buf[j+1];
					k--;
				}
				else break;
			}
			strcpy(z.fio,buf);
			buf[0]='\0';
			i++;
			for(j=0;str[i]!='|';j++,i++)
				buf[j]=str[i];
			buf[j]='\0';
			z.t_ob=atoi(buf);
			buf[0]='\0';
			i++;
			for(j=0;str[i]!='|';j++,i++)
				buf[j]=str[i];
			buf[j]='\0';
			z.t_cp=atoi(buf);
		*right=dob_1(*right,left,z);
	}
	gotoxy(0,22); TAB gotoxy(16,22); printf("список был загружен из файла: %s\n",filename);
	getch();
    fclose(f);
    free(filename);
	return 0;
}
/*
 * сохранение и загрузка
 */
void save_load(struct line **left, struct line **right)
{
	char item_f[][29]={
					 "Сохранение в бинарный файл",
					 "Загрузка из бинарного файла",
					 "Сохранение в текстовый файл",
					 "Загрузка из текcтового файла"
					};
	int num_m=0,k=4;
		switch(menu_level_2(item_f,k,num_m,40,3+3*1+(20-3*k)/2-2))
		{
		case -5: help(); break;
		case 1:
			if(!*left)
			{
				//MessageBox(0,"База данных пуста","",MB_OK|MB_ICONWARNING|MB_TOPMOST|MB_TASKMODAL);
				error_warning_ok("База данных пуста");
				break;
			}
			if (save_bin(*left)==1)
			{
				//MessageBox(0,"Error","",MB_OK|MB_ICONWARNING|MB_TOPMOST|MB_TASKMODAL);
				error_warning_ok("Ошибка доступа");
				break;
			}
			break;
		case 2:
			if (read_bin(left,right)==1)
			{
				//MessageBox(0,"Error","",MB_OK|MB_ICONWARNING|MB_TOPMOST|MB_TASKMODAL);
				error_warning_ok("Ошибка доступа");
				break;
			}
			break;
		case 3:
			if(!*left)
			{
				//MessageBox(0,"База данных пуста","",MB_OK|MB_ICONWARNING|MB_TOPMOST|MB_TASKMODAL);
				error_warning_ok("База данных пуста");
				break;
			}
			if (save_text(*left)==1)
			{
				//MessageBox(0,"Error","",MB_OK|MB_ICONWARNING|MB_TOPMOST|MB_TASKMODAL);
				error_warning_ok("Ошибка доступа");
				break;
			}
			break;
		case 4:
			if (read_text(left,right)==1)
			{
				//MessageBox(0,"error","",MB_OK|MB_ICONWARNING|MB_TOPMOST|MB_TASKMODAL);
				error_warning_ok("Ошибка доступа");
				break;
			}
			break;
		case 0: return;
		}
}
/*
 * вывод строки
 * *str - строка
 * x y - координаты
 * h - дескриптор
 * background - фон
 * text_color - текст
 */
void print_str(char *str, int x, int y, int background, int text_color)
{
	HANDLE h=GetStdHandle(STD_OUTPUT_HANDLE);
	WORD m;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (GetConsoleScreenBufferInfo(h,&csbi))
		m=csbi.wAttributes;
	gotoxy(x,y);
	SetConsoleTextAttribute(h, (WORD) ((background << 4) | text_color));
	printf("%s",str);
	SetConsoleTextAttribute(h, m);
}
/*
 *	очистка памяти
 * сдвигает укаатель начала списка на
 * следующий элемент пока не 0, прошлый
 * элемент удаляется
 */
void del(struct line **left)
{
	struct line *temp;
	while (*left)
	{
		temp=*left;
		*left=(*left)->next_right;
		free(temp);
	}
	//return left;
}
/*
 * удаление по ключевому полю fio
 */
void del_key_fio(struct line **left,struct line **right)
{
	char key[FIO];
	struct line *temp,*del;
	cls(BACKGROUND);
	gotoxy(0,0); printf("*==============================================================================*");
	gotoxy(0,1); printf("                     удаление 1 записи по полю Ф.И.О\n");
	gotoxy(0,2); printf("*==============================================================================*");
	gotoxy(0,3); get_mas("Ф.И.О.: ",key,15);
	if (strcmp((*left)->data.fio,key)==0)
	{
		temp=*left;
		if (*left==*right) *right=(*right)->next_right;
		*left=(*left)->next_right;
		free(temp);
		if (left) (*left)->next_left=0;
		return;
	}
	if (strcmp((*right)->data.fio,key)==0)
	{
		temp=*right;
		*right=(*right)->next_left;
		free(temp);
		(*right)->next_right=0;
		return;
	}
	temp=*left;
	while(temp->next_right)
	{
		if (strcmp(temp->next_right->data.fio,key)==0)
		{
			del=temp->next_right;
			temp->next_right=temp->next_right->next_right;
			temp->next_right->next_left=temp;
			free(del);
			return;
		}
		temp=temp->next_right;
	}
	//MessageBox(0,"Такой записи нет!","",MB_OK|MB_ICONWARNING|MB_TOPMOST|MB_TASKMODAL);
	error_warning_ok("Такой записи нет!");
}
/*
 * сортировка по полю Ф.И.О.
 */
void sort_fio(struct line *left)
{
	struct line *border=left,*temp,*temp1;
	struct inf z;
	while(border->next_right)
	{
		temp1=border;
		temp=temp1->next_right;
		while (temp)
		{
			if (strcmp(temp->data.fio,temp1->data.fio)<0)
				temp1=temp;
			temp=temp->next_right;
		}
		z=border->data;
		border->data=temp1->data;
		temp1->data=z;
		border=border->next_right;
	}
}
void sort_shif(struct line *left)
{
	struct line *border=left,*temp,*temp1;
	struct inf z;
	while(border->next_right)
	{
		temp1=border;
		temp=temp1->next_right;
		while (temp)
		{
			if (temp->data.shif<temp1->data.shif)
				temp1=temp;
			temp=temp->next_right;
		}
		z=border->data;
		border->data=temp1->data;
		temp1->data=z;
		border=border->next_right;
	}
}
void sort_kod(struct line *left)
{
	struct line *border=left,*temp,*temp1;
	struct inf z;
	while(border->next_right)
	{
		temp1=border;
		temp=temp1->next_right;
		while (temp)
		{
			if (temp->data.kod<temp1->data.kod)
				temp1=temp;
			temp=temp->next_right;
		}
		z=border->data;
		border->data=temp1->data;
		temp1->data=z;
		border=border->next_right;
	}
}
void sort_t_ob(struct line *left)
{
	struct line *border=left,*temp,*temp1;
	struct inf z;
	while(border->next_right)
	{
		temp1=border;
		temp=temp1->next_right;
		while (temp)
		{
			if (temp->data.t_ob<temp1->data.t_ob)
				temp1=temp;
			temp=temp->next_right;
		}
		z=border->data;
		border->data=temp1->data;
		temp1->data=z;
		border=border->next_right;
	}
}
void sort_t_cp(struct line *left)
{
	struct line *border=left,*temp,*temp1;
	struct inf z;
	while(border->next_right)
	{
		temp1=border;
		temp=temp1->next_right;
		while (temp)
		{
			if (temp->data.t_cp<temp1->data.t_cp)
				temp1=temp;
			temp=temp->next_right;
		}
		z=border->data;
		border->data=temp1->data;
		temp1->data=z;
		border=border->next_right;
	}
}
/*
 * сортировка по номеру записи
 */
void desort(struct line *left)
{
	struct line *border=left,*temp,*temp1;
	struct inf z;
	while(border->next_right)
	{
		temp1=border;
		temp=temp1->next_right;
		while (temp)
		{
			if (temp->data.num<temp1->data.num)
				temp1=temp;
			temp=temp->next_right;
		}
		z=border->data;
		border->data=temp1->data;
		temp1->data=z;
		border=border->next_right;
	}
}
void show_cap_print()
{
	gotoxy(0,0); printf("*==============================================================================*");
	gotoxy(0,1); printf(" esc - назад                          <-^->                       enter - выбор ");
	gotoxy(0,2); printf("*==============================================================================*");
	gotoxy(0,3); printf("     шифр  | код |    Ф.И.О.     | общее время |  время центрального процессора ");
	gotoxy(0,4); printf(" ------------------------------------------------------------------------------ ");
}
/*
 * просмотр в интерактивном режиме, скролинг
 * принимает адрес указателей на начало и конец списка
 * при нажатие на enter имеется возможность изменить или удалить
 * запись
 */
void print(struct line **left, struct line **right)
{
	int i,k=0,key=-1;
	struct line *res=*left,*temp=*left,*top=*left;
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE); //дескриптор стандартного вывода консоли
	SetConsoleTextAttribute(h, (WORD) ((BACKGROUND_PRINT << 4) | TEXT_COLOR));
	if (*left)
	{
		error_warning_ok("Управление осуществляется|стрелочками, enter, esc|pg up, pg dn");
		cls(BACKGROUND_PRINT);
		show_cap_print();
		show_page_table(*left,K_LINE);
		print_el_inside(*left,k,h);
		//MessageBox(0,"Управление осуществляется\nстрелочками, \"enter\", \"esc\"\n\"pg up\", \"pg dn\"","",MB_OK|MB_ICONWARNING|MB_TOPMOST|MB_TASKMODAL);
	}
	while (1)
	{
		if (!*left)
		{
			//MessageBox(0,"База данных пуста","",MB_OK|MB_TOPMOST|MB_TASKMODAL);
			error_warning_ok("База данных пуста");
			return;
		}
		key=getch();
		if (key==80)  //стрелка вниз
		{
			k++;
			if (res->next_right)
			{
				res=res->next_right;
				if (k<K_LINE)
				{
					print_el_outside(res->next_left,k-1);
					print_el_inside(res,k,h);
				}
				else
				{
					top=top->next_right;
					show_page_table(top,K_LINE);
					print_el_inside(res,k-1,h);
					k--;
				}
			}
			else k--;
			continue;
		}
		else if (key==72)  //стрелка вверх
		{
			k--;
			if (res->next_left)
			{
				res=res->next_left;
				if (k>=0)
				{
					print_el_outside(res->next_right,k+1);
					print_el_inside(res,k,h);
				}
				else
				{
					top=top->next_left;
					show_page_table(top,K_LINE);
					print_el_inside(res,k+1,h);
					k++;
				}
			}
			else k++;
			continue;
		}
		else if (key==81||key==77)   //вправо
		{
			temp=top;
			for(i=0;(i<K_LINE)&&(top->next_right);i++,top=top->next_right);
			if (i!=K_LINE)
			{
				top=temp;
				continue;
			}
			k=0;
			res=top;
			cls(BACKGROUND_PRINT);
			show_cap_print();
			show_page_table(top,K_LINE);
			print_el_inside(res,k,h);
			continue;
		}
		else if ((key==73||key==75)&&top!=*left)     //влево
		{

			for(i=0;(i<K_LINE)&&(top->next_left);i++,top=top->next_left);
			k=0;
			res=top;
			cls(BACKGROUND_PRINT);
			show_cap_print();
			show_page_table(top,K_LINE);
			print_el_inside(res,k,h);
			continue;
		}
		else if (key==13)     //enter
		{
			manipulation(res,left,right);
			top=*left;
			res=top;
			k=0;
			SetConsoleTextAttribute(h, (WORD) ((BACKGROUND_PRINT << 4) | TEXT_COLOR));
			cls(BACKGROUND_PRINT);
			show_cap_print();
			show_page_table(top,K_LINE);
			if (res) print_el_inside(res,k,h);
		}
		else if (key==27)
		{
			SetConsoleTextAttribute(h, (WORD) ((BACKGROUND << 4) | TEXT_COLOR));
			return;    //сделать выход
		}
	}
}
/*
 * вывод на экран строки списка,
 * проверка на выбранный элемент,
 * если соответствует выбранному элементу, то
 * строка окрашивается в бирюзовый цвет
 */

void print_el_inside(struct line *el,int k,HANDLE h)
{
	gotoxy(0,k+5);
	SetConsoleTextAttribute(h, (WORD) ((BACKGROUND_PRINT_P << 4) | TEXT_COLOR_PRINT_P));
	printf(" *|");
	vivod_zap(el);
	SetConsoleTextAttribute(h, (WORD) ((BACKGROUND_PRINT << 4) | TEXT_COLOR));
}
void print_el_outside(struct line *el,int k)
{
	gotoxy(0,k+5);
	printf("  |");
	vivod_zap(el);
}
void show_page_table(struct line *top, int k)
{
	int i;
	//gotoxy(0,5);
	for(i=0;top&&i<k;i++)
	{
		gotoxy(0,5+i);
		printf("  |"); vivod_zap(top);
		top=top->next_right;
	}
}
/*
 * вызов меню:
 *  изменить
 *  удалить
 */
void manipulation(struct line *el, struct line **left, struct line **right)
{
	char item[][29]={
						"изменить",
						"удалить",
					};
	while(1)
	{
		switch(menu(item,2,0))
		{
			case -5: help(); break;
			case 1:
				if ((*left))
					change(el);
				else
				{
					//MessageBox(0,"База данных пуста","",MB_OK|MB_ICONWARNING|MB_TOPMOST|MB_TASKMODAL);
					error_warning_ok("База данных пуста");
				}
				break;
			case 2:
				if ((*left))
					del_record(el,left,right);
				else
				{
					//MessageBox(0,"База данных пуста","",MB_OK|MB_ICONWARNING|MB_TOPMOST|MB_TASKMODAL);
					error_warning_ok("База данных пуста");
				}
				return;
			case 0:return;
		}
	}
}
/*
 * изменение полей выбранного элемента
 * списка,
 * на выбор предлагается изменить каждое поле, или всю
 * запись сразу
 */
void change(struct line *el)
{
	char item[][29]={
				  		"полностью",
					    "шифр",
					    "код",
					    "Ф.И.О.",
					    "общее время",
					    "время центр. процессора"
					};
	int num_m=0;
	struct inf z;
	while (1)
	{
		switch(menu(item,6,num_m))
		{
			case -5: help(); break;
			case 1:
				show_cap_change();
				vivod_zap(el);
				if (vvod(&z)==-1)
					break;
				z.num=el->data.num;
				el->data=z;
				num_m=0;
				break;
			case 2:
				show_cap_change();
				vivod_zap(el);
				gotoxy(20,12);
				z.shif=get_int("шифр(8 цифр): ",SHIFR);
				if (z.shif==-1) break;
				el->data.shif=z.shif;
				num_m=1;
				break;
			case 3:
				show_cap_change();
				vivod_zap(el);
				gotoxy(20,12);
				z.kod=get_int("код(3 цифры): ",KOD);
				if (z.kod==-1) break;
				el->data.kod=z.kod;
				num_m=2;
				break;
			case 4:
				show_cap_change();
				vivod_zap(el);
				gotoxy(20,12);
				if (get_mas("Ф.И.О.(<= 15 символов): ",z.fio,15)==-1)
					break;
				el->data.fio[0]='\0';
				strcpy(el->data.fio,z.fio);
				num_m=3;
				break;
			case 5:
				show_cap_change();
				vivod_zap(el);
				gotoxy(20,12);
				z.t_ob=get_int("общее время(<= 5 цифр): ",EOF);
				if (z.t_ob==-1) break;
				el->data.t_ob=z.t_ob;
				num_m=4;
				break;
			case 6:
				show_cap_change();
				vivod_zap(el);
				gotoxy(20,12);
				z.t_cp=get_int("время центрального процессора(<= 5 цифр): ",EOF);
				if (z.t_cp==-1) break;
				el->data.t_cp=z.t_cp;
				num_m=5;
				break;
			case 0: return;
		}
		gotoxy(2,20);
		vivod_zap(el);
		print_str("нажмите enter",2,22,BACKGROUND,TEXT_COLOR);
		getch();
	}
}
void show_cap_change()
{
	cls(BACKGROUND);
	gotoxy(0,1); printf("                               корректировка");
	gotoxy(0,2); printf("*==============================================================================*");
	gotoxy(0,3); printf("    шифр  | код |    Ф.И.О.     | общее время |  время центрального процессора  ");
	gotoxy(0,4); printf(" ------------------------------------------------------------------------------ ");
	gotoxy(2,5);
}
/*
 * удаление выбранного элемента списка
 * принимает указатель на выбранный элемент,
 * адрес указателей начала и конца списка
 */
void del_record(struct line *el, struct line **left, struct line **right)
{
	if (el==*left)
	{
		if (el==*right) *right=0;
		*left=(*left)->next_right;
		if (*left) (*left)->next_left=0;
		free(el);
	}
	else if (el==*right)
	{
		*right=(*right)->next_left;
		(*right)->next_right=0;
		free(el);
	}
	else
	{
		el->next_left->next_right=el->next_right;
		el->next_left->next_right->next_left=el->next_left;
		free(el);
	}
	//MessageBox(0,"Запись удалена!","",MB_OK|MB_TOPMOST|MB_TASKMODAL);
	error_warning_ok("Запись удалена!");
}
/*
 * проверяет на наличие записи по Ф.И.О
 */
struct line *exist(struct line *temp)
{
	char *str=(char*)calloc(16,sizeof(char));
	cls(BACKGROUND);
	print_str("Поиск по Ф.И.О",20,4,BACKGROUND,TEXT_COLOR);
	gotoxy(20,10);get_mas("Ф.И.О.: ",str,15);
	struct line *left=0,*right=0;
	while(temp)
	{
		if (!strcmp(temp->data.fio,str))
			right=dob_1(right,&left,temp->data);
		temp=temp->next_right;
	}
	return left;
}
void show_exist(struct line *left)
{
	struct line *top,*temp;
	int i,key;
	top=exist(left);
	if (!top)
	{
		//MessageBox(0,"Записи с таким полем не найдено!","",MB_OK|MB_ICONWARNING|MB_TOPMOST|MB_TASKMODAL);
		error_warning_ok("Записи с таким полем не найдено!");
		return;
	}
	struct line *page_t=top;
	cls(BACKGROUND);
	//MessageBox(0,"Управление осуществляется\nстрелочками -> <-, \"esc\"\n\"pg up\", \"pg dn\"","",MB_OK|MB_ICONWARNING|MB_TOPMOST|MB_TASKMODAL);
	error_warning_ok("Управление осуществляется|стрелочками -> <-, esc|pg up, pg dn");
	while(1)
	{
		cls(BACKGROUND);
		show_cap_print();
		temp=page_t;
		show_page_table(temp,K_LINE);
		key=getch();
		if ((key==81||key==77)&&temp)
		{
			temp=page_t;
			for(i=0;(i<K_LINE)&&(page_t->next_right);i++,page_t=page_t->next_right);
			if (i!=K_LINE)
				page_t=temp;
		}
		else if ((key==73||key==75)&&page_t!=top)
		{
			for(i=0;(i<(K_LINE))&&(page_t->next_left);i++,page_t=page_t->next_left);
		}
		else if (key==27) return;    //сделать выход
	}
}
/*
 *  ======================================================================  *
 *  ================   Функции для работы с результатом   ================  *
 *  ======================================================================  *
 */
/*
 * добавление нового элемента к списку,
 * отвечающему за результат
 */
struct line_2 *dob_1_rez(struct line_2 *right,struct line_2 **left, struct inf_2 z)
{
	struct line_2 *el=(struct line_2*)malloc(sizeof(struct line_2));
	el->data=z;
	el->next_right=0;
	if (!right)
	{
		el->data.ish.num=1;
		el->next_left=0;
		right=el;
		*left=el;
		return right;
	}
	el->data.ish.num=(right->data.ish.num)+1;
	right->next_right=el;
	el->next_left=right;
	right=el;
	return right;
}
/*
 * высчитывание процента
 */
struct inf_2 create_inf(struct inf z)
{
	struct inf_2 z1;
	z1.ish=z;
	z1.perc=(100*z.t_cp)/z.t_ob;
	return z1;
}
/*
 * организация списка - результата
 */
void create_line_2(struct line_2 **left,struct line_2 **right,struct line *temp)
{
	del_line_2(left);
	*right=0;
	while (temp)
	{
		*right=dob_1_rez(*right,left,create_inf(temp->data));
		temp=temp->next_right;
	}
}
/*
 * вывод одной записи базы данных
 */
void vivod_zap_rez(struct line_2 *el)
{
	printf("% 8i|%5i|%15s|%5i|%5i|%5i|\n",el->data.ish.shif,el->data.ish.kod,
			el->data.ish.fio,el->data.ish.t_ob,el->data.ish.t_cp,
			el->data.perc);
}
void del_line_2(struct line_2 **left)
{
	struct line_2 *temp;
	while (*left)
	{
		temp=*left;
		*left=(*left)->next_right;
		free(temp);
	}
}
void print_line_2(struct line_2 *left, struct line_2 *right)
{
	struct line_2 *top=left,*temp;
	int i,key,s_t_ob=0,s_t_cp=0,sr_proc=0;
	temp=top;
	while(temp)
	{
		s_t_ob+=temp->data.ish.t_ob;
		s_t_cp+=temp->data.ish.t_cp;
		temp=temp->next_right;
	}
	sr_proc=100*s_t_cp/s_t_ob;
	//MessageBox(0,"Управление осуществляется\nстрелочками -> <-, \"esc\"\n\"pg up\", \"pg dn\"","",MB_OK|MB_ICONWARNING|MB_TOPMOST|MB_TASKMODAL);
	error_warning_ok("Управление осуществляется|стрелочками -> <-, esc|pg up, pg dn");
	while(1)
	{
		cls(BACKGROUND);
		gotoxy(0,0); printf("*================================================*\n");
		gotoxy(0,1); printf("   шифр  | код |    Ф.И.О.     |общее|время|проц.|\n");
		gotoxy(0,2); printf("         |     |               |время| ц.п.| ц.п.|\n");
		gotoxy(0,3); printf("*------------------------------------------------*\n");
		temp=top;
		for(i=0;(i<(K_LINE+1))&&(temp);i++,temp=temp->next_right)
		{
			gotoxy(0,4+i); vivod_zap_rez(temp);
		}
		gotoxy(52,1); printf("общее время: %i",s_t_ob);
		gotoxy(52,3); printf("время централбного");
		gotoxy(52,4); printf("процессора: %i",s_t_cp);
		gotoxy(52,6); printf("средний процент времени");
		gotoxy(52,7); printf("центр. процессора: %i",sr_proc);
		key=getch();
		if ((key==81||key==77)&&temp)
		{
			for(i=0;(i<(K_LINE+1))&&(top);i++,top=top->next_right);
			continue;
		}
		else if ((key==73||key==75)&&top!=left)
		{
			for(i=0;(i<(K_LINE+1))&&(top->next_left);i++,top=top->next_left);
			continue;
		}
		else if (key==27) return;    //сделать выход
	}
}
void rezult(struct line_2 **left, struct line_2 **right, struct line *lleft)
{
	//int key;
	char item_r[][29]={
						"создание",
						"вывод результата"
					  };
	int num_m=0;
		switch(menu_level_2(item_r,2,num_m,40,3+3*4+(20-3*2)/2-5))
		{
		case 1:
			create_line_2(left,right,lleft);
			if(!save_rezult_text(*left))
			{
				//MessageBox(0,"Произведена обработка!\nСписок сформирован!\nПроизошло сохранение в файл!","",MB_OK|MB_TOPMOST|MB_TASKMODAL);
				error_warning_ok("Произведена обработка!|База данных сформированна!|Произошло сохранение в файл!");
			}
			else
			{
				error_warning_ok("Произведена обработка!|База данных сформированна!|\
Невозможно записать в файл!|Доступен только просмотр в текущей сессии");
			}
			return;
		case 2:
			if(!*left)
			{
				error_warning_ok("База данных пуста!|Для формирования базы выберите пункты|обработка->создание");
				break;
			}
			else print_line_2(*left,*right);
			return;
		case 0: return;
		}
}
/*
 * сохранение результата в
 * текстовый файл с названием:
 * день месяц время год
 */
int save_rezult_text(struct line_2 *temp)
{
	FILE *f;
	char *filename=(char*)malloc(250*sizeof(char));
	struct line_2 *top=temp;
	int i,s_t_ob=0,s_t_cp=0,sr_proc=0;
	while(top)
	{
		s_t_ob+=top->data.ish.t_ob;
		s_t_cp+=top->data.ish.t_cp;
		top=top->next_right;
	}
	sr_proc=100*s_t_cp/s_t_ob;
	time_t t;
	t=time(0);
	filename=strcat(ctime(&t),".txt");
	for(i=0;filename[i]!='\n';i++)
		if(filename[i]==':') filename[i]='_';
	while(filename[i]!='\0')
	{
		filename[i]=filename[i+1];
		i++;
	}
	f=fopen(filename,"wt");
	if (!f) return 1;
	fprintf(f,"общее время: %i\n",s_t_ob);
	fprintf(f,"время централбного процессора: %i\n",s_t_cp);
	fprintf(f,"средний процент времени центр. процессора: %i\n",sr_proc);
	fputs("*================================================*\n",f);
	fputs("|  шифр  | код |    Ф.И.О.     |общее|время|проц.|\n",f);
	fputs("|        |     |               |время| ц.п.| ц.п.|\n",f);
	fputs("|------------------------------------------------|\n",f);
    while(temp)
    {
    	fprintf(f,"% 8i|%5i|%15s|%5i|%5i|%5i|\n",temp->data.ish.shif,temp->data.ish.kod,
    			temp->data.ish.fio,temp->data.ish.t_ob,temp->data.ish.t_cp,
				temp->data.perc);
    	temp=temp->next_right;
    }
    fclose(f);

    print_str(filename,TAB_G,22,0,14);
    free(filename);
    return 0;
}
int cat()
{
	FILE *f;
	int c, k;
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE); //дескриптор стандартного вывода консоли
	SetConsoleTextAttribute(h, (WORD) ((0 << 4) | 14));
	f=fopen("cat.txt","rt");
	cls(0);
	if (!f) return 1;
	gotoxy(0,1);
	while((c=getc(f))!=EOF)
	{
		switch(c)
		{
		case '0': k=8;break;
		case '1': k=0;break;
		case '2': k=5;break;
		case '3': k=10;break;
		case '4': k=12;break;
		case '5': k=7;break;
		case '6': k=14; break;
		default: k=0;
		}
		SetConsoleTextAttribute(h, (WORD) ((k << 4) | k));
		printf("%c",c);
		SetConsoleTextAttribute(h, (WORD) ((0 << 4) | 14));
	}
	return 0;
}
void welcome()
{
	char key;
    DWORD cNumRead, j;
    COORD pos;
    INPUT_RECORD irInBuf[128];
    print_str("Приветик)",67,17,0,14);
    print_str("Нажми ->",57,21,0,14);
    window(67,20,9,3,14);
    print_str("Войти",69,21,14,0);
	while(1)
	{
		key=-1;
        if (! ReadConsoleInput(
                h_stdin,      // input buffer handle
                irInBuf,     // buffer to read into
                128,         // size of read buffer
                &cNumRead) ) // number of records read
            ErrorExit("ReadConsoleInput");
		for (j = 0; j < cNumRead; j++)
		{
		    if (irInBuf[j].EventType==MOUSE_EVENT)
		    {
		    	if (!irInBuf[j].Event.MouseEvent.dwEventFlags)
		        {
		            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), irInBuf[j].Event.MouseEvent.dwMousePosition);
		            pos=irInBuf[j].Event.MouseEvent.dwMousePosition;
		            if (pos.X>=67&&pos.X<=75&&pos.Y>=20&&pos.Y<=22) return;
		        }
		    }
		    if (irInBuf[j].EventType==KEY_EVENT)
		    {
		    	if(irInBuf[j].Event.KeyEvent.bKeyDown)
		    	{
		    		key=irInBuf[j].Event.KeyEvent.wVirtualKeyCode;
		    		if (key==13) return;
		    	}
		    }
		}
	}
}
int enquiry_exit()
{
	char key;
	int n=1;
    DWORD cNumRead, j;
    COORD pos;
    INPUT_RECORD irInBuf[128];
    print_str("Уже уходишь?",63,19,0,14);
    print_str("Выбери ->",54,21,0,14);
    print_str(" Да ",64,21,14,0);
    print_str(" Нет ",69,21,0,14);
    while(1)
	{
        if (! ReadConsoleInput(
                h_stdin,      // input buffer handle
                irInBuf,     // buffer to read into
                128,         // size of read buffer
                &cNumRead) ) // number of records read
            ErrorExit("ReadConsoleInput");
		for (j = 0; j < cNumRead; j++)
		{
		    if (irInBuf[j].EventType==MOUSE_EVENT)
		    {
		    	if (irInBuf[j].Event.MouseEvent.dwEventFlags==DOUBLE_CLICK)
		    	{
		    		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), irInBuf[j].Event.MouseEvent.dwMousePosition);
		    		pos=irInBuf[j].Event.MouseEvent.dwMousePosition;
		    		if (pos.X>=64&&pos.X<=68&&pos.Y==21) return 1;
		    		if (pos.X>=69&&pos.X<=73&&pos.Y==21) return 0;
		    	}
		    	if (!irInBuf[j].Event.MouseEvent.dwEventFlags)
		        {
		            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), irInBuf[j].Event.MouseEvent.dwMousePosition);
		            pos=irInBuf[j].Event.MouseEvent.dwMousePosition;
		            if (pos.X>=64&&pos.X<=68&&pos.Y==21)
		            {
		                print_str(" Да ",64,21,14,0);
		                print_str(" Нет ",69,21,0,14);
			   		    n=1;
		            }
		            if (pos.X>=69&&pos.X<=73&&pos.Y==21)
		            {
		                print_str(" Да ",64,21,0,14);
		                print_str(" Нет ",69,21,14,0);
		    		    n=2;
		            }
		        }
		    }
		    if (irInBuf[j].EventType==KEY_EVENT)
		    {
		    	if(irInBuf[j].Event.KeyEvent.bKeyDown)
		    	{
		    		key=irInBuf[j].Event.KeyEvent.wVirtualKeyCode;
		    		switch(key)
		    		{
		    		case 39: //->
		    			n++;
		    			if (n>2) n=1;
		    			break;
		    		case 37: //<-
		    			n--;
		    			if (n<1) n=2;
		    			break;
		    		case 13: return (n==2)?0:1;
		    		case 27: return 0;
		    		}
		    		if (n==1)
		    		{
		    		    print_str(" Да ",64,21,14,0);
		    		    print_str(" Нет ",69,21,0,14);
		    		}
		    		else
		    		{
		    		    print_str(" Да ",64,21,0,14);
		    		    print_str(" Нет ",69,21,14,0);
		    		}
		    	}
		    }
		}
	}
}
void help()
{
	FILE *f;
	int c;
	f=fopen("README.txt","rt");
	if (!f) return;
	cls(0);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD) ((0 << 4) | 14));
	while((c=getc(f))!=EOF)
		printf("%c",c);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD) ((BACKGROUND << 4) | TEXT_COLOR));
	print_str("Для выхода нажмите любую клавишу",40,0,10,0);
	getch();
}
/*
 * вывод выделенной строки *str
 */
void error_warning_ok(char *str)
{
	int s=0,sm=12,i,x,y,k=1,key;  //k - кол-во строк
    DWORD cNumRead, j;
    COORD pos;
    INPUT_RECORD irInBuf[128];
	HANDLE h=GetStdHandle(STD_OUTPUT_HANDLE);
	WORD m;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (GetConsoleScreenBufferInfo(h,&csbi))
		m=csbi.wAttributes;
	for(i=0;str[i]!='\0';i++)
	{
		s++;
		if (str[i]=='|')
		{
			k++;
			if (s>sm) sm=s;
			s=0;
		}
	}
	if (s>sm) sm=s-1;
	x=79-sm-2-2;
	y=1;
	window(x,y,sm+3,k+4,0);
	y++;
	gotoxy(x+1,y);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD) ((0 << 4) | 14));
	for(i=0;str[i]!='\0';i++)
	{
		if(str[i]=='|')
		{
			y++;
			gotoxy(x+1,y);
			continue;
		}
		printf("%c",str[i]);
	}
	y+=2;
	x=x+1;
	//window(x,y,4,3,14);
	print_str("Нажми -> ",x,y,0,14);
	x+=10;
	print_str("ок",x,y,14,0);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),m);
	while(1)
	{
        if (! ReadConsoleInput(
                h_stdin,      // input buffer handle
                irInBuf,     // buffer to read into
                128,         // size of read buffer
                &cNumRead) ) // number of records read
            ErrorExit("ReadConsoleInput");
		for (j = 0; j < cNumRead; j++)
		{
		    if (irInBuf[j].EventType==MOUSE_EVENT)
		    {
		    	if (!irInBuf[j].Event.MouseEvent.dwEventFlags)
		        {
		            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), irInBuf[j].Event.MouseEvent.dwMousePosition);
		            pos=irInBuf[j].Event.MouseEvent.dwMousePosition;
		            if (pos.X>=x&&pos.X<=x+1&&pos.Y==y) return;
		            else printf("\a");
		        }
		    }
		    if (irInBuf[j].EventType==KEY_EVENT)
		    {
		    	if(irInBuf[j].Event.KeyEvent.bKeyDown)
		    	{
		    		key=irInBuf[j].Event.KeyEvent.wVirtualKeyCode;
		    		if (key==13) return;
		    		else printf("\a");
		    	}
		    }
		}
	}
}
int error_warning_yes_no(char *str)
{
	int s=0,sm=19,i,x,y,k=1,key,n=1;  //k - кол-во строк
    DWORD cNumRead, j;
    COORD pos;
    INPUT_RECORD irInBuf[128];
	HANDLE h=GetStdHandle(STD_OUTPUT_HANDLE);
	WORD m;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (GetConsoleScreenBufferInfo(h,&csbi))
		m=csbi.wAttributes;
	for(i=0;str[i]!='\0';i++)
	{
		s++;
		if (str[i]=='|')
		{
			k++;
			if (s>sm) sm=s;
			s=0;
		}
	}
	if (s>sm) sm=s-1;
	x=79-sm-2-2;
	y=1;
	window(x,y,sm+3,k+4,0);
	y++;
	gotoxy(x+1,y);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD) ((0 << 4) | 14));
	for(i=0;str[i]!='\0';i++)
	{
		if(str[i]=='|')
		{
			y++;
			gotoxy(x+1,y);
			continue;
		}
		printf("%c",str[i]);
	}
	y+=2;
	x=x+1;
	print_str("Выбери ->",x,y,0,14);
	x=x+7;
    print_str(" Да ",x+3,y,14,0);
    print_str(" Нет ",x+7,y,0,14);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),m);
    while(1)
	{
        if (! ReadConsoleInput(
                h_stdin,      // input buffer handle
                irInBuf,     // buffer to read into
                128,         // size of read buffer
                &cNumRead) ) // number of records read
            ErrorExit("ReadConsoleInput");
		for (j = 0; j < cNumRead; j++)
		{
		    if (irInBuf[j].EventType==MOUSE_EVENT)
		    {
		    	if (irInBuf[j].Event.MouseEvent.dwEventFlags==DOUBLE_CLICK)
		    	{
		    		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), irInBuf[j].Event.MouseEvent.dwMousePosition);
		    		pos=irInBuf[j].Event.MouseEvent.dwMousePosition;
		    		if (pos.X>=x+3&&pos.X<=x+6&&pos.Y==y) return 1;
		    		else if (pos.X>=x+7&&pos.X<=x+11&&pos.Y==y) return 0;
		    		else printf("\a");
		    	}
		    	if (!irInBuf[j].Event.MouseEvent.dwEventFlags)
		        {
		            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), irInBuf[j].Event.MouseEvent.dwMousePosition);
		            pos=irInBuf[j].Event.MouseEvent.dwMousePosition;
		            if (pos.X>=x+3&&pos.X<=x+6&&pos.Y==y)
		            {
		                print_str(" Да ",x+3,y,14,0);
		                print_str(" Нет ",x+7,y,0,14);
			   		    n=1;
		            }
		            else if (pos.X>=x+7&&pos.X<=x+11&&pos.Y==y)
		            {
		                print_str(" Да ",x+3,y,0,14);
		                print_str(" Нет ",x+7,y,14,0);
		    		    n=2;
		            }
		            else printf("\a");
		        }
		    }
		    if (irInBuf[j].EventType==KEY_EVENT)
		    {
		    	if(irInBuf[j].Event.KeyEvent.bKeyDown)
		    	{
		    		key=irInBuf[j].Event.KeyEvent.wVirtualKeyCode;
		    		switch(key)
		    		{
		    		case 39: //->
		    			n++;
		    			if (n>2) n=1;
		    			break;
		    		case 37: //<-
		    			n--;
		    			if (n<1) n=2;
		    			break;
		    		case 13: return (n==2)?0:1;
		    		case 27: return 0;
		    		default: printf("\a");
		    		}
		    		if (n==1)
		    		{
		    		    print_str(" Да ",x+3,y,14,0);
		    		    print_str(" Нет ",x+7,y,0,14);
		    		}
		    		else
		    		{
		    		    print_str(" Да ",x+3,y,0,14);
		    		    print_str(" Нет ",x+7,y,14,0);
		    		}
		    	}
		    }
		}
	}
}
