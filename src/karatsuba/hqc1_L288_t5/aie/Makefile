SHELL := /bin/bash
.DELETE_ON_ERROR:
export PYTHONDONTWRITEBYTECODE := 1
VITIS ?= /tools/Xilinx/Vitis/2023.2
PLATFORM ?= $(VITIS)/base_platforms/xilinx_vck190_base_202320_1/xilinx_vck190_base_202320_1.xpfm
ENV = . $(VITIS)/settings64.sh >/dev/null 2>&1 && export CPATH=$(CURDIR)/.aie-shim-include &&
GRAPH := project.cpp
WORK_X86 ?= Work_x86
WORK_HW ?= Work
X86_OUTPUT ?= x86simulator_output
HW_OUTPUT ?= aiesimulator_output
RESULTS ?= results
HW_LOG_DIR = logs/$(notdir $(WORK_HW))
# Optional preprocessor flags, e.g. --Xpreproc=-DKARA_PARITY_P1=1.
AIE_FLAGS ?=
CHECK ?= python3 ../../tools/check.py

.PHONY: shim x86 x86run check hw hwfull aiesim check_hw latency pm
shim:
	mkdir -p .aie-shim-include
	ln -sfn /usr/include/x86_64-linux-gnu/asm .aie-shim-include/asm
x86: shim
	$(ENV) aiecompiler -target=x86sim --platform=$(PLATFORM) \
	  -include=. -include=.. -include=src/kernels --Xpreproc=-I$(CURDIR)/.aie-shim-include \
	  $(AIE_FLAGS) -workdir=$(WORK_X86) $(GRAPH)
x86run:
	$(ENV) python3 ../../tools/run_sim.py --kind x86 --work "$(WORK_X86)" --output "$(X86_OUTPUT)"
check:
	$(CHECK) "$(X86_OUTPUT)"
hw: shim
	$(ENV) aiecompiler -target=hw --stacksize=4096 --platform=$(PLATFORM) \
	  -include=. -include=.. -include=src/kernels --Xpreproc=-I$(CURDIR)/.aie-shim-include \
	  $(AIE_FLAGS) -workdir=$(WORK_HW) $(GRAPH)
hwfull:
	@test ! -e "$(WORK_HW)" || { echo 'Use a fresh WORK_HW or preserve the existing directory before hwfull'; exit 2; }
	@mkdir -p "$(HW_LOG_DIR)"; set +e; $(MAKE) hw >"$(HW_LOG_DIR)/hw.compiler.log" 2>&1; rc=$$?; \
	cat "$(HW_LOG_DIR)/hw.compiler.log"; echo $$rc >"$(HW_LOG_DIR)/hw.compiler.exit"; set -e; \
	if [ $$rc -ne 0 ]; then \
	  test -f "$(WORK_HW)/ps/cdo/Makefile" || { echo 'No CDO Makefile; inspect compiler error'; exit $$rc; }; \
	  echo 'Compiler returned nonzero; attempting the existing CDO workaround; inspect full compiler log.'; \
	fi; \
	$(ENV) $(MAKE) -C $(WORK_HW)/ps/cdo -f Makefile all
aiesim:
	$(ENV) python3 ../../tools/run_sim.py --kind hw --work "$(WORK_HW)" --output "$(HW_OUTPUT)"
check_hw:
	$(CHECK) "$(HW_OUTPUT)"
latency:
	python3 ../../tools/latency.py "$(HW_OUTPUT)" --json "$(RESULTS)/latency.json"
pm:
	python3 ../../tools/pm_report.py --work "$(WORK_HW)" --results "$(RESULTS)"
