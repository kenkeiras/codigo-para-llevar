.PHONY: all build

TSC:=tsc

all: build
build: dist/extension.xpi

dist/extension.xpi: popup/injected.js popup/show-element-ids.js manifest.json popup/show-element-ids.html icons/icon-48.png
	zip $@ $+

popup/injected.js: src/injected.ts
	$(TSC) --project tsconfig_injected.json

popup/show-element-ids.js: src/show-element-ids.ts
	$(TSC) --project tsconfig_popup.json
