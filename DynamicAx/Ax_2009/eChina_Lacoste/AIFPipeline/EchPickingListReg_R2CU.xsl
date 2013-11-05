<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:msxsl="urn:schemas-microsoft-com:xslt" exclude-result-prefixes="msxsl m" xmlns:m="http://www.avanade.com/merge">
  <xsl:output method="xml" indent="yes" />

  <xsl:param name="normalize" select="'yes'" />

  <xsl:template match="/">
    <!-- <MessageParts> -->
    <xsl:element name="MessageParts" namespace="http://schemas.microsoft.com/dynamics/2008/01/documents/Message">
      <xsl:copy-of select="//*[local-name()='EntityKeyMessage']/*" />
      
      <!-- <EchPickingListReg> -->
      <xsl:call-template name="m:merge_EchPickingListReg">
        <xsl:with-param name="createNode" select="//*[local-name()='CreateMessage']/*[local-name()='MessageParts']/*[local-name()='EchPickingListReg']" />
        <xsl:with-param name="findNode" select="//*[local-name()='FindMessage']/*[local-name()='EchPickingListReg']" />
      </xsl:call-template>
    </xsl:element>
  </xsl:template>

  <xsl:template name="m:merge_EchPickingListReg">
    <xsl:param name="createNode" />
    <xsl:param name="findNode" />

    <!-- EchPickingListReg -->
    <xsl:for-each select="$findNode">
      <xsl:element name="{name(.)}" namespace="{namespace-uri(.)}">
        <!-- EchPickingListReg/* -->
        <xsl:for-each select="./*">
          <xsl:choose>
            <xsl:when test="local-name(.)='WMSPickingRoute'">
              <xsl:call-template name="m:merge_WMSPickingRoute">
                <xsl:with-param name="createNode" select="$createNode/*" />
                <xsl:with-param name="findNode" select="." />
              </xsl:call-template>
            </xsl:when>
            <xsl:otherwise>
              <xsl:call-template name="m:findNode_localName">
                <xsl:with-param name="nodeSet" select="$createNode/*" />
                <xsl:with-param name="lookupNode" select="." />
              </xsl:call-template>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:for-each>
      </xsl:element>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name="m:merge_WMSPickingRoute">
    <xsl:param name="createNode" />
    <xsl:param name="findNode" />
    
    <!-- EchPickingListReg/WMSPickingRoute -->
    <xsl:for-each select="$findNode">

      <xsl:variable name="sub_TransRefId" select="./*[local-name()='TransRefId']" />
      <xsl:variable name="found_WMSPickingRoute" select="$createNode/*[local-name()='TransRefId' and text()=$sub_TransRefId/text()]/.." />
      
      <xsl:choose>
        <!-- Found matched node -->
        <xsl:when test="count($found_WMSPickingRoute) &gt; 0">
          <xsl:element name="{name(.)}" namespace="{namespace-uri(.)}">
            <!-- Copy all these attribute from source node -->
            <xsl:copy-of select="./@*" />
            <!-- WMSPickingRoute/* -->
            <xsl:for-each select="./*">
              <xsl:choose>
                <xsl:when test="local-name(.)='WMSOrderTrans'">
                  <xsl:call-template name="m:merge_WMSOrderTrans">
                    <xsl:with-param name="createNode" select="$createNode/*" />
                    <xsl:with-param name="findNode" select="." />
                  </xsl:call-template>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:call-template name="m:findNode_localName">
                    <xsl:with-param name="nodeSet" select="$found_WMSPickingRoute/*" />
                    <xsl:with-param name="lookupNode" select="." />
                  </xsl:call-template>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:for-each>
          </xsl:element>
        </xsl:when>
        <!-- Otherwise, copy original node to output xml. -->
        <xsl:otherwise>
          <xsl:copy-of select="." />
        </xsl:otherwise>
      </xsl:choose>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name="m:merge_WMSOrderTrans">
    <xsl:param name="createNode" />
    <xsl:param name="findNode" />

    <!-- EchPickingListReg/WMSPickingRoute/WMSOrderTrans -->
    <xsl:for-each select="$findNode">

      <xsl:variable name="sub_InventTransId" select="./*[local-name()='InventTransId']" />
      <xsl:variable name="found_WMSOrderTrans" select="$createNode/*[local-name()='InventTransId' and text()=$sub_InventTransId/text()]/.." />

      <xsl:choose>
        <!-- Found matched node -->
        <xsl:when test="count($found_WMSOrderTrans) &gt; 0">
          <xsl:element name="{name(.)}" namespace="{namespace-uri(.)}">
            <!-- Copy all these attribute from source node -->
            <xsl:copy-of select="./@*" />
            <!-- WMSPickingRoute/* -->
            <xsl:for-each select="./*">
              <xsl:call-template name="m:findNode_localName">
                <xsl:with-param name="nodeSet" select="$found_WMSOrderTrans/*" />
                <xsl:with-param name="lookupNode" select="." />
              </xsl:call-template>
            </xsl:for-each>
          </xsl:element>
        </xsl:when>
        <!-- Otherwise, copy original node to output xml. -->
        <xsl:otherwise>
          <xsl:copy-of select="." />
        </xsl:otherwise>
      </xsl:choose>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name="m:findNode_localName">
    <xsl:param name="nodeSet" />
    <xsl:param name="lookupNode" />

    <xsl:variable name="foundNode" select="$nodeSet[local-name()=local-name($lookupNode)]" />
    
    <xsl:choose>
      <xsl:when test="$foundNode/text() != ''">
        <xsl:element name="{name($lookupNode)}" namespace="{namespace-uri($lookupNode)}">
          <!-- Copy attribute from $lookupNode -->
          <xsl:copy-of select="./@*" />
          <!-- Fetch the value of $foundNode -->
          <xsl:value-of select="$foundNode/text()" />
        </xsl:element>
      </xsl:when>
      <xsl:otherwise>
        <xsl:copy-of select="$lookupNode" />
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!--
      <xsl:variable name="found_WMSPickingRoute">
        <xsl:call-template name="m:findNode_subnode">
          <xsl:with-param name="nodeSet" select="$createNode" />
          <xsl:with-param name="subNode" select="./*[local-name()='TransRefId']" />
        </xsl:call-template>
      </xsl:variable>
  
  <xsl:template name="m:findNode_subnode">
    <xsl:param name="nodeSet" />
    <xsl:param name="subNode" />

    <xsl:variable name="foundNode" select="$nodeSet/*[local-name()=local-name($subNode) and text()=$subNode/text()]" />

    <xsl:if test="count($foundNode) > 0">
      <xsl:copy-of select="$foundNode/.."/>
    </xsl:if>
  </xsl:template>
  -->
</xsl:stylesheet>
