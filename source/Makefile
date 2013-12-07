HOME_DIR = ..
sources_dir = .

project = flux-analysis
exe_name = flux-analysis

cpp_sources = analysis.cpp \
              base.cpp \
              cmdline-parser.cpp \
              main.cpp \
              statistics.cpp \
              system-class.cpp \
              unknown-with-options.cpp

install_dir = ~/Science/flux-analysis

post_build = install

LDFLAGS += -lmysqlclient -lboost_date_time -lboost_program_options

include $(HOME_DIR)/Makefile.inc

install:
	mkdir -p ~/Science/flux-analysis/graphs
	cp $(BUILD_DIR)/flux-analysis ~/Science/flux-analysis
	cp $(HOME_DIR)/scripts/* ~/Science/flux-analysis
	cp $(HOME_DIR)/configs/* ~/Science/flux-analysis
	cp $(HOME_DIR)/3rdparty/* ~/Science/flux-analysis
