using System;

using DBQuerier.DatabaseObj;

namespace DBQuerier.UIElement
{
    class ExQueryComboItem
    {
        private ExQuery m_ExQuery;

        public ExQueryComboItem(ExQuery exQuery)
        {
            m_ExQuery = exQuery;
        }

        public override String ToString()
        {
            return m_ExQuery.QueryName;
        }

        public ExQuery ExQuery
        {
            get { return m_ExQuery; }
        }
    }
}