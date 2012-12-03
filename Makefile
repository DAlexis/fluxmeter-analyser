CXXFLAGS=-Wall

flux-analysis: main.o analysis.o base.o statistics.cpp
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
use1:
	./flux-analysis -f dataIPFAN-08122012.txt -t 2plot.txt -n 2.4 120 -s "ss.txt" --simple-detection config.txt	 
use2:
	./flux-analysis -f dataIPFAN-08122012.txt -t 2plot.txt -n 5 12000 -s "ss.txt" --simple-detection config.txt
myza:
	./flux-analysis -f Myza_2012_06_02__00_00_00.dat -t Myza.txt -F myza -u 6 -n 1.8 2
myza1:
	./flux-analysis -f Myza_2012_06_02__00_00_00.dat -o Myza.bin -F myza -u 6 -n 1.8 2 
myza2:
	./flux-analysis -i Myza.bin -t Myza.txt -q 2 -n 1.25 0.012
