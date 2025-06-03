all:
	gcc -std=c99 -pedantic -Wall -Wextra -o solution heatmap.c bmp.c -lm