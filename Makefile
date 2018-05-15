all:
	g++ -o p4 main.cpp process.cpp page.cpp -std=c++11
clean:
	rm -f p4
