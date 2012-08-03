.PHONY: package pkg_direxist install

clean: CONF_EXTRACLEAN := $(PACKAGENAME).fpkg

package: export FEOSDEST := $(CURDIR)/.pkg
package: pkg_direxist install

pkg_direxist:
	@rm -fr $(FEOSDEST)
	@[ -d $(FEOSDEST) ] || mkdir -p $(FEOSDEST)

package:
	@fartool $(FEOSDEST) ?stdout | gzip -c9 | fpkgtool $(MANIFEST) $(PACKAGENAME).fpkg
	@rm -fr $(FEOSDEST)
	@echo Package $(PACKAGENAME) successfully built.
