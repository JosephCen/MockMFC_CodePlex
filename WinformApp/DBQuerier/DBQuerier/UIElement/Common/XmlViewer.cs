using System;

using DBQuerier.UIElement;
using DBQuerier.DataObj;


namespace DBQuerier.UIElement.Common
{
    class XmlViewer : IMenuItemInvoker
    {
        private MenuItemXmlViewerDef m_XmlViewerDef;

        public XmlViewer(MenuItemXmlViewerDef xmlViewerDef)
        {
            m_XmlViewerDef = xmlViewerDef;
        }

        #region IMenuItemInvoker Members

        public Boolean CanInvoke(Object[] invokeArgs)
        {
            Boolean bCanInvoke = false;

            if (invokeArgs.Length > 0)
            {
                String xmlStr = invokeArgs[0] as String;

                if (xmlStr != null)
                    bCanInvoke = XmlSaveOpener.IsXmlString(xmlStr);
            }

            return bCanInvoke;
        }

        public void Invoke(Object[] invokeArgs)
        {
            if (invokeArgs.Length > 0)
            {
                String xmlStr = invokeArgs[0] as String;

                if (xmlStr != null)
                    XmlSaveOpener.OpenXmlByExployer(xmlStr, m_XmlViewerDef.ViewApp);
            }
        }

        #endregion
    }
}
