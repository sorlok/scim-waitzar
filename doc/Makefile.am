waitzar_users_guide.pdf : userconfig.xml wz_users_guide_temp.fo
	fop -c  userconfig.xml -fo wz_users_guide_temp.fo -pdf waitzar_users_guide.pdf

userconfig.xml : userconfig.in.xml
	./config_parser.pl userconfig.in.xml userconfig.xml

wz_users_guide_temp.fo : custom_layer.xsl waitzar_users_guide.xml
	xsltproc  --output wz_users_guide_temp.fo  --stringparam fop1.extensions 1  custom_layer.xsl waitzar_users_guide.xml


files_USERGUIDE = \
                       ../doc/config_parser.pl \
                       ../doc/titlepage.templates.xml \
                       ../doc/userconfig.in.xml \
                       ../doc/custom_layer.xsl \
                       ../doc/images/console_rendering_glitch.png \
                       ../doc/waitzar_users_guide.xml \
                       ../doc/waitzar_users_guide.pdf


pkgdata_DATA = $(files_USERGUIDE)

EXTRA_DIST = $(files_USERGUIDE)

