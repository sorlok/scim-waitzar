SUBDIRS = images fonts

waitzar_users_guide.pdf : userconfig.xml wz_users_guide_temp.fo
	fop -c  userconfig.xml -fo wz_users_guide_temp.fo -pdf waitzar_users_guide.pdf

userconfig.xml : userconfig.in.xml config_parser.pl
	chmod 755 config_parser.pl && ./config_parser.pl userconfig.in.xml userconfig.xml

wz_users_guide_temp.fo : custom_layer.xsl waitzar_users_guide.xml
	xsltproc  --output wz_users_guide_temp.fo  --stringparam fop1.extensions 1  custom_layer.xsl waitzar_users_guide.xml


files_USERGUIDE = \
                       config_parser.pl \
                       titlepage.templates.xml \
                       userconfig.in.xml \
                       custom_layer.xsl \
                       waitzar_users_guide.xml \
                       compile_guide.txt \
                       waitzar_users_guide.pdf

dist_doc_DATA = $(files_USERGUIDE)

EXTRA_DIST = $(files_USERGUIDE)



