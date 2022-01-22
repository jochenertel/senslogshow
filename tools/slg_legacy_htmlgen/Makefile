slg_legacy_htmlgen: options.o slg_date.o slg_values.o slg_dayfile.o slg_temper.o slg_rain.o slg_legacy_htmlgen.o
	gcc -Wall -o slg_legacy_htmlgen options.o slg_date.o slg_values.o slg_dayfile.o slg_temper.o slg_rain.o slg_legacy_htmlgen.o

options.o: ../../lib/options.h ../../lib/options.c
	gcc -Wall -c ../../lib/options.c

slg_date.o: ../../lib/slg_date.h ../../lib/slg_date.c
	gcc -Wall -c ../../lib/slg_date.c

slg_values.o: ../../lib/slg_values.h ../../lib/slg_values.c
	gcc -Wall -c ../../lib/slg_values.c

slg_dayfile.o: ../../lib/slg_dayfile.h ../../lib/slg_dayfile.c
	gcc -Wall -c ../../lib/slg_dayfile.c

slg_temper.o: ../../lib/slg_temper.h ../../lib/slg_temper.c
	gcc -Wall -c ../../lib/slg_temper.c

slg_rain.o: ../../lib/slg_rain.h ../../lib/slg_rain.c
	gcc -Wall -c ../../lib/slg_rain.c

slg_legacy_htmlgen.o: slg_legacy_htmlgen.c
	gcc -Wall -c slg_legacy_htmlgen.c

clean:
	rm -f *.o
	rm -f slg_legacy_htmlgen
