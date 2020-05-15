<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<xsl:stylesheet xmlns:xs="http://www.w3.org/2001/XMLSchema"
                xmlns:xsd="http://www.w3.org/2001/XMLSchema"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:sch="http://www.ascc.net/xml/schematron"
                xmlns:iso="http://purl.oclc.org/dsdl/schematron"
                xmlns:gml="http://www.opengis.net/gml"
                xmlns:bldg="http://www.opengis.net/citygml/building/2.0"
                gml:dummy-for-xmlns=""
                bldg:dummy-for-xmlns=""
                version="2.0"><!--Implementers: please note that overriding process-prolog or process-root is 
    the preferred method for meta-stylesheets to use where possible. The name or details of 
    this mode may change during 1Q 2007.-->


<!--PHASES-->


<!--PROLOG-->
   <xsl:output xmlns:svrl="http://purl.oclc.org/dsdl/svrl"
               method="xml"
               omit-xml-declaration="no"
               standalone="yes"
               indent="yes"/>

   <!--KEYS-->


   <!--DEFAULT RULES-->


   <!--MODE: SCHEMATRON-FULL-PATH-->
   <!--This mode can be used to generate an ugly though full XPath for locators-->
   <xsl:template match="*" mode="schematron-get-full-path">
      <xsl:apply-templates select="parent::*" mode="schematron-get-full-path"/>
      <xsl:text>/</xsl:text>
      <xsl:choose>
         <xsl:when test="namespace-uri()=''">
            <xsl:value-of select="name()"/>
         </xsl:when>
         <xsl:otherwise>
            <xsl:text>*:</xsl:text>
            <xsl:value-of select="local-name()"/>
            <xsl:text>[namespace-uri()='</xsl:text>
            <xsl:value-of select="namespace-uri()"/>
            <xsl:text>']</xsl:text>
         </xsl:otherwise>
      </xsl:choose>
      <xsl:variable name="preceding"
                    select="count(preceding-sibling::*[local-name()=local-name(current())                                   and namespace-uri() = namespace-uri(current())])"/>
      <xsl:text>[</xsl:text>
      <xsl:value-of select="1+ $preceding"/>
      <xsl:text>]</xsl:text>
   </xsl:template>
   <xsl:template match="@*" mode="schematron-get-full-path">
      <xsl:apply-templates select="parent::*" mode="schematron-get-full-path"/>
      <xsl:text>/</xsl:text>
      <xsl:choose>
         <xsl:when test="namespace-uri()=''">@schema</xsl:when>
         <xsl:otherwise>
            <xsl:text>@*[local-name()='</xsl:text>
            <xsl:value-of select="local-name()"/>
            <xsl:text>' and namespace-uri()='</xsl:text>
            <xsl:value-of select="namespace-uri()"/>
            <xsl:text>']</xsl:text>
         </xsl:otherwise>
      </xsl:choose>
   </xsl:template>

   <!--MODE: SCHEMATRON-FULL-PATH-2-->
   <!--This mode can be used to generate prefixed XPath for humans-->
   <xsl:template match="node() | @*" mode="schematron-get-full-path-2">
      <xsl:for-each select="ancestor-or-self::*">
         <xsl:text>/</xsl:text>
         <xsl:value-of select="name(.)"/>
         <xsl:if test="preceding-sibling::*[name(.)=name(current())]">
            <xsl:text>[</xsl:text>
            <xsl:value-of select="count(preceding-sibling::*[name(.)=name(current())])+1"/>
            <xsl:text>]</xsl:text>
         </xsl:if>
      </xsl:for-each>
      <xsl:if test="not(self::*)">
         <xsl:text/>/@<xsl:value-of select="name(.)"/>
      </xsl:if>
   </xsl:template>

   <!--MODE: GENERATE-ID-FROM-PATH -->
   <xsl:template match="/" mode="generate-id-from-path"/>
   <xsl:template match="text()" mode="generate-id-from-path">
      <xsl:apply-templates select="parent::*" mode="generate-id-from-path"/>
      <xsl:value-of select="concat('.text-', 1+count(preceding-sibling::text()), '-')"/>
   </xsl:template>
   <xsl:template match="comment()" mode="generate-id-from-path">
      <xsl:apply-templates select="parent::*" mode="generate-id-from-path"/>
      <xsl:value-of select="concat('.comment-', 1+count(preceding-sibling::comment()), '-')"/>
   </xsl:template>
   <xsl:template match="processing-instruction()" mode="generate-id-from-path">
      <xsl:apply-templates select="parent::*" mode="generate-id-from-path"/>
      <xsl:value-of select="concat('.processing-instruction-', 1+count(preceding-sibling::processing-instruction()), '-')"/>
   </xsl:template>
   <xsl:template match="@*" mode="generate-id-from-path">
      <xsl:apply-templates select="parent::*" mode="generate-id-from-path"/>
      <xsl:value-of select="concat('.@', name())"/>
   </xsl:template>
   <xsl:template match="*" mode="generate-id-from-path" priority="-0.5">
      <xsl:apply-templates select="parent::*" mode="generate-id-from-path"/>
      <xsl:text>.</xsl:text>
      <xsl:choose>
         <xsl:when test="count(. | ../namespace::*) = count(../namespace::*)">
            <xsl:value-of select="concat('.namespace::-',1+count(namespace::*),'-')"/>
         </xsl:when>
         <xsl:otherwise>
            <xsl:value-of select="concat('.',name(),'-',1+count(preceding-sibling::*[name()=name(current())]),'-')"/>
         </xsl:otherwise>
      </xsl:choose>
   </xsl:template>

   <!--MODE: GENERATE-ID-2 -->
   <xsl:template match="/" mode="generate-id-2">U</xsl:template>
   <xsl:template match="*" mode="generate-id-2" priority="2">
      <xsl:text>U</xsl:text>
      <xsl:number level="multiple" count="*"/>
   </xsl:template>
   <xsl:template match="node()" mode="generate-id-2">
      <xsl:text>U.</xsl:text>
      <xsl:number level="multiple" count="*"/>
      <xsl:text>n</xsl:text>
      <xsl:number count="node()"/>
   </xsl:template>
   <xsl:template match="@*" mode="generate-id-2">
      <xsl:text>U.</xsl:text>
      <xsl:number level="multiple" count="*"/>
      <xsl:text>_</xsl:text>
      <xsl:value-of select="string-length(local-name(.))"/>
      <xsl:text>_</xsl:text>
      <xsl:value-of select="translate(name(),':','.')"/>
   </xsl:template>
   <!--Strip characters-->
   <xsl:template match="text()" priority="-1"/>

   <!--SCHEMA METADATA-->
   <xsl:template match="/">
      <svrl:schematron-output xmlns:svrl="http://purl.oclc.org/dsdl/svrl" title="" schemaVersion="">
         <svrl:ns-prefix-in-attribute-values uri="http://www.opengis.net/gml" prefix="gml"/>
         <svrl:ns-prefix-in-attribute-values uri="http://www.opengis.net/citygml/building/2.0" prefix="bldg"/>
         <svrl:active-pattern>
            <xsl:attribute name="id">CO-bldg-BU-001</xsl:attribute>
            <xsl:attribute name="name">CO-bldg-BU-001</xsl:attribute>
            <xsl:apply-templates/>
         </svrl:active-pattern>
         <xsl:apply-templates select="/" mode="M2"/>
         <svrl:active-pattern>
            <xsl:attribute name="id">CO-bldg-BU-002-1</xsl:attribute>
            <xsl:attribute name="name">CO-bldg-BU-002-1</xsl:attribute>
            <xsl:apply-templates/>
         </svrl:active-pattern>
         <xsl:apply-templates select="/" mode="M3"/>
         <svrl:active-pattern>
            <xsl:attribute name="id">CO-bldg-BU-002-2</xsl:attribute>
            <xsl:attribute name="name">CO-bldg-BU-002-2</xsl:attribute>
            <xsl:apply-templates/>
         </svrl:active-pattern>
         <xsl:apply-templates select="/" mode="M4"/>
         <svrl:active-pattern>
            <xsl:attribute name="id">CO-bldg-BU-003</xsl:attribute>
            <xsl:attribute name="name">CO-bldg-BU-003</xsl:attribute>
            <xsl:apply-templates/>
         </svrl:active-pattern>
         <xsl:apply-templates select="/" mode="M5"/>
         <svrl:active-pattern>
            <xsl:attribute name="id">CO-bldg-BU-004</xsl:attribute>
            <xsl:attribute name="name">CO-bldg-BU-004</xsl:attribute>
            <xsl:apply-templates/>
         </svrl:active-pattern>
         <xsl:apply-templates select="/" mode="M6"/>
         <svrl:active-pattern>
            <xsl:attribute name="id">CO-bldg-BU-005</xsl:attribute>
            <xsl:attribute name="name">CO-bldg-BU-005</xsl:attribute>
            <xsl:apply-templates/>
         </svrl:active-pattern>
         <xsl:apply-templates select="/" mode="M7"/>
         <svrl:active-pattern>
            <xsl:attribute name="id">CO-bldg-BU-006</xsl:attribute>
            <xsl:attribute name="name">CO-bldg-BU-006</xsl:attribute>
            <xsl:apply-templates/>
         </svrl:active-pattern>
         <xsl:apply-templates select="/" mode="M8"/>
         <svrl:active-pattern>
            <xsl:attribute name="id">CO-bldg-BU-007</xsl:attribute>
            <xsl:attribute name="name">CO-bldg-BU-007</xsl:attribute>
            <xsl:apply-templates/>
         </svrl:active-pattern>
         <xsl:apply-templates select="/" mode="M9"/>
         <svrl:active-pattern>
            <xsl:attribute name="id">CO-bldg-BU-008</xsl:attribute>
            <xsl:attribute name="name">CO-bldg-BU-008</xsl:attribute>
            <xsl:apply-templates/>
         </svrl:active-pattern>
         <xsl:apply-templates select="/" mode="M10"/>
         <svrl:active-pattern>
            <xsl:attribute name="id">CO-bldg-BU-009</xsl:attribute>
            <xsl:attribute name="name">CO-bldg-BU-009</xsl:attribute>
            <xsl:apply-templates/>
         </svrl:active-pattern>
         <xsl:apply-templates select="/" mode="M11"/>
         <svrl:active-pattern>
            <xsl:attribute name="id">CO-bldg-BU-010</xsl:attribute>
            <xsl:attribute name="name">CO-bldg-BU-010</xsl:attribute>
            <xsl:apply-templates/>
         </svrl:active-pattern>
         <xsl:apply-templates select="/" mode="M12"/>
         <svrl:active-pattern>
            <xsl:attribute name="id">CO-bldg-BU-011</xsl:attribute>
            <xsl:attribute name="name">CO-bldg-BU-011</xsl:attribute>
            <xsl:apply-templates/>
         </svrl:active-pattern>
         <xsl:apply-templates select="/" mode="M13"/>
      </svrl:schematron-output>
   </xsl:template>

   <!--SCHEMATRON PATTERNS-->


   <!--PATTERN CO-bldg-BU-001-->


	  <!--RULE -->
   <xsl:template match="bldg:BuildingPart" priority="4000" mode="M2">
      <svrl:fired-rule xmlns:svrl="http://purl.oclc.org/dsdl/svrl" context="bldg:BuildingPart"/>

		    <!--ASSERT -->
      <xsl:choose>
         <xsl:when test="count(../../self::bldg:Building) = 1"/>
         <xsl:otherwise>
            <svrl:failed-assert xmlns:svrl="http://purl.oclc.org/dsdl/svrl"
                                test="count(../../self::bldg:Building) = 1">
               <xsl:attribute name="location">
                  <xsl:apply-templates select="." mode="schematron-get-full-path"/>
               </xsl:attribute>
               <svrl:text>
                  <xsl:text/>
                  <xsl:value-of select="name(.)"/>
                  <xsl:text/> with id <xsl:text/>
                  <xsl:value-of select="@gml:id"/>
                  <xsl:text/> has no Building as parent</svrl:text>
            </svrl:failed-assert>
         </xsl:otherwise>
      </xsl:choose>
      <xsl:apply-templates select="@*|*|comment()|processing-instruction()" mode="M2"/>
   </xsl:template>
   <xsl:template match="text()" priority="-1" mode="M2"/>
   <xsl:template match="@*|node()" priority="-2" mode="M2">
      <xsl:choose><!--Housekeeping: SAXON warns if attempting to find the attribute
                           of an attribute-->
         <xsl:when test="not(@*)">
            <xsl:apply-templates select="node()" mode="M2"/>
         </xsl:when>
         <xsl:otherwise>
            <xsl:apply-templates select="@*|node()" mode="M2"/>
         </xsl:otherwise>
      </xsl:choose>
   </xsl:template>

   <!--PATTERN CO-bldg-BU-002-1-->


	  <!--RULE -->
   <xsl:template match="bldg:Building | bldg:BuildingPart" priority="4000" mode="M3">
      <svrl:fired-rule xmlns:svrl="http://purl.oclc.org/dsdl/svrl"
                       context="bldg:Building | bldg:BuildingPart"/>

		    <!--ASSERT -->
      <xsl:choose>
         <xsl:when test="if (child::bldg:lod0FootPrint/descendant::gml:posList) then                 count(tokenize(child::bldg:lod0FootPrint/descendant::gml:posList/normalize-space(.),'\s+')) mod 3 = 0                 else true()"/>
         <xsl:otherwise>
            <svrl:failed-assert xmlns:svrl="http://purl.oclc.org/dsdl/svrl"
                                test="if (child::bldg:lod0FootPrint/descendant::gml:posList) then count(tokenize(child::bldg:lod0FootPrint/descendant::gml:posList/normalize-space(.),'\s+')) mod 3 = 0 else true()">
               <xsl:attribute name="location">
                  <xsl:apply-templates select="." mode="schematron-get-full-path"/>
               </xsl:attribute>
               <svrl:text>
                  <xsl:text/>
                  <xsl:value-of select="name(.)"/>
                  <xsl:text/> with id <xsl:text/>
                  <xsl:value-of select="@gml:id"/>
                  <xsl:text/> contains an lod0FootPrint with non-3D geometry (number of coordinates in gml:posList element is not a multiple of 3).
            </svrl:text>
            </svrl:failed-assert>
         </xsl:otherwise>
      </xsl:choose>

		    <!--ASSERT -->
      <xsl:choose>
         <xsl:when test="if (child::bldg:lod0FootPrint/descendant::gml:pos) then                 count(tokenize(child::bldg:lod0FootPrint/descendant::gml:pos/normalize-space(.),'\s+')) = 3                 else true()"/>
         <xsl:otherwise>
            <svrl:failed-assert xmlns:svrl="http://purl.oclc.org/dsdl/svrl"
                                test="if (child::bldg:lod0FootPrint/descendant::gml:pos) then count(tokenize(child::bldg:lod0FootPrint/descendant::gml:pos/normalize-space(.),'\s+')) = 3 else true()">
               <xsl:attribute name="location">
                  <xsl:apply-templates select="." mode="schematron-get-full-path"/>
               </xsl:attribute>
               <svrl:text>
                  <xsl:text/>
                  <xsl:value-of select="name(.)"/>
                  <xsl:text/> with id <xsl:text/>
                  <xsl:value-of select="@gml:id"/>
                  <xsl:text/> contains an lod0FootPrint with non-3D geometry (number of coordinates in gml:pos element is not 3).
            </svrl:text>
            </svrl:failed-assert>
         </xsl:otherwise>
      </xsl:choose>
      <xsl:apply-templates select="@*|*|comment()|processing-instruction()" mode="M3"/>
   </xsl:template>
   <xsl:template match="text()" priority="-1" mode="M3"/>
   <xsl:template match="@*|node()" priority="-2" mode="M3">
      <xsl:choose><!--Housekeeping: SAXON warns if attempting to find the attribute
                           of an attribute-->
         <xsl:when test="not(@*)">
            <xsl:apply-templates select="node()" mode="M3"/>
         </xsl:when>
         <xsl:otherwise>
            <xsl:apply-templates select="@*|node()" mode="M3"/>
         </xsl:otherwise>
      </xsl:choose>
   </xsl:template>

   <!--PATTERN CO-bldg-BU-002-2-->


	  <!--RULE -->
   <xsl:template match="bldg:Building | bldg:BuildingPart" priority="4000" mode="M4">
      <svrl:fired-rule xmlns:svrl="http://purl.oclc.org/dsdl/svrl"
                       context="bldg:Building | bldg:BuildingPart"/>

		    <!--ASSERT -->
      <xsl:choose>
         <xsl:when test="if (child::bldg:lod0RoofEdge/descendant::gml:posList) then                 count(tokenize(child::bldg:lod0RoofEdge/descendant::gml:posList/normalize-space(.),'\s+')) mod 3 = 0                 else true()"/>
         <xsl:otherwise>
            <svrl:failed-assert xmlns:svrl="http://purl.oclc.org/dsdl/svrl"
                                test="if (child::bldg:lod0RoofEdge/descendant::gml:posList) then count(tokenize(child::bldg:lod0RoofEdge/descendant::gml:posList/normalize-space(.),'\s+')) mod 3 = 0 else true()">
               <xsl:attribute name="location">
                  <xsl:apply-templates select="." mode="schematron-get-full-path"/>
               </xsl:attribute>
               <svrl:text>
                  <xsl:text/>
                  <xsl:value-of select="name(.)"/>
                  <xsl:text/> with id <xsl:text/>
                  <xsl:value-of select="@gml:id"/>
                  <xsl:text/> contains an lod0RoofEdge with non-3D geometry (number of coordinates in gml:posList element is not a multiple of 3).
            </svrl:text>
            </svrl:failed-assert>
         </xsl:otherwise>
      </xsl:choose>

		    <!--ASSERT -->
      <xsl:choose>
         <xsl:when test="if (child::bldg:lod0RoofEdge/descendant::gml:pos) then                 count(tokenize(child::bldg:lod0RoofEdge/descendant::gml:pos/normalize-space(.),'\s+')) = 3                 else true()"/>
         <xsl:otherwise>
            <svrl:failed-assert xmlns:svrl="http://purl.oclc.org/dsdl/svrl"
                                test="if (child::bldg:lod0RoofEdge/descendant::gml:pos) then count(tokenize(child::bldg:lod0RoofEdge/descendant::gml:pos/normalize-space(.),'\s+')) = 3 else true()">
               <xsl:attribute name="location">
                  <xsl:apply-templates select="." mode="schematron-get-full-path"/>
               </xsl:attribute>
               <svrl:text>
                  <xsl:text/>
                  <xsl:value-of select="name(.)"/>
                  <xsl:text/> with id <xsl:text/>
                  <xsl:value-of select="@gml:id"/>
                  <xsl:text/> contains an lod0RoofEdge with non-3D geometry (number of coordinates in gml:pos element is not 3).
            </svrl:text>
            </svrl:failed-assert>
         </xsl:otherwise>
      </xsl:choose>
      <xsl:apply-templates select="@*|*|comment()|processing-instruction()" mode="M4"/>
   </xsl:template>
   <xsl:template match="text()" priority="-1" mode="M4"/>
   <xsl:template match="@*|node()" priority="-2" mode="M4">
      <xsl:choose><!--Housekeeping: SAXON warns if attempting to find the attribute
                           of an attribute-->
         <xsl:when test="not(@*)">
            <xsl:apply-templates select="node()" mode="M4"/>
         </xsl:when>
         <xsl:otherwise>
            <xsl:apply-templates select="@*|node()" mode="M4"/>
         </xsl:otherwise>
      </xsl:choose>
   </xsl:template>

   <!--PATTERN CO-bldg-BU-003-->


	  <!--RULE -->
   <xsl:template match="bldg:BuildingPart | bldg:Building" priority="4000" mode="M5">
      <svrl:fired-rule xmlns:svrl="http://purl.oclc.org/dsdl/svrl"
                       context="bldg:BuildingPart | bldg:Building"/>

		    <!--REPORT -->
      <xsl:if test="child::bldg:lod1Solid and child::bldg:lod1MultiSurface">
         <svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl"
                                 test="child::bldg:lod1Solid and child::bldg:lod1MultiSurface">
            <xsl:attribute name="location">
               <xsl:apply-templates select="." mode="schematron-get-full-path"/>
            </xsl:attribute>
            <svrl:text>
                <xsl:text/>
               <xsl:value-of select="name(.)"/>
               <xsl:text/> with id <xsl:text/>
               <xsl:value-of select="@gml:id"/>
               <xsl:text/> consists of both Solid and MultiSurface LoD1 geometry.
            </svrl:text>
         </svrl:successful-report>
      </xsl:if>
      <xsl:apply-templates select="@*|*|comment()|processing-instruction()" mode="M5"/>
   </xsl:template>
   <xsl:template match="text()" priority="-1" mode="M5"/>
   <xsl:template match="@*|node()" priority="-2" mode="M5">
      <xsl:choose><!--Housekeeping: SAXON warns if attempting to find the attribute
                           of an attribute-->
         <xsl:when test="not(@*)">
            <xsl:apply-templates select="node()" mode="M5"/>
         </xsl:when>
         <xsl:otherwise>
            <xsl:apply-templates select="@*|node()" mode="M5"/>
         </xsl:otherwise>
      </xsl:choose>
   </xsl:template>

   <!--PATTERN CO-bldg-BU-004-->


	  <!--RULE -->
   <xsl:template match="bldg:BuildingPart | bldg:Building" priority="4000" mode="M6">
      <svrl:fired-rule xmlns:svrl="http://purl.oclc.org/dsdl/svrl"
                       context="bldg:BuildingPart | bldg:Building"/>

		    <!--REPORT -->
      <xsl:if test="child::bldg:boundedBy/child::bldg:CeilingSurface">
         <svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl"
                                 test="child::bldg:boundedBy/child::bldg:CeilingSurface">
            <xsl:attribute name="location">
               <xsl:apply-templates select="." mode="schematron-get-full-path"/>
            </xsl:attribute>
            <svrl:text>
                <xsl:text/>
               <xsl:value-of select="name(.)"/>
               <xsl:text/> with id <xsl:text/>
               <xsl:value-of select="@gml:id"/>
               <xsl:text/> refers to CeilingSurface elements
            </svrl:text>
         </svrl:successful-report>
      </xsl:if>

		    <!--REPORT -->
      <xsl:if test="child::bldg:boundedBy/child::bldg:InteriorWallSurface">
         <svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl"
                                 test="child::bldg:boundedBy/child::bldg:InteriorWallSurface">
            <xsl:attribute name="location">
               <xsl:apply-templates select="." mode="schematron-get-full-path"/>
            </xsl:attribute>
            <svrl:text>
                <xsl:text/>
               <xsl:value-of select="name(.)"/>
               <xsl:text/> with id <xsl:text/>
               <xsl:value-of select="@gml:id"/>
               <xsl:text/> refers to InteriorWallSurface elements
            </svrl:text>
         </svrl:successful-report>
      </xsl:if>

		    <!--REPORT -->
      <xsl:if test="child::bldg:boundedBy/child::bldg:FloorSurface">
         <svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl"
                                 test="child::bldg:boundedBy/child::bldg:FloorSurface">
            <xsl:attribute name="location">
               <xsl:apply-templates select="." mode="schematron-get-full-path"/>
            </xsl:attribute>
            <svrl:text>
                <xsl:text/>
               <xsl:value-of select="name(.)"/>
               <xsl:text/> with id <xsl:text/>
               <xsl:value-of select="@gml:id"/>
               <xsl:text/> refers to  FloorSurface elements
            </svrl:text>
         </svrl:successful-report>
      </xsl:if>
      <xsl:apply-templates select="@*|*|comment()|processing-instruction()" mode="M6"/>
   </xsl:template>
   <xsl:template match="text()" priority="-1" mode="M6"/>
   <xsl:template match="@*|node()" priority="-2" mode="M6">
      <xsl:choose><!--Housekeeping: SAXON warns if attempting to find the attribute
                           of an attribute-->
         <xsl:when test="not(@*)">
            <xsl:apply-templates select="node()" mode="M6"/>
         </xsl:when>
         <xsl:otherwise>
            <xsl:apply-templates select="@*|node()" mode="M6"/>
         </xsl:otherwise>
      </xsl:choose>
   </xsl:template>

   <!--PATTERN CO-bldg-BU-005-->


	  <!--RULE -->
   <xsl:template match="bldg:BuildingPart | bldg:Building" priority="4000" mode="M7">
      <svrl:fired-rule xmlns:svrl="http://purl.oclc.org/dsdl/svrl"
                       context="bldg:BuildingPart | bldg:Building"/>

		    <!--ASSERT -->
      <xsl:choose>
         <xsl:when test="if (child::bldg:lod2MultiCurve )                 then                  child::bldg:lod2Solid or                  child::bldg:lod2MultiSurface or                 child::bldg:boundedBy/child::bldg:lod2MultiSurface                 else true()"/>
         <xsl:otherwise>
            <svrl:failed-assert xmlns:svrl="http://purl.oclc.org/dsdl/svrl"
                                test="if (child::bldg:lod2MultiCurve ) then child::bldg:lod2Solid or child::bldg:lod2MultiSurface or child::bldg:boundedBy/child::bldg:lod2MultiSurface else true()">
               <xsl:attribute name="location">
                  <xsl:apply-templates select="." mode="schematron-get-full-path"/>
               </xsl:attribute>
               <svrl:text>
                  <xsl:text/>
                  <xsl:value-of select="name(.)"/>
                  <xsl:text/> with id <xsl:text/>
                  <xsl:value-of select="@gml:id"/>
                  <xsl:text/> contains a lod2MultiCurve property without other LoD2 geometry
            </svrl:text>
            </svrl:failed-assert>
         </xsl:otherwise>
      </xsl:choose>

		    <!--ASSERT -->
      <xsl:choose>
         <xsl:when test="if ( child::bldg:lod3MultiCurve )                 then                                 child::bldg:lod3Solid or                  child::bldg:lod3MultiSurface or                 child::bldg:boundedBy/child::bldg:lod3MultiSurface                 else true()"/>
         <xsl:otherwise>
            <svrl:failed-assert xmlns:svrl="http://purl.oclc.org/dsdl/svrl"
                                test="if ( child::bldg:lod3MultiCurve ) then child::bldg:lod3Solid or child::bldg:lod3MultiSurface or child::bldg:boundedBy/child::bldg:lod3MultiSurface else true()">
               <xsl:attribute name="location">
                  <xsl:apply-templates select="." mode="schematron-get-full-path"/>
               </xsl:attribute>
               <svrl:text>
                  <xsl:text/>
                  <xsl:value-of select="name(.)"/>
                  <xsl:text/> with id <xsl:text/>
                  <xsl:value-of select="@gml:id"/>
                  <xsl:text/> contains a lod3MultiCurve property without other LoD3 geometry
            </svrl:text>
            </svrl:failed-assert>
         </xsl:otherwise>
      </xsl:choose>

		    <!--ASSERT -->
      <xsl:choose>
         <xsl:when test="if ( child::bldg:lod4MultiCurve )                 then                                  child::bldg:lod4Solid or                  child::bldg:lod4MultiSurface or                 child::bldg:boundedBy/child::bldg:lod4MultiSurface                 else true()"/>
         <xsl:otherwise>
            <svrl:failed-assert xmlns:svrl="http://purl.oclc.org/dsdl/svrl"
                                test="if ( child::bldg:lod4MultiCurve ) then child::bldg:lod4Solid or child::bldg:lod4MultiSurface or child::bldg:boundedBy/child::bldg:lod4MultiSurface else true()">
               <xsl:attribute name="location">
                  <xsl:apply-templates select="." mode="schematron-get-full-path"/>
               </xsl:attribute>
               <svrl:text>
                  <xsl:text/>
                  <xsl:value-of select="name(.)"/>
                  <xsl:text/> with id <xsl:text/>
                  <xsl:value-of select="@gml:id"/>
                  <xsl:text/> contains a lod4MultiCurve property without other LoD4 geometry
            </svrl:text>
            </svrl:failed-assert>
         </xsl:otherwise>
      </xsl:choose>
      <xsl:apply-templates select="@*|*|comment()|processing-instruction()" mode="M7"/>
   </xsl:template>
   <xsl:template match="text()" priority="-1" mode="M7"/>
   <xsl:template match="@*|node()" priority="-2" mode="M7">
      <xsl:choose><!--Housekeeping: SAXON warns if attempting to find the attribute
                           of an attribute-->
         <xsl:when test="not(@*)">
            <xsl:apply-templates select="node()" mode="M7"/>
         </xsl:when>
         <xsl:otherwise>
            <xsl:apply-templates select="@*|node()" mode="M7"/>
         </xsl:otherwise>
      </xsl:choose>
   </xsl:template>

   <!--PATTERN CO-bldg-BU-006-->


	  <!--RULE -->
   <xsl:template match="bldg:BuildingPart | bldg:Building" priority="4000" mode="M8">
      <svrl:fired-rule xmlns:svrl="http://purl.oclc.org/dsdl/svrl"
                       context="bldg:BuildingPart | bldg:Building"/>

		    <!--ASSERT -->
      <xsl:choose>
         <xsl:when test="if (child::bldg:outerBuildingInstallation/child::bldg:lod2Geometry or                               child::bldg:outerBuildingInstallation/child::bldg:lod2ImplicitRepresentation )                 then                  ( child::bldg:lod2Solid or                    child::bldg:lod2MultiSurface or                   child::bldg:boundedBy/child::bldg:lod2MultiSurface )                 else true()"/>
         <xsl:otherwise>
            <svrl:failed-assert xmlns:svrl="http://purl.oclc.org/dsdl/svrl"
                                test="if (child::bldg:outerBuildingInstallation/child::bldg:lod2Geometry or child::bldg:outerBuildingInstallation/child::bldg:lod2ImplicitRepresentation ) then ( child::bldg:lod2Solid or child::bldg:lod2MultiSurface or child::bldg:boundedBy/child::bldg:lod2MultiSurface ) else true()">
               <xsl:attribute name="location">
                  <xsl:apply-templates select="." mode="schematron-get-full-path"/>
               </xsl:attribute>
               <svrl:text>
                  <xsl:text/>
                  <xsl:value-of select="name(.)"/>
                  <xsl:text/> with id <xsl:text/>
                  <xsl:value-of select="@gml:id"/>
                  <xsl:text/> has a LoD2 BuildingInstallation, but no LoD2 geometry.
            </svrl:text>
            </svrl:failed-assert>
         </xsl:otherwise>
      </xsl:choose>

		    <!--ASSERT -->
      <xsl:choose>
         <xsl:when test="if (child::bldg:outerBuildingInstallation/child::bldg:lod3Geometry or                               child::bldg:outerBuildingInstallation/child::bldg:lod3ImplicitRepresentation )                 then                  ( child::bldg:lod3Solid or                    child::bldg:lod3MultiSurface or                   child::bldg:boundedBy/child::bldg:lod3MultiSurface )                 else true()"/>
         <xsl:otherwise>
            <svrl:failed-assert xmlns:svrl="http://purl.oclc.org/dsdl/svrl"
                                test="if (child::bldg:outerBuildingInstallation/child::bldg:lod3Geometry or child::bldg:outerBuildingInstallation/child::bldg:lod3ImplicitRepresentation ) then ( child::bldg:lod3Solid or child::bldg:lod3MultiSurface or child::bldg:boundedBy/child::bldg:lod3MultiSurface ) else true()">
               <xsl:attribute name="location">
                  <xsl:apply-templates select="." mode="schematron-get-full-path"/>
               </xsl:attribute>
               <svrl:text>
                  <xsl:text/>
                  <xsl:value-of select="name(.)"/>
                  <xsl:text/> with id:<xsl:text/>
                  <xsl:value-of select="@gml:id"/>
                  <xsl:text/> has a LoD3 BuildingInstallation, but no LoD3 geometry.
            </svrl:text>
            </svrl:failed-assert>
         </xsl:otherwise>
      </xsl:choose>

		    <!--ASSERT -->
      <xsl:choose>
         <xsl:when test="if (child::bldg:outerBuildingInstallation/child::bldg:lod4Geometry or                               child::bldg:outerBuildingInstallation/child::bldg:lod4ImplicitRepresentation )                 then                  ( child::bldg:lod4Solid or                   child::bldg:lod4MultiSurface or                  child::bldg:boundedBy/child::bldg:lod4MultiSurface )                 else true()"/>
         <xsl:otherwise>
            <svrl:failed-assert xmlns:svrl="http://purl.oclc.org/dsdl/svrl"
                                test="if (child::bldg:outerBuildingInstallation/child::bldg:lod4Geometry or child::bldg:outerBuildingInstallation/child::bldg:lod4ImplicitRepresentation ) then ( child::bldg:lod4Solid or child::bldg:lod4MultiSurface or child::bldg:boundedBy/child::bldg:lod4MultiSurface ) else true()">
               <xsl:attribute name="location">
                  <xsl:apply-templates select="." mode="schematron-get-full-path"/>
               </xsl:attribute>
               <svrl:text>
                  <xsl:text/>
                  <xsl:value-of select="name(.)"/>
                  <xsl:text/> with id:<xsl:text/>
                  <xsl:value-of select="@gml:id"/>
                  <xsl:text/> has a LoD4 BuildingInstallation, but no LoD4 geometry.
            </svrl:text>
            </svrl:failed-assert>
         </xsl:otherwise>
      </xsl:choose>
      <xsl:apply-templates select="@*|*|comment()|processing-instruction()" mode="M8"/>
   </xsl:template>
   <xsl:template match="text()" priority="-1" mode="M8"/>
   <xsl:template match="@*|node()" priority="-2" mode="M8">
      <xsl:choose><!--Housekeeping: SAXON warns if attempting to find the attribute
                           of an attribute-->
         <xsl:when test="not(@*)">
            <xsl:apply-templates select="node()" mode="M8"/>
         </xsl:when>
         <xsl:otherwise>
            <xsl:apply-templates select="@*|node()" mode="M8"/>
         </xsl:otherwise>
      </xsl:choose>
   </xsl:template>

   <!--PATTERN CO-bldg-BU-007-->


	  <!--RULE -->
   <xsl:template match="bldg:BuildingInstallation" priority="4000" mode="M9">
      <svrl:fired-rule xmlns:svrl="http://purl.oclc.org/dsdl/svrl"
                       context="bldg:BuildingInstallation"/>

		    <!--REPORT -->
      <xsl:if test="child::bldg:boundedBy/child::bldg:CeilingSurface">
         <svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl"
                                 test="child::bldg:boundedBy/child::bldg:CeilingSurface">
            <xsl:attribute name="location">
               <xsl:apply-templates select="." mode="schematron-get-full-path"/>
            </xsl:attribute>
            <svrl:text>
                <xsl:text/>
               <xsl:value-of select="name(.)"/>
               <xsl:text/> with id <xsl:text/>
               <xsl:value-of select="@gml:id"/>
               <xsl:text/> refers to CeilingSurface elements 
            </svrl:text>
         </svrl:successful-report>
      </xsl:if>

		    <!--REPORT -->
      <xsl:if test="child::bldg:boundedBy/child::bldg:InteriorWallSurface">
         <svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl"
                                 test="child::bldg:boundedBy/child::bldg:InteriorWallSurface">
            <xsl:attribute name="location">
               <xsl:apply-templates select="." mode="schematron-get-full-path"/>
            </xsl:attribute>
            <svrl:text>
                <xsl:text/>
               <xsl:value-of select="name(.)"/>
               <xsl:text/> with id <xsl:text/>
               <xsl:value-of select="@gml:id"/>
               <xsl:text/> refers toInteriorWallSurface elements
            </svrl:text>
         </svrl:successful-report>
      </xsl:if>

		    <!--REPORT -->
      <xsl:if test="child::bldg:boundedBy/child::bldg:FloorSurface">
         <svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl"
                                 test="child::bldg:boundedBy/child::bldg:FloorSurface">
            <xsl:attribute name="location">
               <xsl:apply-templates select="." mode="schematron-get-full-path"/>
            </xsl:attribute>
            <svrl:text>
                <xsl:text/>
               <xsl:value-of select="name(.)"/>
               <xsl:text/> with id <xsl:text/>
               <xsl:value-of select="@gml:id"/>
               <xsl:text/> refers to FloorSurface elements 
            </svrl:text>
         </svrl:successful-report>
      </xsl:if>
      <xsl:apply-templates select="@*|*|comment()|processing-instruction()" mode="M9"/>
   </xsl:template>
   <xsl:template match="text()" priority="-1" mode="M9"/>
   <xsl:template match="@*|node()" priority="-2" mode="M9">
      <xsl:choose><!--Housekeeping: SAXON warns if attempting to find the attribute
                           of an attribute-->
         <xsl:when test="not(@*)">
            <xsl:apply-templates select="node()" mode="M9"/>
         </xsl:when>
         <xsl:otherwise>
            <xsl:apply-templates select="@*|node()" mode="M9"/>
         </xsl:otherwise>
      </xsl:choose>
   </xsl:template>

   <!--PATTERN CO-bldg-BU-008-->


	  <!--RULE -->
   <xsl:template match="bldg:RoofSurface |                         bldg:WallSurface |                         bldg:GroundSurface |                         bldg:ClosureSurface |                         bldg:CeilingSurface |                        bldg:InteriorWallSurface |                         bldg:FloorSurface |                         bldg:OuterCeilingSurface |                         bldg:OuterFloorSurface"
                 priority="4000"
                 mode="M10">
      <svrl:fired-rule xmlns:svrl="http://purl.oclc.org/dsdl/svrl"
                       context="bldg:RoofSurface |                         bldg:WallSurface |                         bldg:GroundSurface |                         bldg:ClosureSurface |                         bldg:CeilingSurface |                        bldg:InteriorWallSurface |                         bldg:FloorSurface |                         bldg:OuterCeilingSurface |                         bldg:OuterFloorSurface"/>

		    <!--ASSERT -->
      <xsl:choose>
         <xsl:when test="if (child::bldg:Door/child::bldg:lod3MultiSurface |                               child::bldg:Window/child::bldg:lod3MultiSurface |                               child::bldg:Window/child::bldg:lod3ImplicitRepresentation |                               child::bldg:Door/child::bldg:lod3ImplicitRepresentation) then                               child::bldg:lod3MultiSurface else                          true()"/>
         <xsl:otherwise>
            <svrl:failed-assert xmlns:svrl="http://purl.oclc.org/dsdl/svrl"
                                test="if (child::bldg:Door/child::bldg:lod3MultiSurface | child::bldg:Window/child::bldg:lod3MultiSurface | child::bldg:Window/child::bldg:lod3ImplicitRepresentation | child::bldg:Door/child::bldg:lod3ImplicitRepresentation) then child::bldg:lod3MultiSurface else true()">
               <xsl:attribute name="location">
                  <xsl:apply-templates select="." mode="schematron-get-full-path"/>
               </xsl:attribute>
               <svrl:text>
                  <xsl:text/>
                  <xsl:value-of select="name(.)"/>
                  <xsl:text/> with id <xsl:text/>
                  <xsl:value-of select="@gml:id"/>
                  <xsl:text/> has an LoD3 opening, but no LoD3 geometry.
            </svrl:text>
            </svrl:failed-assert>
         </xsl:otherwise>
      </xsl:choose>

		    <!--ASSERT -->
      <xsl:choose>
         <xsl:when test="if (child::bldg:Door/child::bldg:lod4MultiSurface |                 child::bldg:Window/child::bldg:lod4MultiSurface |                 child::bldg:Window/child::bldg:lod4ImplicitRepresentation |                 child::bldg:Door/child::bldg:lod4ImplicitRepresentation) then                 child::bldg:lod4MultiSurface else                  true()"/>
         <xsl:otherwise>
            <svrl:failed-assert xmlns:svrl="http://purl.oclc.org/dsdl/svrl"
                                test="if (child::bldg:Door/child::bldg:lod4MultiSurface | child::bldg:Window/child::bldg:lod4MultiSurface | child::bldg:Window/child::bldg:lod4ImplicitRepresentation | child::bldg:Door/child::bldg:lod4ImplicitRepresentation) then child::bldg:lod4MultiSurface else true()">
               <xsl:attribute name="location">
                  <xsl:apply-templates select="." mode="schematron-get-full-path"/>
               </xsl:attribute>
               <svrl:text>
                  <xsl:text/>
                  <xsl:value-of select="name(.)"/>
                  <xsl:text/> with id <xsl:text/>
                  <xsl:value-of select="@gml:id"/>
                  <xsl:text/> has an LoD4 opening, but no LoD4 geometry.
            </svrl:text>
            </svrl:failed-assert>
         </xsl:otherwise>
      </xsl:choose>
      <xsl:apply-templates select="@*|*|comment()|processing-instruction()" mode="M10"/>
   </xsl:template>
   <xsl:template match="text()" priority="-1" mode="M10"/>
   <xsl:template match="@*|node()" priority="-2" mode="M10">
      <xsl:choose><!--Housekeeping: SAXON warns if attempting to find the attribute
                           of an attribute-->
         <xsl:when test="not(@*)">
            <xsl:apply-templates select="node()" mode="M10"/>
         </xsl:when>
         <xsl:otherwise>
            <xsl:apply-templates select="@*|node()" mode="M10"/>
         </xsl:otherwise>
      </xsl:choose>
   </xsl:template>

   <!--PATTERN CO-bldg-BU-009-->


	  <!--RULE -->
   <xsl:template match="bldg:BuildingPart | bldg:Building"
                 priority="4000"
                 mode="M11">
      <svrl:fired-rule xmlns:svrl="http://purl.oclc.org/dsdl/svrl"
                       context="bldg:BuildingPart | bldg:Building"/>

		    <!--ASSERT -->
      <xsl:choose>
         <xsl:when test="if (child::bldg:interiorRoom)                 then                   ( child::bldg:lod4Solid or                   child::bldg:lod4MultiSurface or                   child::bldg:boundedBy/child::bldg:lod4MultiSurface )                 else true()"/>
         <xsl:otherwise>
            <svrl:failed-assert xmlns:svrl="http://purl.oclc.org/dsdl/svrl"
                                test="if (child::bldg:interiorRoom) then ( child::bldg:lod4Solid or child::bldg:lod4MultiSurface or child::bldg:boundedBy/child::bldg:lod4MultiSurface ) else true()">
               <xsl:attribute name="location">
                  <xsl:apply-templates select="." mode="schematron-get-full-path"/>
               </xsl:attribute>
               <svrl:text>
                  <xsl:text/>
                  <xsl:value-of select="name(.)"/>
                  <xsl:text/> with id <xsl:text/>
                  <xsl:value-of select="@gml:id"/>
                  <xsl:text/> refers to a Room, but has no LoD 4 geometry.
            </svrl:text>
            </svrl:failed-assert>
         </xsl:otherwise>
      </xsl:choose>
      <xsl:apply-templates select="@*|*|comment()|processing-instruction()" mode="M11"/>
   </xsl:template>

	  <!--RULE -->
   <xsl:template match="bldg:Room" priority="3999" mode="M11">
      <svrl:fired-rule xmlns:svrl="http://purl.oclc.org/dsdl/svrl" context="bldg:Room"/>

		    <!--REPORT -->
      <xsl:if test="child::bldg:boundedBy/child::bldg:RoofSurface">
         <svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl"
                                 test="child::bldg:boundedBy/child::bldg:RoofSurface">
            <xsl:attribute name="location">
               <xsl:apply-templates select="." mode="schematron-get-full-path"/>
            </xsl:attribute>
            <svrl:text>
                <xsl:text/>
               <xsl:value-of select="name(.)"/>
               <xsl:text/> with id <xsl:text/>
               <xsl:value-of select="@gml:id"/>
               <xsl:text/> refers to a RoofSurface element 
            </svrl:text>
         </svrl:successful-report>
      </xsl:if>

		    <!--REPORT -->
      <xsl:if test="child::bldg:boundedBy/child::bldg:WallSurface">
         <svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl"
                                 test="child::bldg:boundedBy/child::bldg:WallSurface">
            <xsl:attribute name="location">
               <xsl:apply-templates select="." mode="schematron-get-full-path"/>
            </xsl:attribute>
            <svrl:text>
                <xsl:text/>
               <xsl:value-of select="name(.)"/>
               <xsl:text/> with id <xsl:text/>
               <xsl:value-of select="@gml:id"/>
               <xsl:text/> refers to a WallSurface element 
            </svrl:text>
         </svrl:successful-report>
      </xsl:if>

		    <!--REPORT -->
      <xsl:if test="child::bldg:boundedBy/child::bldg:GroundSurface">
         <svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl"
                                 test="child::bldg:boundedBy/child::bldg:GroundSurface">
            <xsl:attribute name="location">
               <xsl:apply-templates select="." mode="schematron-get-full-path"/>
            </xsl:attribute>
            <svrl:text>
                <xsl:text/>
               <xsl:value-of select="name(.)"/>
               <xsl:text/> with id <xsl:text/>
               <xsl:value-of select="@gml:id"/>
               <xsl:text/> refers to a GroundSurface element 
            </svrl:text>
         </svrl:successful-report>
      </xsl:if>

		    <!--REPORT -->
      <xsl:if test="child::bldg:boundedBy/child::bldg:OuterCeilingSurface">
         <svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl"
                                 test="child::bldg:boundedBy/child::bldg:OuterCeilingSurface">
            <xsl:attribute name="location">
               <xsl:apply-templates select="." mode="schematron-get-full-path"/>
            </xsl:attribute>
            <svrl:text>
                <xsl:text/>
               <xsl:value-of select="name(.)"/>
               <xsl:text/> with id <xsl:text/>
               <xsl:value-of select="@gml:id"/>
               <xsl:text/> refers to an OuterCeilingSurface element 
            </svrl:text>
         </svrl:successful-report>
      </xsl:if>

		    <!--REPORT -->
      <xsl:if test="child::bldg:boundedBy/child::bldg:OuterFloorSurface">
         <svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl"
                                 test="child::bldg:boundedBy/child::bldg:OuterFloorSurface">
            <xsl:attribute name="location">
               <xsl:apply-templates select="." mode="schematron-get-full-path"/>
            </xsl:attribute>
            <svrl:text>
                <xsl:text/>
               <xsl:value-of select="name(.)"/>
               <xsl:text/> with id <xsl:text/>
               <xsl:value-of select="@gml:id"/>
               <xsl:text/> refers to an OuterFloorSurface element 
            </svrl:text>
         </svrl:successful-report>
      </xsl:if>
      <xsl:apply-templates select="@*|*|comment()|processing-instruction()" mode="M11"/>
   </xsl:template>

	  <!--RULE -->
   <xsl:template match="bldg:Room" priority="3998" mode="M11">
      <svrl:fired-rule xmlns:svrl="http://purl.oclc.org/dsdl/svrl" context="bldg:Room"/>

		    <!--REPORT -->
      <xsl:if test="child::bldg:boundedBy/descendant::bldg:lod2MultiSurface">
         <svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl"
                                 test="child::bldg:boundedBy/descendant::bldg:lod2MultiSurface">
            <xsl:attribute name="location">
               <xsl:apply-templates select="." mode="schematron-get-full-path"/>
            </xsl:attribute>
            <svrl:text>
                <xsl:text/>
               <xsl:value-of select="name(.)"/>
               <xsl:text/> with id  <xsl:text/>
               <xsl:value-of select="@gml:id"/>
               <xsl:text/> refers to a LoD2 BoundarySurface
            </svrl:text>
         </svrl:successful-report>
      </xsl:if>

		    <!--REPORT -->
      <xsl:if test="child::bldg:boundedBy/descendant::bldg:lod3MultiSurface">
         <svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl"
                                 test="child::bldg:boundedBy/descendant::bldg:lod3MultiSurface">
            <xsl:attribute name="location">
               <xsl:apply-templates select="." mode="schematron-get-full-path"/>
            </xsl:attribute>
            <svrl:text>
                <xsl:text/>
               <xsl:value-of select="name(.)"/>
               <xsl:text/> with id  <xsl:text/>
               <xsl:value-of select="@gml:id"/>
               <xsl:text/> refers to a LoD3 BoundarySurface
            </svrl:text>
         </svrl:successful-report>
      </xsl:if>
      <xsl:apply-templates select="@*|*|comment()|processing-instruction()" mode="M11"/>
   </xsl:template>
   <xsl:template match="text()" priority="-1" mode="M11"/>
   <xsl:template match="@*|node()" priority="-2" mode="M11">
      <xsl:choose><!--Housekeeping: SAXON warns if attempting to find the attribute
                           of an attribute-->
         <xsl:when test="not(@*)">
            <xsl:apply-templates select="node()" mode="M11"/>
         </xsl:when>
         <xsl:otherwise>
            <xsl:apply-templates select="@*|node()" mode="M11"/>
         </xsl:otherwise>
      </xsl:choose>
   </xsl:template>

   <!--PATTERN CO-bldg-BU-010-->


	  <!--RULE -->
   <xsl:template match="bldg:BuildingPart | bldg:Building"
                 priority="4000"
                 mode="M12">
      <svrl:fired-rule xmlns:svrl="http://purl.oclc.org/dsdl/svrl"
                       context="bldg:BuildingPart | bldg:Building"/>

		    <!--ASSERT -->
      <xsl:choose>
         <xsl:when test="if (child::bldg:interiorBuildingInstallation)                 then                  ( child::bldg:lod4Solid or                   child::bldg:lod4MultiSurface or                   child::bldg:boundedBy/child::bldg:lod4MultiSurface )                 else true()"/>
         <xsl:otherwise>
            <svrl:failed-assert xmlns:svrl="http://purl.oclc.org/dsdl/svrl"
                                test="if (child::bldg:interiorBuildingInstallation) then ( child::bldg:lod4Solid or child::bldg:lod4MultiSurface or child::bldg:boundedBy/child::bldg:lod4MultiSurface ) else true()">
               <xsl:attribute name="location">
                  <xsl:apply-templates select="." mode="schematron-get-full-path"/>
               </xsl:attribute>
               <svrl:text>
                  <xsl:text/>
                  <xsl:value-of select="name(.)"/>
                  <xsl:text/> with id <xsl:text/>
                  <xsl:value-of select="@gml:id"/>
                  <xsl:text/> refers to an IntBuildingInstallation, but has no LoD4 geometry
            </svrl:text>
            </svrl:failed-assert>
         </xsl:otherwise>
      </xsl:choose>
      <xsl:apply-templates select="@*|*|comment()|processing-instruction()" mode="M12"/>
   </xsl:template>
   <xsl:template match="text()" priority="-1" mode="M12"/>
   <xsl:template match="@*|node()" priority="-2" mode="M12">
      <xsl:choose><!--Housekeeping: SAXON warns if attempting to find the attribute
                           of an attribute-->
         <xsl:when test="not(@*)">
            <xsl:apply-templates select="node()" mode="M12"/>
         </xsl:when>
         <xsl:otherwise>
            <xsl:apply-templates select="@*|node()" mode="M12"/>
         </xsl:otherwise>
      </xsl:choose>
   </xsl:template>

   <!--PATTERN CO-bldg-BU-011-->


	  <!--RULE -->
   <xsl:template match="bldg:intBuildingInstallation" priority="4000" mode="M13">
      <svrl:fired-rule xmlns:svrl="http://purl.oclc.org/dsdl/svrl"
                       context="bldg:intBuildingInstallation"/>

		    <!--REPORT -->
      <xsl:if test="child::bldg:boundedBy/child::bldg:RoofSurface">
         <svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl"
                                 test="child::bldg:boundedBy/child::bldg:RoofSurface">
            <xsl:attribute name="location">
               <xsl:apply-templates select="." mode="schematron-get-full-path"/>
            </xsl:attribute>
            <svrl:text>
                <xsl:text/>
               <xsl:value-of select="name(.)"/>
               <xsl:text/> with id <xsl:text/>
               <xsl:value-of select="@gml:id"/>
               <xsl:text/> refers to a RoofSurface element 
            </svrl:text>
         </svrl:successful-report>
      </xsl:if>

		    <!--REPORT -->
      <xsl:if test="child::bldg:boundedBy/child::bldg:WallSurface">
         <svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl"
                                 test="child::bldg:boundedBy/child::bldg:WallSurface">
            <xsl:attribute name="location">
               <xsl:apply-templates select="." mode="schematron-get-full-path"/>
            </xsl:attribute>
            <svrl:text>
                <xsl:text/>
               <xsl:value-of select="name(.)"/>
               <xsl:text/> with id:<xsl:text/>
               <xsl:value-of select="@gml:id"/>
               <xsl:text/> refers to a WallSurface element
            </svrl:text>
         </svrl:successful-report>
      </xsl:if>

		    <!--REPORT -->
      <xsl:if test="child::bldg:boundedBy/child::bldg:GroundSurface">
         <svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl"
                                 test="child::bldg:boundedBy/child::bldg:GroundSurface">
            <xsl:attribute name="location">
               <xsl:apply-templates select="." mode="schematron-get-full-path"/>
            </xsl:attribute>
            <svrl:text>
                <xsl:text/>
               <xsl:value-of select="name(.)"/>
               <xsl:text/> with id:<xsl:text/>
               <xsl:value-of select="@gml:id"/>
               <xsl:text/> refers to a GroundSurface element
            </svrl:text>
         </svrl:successful-report>
      </xsl:if>

		    <!--REPORT -->
      <xsl:if test="child::bldg:boundedBy/child::bldg:OuterCeilingSurface">
         <svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl"
                                 test="child::bldg:boundedBy/child::bldg:OuterCeilingSurface">
            <xsl:attribute name="location">
               <xsl:apply-templates select="." mode="schematron-get-full-path"/>
            </xsl:attribute>
            <svrl:text>
                <xsl:text/>
               <xsl:value-of select="name(.)"/>
               <xsl:text/> with id:<xsl:text/>
               <xsl:value-of select="@gml:id"/>
               <xsl:text/> refers to a OuterCeilingSurface element 
            </svrl:text>
         </svrl:successful-report>
      </xsl:if>

		    <!--REPORT -->
      <xsl:if test="child::bldg:boundedBy/child::bldg:OuterFloorSurface">
         <svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl"
                                 test="child::bldg:boundedBy/child::bldg:OuterFloorSurface">
            <xsl:attribute name="location">
               <xsl:apply-templates select="." mode="schematron-get-full-path"/>
            </xsl:attribute>
            <svrl:text>
                <xsl:text/>
               <xsl:value-of select="name(.)"/>
               <xsl:text/> with id:<xsl:text/>
               <xsl:value-of select="@gml:id"/>
               <xsl:text/> refers to a OuterFloorSurface element 
            </svrl:text>
         </svrl:successful-report>
      </xsl:if>
      <xsl:apply-templates select="@*|*|comment()|processing-instruction()" mode="M13"/>
   </xsl:template>

	  <!--RULE -->
   <xsl:template match="bldg:intBuildingInstallation" priority="3999" mode="M13">
      <svrl:fired-rule xmlns:svrl="http://purl.oclc.org/dsdl/svrl"
                       context="bldg:intBuildingInstallation"/>

		    <!--REPORT -->
      <xsl:if test="child::bldg:boundedBy/descendant::bldg:lod2MultiSurface">
         <svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl"
                                 test="child::bldg:boundedBy/descendant::bldg:lod2MultiSurface">
            <xsl:attribute name="location">
               <xsl:apply-templates select="." mode="schematron-get-full-path"/>
            </xsl:attribute>
            <svrl:text>
                <xsl:text/>
               <xsl:value-of select="name(.)"/>
               <xsl:text/> with id  <xsl:text/>
               <xsl:value-of select="@gml:id"/>
               <xsl:text/> refers to a LoD2 BoundarySurface
            </svrl:text>
         </svrl:successful-report>
      </xsl:if>

		    <!--REPORT -->
      <xsl:if test="child::bldg:boundedBy/descendant::bldg:lod3MultiSurface">
         <svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl"
                                 test="child::bldg:boundedBy/descendant::bldg:lod3MultiSurface">
            <xsl:attribute name="location">
               <xsl:apply-templates select="." mode="schematron-get-full-path"/>
            </xsl:attribute>
            <svrl:text>
                <xsl:text/>
               <xsl:value-of select="name(.)"/>
               <xsl:text/> with id  <xsl:text/>
               <xsl:value-of select="@gml:id"/>
               <xsl:text/> refers to a LoD3 BoundarySurface
            </svrl:text>
         </svrl:successful-report>
      </xsl:if>
      <xsl:apply-templates select="@*|*|comment()|processing-instruction()" mode="M13"/>
   </xsl:template>
   <xsl:template match="text()" priority="-1" mode="M13"/>
   <xsl:template match="@*|node()" priority="-2" mode="M13">
      <xsl:choose><!--Housekeeping: SAXON warns if attempting to find the attribute
                           of an attribute-->
         <xsl:when test="not(@*)">
            <xsl:apply-templates select="node()" mode="M13"/>
         </xsl:when>
         <xsl:otherwise>
            <xsl:apply-templates select="@*|node()" mode="M13"/>
         </xsl:otherwise>
      </xsl:choose>
   </xsl:template>
</xsl:stylesheet>
