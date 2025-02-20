#!/bin/sh
# Hey!  Those limericks are pretty nasty, IMHO.  And I'm Irish! :^)
# So, we're going to change the suffix to -o, so they aren't part of
# the default fare when you run 'fortune'.

mv fortune/datfiles/limerick.fake fortune/datfiles/limerick-o.fake
mv fortune/datfiles/limerick.real fortune/datfiles/limerick-o.real
patch < ../limerick-o.sh
exit

--- fortune/datfiles/Makefile.orig	Fri Aug 21 21:15:28 1998
+++ fortune/datfiles/Makefile	Fri Aug 21 21:16:32 1998
@@ -4,7 +4,7 @@
 
 DATFILES = fortunes.dat fortunes2.dat startrek.dat zippy.dat \
 	fortunes-o fortunes-o.dat fortunes2-o fortunes2-o.dat \
-	limerick limerick.dat
+	limerick-o limerick-o.dat
 SRCFILES = fortunes fortunes2 startrek zippy
 
 all:	$(DATFILES)
@@ -39,11 +39,11 @@
 fortunes2-o.dat:	fortunes2-o
 	../strfile/strfile -rsx fortunes2-o fortunes2-o.dat
 
-limerick:	limerick.$(FORTUNE_TYPE)
-	tr a-zA-Z n-za-mN-ZA-M <limerick.$(FORTUNE_TYPE) >limerick
+limerick-o:	limerick-o.$(FORTUNE_TYPE)
+	tr a-zA-Z n-za-mN-ZA-M <limerick-o.$(FORTUNE_TYPE) >limerick-o
 
-limerick.dat:	limerick
-	../strfile/strfile -rsx limerick limerick.dat
+limerick-o.dat:	limerick-o
+	../strfile/strfile -rsx limerick-o limerick-o.dat
 
 clean:
 	rm -f $(DATFILES)
