LIBS := zmq glog thostmduserapi# 静态库文件名
INCLUDES:= ./include ./api          # 头文件目录
VPATH = md:include:trade

LIBNAME := $(addprefix -l,$(LIBS))
LIBNAME += `pkg-config --cflags --libs protobuf`

ctp_md_obj = ctp_md.o md_spi.o ctp.pb.o file_utils.o
ctp_trade_obj = ctp_trade.o trade_spi.o ctp.pb.o

CC:=g++
CFLAGS := -std=c++11 -g -Wall -O3
CPPFLAGS := $(CFLAGS)
CPPFLAGS += $(addprefix -I,$(INCLUDES))
CPPFLAGS += -MMD
CC = g++

ctp_md: $(ctp_md_obj)
	$(CC) -o ctp_md $(ctp_md_obj) $(LIBNAME)
	mv *.d build/
	mv *.o build/

ctp_trade: $(ctp_trade_obj)
	$(CC) -o ctp_trade $(ctp_trade_obj) $(LIBNAME)
	mv *.d build/
	mv *.o build/

ctp_md.o: ctp_md.cpp md_spi.h

ctp_trade.o: ctp_trade.cpp trade_spi.h

md_spi.o: md_spi.h md_spi.cpp 

trade_spi.o : trade_spi.h trade_spi.cpp

ctp.pb.o : ctp.pb.h

file_utils.o : file_utils.h

clean:
	rm -f *.o 
	rm -f *.d
	rm -f ctp_md ctp_trade

# ${OBJDIR}/%.o: %.cpp
#         gcc -c $< -o $@
