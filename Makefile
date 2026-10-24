all:
	gcc -std=c99 -pedantic -Wall -Wextra -o heatmap heatmap.c bmp.c -lm

clean:
	rm -f heatmap *.bmp
