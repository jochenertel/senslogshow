slg_dfcheck: options.o slg_date.o slg_values.o slg_dayfile.o slg_dfcheck.o
	gcc -Wall -o slg_dfcheck options.o slg_date.o slg_values.o slg_dayfile.o slg_dfcheck.o

options.o: ../../lib/options.h ../../lib/options.c
	gcc -Wall -c ../../lib/options.c

slg_date.o: ../../lib/slg_date.h ../../lib/slg_date.c
	gcc -Wall -c ../../lib/slg_date.c

slg_values.o: ../../lib/slg_values.h ../../lib/slg_values.c
	gcc -Wall -c ../../lib/slg_values.c

slg_dayfile.o: ../../lib/slg_dayfile.h ../../lib/slg_dayfile.c
	gcc -Wall -c ../../lib/slg_dayfile.c

slg_dfcheck.o: slg_dfcheck.c
	gcc -Wall -c slg_dfcheck.c

clean:
	rm -f *.o
	rm -f slg_dfcheck
