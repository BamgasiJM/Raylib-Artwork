# Source-controlled entry point. Premake writes generated files into build/.
.DEFAULT_GOAL := all
config ?= debug_x64

ifeq ($(OS),Windows_NT)
  export PATH := $(CURDIR)/tools/w64devkit/bin;$(PATH)
  PREMAKE := tools/premake/premake5.exe
else ifeq ($(shell uname -s),Darwin)
  PREMAKE := tools/premake/premake5.osx
else
  PREMAKE := tools/premake/premake5
endif

.PHONY: all generate clean help

all: generate
	+$(MAKE) --no-print-directory -C build config=$(config)

generate:
	"$(PREMAKE)" --file=tools/premake/premake5.lua gmake
	"$(PREMAKE)" --file=tools/premake/premake5.lua --config=$(config) ecc

clean: generate
	+$(MAKE) --no-print-directory -C build config=$(config) clean

help:
	@echo make [config=debug_x64 or release_x64] [all, generate, clean]
