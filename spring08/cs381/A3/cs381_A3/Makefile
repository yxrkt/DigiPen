#if you are using Windows command prompt, you may need to change 
#"/" to "\" (slashes to backslashes, 
#also cygwin-style path should be changed to windows-style:
#"/cygdrive/y/shared/" to "Y:\shared\"

mm:
	g++ -o mm main.c -I/cygdrive/y/shared/CLIPSSrc/CLIPSSrc/CLIPSSrc -L. -lclips++
mm-simple:
	g++ -o mm-simple main-simple.c -I/cygdrive/y/shared/CLIPSSrc/CLIPSSrc/CLIPSSrc -L. -lclips++
mm-linux:
	gcc -o mm main.c -lclips -lm -lrt 
mm-simple-linux:
	gcc -o mm-simple main-simple.c -lclips -lm -lrt 
clean:
	rm -f *.o mm.exe mm-simple.exe mm mm-simple
