using System;
using System.Data.Common;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using DBQuerier.DataObj;


namespace DBQuerier.DatabaseObj
{
    class SearchParameterSet
    {
        private QueryParmDef m_ParmDef;
        private List<BaseSearchParameter> m_SearchParmList;

        public SearchParameterSet(QueryParmDef parmDef)
        {
            m_ParmDef = parmDef;
            m_SearchParmList = new List<BaseSearchParameter>();
        }

        public void AddSearchParameter(BaseSearchParameter searchParm)
        {
            if (m_ParmDef != searchParm.QueryParmDef)
                throw new Exception("Cannot add different QueryParmDef instance into same SearchParameterSet.");
            else
                m_SearchParmList.Add(searchParm);
        }

        public String GetSearchSql()
        {
            Int32 parmIdx = 1;
            StringBuilder sqlStrB = new StringBuilder();

            for (Int32 i = 0; i < m_SearchParmList.Count; ++i)
            {
                sqlStrB.Append(m_SearchParmList[i].GetSearchSql(parmIdx));
                ++parmIdx;
                if ((i + 1) < m_SearchParmList.Count)
                    sqlStrB.Append(" or ");
            }

            return String.Format("and ({0})", sqlStrB.ToString());
        }

        public DbParameter[] GetDbParameters(DbCommand dbCmd)
        {
            List<DbParameter> dbParmList = new List<DbParameter>();

            if (CanGenerateSearchSql())
            {
                Int32 parmIdx = 1;

                for (Int32 i = 0; i < m_SearchParmList.Count; ++i)
                {
                    dbParmList.Add(m_SearchParmList[i].GetDbParameter(dbCmd, parmIdx));
                    ++parmIdx;
                }
            }
            else
            {
                if (!this.IsEmpty)
                    dbParmList.Add(m_SearchParmList[0].GetDbParameter(dbCmd, 0));
            }

            return dbParmList.ToArray();
        }

        public Boolean CanGenerateSearchSql()
        {
            Boolean canGenerate = false;

            if (m_SearchParmList.Count > 0)
            {
                if (!m_SearchParmList.Exists(s => !s.CanGenerateSearchSql()))
                    canGenerate = true;
            }

            return canGenerate;
        }

        public QueryParmDef QueryParmDef
        {
            get { return m_ParmDef; }
        }

        public Int32 ParameterCount
        {
            get { return m_SearchParmList.Count; }
        }

        public Boolean IsEmpty
        {
            get { return (m_SearchParmList.Count == 0); }
        }
    }
}
