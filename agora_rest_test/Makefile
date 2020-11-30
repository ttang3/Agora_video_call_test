TOPDIR  := $(shell pwd)

GOTARGET   := agora_rest_test
GOPATH     := $(TOPDIR)
BASE       := ./src/agora_rest_test
GOSRCDIR   := $(TOPDIR)/src/agora_rest_test
#GO         := GOPATH=$(GOPATH) go
GOOPTS     := -v
GO	   := go
ifeq ($(V),1)
GOOPTS     := -v -x
endif

all: build

build:
	$(GO) build $(GOOPTS) -o $(GOPATH)/bin/$(GOTARGET) $(BASE) 

fmt:
	$(GO) fmt $(BASE)/$(GOTARGET)

clean:
	rm -rf $(TOPDIR)/deps/pkg $(TOPDIR)/bin/agora_rest_test

over: clean all

run: build
	$(GOPATH)/bin/$(GOTARGET) 8888

run_secure: build
	cd $(GOPATH)/bin
	$(GOPATH)/bin/$(GOTARGET) 8889 $(GOPATH)/bin/server_cert.pem $(GOPATH)/bin/server.key.pem


