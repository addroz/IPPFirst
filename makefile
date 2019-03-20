make:
	gcc -c -Wall -Wextra -std=c11 -O2 -g utils.c
	gcc -c -Wall -Wextra -std=c11 -O2 -g history.c
	gcc -c -Wall -Wextra -std=c11 -O2 -g main.c
	gcc -o quantization main.o utils.o history.o

clean:
	rm utils.o
	rm history.o
	rm main.o
