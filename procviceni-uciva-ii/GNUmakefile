#YUP2 zapocet 2014: vypracovano:
# 1. hledani-nejdelsich-slov
#    * umi to i soubor v utf-8
#    * pouziva knihovnu ../libup -- obousmerny seznam na implementaci zebricku
#
# 2. maticova-kalkulacka
#    * pouziva knihovnu ../libup
#    * pouziva modul shunting3.c -- pomerne obecny evaluator
#    * pouziva matice3.c
#TODO
# * code::block projekty?
# * cmake
# * zapracovat dale na libup, Doxygen dokumentace
# * shunting3.c a matice3.c zapracovat do libup
#
# Prostredi: Linux/Fedora19, GCC, mozna neco z POSIXu mimo C standard library

#default : progs
default : z2

z2 : libup
	$(MAKE) test1 #hledani-nejdelsich-slov
	$(MAKE) progs PROGS=maticova-kalkulacka

htm = $(wildcard *.htm)
txt = $(htm:.htm=.txt)

PROGS = c2dw
PROGS = list
PROGS = matice
PROGS = maticel
PROGS = shunting_yard_alg
PROGS = shunting
PROGS = shunting2
PROGS = matice
PROGS = shunting3
PROGS = hledani-nejdelsich-slov
PROGS = maticova-kalkulacka

maticova-kalkulacka_SRC = matice3.c shunting3.c
maticova-kalkulacka_CFLAGS = -DNO_MAIN

#FIXME doladit vse na -Werror!!!
CFLAGS += -Wall -Werror

 hledani-nejdelsich-slov : CFLAGS += -I../libup -funsigned-char
#hledani-nejdelsich-slov : export LANG = cs_CZ.UTF-8

BUG = -g
#programy, ktere pouzivaji libup:
up_PROGS = shunting matice shunting_yard_alg maticova-kalkulacka shunting2 shunting3
$(up_PROGS) :  CFLAGS += -I../libup $(BUG)
$(up_PROGS) : LDFLAGS += $(BUG) -L../libup -lup -lm


progs : $(PROGS)

libup : ; $(MAKE) -C ../$@ clean all ; nm ../$@/$@.a

#matice pro long, long long, double generujeme

matice : matice.c
matice.c   : matice-template.c ; ./matice-template.c double | tee $@ | head
maticel.c  : matice-template.c ; ./matice-template.c long   | tee $@ | head

prog : CFLAGS += -m32 #pokus s 32bitovym long
prog : LDFLAGS += -lm -g

$(PROGS) : FORCE
	data=$@.data ; test -f $$data || data=$@.c ; export data ; \
	$(CC) $(CFLAGS) -o $@ $@.c $($(@)_SRC) $($(@)_CFLAGS) $(LDFLAGS) && ./$@ $$data 30

#NOTE: locale .cp1250 určitě na Linuxu nebude zkompilované, použijeme iso-8859-2,
#to je velmi podobné, MalyMekký akorát sabotoval šŠžŽ
test1 : hledani-nejdelsich-slov
	./hledani-nejdelsich-slov vstup.data 15
	./hledani-nejdelsich-slov vstup-utf.data 15
	LANG=cs_CZ.iso8859-2 ./hledani-nejdelsich-slov vstup-cp1250.data 15 | iconv -f CP1250

tags : FORCE  ; ctags *.[ch]

txt : $(txt)
$(txt) : ; html2text -nobs $(basename $@).htm > $@

c.txt : $(addsuffix .txt,$(wildcard *.c))
%.c.txt : %.c ; ./c2dw $< > $@


clean : FORCE
	rm -f -- $(PROGS)

FORCE : ;

var-% : ; @echo '$*=|$($*)|'
