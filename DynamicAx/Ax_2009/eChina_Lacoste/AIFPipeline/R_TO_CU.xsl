<?xml version="1.0" encoding="utf-8"?>
<!-- version 20120727 -->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:msxsl="urn:schemas-microsoft-com:xslt" exclude-result-prefixes="msxsl m" xmlns:m="http://www.avanade.com/merge">
  <xsl:output method="xml" indent="yes" />

  <xsl:param name="normalize" select="'yes'" />

  <xsl:template match="/">
    <!--<xsl:element name="Envelope" namespace="http://schemas.microsoft.com/dynamics/2008/01/documents/Message">
      <xsl:copy-of select="//*[local-name()='Header']"/>-->

      <!--<xsl:element name="Body" namespace="http://schemas.microsoft.com/dynamics/2008/01/documents/Message" >-->
        <xsl:element name="MessageParts" namespace="http://schemas.microsoft.com/dynamics/2008/01/documents/Message">
		    <xsl:copy-of select="//*[local-name()='EntityKeyMessage']/*" />
          <xsl:call-template name="m:mergeAndSort">
            <!--<xsl:with-param name="nodes1" select="//*[local-name()='CreateMessage']//*[local-name()='Customer']" />
				<xsl:with-param name="nodes2" select="//*[local-name()='FindMessage']/*[local-name()='Customer']" />
			-->		
            
			<xsl:with-param name="nodes1" select="//*[local-name()='CreateMessage']/*[local-name()='MessageParts']/*[2]" />
            <xsl:with-param name="nodes2" select="//*[local-name()='FindMessage']/*[1]" />
                        
            
          </xsl:call-template>

        </xsl:element>
      <!--</xsl:element>-->
    <!--</xsl:element>-->
  </xsl:template>

  <xsl:template name="m:mergeAndSort">
    <xsl:param name="nodes1" />
    <xsl:param name="nodes2" />

    <xsl:variable name="tree">
      <xsl:call-template name="m:merge">
        <xsl:with-param name="nodes1" select="$nodes1" />
        <xsl:with-param name="nodes2" select="$nodes2" />
      </xsl:call-template>
    </xsl:variable>

    <xsl:for-each select="msxsl:node-set($tree)">

      <xsl:variable name="type1">
        <xsl:apply-templates mode="m:detect-type" select="current()" />
      </xsl:variable>

      <xsl:choose>
        <xsl:when test="$type1='text'">
          <xsl:copy-of select="text()" />
        </xsl:when>

        <xsl:otherwise>
          <xsl:for-each select="*[(local-name()='DocPurpose' or local-name()='SenderId')]">
            <xsl:comment>
              <xsl:text>Excluded from sort </xsl:text>
            </xsl:comment>
            <xsl:copy-of select="." />
          </xsl:for-each>

          <xsl:for-each select="*[not (local-name()='DocPurpose' or local-name()='SenderId') and not(@class='entity')]">
            <xsl:sort select="local-name()" />
            <xsl:copy-of select="." />
          </xsl:for-each>

          <xsl:for-each select="*[not (local-name()='DocPurpose' or local-name()='SenderId') and (@class='entity')]">
            <xsl:sort select="local-name()" />
            <xsl:copy-of select="." />
          </xsl:for-each>


        
        </xsl:otherwise>
        
      </xsl:choose>

    </xsl:for-each>  

  </xsl:template>
  
  <!-- ====================================-->
  <!-- The "merge" template -->

  <xsl:template name="m:merge">
    <xsl:param name="nodes1" />
    <xsl:param name="nodes2" />
    <xsl:choose>
      <!-- Is $nodes1 resp. $nodes2 empty? -->
      <xsl:when test="count($nodes1)=0">
        <xsl:copy-of select="$nodes2" />
      </xsl:when>
      <xsl:when test="count($nodes2)=0">
        <xsl:copy-of select="$nodes1" />
      </xsl:when>
      <xsl:otherwise>
        <!-- Split $nodes1 and $nodes2 -->
        <xsl:variable name="first1" select="$nodes1[1]" />
        <xsl:variable name="rest1" select="$nodes1[position()!=1]" />
        <xsl:variable name="first2" select="$nodes2[1]" />
        <xsl:variable name="rest2" select="$nodes2[position()!=1]" />
        <!-- Determine type of node $first1 -->
        <xsl:variable name="type1">
          <xsl:apply-templates mode="m:detect-type" select="$first1" />
        </xsl:variable>     

        <!-- Compare $first1 and $first2 -->
        <xsl:variable name="diff-first">
          <xsl:call-template name="m:compare-nodes">
            <xsl:with-param name="node1" select="$first1" />
            <xsl:with-param name="node2" select="$first2" />
          </xsl:call-template>
        </xsl:variable>
        <xsl:choose>
          <!-- $first1 != $first2 -->
          <xsl:when test="$diff-first='!'">
            <!-- Compare $first1 and $rest2 -->
            <xsl:variable name="diff-rest">
              <xsl:for-each select="$rest2">
                <xsl:call-template name="m:compare-nodes">
                  <xsl:with-param name="node1" select="$first1" />
                  <xsl:with-param name="node2" select="." />
                </xsl:call-template>
              </xsl:for-each>
            </xsl:variable>
            <xsl:choose>

              <!-- $first1 is in $rest2 and $first1 is *not* an empty text node -->

              <xsl:when test="contains($diff-rest,'=') and not($type1='text' and normalize-space($first1)='')">

                <!-- determine position of $first1 in $nodes2 and copy all preceding nodes of $nodes2 -->

                <xsl:variable name="pos" select="string-length(substring-before( $diff-rest,'=')) + 2" />
                <xsl:copy-of select="$nodes2[position() &lt; $pos]" />

                <!-- merge $first1 with its equivalent node -->

                <xsl:choose>
                  <!-- Elements: merge -->
                  <xsl:when test="$type1='element'">
                    <xsl:element name="{name($first1)}" namespace="{namespace-uri($first1)}">
                      <xsl:copy-of select="$first1/@*" />

                      <xsl:call-template name="m:mergeAndSort">
                        <xsl:with-param name="nodes1" select="$first1/node()" />
                        <xsl:with-param name="nodes2" select="$nodes2[position()=$pos]/node()" />
                      </xsl:call-template>
                    </xsl:element>
                  </xsl:when>

                  <!-- Other: copy -->

                  <xsl:otherwise>
                    <xsl:copy-of select="$first1" />
                  </xsl:otherwise>

                </xsl:choose>

                <!-- Merge $rest1 and rest of $nodes2 -->

                <xsl:call-template name="m:mergeAndSort">
                  <xsl:with-param name="nodes1" select="$rest1" />
                  <xsl:with-param name="nodes2" select="$nodes2[position() &gt; $pos]" />
                </xsl:call-template>
              </xsl:when>
              <!-- else: $first1 is not in $rest2 or $first1 is an empty text node -->
              <xsl:otherwise>
                <xsl:copy-of select="$first1" />
                <xsl:call-template name="m:mergeAndSort">
                  <xsl:with-param name="nodes1" select="$rest1" />
                  <xsl:with-param name="nodes2" select="$nodes2" />
                </xsl:call-template>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:when>
          <!-- else: $first1 = $first2 -->
          <xsl:otherwise>
            <xsl:choose>

              <!-- Elements: merge -->

              <xsl:when test="$type1='element'">
                <xsl:element name="{name($first1)}" namespace="{namespace-uri($first1)}">
                  <xsl:copy-of select="$first1/@*" />
                  <xsl:call-template name="m:mergeAndSort">
                    <xsl:with-param name="nodes1" select="$first1/node()" />
                    <xsl:with-param name="nodes2" select="$first2/node()" />
                  </xsl:call-template>
                </xsl:element>
              </xsl:when>

              <!-- Other: copy -->

              <xsl:otherwise>
                <xsl:copy-of select="$first1" />
              </xsl:otherwise>
            </xsl:choose>
            <!-- Merge $rest1 and $rest2 -->
        
            <xsl:call-template name="m:mergeAndSort">
              <xsl:with-param name="nodes1" select="$rest1" />
              <xsl:with-param name="nodes2" select="$rest2" />
            </xsl:call-template>
        
          </xsl:otherwise>
        </xsl:choose>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!-- Comparing single nodes: 
