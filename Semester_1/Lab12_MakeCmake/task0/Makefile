# Собрать многофайловый проект
# Подсказка: Опция -I в gcc помогает решить проблему с поиском заголовочных файлов. (#include "" No such file or directory)
# Для использования make его необходимо установить: sudo apt install make

all:
	gcc ./pipes/write_pipe.c -c
	gcc ./pipes/read_pipe.c -c 
	gcc ./background/background.c -c -I ./pipes
	gcc ./app/main.c -c -I ./background
	gcc main.o background.o read_pipe.o write_pipe.o 

#all: task0.o
#
#task0.o: pipe.o bg.o main.o
#	gcc pipe.o bg.o main.o -o task0.o
#	
#pipe.o:
#	gcc ./pipes/read_pipe.c -c ./pipes/write_pipe.c -c -o pipe.o
#
#bg.o:
#	gcc ./background/background.c -c -I ./pipes/ -o bg.o
#
#main.o:
#	gcc ./app/main.c -c -I ./background/ -o main.o