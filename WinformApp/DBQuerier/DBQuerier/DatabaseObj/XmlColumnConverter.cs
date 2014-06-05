using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;

using DBQuerier.DataObj;


namespace DBQuerier.DatabaseObj
{
    class XmlColumnConverter : IDataColumnConverter
    {
        private ExtraColumnDef m_ColumnDef;
        private String m_XPath;

        public XmlColumnConverter(ExtraColumnDef columnDef)
        {
            m_ColumnDef = columnDef;
            m_XPath = (columnDef.xPath != null ? columnDef.xPath : String.Empty);
        }

        #region IDataColumnConverter Members

        public String Convert(Object colValue)
        {
            String xmlStr = colValue as String;
            StringBuilder retStrB = new StringBuilder();
            String splitStr = " | ";

            if (!String.IsNullOrEmpty(m_XPath) && !String.IsNullOrEmpty(xmlStr))
            {
                XmlDocument xmlDoc = new XmlDocument();
                XmlNamespaceManager xmlNsMgr = null;
                XmlNodeList xmlNodes = null;

                xmlDoc.LoadXml(xmlStr);
                
                xmlNsMgr = new XmlNamespaceManager(xmlDoc.NameTable);

                xmlNodes = xmlDoc.SelectNodes(m_XPath);
                foreach (XmlNode eachNode in xmlNodes)
                    retStrB.Append(eachNode.InnerText).Append(splitStr);

                if (xmlNodes.Count > 0 && retStrB.Length > 0)
                    retStrB.Remove(retStrB.Length - splitStr.Length, splitStr.Length);
            }

            return retStrB.ToString();
        }

        public Int32 ColumnIndex
        { get; set; }

        #endregion
    }
}
