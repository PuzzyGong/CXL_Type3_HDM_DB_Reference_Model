# ****************************************  CPP ONLY ****************************************

CPP_INC_DIR = \
	cpp/include \
	cpp/include/channel_model \
	cpp/include/device_model \
	cpp/include/host_model \
	cpp/include/dpic
# to be added

CPP_SRC_DIR = \
	cpp/src \
	cpp/src/channel_model \
	cpp/src/device_model \
	cpp/src/host_model \
	cpp/src/dpic
# to be added

CPP_OBJ_DIR = cpp/out
CPP_OUT_DIR = cpp/out

CPP_INCS = $(foreach dir, $(CPP_INC_DIR), $(wildcard $(dir)/*.h))
CPP_SRCS = $(foreach dir, $(CPP_SRC_DIR), $(wildcard $(dir)/*.cpp))
CPP_OBJS = $(addprefix $(CPP_OBJ_DIR)/, $(patsubst %.cpp, %.o, $(notdir $(CPP_SRCS))))
CPP_OUT  = $(CPP_OUT_DIR)/main

CPP         = g++
CPP_FLAGS   = -g -std=c++11 -Wall -fPIC -O0 #-I$(CPP_INC_DIR)

cppcompile: $(CPP_OBJS)
	$(CPP) $(CPP_FLAGS) -o $(CPP_OUT) $(CPP_OBJS)

VPATH = $(CPP_SRC_DIR)
$(CPP_OBJ_DIR)/%.o: %.cpp $(CPP_INCS)
	$(CPP) $(CPP_FLAGS) -c $< -o $@

# ****************************************  MAIN 1 ****************************************



# ****************************************  CPP ADDING TO SV  ****************************************

CPP_SV         = $(CPP)
CPP_SV_FLAGS   = $(CPP_FLAGS)
CPP_SV_INCS    = $(CPP_INCS)
CPP_SV_SRCS    = $(filter-out cpp/src/main.cpp, $(CPP_SRCS))

# $(info $(CPP_SV_SRCS))
# $(info $(CPP_SRCS))

# ****************************************  SV  ****************************************

VCS         = vcs
UVM_HOME    = $(VCS_HOME)/etc/uvm-1.2
VCS_OPTS    = -full64 -sverilog  $(UVM_HOME)/src/dpi/uvm_dpi.cc -CFLAGS -DVCS -lca -debug_access+all -timescale=1ns/1ps
VLOGAN_OPTS = -full64 -sverilog -debug_access+all -timescale=1ns/1ps -ntb_opts uvm-1.2

# Include directories
UVM_INCDIRS = +incdir+$(UVM_HOME)/src +incdir+./sv

# Combine all sources in proper order
UVM_SRCS = sv/env.svh sv/dut.sv sv/test.sv sv/top_tb.sv

# Step 1: Analyze VHDL files

# Step 2.1: Analyze DUT Verilog/SystemVerilog files

# Step 2.2: Analyze UVM and remaining Verilog/SystemVerilog files
analyze_verilog: $(UVM_SRCS)
	@mkdir -p libs/vcs/work
	vlogan $(VLOGAN_OPTS) $(UVM_INCDIRS) -work work $(UVM_HOME)/src/uvm_pkg.sv -l vlogan_uvm_pkg.log
	vlogan $(VLOGAN_OPTS) $(UVM_INCDIRS) -work work $(UVM_SRCS) -l vlogan_tb.log +define+SAMT_ENV

# Step 3: Elaborate and compile
simv: analyze_verilog $(CPP_SV_SRCS) $(CPP_SV_INCS)							# <-------------------------- Modified
	$(VCS) $(VCS_OPTS) \
	$(CPP_SV_SRCS) -CFLAGS "$(CPP_SV_FLAGS)" \
	-top top_tb -l elaborate.log

run: simv
	./simv -l simulation.log

gui: simv
	./simv -gui -l simulation.log

clean: vcs_clean

vcs_clean:
	rm -rf csrc simv simv.daidir ucli.key
	rm -rf DVEfiles inter.vpd vc_hdrs.h
	rm -rf *.log *.vpd *.fsdb *.trn *.vdb
	rm -rf .__snapshot* .synopsys_dve_rebuild.tcl
	rm -rf AN.DB urgReport
	rm -rf libs
# Modified
	rm -rf .inter.vpd.uvm .Makefile.swp *.tar.gz .vscode/settings.json
	rm -rf cpp/out/main cpp/out/main.exe cpp/out/*.o

	