all: morthe tioclinux
	(echo 'd4 pl pl'; echo 'n1 n2 n3 c4 pr') | ./morthe

morthe: m.cc i.cc 
	gcc -std=c++17 -g  -o morthe  m.cc i.cc

tioclinux: tioclinux.cc
	gcc -std=c++17 -g  -o tioclinux tioclinux.cc

clean:
	rm -fv morthe tioclinux
