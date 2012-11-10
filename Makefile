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
test1:
	 ./flux-analysis --output-text textout.txt --fresh-input dataIPFAN-08122012.txt
test2:
	 ./flux-analysis --output-text textout.txt --trunc 1000 --fresh-input dataIPFAN-08122012.txt
	 