if $node1 and $node2 are equivalent then the template creates a 
text node "=" otherwise a text node "!" -->

  <xsl:template name="m:compare-nodes">
    <xsl:param name="node1" />
    <xsl:param name="node2" />
    <xsl:variable name="type1">
      <xsl:apply-templates mode="m:detect-type" select="$node1" />
    </xsl:variable>
    <xsl:variable name="type2">
      <xsl:apply-templates mode="m:detect-type" select="$node2" />
    </xsl:variable>
    <xsl:choose>

      <!-- Are $node1 and $node2 element nodes with the same name? -->

      <xsl:when test="$type1='element' and $type2='element' and local-name($node1)=local-name($node2)">
        <!--<xsl:when test="$type1='element' and $type2='element' and local-name($node1)=local-name($node2) and namespace-uri($node1)=namespace-uri($node2)">-->

        <!-- Comparing the attributes -->

        <xsl:variable name="diff-att">

          <!-- same number ... -->

          <xsl:if test="count($node1/@*)!=count($node2/@*)">.</xsl:if>

          <!-- ... and same name/content -->

          <xsl:for-each select="$node1/@*">
            <!--<xsl:if test="not($node2/@* [local-name()=local-name(current()) and namespace-uri()=namespace-uri(current()) and .=current()])">.</xsl:if>-->
            <xsl:if test="not($node2/@* [local-name()=local-name(current()) and .=current()])">.</xsl:if>
          </xsl:for-each>
        </xsl:variable>
        <xsl:choose>
          <xsl:when test="string-length($diff-att)!=0">!</xsl:when>
          <xsl:otherwise>=</xsl:otherwise>
        </xsl:choose>
      </xsl:when>
      <!-- Other nodes: test for the same type and content -->
      <xsl:when test="$type1!='element' and $type1=$type2 and name($node1)=name($node2) and ($node1=$node2 or ($normalize='yes' and normalize-space($node1)= normalize-space($node2)))">=</xsl:when>

      <xsl:when test="$type1='text' and $type2='text'">=</xsl:when>
      
      <!-- Otherwise: different node types or different name/content -->
      <xsl:otherwise>!</xsl:otherwise>
    </xsl:choose>
  </xsl:template>


  <xsl:template match="*" mode="m:detect-type">element</xsl:template>
  <xsl:template match="text()" mode="m:detect-type">text</xsl:template>
  <xsl:template match="comment()" mode="m:detect-type">comment</xsl:template>
  <xsl:template match="processing-instruction()" mode="m:detect-type">pi</xsl:template>


</xsl:stylesheet>