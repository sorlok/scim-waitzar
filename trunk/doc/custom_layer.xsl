<?xml version='1.0'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fo="http://www.w3.org/1999/XSL/Format" version="1.0">
  <xsl:import href="/usr/share/xml/docbook/stylesheet/nwalsh/fo/docbook.xsl"/>
  <xsl:include href="wz_titlepage.xsl"/>
  
  <!-- Page numbering: Skip the first page -->
  <!-- Still can't figure out how to do this... -->
  
  <!-- Add a border around figures -->
  <xsl:attribute-set name="formal.object.properties">
    <xsl:attribute name="border-style">solid</xsl:attribute>
    <xsl:attribute name="border-width">1px</xsl:attribute>
    <xsl:attribute name="padding-left">5px</xsl:attribute>
    <xsl:attribute name="padding-right">5px</xsl:attribute>
    <xsl:attribute name="padding-bottom">5px</xsl:attribute>
  </xsl:attribute-set> 

<!-- This necessitates some changes to figure titles, as well. -->
  <xsl:attribute-set name="formal.title.properties">
      <xsl:attribute name="font-style">italic</xsl:attribute>
      <xsl:attribute name="font-weight">normal</xsl:attribute>
      <xsl:attribute name="hyphenate">false</xsl:attribute>
  </xsl:attribute-set>

  
  <!-- Customization parameters -->
  <xsl:param name="admon.graphics" select="1"/>
  <xsl:param name="section.autolabel" select="1"/>
  <xsl:param name="symbol.font.family">Padauk,Myanmar3,Bitstream Vera Sans</xsl:param>
  <xsl:param name="body.start.indent">0pt</xsl:param>
  
  <!-- Break on every section (I was just going to do the TOC, but this looks nice. -->
  <xsl:attribute-set name="section.level1.properties">
    <xsl:attribute name="break-before">page</xsl:attribute>
  </xsl:attribute-set>
  
  <!-- Bold & blue hyperlinks -->
  <xsl:attribute-set name="xref.properties">
    <xsl:attribute name="color">
      <xsl:choose>
        <xsl:when test="self::ulink">blue</xsl:when>
        <xsl:otherwise>inherit</xsl:otherwise>
      </xsl:choose>
    </xsl:attribute>
    <xsl:attribute name="font-weight">
      <xsl:choose>
        <xsl:when test="self::ulink">bold</xsl:when>
        <xsl:otherwise>inherit</xsl:otherwise>
      </xsl:choose>
    </xsl:attribute>
  </xsl:attribute-set>
  
  <!-- Green & bold commands -->
  <xsl:template match="command">
    <fo:inline color="green" font-weight="bold">
      <xsl:apply-templates/>
    </fo:inline>
  </xsl:template>

  <!-- Foreign phrases in Myanmar3 -->
  <xsl:template match="foreignphrase">
    <fo:inline font-family="Myanmar3">
      <xsl:apply-templates/>
    </fo:inline>
  </xsl:template>

  <!-- Program Listings in Monospace -->
  <xsl:template match="programlisting">
    <fo:block text-align="start" wrap-option="wrap" space-before.minimum="0.8em" space-before.optimum="1em" space-before.maximum="1.2em" space-after.minimum="0.8em" space-after.optimum="1em" space-after.maximum="1.2em" hyphenate="false" white-space-collapse="false" white-space-treatment="preserve" linefeed-treatment="preserve" font-family="Bitstream Vera Sans" font-weight="normal" font-style="normal" font-size="8pt">
      <xsl:apply-templates/>
    </fo:block>
  </xsl:template>


  
</xsl:stylesheet>
