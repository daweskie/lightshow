# Global makefile


PROJECTS=$(patsubst %.,%,$(patsubst ./%,%,$(patsubst %/Makefile,%,$(shell find . -name Makefile))))

$(info projects:$(PROJECTS))

THIS=$(shell pwd)

clean: $(PROJECTS)
	for i in $(PROJECTS);  do $(MAKE) -C $${i} clean; done 

doc:$(PROJECTS)
	for i in $(PROJECTS);  do $(MAKE) -C $${i} doc; done 

commit: clean 
	git add --all
	git commit -a

push: commit
	git push origin master

pull: 
	git pull origin master

include help.mk

.PHONY:clean commit doc

