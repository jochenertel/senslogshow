slg_test: slg_date.o slg_values.o slg_test.o
	gcc -Wall -o slg_test slg_date.o slg_values.o slg_test.o

slg_date.o: ../lib/slg_date.h ../lib/slg_date.c
	gcc -Wall -c ../lib/slg_date.c

slg_values.o: ../lib/slg_values.h ../lib/slg_values.c
	gcc -Wall -c ../lib/slg_values.c

slg_test.o: slg_test.c
	gcc -Wall -c slg_test.c

clean:
	rm -f *.o
	rm -f slg_test
