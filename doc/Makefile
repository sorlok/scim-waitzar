waitzar_users_guide.pdf : userconfig.xml wz_users_guide_temp.fo
	fop -c  userconfig.xml -fo wz_users_guide_temp.fo -pdf waitzar_users_guide.pdf

userconfig.xml : userconfig.in.xml
	./config_parser.pl userconfig.in.xml userconfig.xml

wz_users_guide_temp.fo : custom_layer.xsl waitzar_users_guide.xml
	xsltproc  --output wz_users_guide_temp.fo  --stringparam fop1.extensions 1  custom_layer.xsl waitzar_users_guide.xml

all : waitzar_users_guide.pdf

mostlyclean :
	rm -f waitzar_users_guide.pdf

clean :
	rm -f waitzar_users_guide.pdf

distclean : 
	rm -f waitzar_users_guide.pdf

maintainer-clean :
	rm -f waitzar_users_guide.pdf

EMPTY_AUTOMAKE_TARGETS = distdir install install-data install-exec uninstall install-dvi install-html install-info install-ps install-pdf installdirs check installcheck dvi pdf ps info html tags ctags
.PHONY: $(EMPTY_AUTOMAKE_TARGETS)
$(EMPTY_AUTOMAKE_TARGETS):


