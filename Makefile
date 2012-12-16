CXXFLAGS=-Wall

all: flux-analysis install

flux-analysis: main.o analysis.o base.o statistics.cpp cmdline-parser.o system-class.o
	g++ -Wall -o $@ $^ 
clean:
	rm main.o
	rm flux-analysis
	rm analysis.o
	rm base.o
	rm statistics.o
	rm ~/Science/flux-analysis/flux-analysis
install: 
	cp ./flux-analysis ~/Science/flux-analysis
myza:
	./flux-analysis -f Myza_2012_06_02__00_00_00.dat -t Myza.txt -F myza -u 6 -n 1.8 2
myza1:
	./flux-analysis -f Myza_2012_06_02__00_00_00.dat -o Myza.bin -F myza -u 6 -n 1.8 2 
myza2:
	./flux-analysis -i Myza.bin -t Myza.txt -q 2 -n 1.25 0.012
usenew:
	./flux-analysis -f dataIPFAN-08122012.txt -t 2plot.txt -n 2.4 120 --simple-detection config.txt	 -s "ss.txt"
test1:
	./flux-analysis -f dataIPFAN-08122012.txt -t 2plot.txt -n 2.4 120 --simple-detection config.txt	 -s "ss.txt" -M "stat.txt"
plain:
	./flux-analysis -f Myza_2012_06_02__00_00_00.dat  -F myza -t Myza-plain.txt -u 6
