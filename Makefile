CXXFLAGS=-Wall

flux-analysis: main.o analysis.o base.o
	g++ -Wall -o $@ $^ 
	#main.o base.o analysis.o
#analysis.o:
#	g++ -Wall -c analysis.cpp
#inpout.o:
#	g++ -Wall -c inpout.cpp
#main.o:
#	g++ -Wall -c main.cpp
#base.o:
#	g++ -Wall -c main.cpp
clean:
	rm main.o
	rm flux-analysis
	rm analysis.o 
	rm base.o
testPatt1:
	./flux-analysis --pattern pattern1.txt --input-fresh dataIPFAN-08122012.txt --strikes-list strikes.txt
	 
