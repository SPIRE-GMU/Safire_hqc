# FAFFT multiplier on the AIE array: one ADF project per configuration, selected with CFG=<name>.
SHELL := /bin/bash
.DELETE_ON_ERROR:
export PYTHONDONTWRITEBYTECODE := 1
CFG ?= n16
VITIS ?= /tools/Xilinx/Vitis/2023.2
PLATFORM ?= $(VITIS)/base_platforms/xilinx_vck190_base_202320_1/xilinx_vck190_base_202320_1.xpfm
ENV = . $(VITIS)/settings64.sh >/dev/null 2>&1 && export CPATH=$(CURDIR)/.aie-shim-include &&
# The kernels include a few C files of the upstream HQC code (fetched by baseline/hqc-a72/setup.sh):
# h3* configurations are HQC-3 (hqc-192), h5* are HQC-5 (hqc-256), all others HQC-1 (hqc-128).
HQC = $(if $(filter h3%,$(CFG)),192,$(if $(filter h5%,$(CFG)),256,128))
UPSTREAM ?= ../../baseline/hqc-a72/src/hqc-$(HQC)
GRAPH = $(CFG)/graph.cpp
WORK_X86 ?= Work_x86_$(CFG)
WORK_HW ?= Work_hw_$(CFG)
X86_OUTPUT ?= x86simulator_output_$(CFG)
HW_OUTPUT ?= aiesimulator_output_$(CFG)
RESULTS ?= results/$(CFG)
HW_LOG_DIR = logs/$(CFG)
AIE_FLAGS ?=
INCLUDES = -include=. -include=$(UPSTREAM) --Xpreproc=-I$(CURDIR)/.aie-shim-include

.PHONY: upstream shim x86 x86run check hw hwfull pm aiesim check_hw latency
upstream:
	@test -f "$(UPSTREAM)/gfv_tower.h" || { echo 'upstream HQC code not found in $(UPSTREAM): run baseline/hqc-a72/setup.sh first'; exit 1; }
shim:
	mkdir -p .aie-shim-include
	ln -sfn /usr/include/x86_64-linux-gnu/asm .aie-shim-include/asm
x86: upstream shim
	$(ENV) aiecompiler -target=x86sim --platform=$(PLATFORM) $(INCLUDES) $(AIE_FLAGS) -workdir=$(WORK_X86) $(GRAPH)
x86run:
	$(ENV) x86simulator --pkg-dir="$(WORK_X86)" --output-dir="$(X86_OUTPUT)"
check:
	python3 tools/check.py "$(X86_OUTPUT)" --config "$(CFG)"
# aiecompiler 2023.2 keeps three file descriptors open per kernel; more than about 330 kernels need a higher limit.
hw: upstream shim
	ulimit -n 65536 2>/dev/null; $(ENV) aiecompiler -target=hw --stacksize=4096 --platform=$(PLATFORM) $(INCLUDES) $(AIE_FLAGS) -workdir=$(WORK_HW) $(GRAPH)
hwfull:
	@test ! -e "$(WORK_HW)" || { echo 'Use a fresh WORK_HW or preserve the existing directory before hwfull'; exit 2; }
	@mkdir -p "$(HW_LOG_DIR)"; set +e; $(MAKE) hw >"$(HW_LOG_DIR)/hw.compiler.log" 2>&1; rc=$$?; \
	cat "$(HW_LOG_DIR)/hw.compiler.log"; echo $$rc >"$(HW_LOG_DIR)/hw.compiler.exit"; set -e; \
	if [ $$rc -ne 0 ]; then \
	  test -f "$(WORK_HW)/ps/cdo/Makefile" || { echo 'No CDO Makefile; inspect compiler error'; exit $$rc; }; \
	  echo 'Compiler returned nonzero; attempting the CDO step again; inspect full compiler log.'; \
	fi; \
	$(ENV) $(MAKE) -C $(WORK_HW)/ps/cdo -f Makefile all
pm:
	python3 tools/pm_report.py --work "$(WORK_HW)" --out "$(RESULTS)/pm_report.txt"
aiesim:
	$(ENV) aiesimulator --pkg-dir="$(WORK_HW)" --profile --output-dir="$(HW_OUTPUT)"
check_hw:
	python3 tools/check.py "$(HW_OUTPUT)" --config "$(CFG)"
latency:
	python3 tools/latency.py "$(HW_OUTPUT)" --config "$(CFG)" --json "$(RESULTS)/latency.json"
