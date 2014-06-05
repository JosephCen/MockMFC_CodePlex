using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Text;

using DBQuerier.DataObj;


namespace DBQuerier.UIElement
{
    class GridContextMenuItem
    {
        private IMenuItemInvoker m_Invoker;
        private ContextMenuItemDef m_MenuItemDef;
        private DataRow m_CurRow;
        private Object[] m_CellDatas;

        public GridContextMenuItem(ContextMenuItemDef menuItemDef, IMenuItemInvoker invoker)
        {
            m_MenuItemDef = menuItemDef;
            m_Invoker = invoker;
            m_CurRow = null;
            m_CellDatas = null;
        }

        public Boolean GetMenuEnabled(DataRow row)
        {
            Boolean bEnabled = false;
            Object[] cellDatas = null;
                
            cellDatas = m_MenuItemDef.SourceColumn.Select(s => row[s]).ToArray();

            if (m_Invoker.CanInvoke(cellDatas))
            {
                bEnabled = true;
                m_CurRow = row;
                m_CellDatas = cellDatas;
            }

            return bEnabled;
        }

        public void MenuClick(DataRow row)
        {
            Object[] cellDatas = null;

            if (row != null)
                cellDatas = m_MenuItemDef.SourceColumn.Select(s => row[s]).ToArray();
            else
                cellDatas = m_CellDatas;

            if (cellDatas != null)
                m_Invoker.Invoke(cellDatas);
        }

        public void MenuClickWithNoParams()
        {
            MenuClick(null);
        }

        public override String ToString()
        {
            return m_MenuItemDef.ItemName;
        }
    }

    interface IMenuItemInvoker
    {
        Boolean CanInvoke(Object[] invokeArgs);

        void Invoke(Object[] invokeArgs);
    }
}
