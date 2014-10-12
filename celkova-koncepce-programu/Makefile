htm = $(wildcard *.htm)
txt = $(htm:.htm=.txt)


#objemy-a-povrchy.c obsahuje main(), ve vypocet.c jsou pomocne funkce viz vypocet.h
objemy-a-povrchy : MODULES = vypocet.c
objemy-a-povrchy : LDFLAGS = -lm

ascii-art : MODULES = canvas.c

ascii-art \
objemy-a-povrchy \
file_h : FORCE
	$(CC) $(CFLAGS) -o $@ $(LDFLAGS) $@.c $(MODULES) && ./$@ 0 1.2 2.4 && ./$@ 4 1 1 && ./$@ 3 2.3 4.5


%.png : %.ditaa
	ditaa -S -o -E $< $@


txt : $(txt)
$(txt) : ; html2text -nobs $(basename $@).htm > $@

FORCE : ;

dynamicka-vicerozmerna-pole.zip :
	zip $@ *.c *.png
