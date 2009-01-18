SUBDIRS = images fonts

waitzar_users_guide.html : wz_styles.css  waitzar_users_guide.html.xml  imghead.html
	xsltproc --output @srcdir@/waitzar_users_guide.html --stringparam html.stylesheet  @srcdir@/wz_styles.css  /usr/share/xml/docbook/stylesheet/nwalsh/html/docbook.xsl  @srcdir@/waitzar_users_guide.html.xml

html: waitzar_users_guide.html

pdf : userconfig.xml wz_users_guide_temp.fo
	cd @srcdir@ && fop -c userconfig.xml -fo wz_users_guide_temp.fo -pdf waitzar_users_guide.pdf

userconfig.xml : userconfig.in.xml config_parser.pl
	chmod 755 @srcdir@/config_parser.pl && ./@srcdir@/config_parser.pl @srcdir@/userconfig.in.xml @srcdir@/userconfig.xml

wz_titlepage.xsl : titlepage.templates.xml
	xsltproc --output  @srcdir@/wz_titlepage.xsl /usr/share/xml/docbook/stylesheet/nwalsh/template/titlepage.xsl @srcdir@/titlepage.templates.xml

waitzar_users_guide.html.xml : waitzar_users_guide.xml
	xsltproc  --output  @srcdir@/waitzar_users_guide.html.xml --stringparam  profile.role  "html" /usr/share/xml/docbook/stylesheet/nwalsh/profiling/profile.xsl @srcdir@/waitzar_users_guide.xml

waitzar_users_guide.pdf.xml : waitzar_users_guide.xml
	xsltproc  --output  @srcdir@/waitzar_users_guide.pdf.xml --stringparam  profile.role  "pdf" /usr/share/xml/docbook/stylesheet/nwalsh/profiling/profile.xsl @srcdir@/waitzar_users_guide.xml


wz_users_guide_temp.fo : custom_layer.xsl waitzar_users_guide.pdf.xml wz_titlepage.xsl
	xsltproc  --output @srcdir@/wz_users_guide_temp.fo  --stringparam fop1.extensions 1  @srcdir@/custom_layer.xsl @srcdir@/waitzar_users_guide.pdf.xml

mydocdir=$(docdir)
mydoc_DATA=	\
                       config_parser.pl \
                       titlepage.templates.xml \
                       userconfig.in.xml \
                       custom_layer.xsl \
                       waitzar_users_guide.xml \
                       wz_styles.css \
                       compile_guide.txt \
                       waitzar_users_guide.pdf \
                       imghead.html

nodist_mydocdir=$(docdir)
nodist_mydoc_DATA = waitzar_users_guide.html


#mydoc2dir=$(docdir)
#mydoc2_DATA = waitzar_users_guide.pdf

EXTRA_DIST = $(mydoc_DATA)



