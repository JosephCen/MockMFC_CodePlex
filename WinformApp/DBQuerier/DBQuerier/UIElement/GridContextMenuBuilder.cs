using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Windows.Forms;

using DBQuerier.DataObj;
using DBQuerier.UIElement.Common;


namespace DBQuerier.UIElement
{
    class GridContextMenuBuilder
    {
        private ContextMenuDef m_MenuDef;
        private ContextMenuStrip m_CurrentContextMenu;

        public GridContextMenuBuilder(ContextMenuDef menuDef)
        {
            m_CurrentContextMenu = null;
            m_MenuDef = menuDef;
        }

        public void BuildContextMenu(ContextMenuStrip gridCtxMenu, DataTable srcDataTable)
        {
            gridCtxMenu.Items.Clear();

            if (m_MenuDef != null && m_MenuDef.Items != null)
            {
                IDictionary<String, DataColumn> colNames = 
                    srcDataTable.Columns.Cast<DataColumn>().ToDictionary(c => c.ColumnName, StringComparer.Ordinal);

                foreach (ContextMenuItemDef eachItemDef in m_MenuDef.Items)
                {
                    Int32 matchColCount = eachItemDef.SourceColumn.Count(s => colNames.ContainsKey(s));

                    if (matchColCount == eachItemDef.SourceColumn.Length)
                    {
                        ToolStripItem menuItem = gridCtxMenu.Items.Add(eachItemDef.ItemName);

                        if (eachItemDef is MenuItemXmlViewerDef)
                            menuItem.Tag = new GridContextMenuItem(eachItemDef, new XmlViewer((MenuItemXmlViewerDef)eachItemDef));
                        if (eachItemDef is MenuItemXmlSaverDef)
                            menuItem.Tag = new GridContextMenuItem(eachItemDef, new XmlSaver((MenuItemXmlSaverDef)eachItemDef));
                    }
                }
            }
        }

        public ContextMenuStrip CurrentContextMenu
        {
            get { return m_CurrentContextMenu; }
        }
    }
}
